TARGET = qloongify
TEMPLATE = app
# QT       += core gui opengl widgets multimedia
QT       += core gui multimedia opengl widgets

# greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# # In Qt6 to use QOpenglWidget, we need add QT += openglwidgets.
# greaterThan(QT_MAJOR_VERSION, 5){
#     message(Building with Qt6 or Higher)
#     QT += openglwidgets
# }


DEFINES += QMAINIFY_LIBRARY
CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17 -fPIC
# DEFINES += QT_NO_DEBUG_OUTPUT
# QMAKE_CXXFLAGS += -D_GLIBCXX_USE_CXX11_ABI=1



INCLUDEPATH +=$$PWD/../Qt5FFmpeg/include
LIBS +=-L$$OUT_PWD/../Qt5FFmpeg -lQt5FFmpeg -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lswresample -lswscale


# OpenGL 配置
LIBS += -lGLESv2

# 发布版本优化
CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
    QMAKE_CXXFLAGS += -O2
}

# 调试版本
CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS += -g
}




# Default rules for deployment.
qnx: target.path = /tmp/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    QMailloader.h

SOURCES += \
    QMailloader.cpp \
    main.cpp
