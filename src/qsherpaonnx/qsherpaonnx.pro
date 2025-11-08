#
TARGET = Qt5SherpaOnnx
TEMPLATE = lib
QT       += core opengl multimedia
DEFINES += QT5SHERPAONNX_LIBRARY

# Default rules for deployment.
qnx: target.path = /com/wangyonglin/qloongify/tmp/lib
else: unix:!android: target.path = /usr/lib
!isEmpty(target.path): INSTALLS += target

message(The root directory is: $$PWD)
INCLUDEPATH += $$PWD/../../usr/include
LIBS += -L$$PWD/../../usr/lib \
        -lsherpa-onnx-c-api \
        -lsherpa-onnx-core \
        -lkaldi-native-fbank-core \
        -lkaldi-decoder-core \
        -lsherpa-onnx-fst \
        -lsherpa-onnx-fstfar \
        -lsherpa-onnx-kaldifst-core \
        -lssentencepiece_core \
        -lonnxruntime \
        -lespeak-ng \
        -lucd \
        -lpiper_phonemize \
        -lcargs

HEADERS += \
    audiohandler.h \
    qkeywordconfig.h \
    qkeywordspotting.h \
    qsherpaonnx_global.h \
    qsoundrecorder.h \
    qsoundspeaker.h

SOURCES += \
    audiohandler.cpp \
    qkeywordconfig.cpp \
    qkeywordspotting.cpp \
    qsoundrecorder.cpp \
    qsoundspeaker.cpp

