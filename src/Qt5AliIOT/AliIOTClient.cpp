#include "AliIOTClient.h"
#include <QDebug>
#include <QCoreApplication>

// 日志回调
int32_t AliIOTClient::logCallback(int32_t code, char *message)
{
    qDebug() << "[AliMqtt]" << message;
    return 0;
}

// 事件回调转发
void AliIOTClient::eventHandler(void *handle, const aiot_mqtt_event_t *event, void *userdata)
{
    Q_UNUSED(handle);
    AliIOTClient *client = reinterpret_cast<AliIOTClient*>(userdata);
    if (client) {
        // 由于回调在工作线程中执行，直接调用处理函数
        client->handleMqttEvent(event);
    }
}

// 消息接收回调转发
void AliIOTClient::defaultRecvHandler(void *handle, const aiot_mqtt_recv_t *packet, void *userdata)
{
    Q_UNUSED(handle);
    AliIOTClient *client = reinterpret_cast<AliIOTClient*>(userdata);
    if (client) {
        client->handleMqttRecv(packet);
    }
}

AliIOTClient::AliIOTClient(QObject *parent)
    : QThread(parent)
    , m_mqtt_handle(nullptr)
    , m_running(false)
    , m_state(StateDisconnected)
    , m_reconnectInterval(5000)
{
    // 配置SDK依赖
    aiot_sysdep_set_portfile(&g_aiot_sysdep_portfile);
    aiot_state_set_logcb(logCallback);

    // 初始化网络凭据（默认使用TLS）
    memset(&m_cred, 0, sizeof(aiot_sysdep_network_cred_t));
    m_cred.option = AIOT_SYSDEP_NETWORK_CRED_SVRCERT_CA;
    m_cred.max_tls_fragment = 16384;
    m_cred.sni_enabled = 1;
    m_cred.x509_server_cert = ali_ca_cert;
    m_cred.x509_server_cert_len = strlen(ali_ca_cert);
}

AliIOTClient::~AliIOTClient()
{
    disconnectFromHost();
    wait(); // 等待线程结束
}





bool AliIOTClient::connectToHost()
{
    QMutexLocker locker(&m_mutex);
    if (m_state == StateConnected || m_state == StateConnecting) {
        qWarning() << "Already connecting or connected";
        return false;
    }

    if (isRunning()) {
        // 如果线程已经在运行，先停止
        locker.unlock();
        disconnectFromHost();
        locker.relock();
    }

    setState(StateConnecting);
    m_running = true;
    start(); // 启动线程，run() 将在新线程中执行
    return true;
}

void AliIOTClient::disconnectFromHost()
{
    {
        QMutexLocker locker(&m_mutex);
        if (!m_running && m_state == StateDisconnected) {
            return;
        }
        m_running = false; // 通知线程退出
    }

    wait(); // 等待线程结束

    QMutexLocker locker(&m_mutex);
    setState(StateDisconnected);
    emit disconnected();
}

bool AliIOTClient::publish(const QString &topic, const QByteArray &payload, int qos)
{
    QMutexLocker locker(&m_mutex);
    if (m_state != StateConnected || !m_mqtt_handle) {
        qWarning() << "Not connected";
        return false;
    }
    QByteArray topicUtf8 = topic.toUtf8();
    int32_t res = aiot_mqtt_pub(m_mqtt_handle,
                                topicUtf8.data(),
                                (uint8_t*)(payload.data()),
                                payload.size(),
                                qos);
    if (res < STATE_SUCCESS) {
        qCritical() << "Publish failed: -0x" << QString::number(-res, 16);
        return false;
    }
    return true;
}

bool AliIOTClient::subscribe(const QString &topic, int qos)
{
    QMutexLocker locker(&m_mutex);
    if (m_state != StateConnected || !m_mqtt_handle) {
        qWarning() << "Not connected";
        return false;
    }
    QByteArray topicUtf8 = topic.toUtf8();
    int32_t res = aiot_mqtt_sub(m_mqtt_handle,
                                topicUtf8.data(),
                                nullptr,
                                qos,
                                nullptr);
    if (res < STATE_SUCCESS) {
        qCritical() << "Subscribe failed: -0x" << QString::number(-res, 16);
        return false;
    }
    return true;
}

bool AliIOTClient::unsubscribe(const QString &topic)
{
    QMutexLocker locker(&m_mutex);
    if (m_state != StateConnected || !m_mqtt_handle) {
        qWarning() << "Not connected";
        return false;
    }
    QByteArray topicUtf8 = topic.toUtf8();
    int32_t res = aiot_mqtt_unsub(m_mqtt_handle, topicUtf8.data());
    if (res < STATE_SUCCESS) {
        qCritical() << "Unsubscribe failed: -0x" << QString::number(-res, 16);
        return false;
    }
    return true;
}

void AliIOTClient::run()
{
    // 线程运行函数，执行连接和收发循环
    while (m_running) {
        // 初始化句柄（如果尚未初始化）
        if (!m_mqtt_handle) {
            if (!initMqttHandle()) {
                qCritical() << "Failed to init MQTT handle";
                msleep(m_reconnectInterval);
                continue;
            }
        }

        // 连接
        if (!connectMqtt()) {
            qWarning() << "MQTT connect failed, will retry after" << m_reconnectInterval << "ms";
            destroyMqttHandle();
            setState(StateDisconnected);
            emit disconnected(); // 通知上层连接失败
            msleep(m_reconnectInterval);
            continue;
        }

        // 连接成功，进入收发循环
        setState(StateConnected);
        emit connected();

        while (m_running) {
            // 处理心跳和重发
          int32_t prores=  aiot_mqtt_process(m_mqtt_handle);
            if (prores < STATE_SUCCESS) {
                qWarning() << "心跳和重发 aiot_mqtt_process failed: -0x" << QString::number(-prores, 16);
                break;
            }
            // 接收消息（会阻塞直到有消息或超时）
            int32_t res = aiot_mqtt_recv(m_mqtt_handle);
            if (res < STATE_SUCCESS) {
                if (res == STATE_USER_INPUT_EXEC_DISABLED) {
                    // 主动停止，退出循环
                    break;
                }
                // 网络错误或断线，退出内层循环，外层会重连
                qWarning() << "aiot_mqtt_recv failed: -0x" << QString::number(-res, 16);
                break;
            }

            // 避免忙等，根据网络情况适当休眠
            msleep(100);
        }

        // 连接已断开
        setState(StateDisconnected);
        emit disconnected();

        // 清理句柄
        destroyMqttHandle();

        // 如果是因为主动停止而退出，则不再重连
        if (!m_running) break;

        // 否则等待一段时间后重试
        msleep(m_reconnectInterval);
    }

    // 线程结束，确保状态为断开
    setState(StateDisconnected);
    emit disconnected();
}

bool AliIOTClient::initMqttHandle()
{
    if (m_mqtt_handle) return true;

    m_mqtt_handle = aiot_mqtt_init();
    if (!m_mqtt_handle) return false;

    // 设置参数（从成员变量获取）
   uint16_t port= mqttPort();
    aiot_mqtt_setopt(m_mqtt_handle, AIOT_MQTTOPT_HOST, mqttHost2Utf8().data());

    aiot_mqtt_setopt(m_mqtt_handle, AIOT_MQTTOPT_PORT, &port);
    aiot_mqtt_setopt(m_mqtt_handle, AIOT_MQTTOPT_PRODUCT_KEY, productKey2Utf8().data());
    aiot_mqtt_setopt(m_mqtt_handle, AIOT_MQTTOPT_DEVICE_NAME, deviceNameUtf8().data());
    aiot_mqtt_setopt(m_mqtt_handle, AIOT_MQTTOPT_DEVICE_SECRET, deviceSecretUtf8().data());
    aiot_mqtt_setopt(m_mqtt_handle, AIOT_MQTTOPT_NETWORK_CRED, &m_cred);
    aiot_mqtt_setopt(m_mqtt_handle, AIOT_MQTTOPT_RECV_HANDLER, (void*)defaultRecvHandler);
    aiot_mqtt_setopt(m_mqtt_handle, AIOT_MQTTOPT_EVENT_HANDLER, (void*)eventHandler);
    aiot_mqtt_setopt(m_mqtt_handle, AIOT_MQTTOPT_USERDATA, this);

    return true;
}

void AliIOTClient::destroyMqttHandle()
{
    if (m_mqtt_handle) {
        aiot_mqtt_deinit(&m_mqtt_handle);
        m_mqtt_handle = nullptr;
    }
}

bool AliIOTClient::connectMqtt()
{
    if (!m_mqtt_handle) return false;
    int32_t res = aiot_mqtt_connect(m_mqtt_handle);
    return (res >= STATE_SUCCESS);
}

void AliIOTClient::setState(ConnectionState newState)
{
    if (m_state != newState) {
        m_state = newState;
        emit stateChanged(m_state);
    }
}

void AliIOTClient::handleMqttEvent(const aiot_mqtt_event_t *event)
{
    switch (event->type) {
    case AIOT_MQTTEVT_CONNECT:
        qDebug() << "MQTT connected (first time)";
        setState(StateConnected);
        emit connected();
        break;
    case AIOT_MQTTEVT_RECONNECT:
        qDebug() << "MQTT reconnected";
        setState(StateConnected);
        emit connected();
        break;
    case AIOT_MQTTEVT_DISCONNECT:
    {
        const char *cause = (event->data.disconnect == AIOT_MQTTDISCONNEVT_NETWORK_DISCONNECT) ?
                                "network disconnect" : "heartbeat disconnect";
        qWarning() << "MQTT disconnected:" << cause;
        setState(StateDisconnected);
        emit disconnected();
        break;
    }
    default:
        break;
    }
}

void AliIOTClient::handleMqttRecv(const aiot_mqtt_recv_t *packet)
{
    switch (packet->type) {
    case AIOT_MQTTRECV_PUB:
    {
        QByteArray topic(packet->data.pub.topic, packet->data.pub.topic_len);
        QByteArray payload(reinterpret_cast<const char*>(packet->data.pub.payload),
                           packet->data.pub.payload_len);
        emit messageReceived(topic,payload);
        break;
    }
    case AIOT_MQTTRECV_SUB_ACK:
        qDebug() << "Sub ACK, packet id:" << packet->data.sub_ack.packet_id;
        break;
    case AIOT_MQTTRECV_PUB_ACK:
        qDebug() << "Pub ACK, packet id:" << packet->data.pub_ack.packet_id;
        break;
    case AIOT_MQTTRECV_HEARTBEAT_RESPONSE:
        // 心跳响应，一般忽略
        break;
    default:
        break;
    }


}

void AliIOTClient::setProductKey(const QString &newProductKey)
{
    QMutexLocker locker(&m_mutex);
    m_productKey = newProductKey;
}

void AliIOTClient::setDeviceSecret(const QString &newDeviceSecret)
{
    QMutexLocker locker(&m_mutex);
    m_deviceSecret = newDeviceSecret;
}



QByteArray AliIOTClient::mqttHost2Utf8() const
{
    return m_mqttHost.toUtf8();
}

void AliIOTClient::setMqttHost(const QString &newMqttHost)
{
    QMutexLocker locker(&m_mutex);
    m_mqttHost = QString("%0.%1").arg(m_productKey).arg(newMqttHost);
}

QByteArray AliIOTClient::productKey2Utf8() const
{
    return m_productKey.toUtf8();
}



QByteArray AliIOTClient::deviceNameUtf8() const
{
    return  QSysInfo::machineUniqueId();
}

void AliIOTClient::setDeviceName(const QString &newDeviceName)
{
    QMutexLocker locker(&m_mutex);
    m_deviceName = newDeviceName;
}

QByteArray AliIOTClient::deviceSecretUtf8() const
{
    return m_deviceSecret.toUtf8();
}




uint16_t AliIOTClient::mqttPort() const
{
    return m_mqttPort;
}

void AliIOTClient::setMqttPort(uint16_t newPort)
{
    QMutexLocker locker(&m_mutex);
    m_mqttPort = newPort;
}

QString AliIOTClient::topicFullName(const QString &productKey, const QString &deviceName, const QString &newTopic)
{
    return QString("/%0/%1%2").arg(productKey).arg(deviceName).arg(newTopic);
}
