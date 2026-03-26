#ifndef DYNREGOBJECT_H
#define DYNREGOBJECT_H

#include <QObject>
#include <QString>

class DynregObjectPrivate;

class DynregObject : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DynregObject)
public:
    explicit DynregObject(QObject *parent = nullptr);
    ~DynregObject();

    // 配置动态注册参数
    void setProductKey(const QString &productKey);
    void setDeviceName(const QString &deviceName);
    void setProductSecret(const QString &productSecret);
    void setHost(const QString &host);              // 默认 "iot-auth.cn-shanghai.aliyuncs.com"
    void setPort(uint16_t port);                    // 默认 443

    // 启动注册流程（内部自动处理重试）
    void startRegistration();

    // 停止重试（若正在等待重试则取消）
    void stopRetry();

signals:
    // 注册完成信号
    // success: true 表示成功，false 表示失败
    // statusCode: 服务器返回的状态码（成功时为 200）
    // deviceSecret: 成功时返回的 DeviceSecret
    void registrationResult(bool success, int statusCode, const QString &deviceSecret);

private slots:
    void onRegistrationResult(bool success, int statusCode, const QString &deviceSecret);
    void doRetry();

private:
    QScopedPointer<DynregObjectPrivate> d_ptr;
    bool m_isRunning;       // 是否正在注册中
    bool m_retryScheduled;  // 是否已安排重试
};

#endif // DYNREGOBJECT_H
