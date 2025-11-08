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
    qkeywordspotting.h \
    qkeywordspottingcontroller.h \
    qsherpaonnx_global.h \
    qsherpaonnxthreader.h \
    qsoundrecorder.h \
    qsoundspeaker.h

SOURCES += \
    qkeywordspotting.cpp \
    qkeywordspottingcontroller.cpp \
    qsherpaonnxthreader.cpp \
    qsoundrecorder.cpp \
    qsoundspeaker.cpp

