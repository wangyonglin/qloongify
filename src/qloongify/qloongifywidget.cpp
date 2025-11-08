#include "qloongifywidget.h"
#include <QDebug>

QLoongifyWidget::QLoongifyWidget(QWidget *parent)
    : QWidget{parent}
{
    qvboxlayout = new QVBoxLayout(this);
    qlabel = new QLabel("RK3588 Qt EGLFS KMS&DRM Test - Success!");
    qlabel->setAlignment(Qt::AlignCenter);
    qlabel->setStyleSheet("QLabel { background-color: blue; color: white; font-size: 24px; }");
    qvboxlayout->addWidget(qlabel);

    QAudioFormat qaudioformat;
    qaudioformat.setSampleRate(44100);       // 采样率 44.1kHz 44100
    qaudioformat.setChannelCount(2);         // 单声道
    qaudioformat.setSampleSize(16);          // 样本大小 16位
    qaudioformat.setCodec("audio/pcm");      // 编码格式 PCM
    qaudioformat.setByteOrder(QAudioFormat::LittleEndian); // 字节序 小端
    qaudioformat.setSampleType(QAudioFormat::SignedInt);   // 样本类型 有符号整数

    qsoundrecorder = new QSoundRecorder(this,qaudioformat);
    qkeywordspotting=new QKeywordSpotting(this,qaudioformat);

    connect(qsoundrecorder, &QSoundRecorder::readyRead,[&](){
        QByteArray data=qsoundrecorder->readAll();
        qkeywordspotting->write(data);
    });

    qsoundrecorder->start();
    qkeywordspotting->start();
}

QLoongifyWidget::~QLoongifyWidget()
{
    qsoundspeaker->stop();
    qkeywordspotting->stop();
}
