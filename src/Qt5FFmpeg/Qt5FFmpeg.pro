TARGET = Qt5FFmpeg
TEMPLATE = lib
QT += core opengl multimedia widgets
# DEFINES += QT_SHERPAONNX_LIBRARY

# C++ 标准设置
CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17
INCLUDEPATH += $$PWD/../usr/include
LIBS += -L$$PWD/../usr/lib
LIBS += -Wl,--start-group
LIBS += -Wl,--end-group
HEADERS += \
    AudioController.h \
    Qt5FFmpegContext.h \
    Qt5FFmpegController.h \
    Qt5FFmpegDecoder.h \
    Qt5FFmpegDemuxer.h \
    Qt5FFmpegPlayer.h \
    Qt5FFmpegRender.h \
    Qt5FFmpegSync.h \
    Qt5FFmpegUtils.h \
    Qt5FrameObject.h \
    Qt5FrameQueue.h \
    Qt5FrameSync.h \
    Qt5FrameUtils.h \
    Qt5PacketObject.h \
    Qt5ThreadObject.h \
    VideoController.h \
    include/Qt5FFmpegPlayer
SOURCES += \
    AudioController.cpp \
    Qt5FFmpegContext.cpp \
    Qt5FFmpegController.cpp \
    Qt5FFmpegDecoder.cpp \
    Qt5FFmpegDemuxer.cpp \
    Qt5FFmpegPlayer.cpp \
    Qt5FFmpegRender.cpp \
    Qt5FFmpegSync.cpp \
    Qt5FFmpegUtils.cpp \
    Qt5FrameObject.cpp \
    Qt5FrameQueue.cpp \
    Qt5FrameSync.cpp \
    Qt5FrameUtils.cpp \
    Qt5PacketObject.cpp \
    Qt5ThreadObject.cpp \
    VideoController.cpp
# Default rules for deployment.
qnx: target.path = /tmp/lib
else: unix:!android: target.path = /usr/lib
!isEmpty(target.path): INSTALLS += target
