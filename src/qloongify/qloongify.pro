QT       += core gui opengl widgets multimedia

# In Qt6 to use QOpenglWidget, we need add QT += openglwidgets.
greaterThan(QT_MAJOR_VERSION, 5){
    message(Building with Qt6 or Higher)
    QT += openglwidgets
}

CONFIG += c++11
TARGET = qloongify
TEMPLATE = app
DEFINES += QT5SHERPAONNX_LIBRARY


INCLUDEPATH +=$$PWD/../qsherpaonnx/include
INCLUDEPATH += $$PWD/../../usr/include
LIBS +=-L$$OUT_PWD/../qsherpaonnx -lQt5SherpaOnnx

SOURCES += \
    main.cpp \
    qloongifywidget.cpp

# Default rules for deployment.
qnx: target.path = /com/wangyonglin/qloongify/tmp/bin
else: unix:!android: target.path = /com/wangyonglin/qloongify/usr/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    qloongifywidget.h
