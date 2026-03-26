#include "AliIOTConnect.h"
#include <QDebug>

AliIOTConnect::AliIOTConnect(QObject *parent)
    : AliIOTSettings{parent}
{
    this->initSettings("wangyonglin","qloongify");


    m_aliiot_client = new AliIOTClient(this);
   connect(m_aliiot_client, &AliIOTClient::connected, [&](){
        qDebug() << "Connected to Aliyun IoT!";
        m_aliiot_client->subscribe(AliIOTClient::topicFullName(productKey(),deviceName(),"/user/get"),0);
    });
    connect(m_aliiot_client, &AliIOTClient::disconnected, [](){
        qDebug() << "Disconnected from Aliyun IoT";
    });
    connect(m_aliiot_client, &AliIOTClient::messageReceived,this,&AliIOTConnect::slotMessageReceived);
}

void AliIOTConnect::startConnect()
{
    m_aliiot_client->setProductKey(productKey());
    m_aliiot_client->setDeviceName(deviceName());
    m_aliiot_client->setDeviceSecret(deviceSecret());
    m_aliiot_client->setMqttHost(mqttHost());
    m_aliiot_client->setMqttPort(mqttPort());
    m_aliiot_client->connectToHost();
}

void AliIOTConnect::stopConnect()
{
    m_aliiot_client->disconnectFromHost();
}

void AliIOTConnect::slotRegistrationResult(bool success, int statusCode, const QString &deviceSecret)
{
    if (success) {
        qDebug() << "动态注册成功";
        qDebug() << "状态码:" << statusCode;
        qDebug() << "DeviceSecret:" << deviceSecret;
    } else {
        qDebug() << "动态注册失败，状态码:" << statusCode;
    }
}

void AliIOTConnect::slotMessageReceived(const QString &topic, const QByteArray &payload)
{
       qDebug() << "Message received on" << topic << ":" << payload;
}





