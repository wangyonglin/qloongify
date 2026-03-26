TARGET = Qt5AliIOT
TEMPLATE = lib
QT += core opengl multimedia widgets
# DEFINES += QT_SHERPAONNX_LIBRARY

# C++ 标准设置
CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17


INCLUDEPATH += /home/wangyonglin/github/qloongify/usr/include

INCLUDEPATH += $$PWD/../../usr/include
LIBS += -L $$PWD/../usr/lib

LIBS += -Wl,--start-group
LIBS += -Wl,--end-group

# Default rules for deployment.
qnx: target.path = /tmp/lib
else: unix:!android: target.path = /usr/lib
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    AliIOTClient.h \
    AliIOTConnect.h \
    AliIOTSettings.h \
    DynregObject.h

SOURCES += \
    AliIOTClient.cpp \
    AliIOTConnect.cpp \
    AliIOTSettings.cpp \
    DynregObject.cpp



