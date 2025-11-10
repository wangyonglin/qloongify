#
TARGET = Qt5SherpaOnnx
TEMPLATE = lib
QT += core opengl multimedia
DEFINES += QT_SHERPAONNX_LIBRARY

# C++ 标准设置
CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17 -fPIC
# QMAKE_CXXFLAGS += -D_GLIBCXX_USE_CXX11_ABI=1

INCLUDEPATH += $$PWD/../../usr/include

LIBS += -L$$PWD/../../usr/lib
LIBS += -Wl,--start-group
LIBS += -lsherpa-onnx-cxx-api
LIBS += -lsherpa-onnx-core
LIBS += -lsherpa-onnx-c-api
LIBS += -lsherpa-onnx-kaldifst-core
LIBS += -lsherpa-onnx-fst
LIBS += -lsherpa-onnx-fstfar
LIBS += -lkaldi-decoder-core
LIBS += -lkaldi-native-fbank-core
LIBS += -lcargs
LIBS += -lcppinyin_core
LIBS += -lpiper_phonemize
LIBS += -lespeak-ng
LIBS += -lssentencepiece_core
LIBS += -lucd
LIBS += -lkissfft-float
LIBS += -lonnxruntime
LIBS += -lsherpa-onnx-portaudio_static
LIBS += -lstdc++ -lm -lpthread -ldl
LIBS += -Wl,--end-group

HEADERS += \
    qkeywordspotting.h \
    qkwstokens.h \
    qsherpabuilder.h \
    qsherpaonnx_global.h \
    qsoundrecorder.h \
    qsoundspeaker.h

SOURCES += \
    qkeywordspotting.cpp \
    qsherpabuilder.cpp \
    qsoundrecorder.cpp \
    qsoundspeaker.cpp

# Default rules for deployment.
qnx: target.path = /tmp/lib
else: unix:!android: target.path = /usr/lib
!isEmpty(target.path): INSTALLS += target
