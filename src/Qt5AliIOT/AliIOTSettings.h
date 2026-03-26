#ifndef ALIIOTSETTINGS_H
#define ALIIOTSETTINGS_H

#include <QSettings>
#include <QMutex>

class AliIOTSettings : public QObject
{
    Q_OBJECT
public:
    explicit AliIOTSettings(QObject *parent = nullptr);
    void initSettings(const QString &organization, const QString &application);
    QString productKey();
    QString productSecret();
    QString deviceName();
    QString deviceSecret();
    QString mqttHost();
    QStringList allKeys();
    int mqttPort();

    void setProductKey(const QString &newProductKey);

    void setProductSecret(const QString &newProductSecret);

    void setDeviceName(const QString &newDeviceName);

    void setDeviceSecret(const QString &newDeviceSecret);

    void setMqttHost(const QString &newMqttHost);

    void setMqttPort(int newMqttPort);

    void printSettings();
private:
    QString m_groupName;
    QMutex m_mutex;
    QSettings *m_settings=nullptr;
    QString m_productKey;
    QString m_productSecret;
    QString m_deviceName;
    QString m_deviceSecret;
    QString m_mqttHost;
    int m_mqttPort;
};

#endif // ALIIOTSETTINGS_H
