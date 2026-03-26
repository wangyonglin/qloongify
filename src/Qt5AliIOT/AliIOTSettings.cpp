#include "AliIOTSettings.h"
#include <QDebug>

AliIOTSettings::AliIOTSettings(QObject *parent)
    : QObject{parent},
    m_settings(nullptr),
    m_groupName("ALIIOT")
{}
void AliIOTSettings::initSettings(const QString &organization, const QString &application)
{
    m_settings = new QSettings(QSettings::NativeFormat, QSettings::UserScope, organization, application);
    m_settings->beginGroup(m_groupName);
    m_productKey=m_settings->value("productKey", "").toString();
    m_productSecret=m_settings->value("productSecret","").toString();
    m_deviceName=m_settings->value("deviceName", "").toString();
    m_deviceSecret=m_settings->value("deviceSecret", "").toString();
    m_mqttHost=m_settings->value("mqttHost", "").toString();
    m_mqttPort=m_settings->value("mqttPort", 8883).toInt();
    m_settings->endGroup();
}

QString AliIOTSettings::productKey()
{
    return  m_productKey;
}

QString AliIOTSettings::productSecret()
{
    return m_productSecret;
}

QString AliIOTSettings::deviceName()
{
    return m_deviceName;
}

QString AliIOTSettings::deviceSecret()
{
    return m_deviceSecret;
}

QString AliIOTSettings::mqttHost()
{
    return m_mqttHost;
}

QStringList AliIOTSettings::allKeys()
{
    if(m_settings){
      return  m_settings->allKeys();
    }
    return QStringList();
}
void AliIOTSettings::printSettings() {
    // 进入指定分组（如果 m_groupName 非空）
    if (!m_groupName.isEmpty()) {
        m_settings->beginGroup(m_groupName);
    }

    // 获取当前分组下的所有键（包括子分组中的键，路径相对于当前分组）
    QStringList keys = m_settings->allKeys();
    for (const QString &key : keys) {
        QVariant value = m_settings->value(key);
        qDebug() << key << ":" << value;
    }

    // 退出分组，恢复之前的状态（仅当之前进入过分组时）
    if (!m_groupName.isEmpty()) {
        m_settings->endGroup();
    }
}

int AliIOTSettings::mqttPort()
{
    return m_mqttPort;
}

void AliIOTSettings::setProductKey(const QString &newProductKey)
{
    QMutexLocker locker(&m_mutex);
    m_productKey = newProductKey;
    if (m_settings){
        m_settings->beginGroup(m_groupName);
        m_settings->setValue("productKey", m_productKey);
        m_settings->endGroup();
    }
}

void AliIOTSettings::setProductSecret(const QString &newProductSecret)
{
    QMutexLocker locker(&m_mutex);
    m_productSecret = newProductSecret;
    if (m_settings){
        m_settings->beginGroup(m_groupName);
        m_settings->setValue("productSecre", m_productSecret);
        m_settings->endGroup();
    }
}

void AliIOTSettings::setDeviceName(const QString &newDeviceName)
{
    QMutexLocker locker(&m_mutex);
    m_deviceName = newDeviceName;
    if (m_settings){
        m_settings->beginGroup(m_groupName);
        m_settings->setValue("deviceName", m_deviceName);
        m_settings->endGroup();
    }
}

void AliIOTSettings::setDeviceSecret(const QString &newDeviceSecret)
{
    QMutexLocker locker(&m_mutex);
    m_deviceSecret = newDeviceSecret;
    if (m_settings){
        m_settings->beginGroup(m_groupName);
        m_settings->setValue("deviceSecret", m_deviceSecret);
        m_settings->endGroup();
    }
}

void AliIOTSettings::setMqttHost(const QString &newMqttHost)
{
    QMutexLocker locker(&m_mutex);
    m_mqttHost = newMqttHost;
    if (m_settings){
        m_settings->beginGroup(m_groupName);
        m_settings->setValue("mqttHost", m_mqttHost);
        m_settings->endGroup();
    }
}

void AliIOTSettings::setMqttPort(int newMqttPort)
{
    QMutexLocker locker(&m_mutex);
    m_mqttPort = newMqttPort;
    if (m_settings){
        m_settings->beginGroup(m_groupName);
        m_settings->setValue("mqttPort", m_mqttPort);
        m_settings->endGroup();
    }
}

