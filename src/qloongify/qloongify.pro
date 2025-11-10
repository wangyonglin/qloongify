TARGET = qloongify
TEMPLATE = app
QT       += core gui opengl widgets multimedia

# In Qt6 to use QOpenglWidget, we need add QT += openglwidgets.
greaterThan(QT_MAJOR_VERSION, 5){
    message(Building with Qt6 or Higher)
    QT += openglwidgets
}

DEFINES += QT_SHERPAONNX_LIBRARY

CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17 -fPIC
# QMAKE_CXXFLAGS += -D_GLIBCXX_USE_CXX11_ABI=1

INCLUDEPATH +=$$PWD/../qsherpaonnx/include
INCLUDEPATH += $$PWD/../../usr/include
LIBS +=-L$$OUT_PWD/../qsherpaonnx -lQt5SherpaOnnx

SOURCES += \
    main.cpp \
    qloongifywidget.cpp

HEADERS += \
    qloongifywidget.h


# Default rules for deployment.
qnx: target.path = /tmp/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../../README.md
