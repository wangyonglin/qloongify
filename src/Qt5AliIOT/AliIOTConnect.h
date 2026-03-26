#ifndef ALIIOTCONNECT_H
#define ALIIOTCONNECT_H

#include "AliIOTSettings.h"
#include "DynregObject.h"
#include "AliIOTClient.h"

class AliIOTConnect : public AliIOTSettings
{
    Q_OBJECT
public:
    explicit AliIOTConnect(QObject *parent = nullptr);
    void startConnect();
    void stopConnect();
public slots:
    void slotRegistrationResult(bool success, int statusCode, const QString &deviceSecret);
    void slotMessageReceived(const QString &topic, const QByteArray &payload);
private:
    QSettings * final;
    AliIOTClient *m_aliiot_client ;
    DynregObject *dynregObject=nullptr;
};

#endif // ALIIOTCONNECT_H
