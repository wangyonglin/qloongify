#include <QApplication>
#include <QDebug>
#include <QDateTime>
#include <QTimer>
#include <istream>
#include <QGuiApplication>
#include <QScreen>
#include "QloongifyController.h"

#include <Qt5AliIOT/AliIOTSettings>

// 基本遍历方式

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // 获取屏幕尺寸
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    // 设备发布消息Topic：/a1uzcH0****/${deviceName}/user/devmsg，权限为发布。

    // 设备订阅消息Topic：/a1uzcH0****/${deviceName}/user/cloudmsg，权限为订阅。
    QloongifyController loongify;
    loongify.resize(screenGeometry.size());
    loongify.showFullScreen();

    return app.exec();
}
