#include "QloongifyController.h"
#include <QKeyEvent>
#include <QResizeEvent>
#include <QSettings>

QloongifyController::QloongifyController(QWidget *parent)
    : QWidget{parent}
    {
    m_aliiot_connect = new AliIOTConnect(this);
    m_aliiot_connect->setProductKey("a1rn3JWJD0y");
    m_aliiot_connect->setProductSecret("xPHebYHk7YFPIRtY");
    m_aliiot_connect->setDeviceSecret("3f1c35d218d52a273e2f48b588b1e496");
    m_aliiot_connect->setDeviceName(QSysInfo::machineUniqueId());
    m_aliiot_connect->printSettings();
    m_aliiot_connect->startConnect();

}
 //ffmpeg->play("/usr/etc/video/trailer.mp4");
void QloongifyController::keyPressEvent(QKeyEvent *event)
{
    // 特殊处理
    if (event->key() == Qt::Key_F1)
    {
        qDebug() << "Key_F1 pressed - taking screenshot";
    }else if (event->key() == Qt::Key_F2)
    {
        qDebug() << "Key_F2 pressed - taking screenshot";
    }
}


void QloongifyController::resizeEvent(QResizeEvent *event)
{


}
