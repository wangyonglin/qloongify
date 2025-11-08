#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QOpenGLWidget>
#include <QTimer>
#include <QDebug>

#include "qloongifywidget.h"

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "eglfs");
    qputenv("QT_QPA_EGLFS_INTEGRATION", "eglfs_kms");
    QApplication app(argc, argv);
    QLoongifyWidget qloongify;
    qloongify.showFullScreen();
    return app.exec();
}
