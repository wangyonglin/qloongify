#include "DynregObject.h"
#include <QDebug>
#include <QThread>
#include <QMetaObject>
#include <QTimer>
#include <cstring>

// 阿里云IoT SDK 头文件
#include "aiot_state_api.h"
#include "aiot_sysdep_api.h"
#include "aiot_dynreg_api.h"

// 外部依赖（由SDK提供）
extern aiot_sysdep_portfile_t g_aiot_sysdep_portfile;
extern const char *ali_ca_cert;

// 日志回调（将SDK日志输出到Qt调试信息）
static int32_t sdk_log_cb(int32_t code, char *message)
{
    qDebug() << message;
    return 0;
}

// 内部数据结构，用于在回调中暂存结果（与官方示例的 demo_info_t 相同）
struct RegistrationResult {
    uint32_t code = 0;          // 状态码
    char *deviceSecret = nullptr;

    ~RegistrationResult() {
        if (deviceSecret) {
            free(deviceSecret);
            deviceSecret = nullptr;
        }
    }
};

// 工作类，运行在独立线程中执行实际的动态注册（相当于官方示例的 main 函数逻辑）
class RegistrationWorker : public QObject
{
    Q_OBJECT
public:
    RegistrationWorker() : port(443) {
        host = "iot-auth.cn-shanghai.aliyuncs.com";
    }

    void setProductKey(const QString &key)      { productKey = key.toLocal8Bit(); }
    void setDeviceName(const QString &name)     { deviceName = name.toLocal8Bit(); }
    void setProductSecret(const QString &secret){ productSecret = secret.toLocal8Bit(); }
    void setHost(const QString &h)              { host = h.toLocal8Bit(); }
    void setPort(uint16_t p)                    { port = p; }

public slots:
    void doWork() {
        int32_t res = STATE_SUCCESS;
        void *dynregHandle = nullptr;
        aiot_sysdep_network_cred_t cred;
        bool success = false;
        int statusCode = 0;
        QString deviceSecret;
        RegistrationResult result;

        // 1. 配置SDK底层依赖和日志（与官方示例相同）
        aiot_sysdep_set_portfile(&g_aiot_sysdep_portfile);
        aiot_state_set_logcb(sdk_log_cb);

        // 2. 创建安全凭据（TLS）
        memset(&cred, 0, sizeof(aiot_sysdep_network_cred_t));
        cred.option = AIOT_SYSDEP_NETWORK_CRED_SVRCERT_CA;
        cred.max_tls_fragment = 16384;
        cred.sni_enabled = 1;
        cred.x509_server_cert = ali_ca_cert;
        cred.x509_server_cert_len = strlen(ali_ca_cert);

        // 3. 创建动态注册实例
        dynregHandle = aiot_dynreg_init();
        if (!dynregHandle) {
            qCritical() << "aiot_dynreg_init failed";
            emit workFinished(false, -1, QString());
            return;
        }

        // 4. 设置连接参数（host, port）
        res = aiot_dynreg_setopt(dynregHandle, AIOT_DYNREGOPT_HOST, (void*)host.data());
        if (res < STATE_SUCCESS) {
            qCritical() << "set host failed, res: -0x" << QString::number(-res, 16);
            aiot_dynreg_deinit(&dynregHandle);
            emit workFinished(false, -1, QString());
            return;
        }

        res = aiot_dynreg_setopt(dynregHandle, AIOT_DYNREGOPT_PORT, (void*)&port);
        if (res < STATE_SUCCESS) {
            qCritical() << "set port failed, res: -0x" << QString::number(-res, 16);
            aiot_dynreg_deinit(&dynregHandle);
            emit workFinished(false, -1, QString());
            return;
        }

        // 5. 设置设备信息（productKey, productSecret, deviceName）
        res = aiot_dynreg_setopt(dynregHandle, AIOT_DYNREGOPT_PRODUCT_KEY, (void*)productKey.data());
        if (res < STATE_SUCCESS) {
            qCritical() << "set product key failed, res: -0x" << QString::number(-res, 16);
            aiot_dynreg_deinit(&dynregHandle);
            emit workFinished(false, -1, QString());
            return;
        }

        res = aiot_dynreg_setopt(dynregHandle, AIOT_DYNREGOPT_PRODUCT_SECRET, (void*)productSecret.data());
        if (res < STATE_SUCCESS) {
            qCritical() << "set product secret failed, res: -0x" << QString::number(-res, 16);
            aiot_dynreg_deinit(&dynregHandle);
            emit workFinished(false, -1, QString());
            return;
        }

        res = aiot_dynreg_setopt(dynregHandle, AIOT_DYNREGOPT_DEVICE_NAME, (void*)deviceName.data());
        if (res < STATE_SUCCESS) {
            qCritical() << "set device name failed, res: -0x" << QString::number(-res, 16);
            aiot_dynreg_deinit(&dynregHandle);
            emit workFinished(false, -1, QString());
            return;
        }

        // 6. 设置网络凭据
        res = aiot_dynreg_setopt(dynregHandle, AIOT_DYNREGOPT_NETWORK_CRED, (void*)&cred);
        if (res < STATE_SUCCESS) {
            qCritical() << "set network cred failed, res: -0x" << QString::number(-res, 16);
            aiot_dynreg_deinit(&dynregHandle);
            emit workFinished(false, -1, QString());
            return;
        }

        // 7. 设置回调函数和用户数据（与官方示例一致）
        res = aiot_dynreg_setopt(dynregHandle, AIOT_DYNREGOPT_RECV_HANDLER,
                                 (void*)recvCallback);
        if (res < STATE_SUCCESS) {
            qCritical() << "set recv handler failed, res: -0x" << QString::number(-res, 16);
            aiot_dynreg_deinit(&dynregHandle);
            emit workFinished(false, -1, QString());
            return;
        }

        res = aiot_dynreg_setopt(dynregHandle, AIOT_DYNREGOPT_USERDATA, (void*)&result);
        if (res < STATE_SUCCESS) {
            qCritical() << "set userdata failed, res: -0x" << QString::number(-res, 16);
            aiot_dynreg_deinit(&dynregHandle);
            emit workFinished(false, -1, QString());
            return;
        }

        // 8. 发送注册请求
        res = aiot_dynreg_send_request(dynregHandle);
        if (res < STATE_SUCCESS) {
            qCritical() << "send request failed: -0x" << QString::number(-res, 16);
            aiot_dynreg_deinit(&dynregHandle);
            emit workFinished(false, -1, QString());
            return;
        }

        // 9. 接收响应（阻塞，直到收到完整应答或出错）
        res = aiot_dynreg_recv(dynregHandle);
        if (res < STATE_SUCCESS) {
            qCritical() << "recv failed: -0x" << QString::number(-res, 16);
            statusCode = result.code;
            if (result.deviceSecret) {
                free(result.deviceSecret);
                result.deviceSecret = nullptr;
            }
            aiot_dynreg_deinit(&dynregHandle);
            emit workFinished(false, statusCode, QString());
            return;
        }

        // 10. 处理成功结果
        statusCode = result.code;
        if (result.deviceSecret) {
            deviceSecret = QString::fromLocal8Bit(result.deviceSecret);
            free(result.deviceSecret);
            result.deviceSecret = nullptr;
        }
        success = true;

        // 11. 销毁动态注册实例
        aiot_dynreg_deinit(&dynregHandle);

        emit workFinished(success, statusCode, deviceSecret);
    }

signals:
    void workFinished(bool success, int statusCode, const QString &deviceSecret);

private:
    // 静态回调函数，由SDK调用（与官方示例的 demo_dynreg_recv_handler 相同）
    static void recvCallback(void *handle, const aiot_dynreg_recv_t *packet, void *userdata) {
        auto *result = static_cast<RegistrationResult*>(userdata);
        if (!result) return;

        switch (packet->type) {
        case AIOT_DYNREGRECV_STATUS_CODE:
            result->code = packet->data.status_code.code;
            break;
        case AIOT_DYNREGRECV_DEVICE_INFO:
            // 必须深拷贝，因为 packet 在回调返回后会被释放
            if (result->deviceSecret) {
                free(result->deviceSecret);
                result->deviceSecret = nullptr;
            }
            result->deviceSecret = static_cast<char*>(malloc(strlen(packet->data.device_info.device_secret) + 1));
            if (result->deviceSecret) {
                strcpy(result->deviceSecret, packet->data.device_info.device_secret);
            }
            break;
        default:
            break;
        }
    }

    QByteArray productKey;
    QByteArray deviceName;
    QByteArray productSecret;
    QByteArray host;
    uint16_t port;
};

// 私有实现类，管理线程和Worker
class DynregObjectPrivate
{
public:
    DynregObjectPrivate(DynregObject *parent) : q_ptr(parent), worker(nullptr), thread(nullptr) {}
    ~DynregObjectPrivate() {
        if (thread) {
            thread->quit();
            thread->wait();
            delete thread;
        }
    }

    // 每次调用都会重新创建 worker 和 thread（保证每次重试时资源全新）
    void setupWorker() {
        // 清理旧的 worker 和 thread
        if (thread) {
            thread->quit();
            thread->wait();
            delete thread;
            thread = nullptr;
        }
        if (worker) {
            worker->deleteLater();
            worker = nullptr;
        }

        thread = new QThread();
        worker = new RegistrationWorker();
        worker->moveToThread(thread);

        // 连接 worker 完成信号，转发给主对象，并清理线程资源
        QObject::connect(worker, &RegistrationWorker::workFinished,
                         q_ptr, [this](bool success, int code, const QString &secret) {
                             emit q_ptr->registrationResult(success, code, secret);
                             if (thread) {
                                 thread->quit();
                                 // 在线程结束后删除 thread 和 worker
                                 QObject::connect(thread, &QThread::finished, [this]() {
                                     thread->deleteLater();
                                     worker->deleteLater();
                                     thread = nullptr;
                                     worker = nullptr;
                                 });
                             }
                         });
        QObject::connect(thread, &QThread::finished, worker, &QObject::deleteLater);
    }

    void startWork() {
        if (!worker) return;
        worker->setProductKey(productKey);
        worker->setDeviceName(deviceName);
        worker->setProductSecret(productSecret);
        worker->setHost(host);
        worker->setPort(port);
        if (thread) {
            thread->start();
            QMetaObject::invokeMethod(worker, "doWork", Qt::QueuedConnection);
        }
    }

    DynregObject *q_ptr;
    RegistrationWorker *worker;
    QThread *thread;

    // 配置参数
    QString productKey;
    QString deviceName;
    QString productSecret;
    QString host = "iot-auth.cn-shanghai.aliyuncs.com";
    uint16_t port = 443;
};

// ----------------------------------------------------------------------
// DynregObject 公有接口实现
// ----------------------------------------------------------------------
DynregObject::DynregObject(QObject *parent)
    : QObject(parent), d_ptr(new DynregObjectPrivate(this)), m_isRunning(false), m_retryScheduled(false)
{
    connect(this, &DynregObject::registrationResult,
            this, &DynregObject::onRegistrationResult);
}

DynregObject::~DynregObject()
{
    stopRetry();
}

void DynregObject::setProductKey(const QString &productKey)
{
    Q_D(DynregObject);
    d->productKey = productKey;
}

void DynregObject::setDeviceName(const QString &deviceName)
{
    Q_D(DynregObject);
    d->deviceName = deviceName;
}

void DynregObject::setProductSecret(const QString &productSecret)
{
    Q_D(DynregObject);
    d->productSecret = productSecret;
}

void DynregObject::setHost(const QString &host)
{
    Q_D(DynregObject);
    d->host = host;
}

void DynregObject::setPort(uint16_t port)
{
    Q_D(DynregObject);
    d->port = port;
}

void DynregObject::startRegistration()
{
    Q_D(DynregObject);
    if (m_isRunning) {
        qDebug() << "Registration already in progress, ignoring duplicate start";
        return;
    }
    // 取消任何待执行的重试
    stopRetry();

    m_isRunning = true;
    d->setupWorker();
    d->startWork();
}

void DynregObject::stopRetry()
{
    if (m_retryScheduled) {
        // 如果有待执行的重试定时器，这里没有直接保存 QTimer 对象，可以通过其他方式取消
        // 简单做法：不提供精确取消，但用户可以调用 stopRetry 阻止新重试
        m_retryScheduled = false;
    }
}

void DynregObject::onRegistrationResult(bool success, int statusCode, const QString &deviceSecret)
{
    Q_UNUSED(statusCode);
    Q_UNUSED(deviceSecret);
    m_isRunning = false;
    if (!success) {
        qDebug() << "Registration failed, retry in 5 seconds...";
        m_retryScheduled = true;
        QTimer::singleShot(5000, this, &DynregObject::doRetry);
    } else {
        qDebug() << "Registration succeeded, device secret:" << deviceSecret;
        m_retryScheduled = false;
    }
}

void DynregObject::doRetry()
{
    if (m_retryScheduled) {
        m_retryScheduled = false;
        startRegistration();
    }
}

#include "DynregObject.moc"
