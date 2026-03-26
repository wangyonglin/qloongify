#ifndef ALIIOTCLIENT_H
#define ALIIOTCLIENT_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QTimer>
#include <QByteArray>
extern "C" {
#include "aiot_state_api.h"
#include "aiot_sysdep_api.h"
#include "aiot_mqtt_api.h"
}

// 外部提供的阿里云根证书（通常位于 external/ali_ca_cert.c）
extern const char *ali_ca_cert;
/* 位于portfiles/aiot_port文件夹下的系统适配函数集合 */
extern aiot_sysdep_portfile_t g_aiot_sysdep_portfile;

class AliIOTClient : public QThread
{
    Q_OBJECT
    Q_PROPERTY(ConnectionState state READ state NOTIFY stateChanged)
public:
    enum ConnectionState {
        StateDisconnected,
        StateConnecting,
        StateConnected,
        StateReconnecting
    };
    Q_ENUM(ConnectionState)

    explicit AliIOTClient(QObject *parent = nullptr);
    ~AliIOTClient();
    // 配置参数（应在调用 connectToHost 前设置）
    QByteArray mqttHost2Utf8() const;
    void setMqttHost(const QString &newMqttHost);

    QByteArray productKey2Utf8() const;
    void setProductKey(const QString &newProductKey);

    QByteArray deviceNameUtf8() const;
    void setDeviceName(const QString &newDeviceName);

    QByteArray deviceSecretUtf8() const;
    void setDeviceSecret(const QString &newDeviceSecret);

    uint16_t mqttPort() const;
    void setMqttPort(uint16_t newPort);
    static QString topicFullName(const QString  &productKey,const QString  &deviceName,const QString  &newTopic);



    // 连接管理（启动/停止线程）
    bool connectToHost();               // 启动线程并尝试连接
    void disconnectFromHost();          // 断开连接并停止线程

    // 发布/订阅（线程安全）
    bool publish(const QString &topic, const QByteArray &payload, int qos = 0);
    bool subscribe(const QString &topic, int qos = 0);
    bool unsubscribe(const QString &topic);

    ConnectionState state() const { return m_state; }

signals:
    void connected();                    // 连接成功（包括首次连接和重连）
    void disconnected();                 // 连接断开（主动或被动）
    void stateChanged(ConnectionState state);
    void messageReceived(const QString &topic, const QByteArray &payload);
    void errorOccurred(int errorCode, const QString &errorString);

protected:
    void run() override;                 // 线程入口：执行 MQTT 收发循环

private:

    // SDK 静态回调（通过 userdata 传递 this）
    static int32_t logCallback(int32_t code, char *message);
    static void eventHandler(void *handle, const aiot_mqtt_event_t *event, void *userdata);
    static void defaultRecvHandler(void *handle, const aiot_mqtt_recv_t *packet, void *userdata);

    // 内部辅助函数（在工作线程中调用）
    bool initMqttHandle();               // 创建并配置 mqtt_handle
    void destroyMqttHandle();            // 销毁 mqtt_handle
    bool connectMqtt();                  // 执行实际连接
    void setState(ConnectionState newState);
    void handleMqttEvent(const aiot_mqtt_event_t *event);
    void handleMqttRecv(const aiot_mqtt_recv_t *packet);

private:
    // SDK 相关成员
    void *m_mqtt_handle;                 // MQTT 实例句柄（在工作线程中使用）
    aiot_sysdep_network_cred_t m_cred;   // 网络凭据


    // 线程控制
    volatile bool m_running;              // 控制线程是否运行
    QString m_productKey;
    QString m_deviceName;
    QString m_deviceSecret;
    QString m_mqttHost;
    uint16_t m_mqttPort;
    QMutex m_mutex;                       // 保护 m_state 和跨线程访问
    QWaitCondition m_cond;
    // 状态管理
    ConnectionState m_state;
    int m_reconnectInterval;               // 重连间隔（毫秒），默认 5000
};

#endif // ALIIOTCLIENT_H
