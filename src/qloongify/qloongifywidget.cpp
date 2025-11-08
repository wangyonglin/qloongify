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
    qaudioformat.setSampleRate(16000);       // 采样率 44.1kHz 44100
    qaudioformat.setChannelCount(1);         // 单声道
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
    QKeywordConfig config;
    config.zipformer_encoder="/com/wangyonglin/qloongify/usr/etc/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/encoder-epoch-12-avg-2-chunk-16-left-64.onnx";
    config.zipformer_decoder="/com/wangyonglin/qloongify/usr/etc/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/decoder-epoch-12-avg-2-chunk-16-left-64.onnx";
    config.zipformer_joiner="/com/wangyonglin/qloongify/usr/etc/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/joiner-epoch-12-avg-2-chunk-16-left-64.int8.onnx";
    config.online_model_tokens="/com/wangyonglin/qloongify/usr/etc/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/tokens.txt";
    config.keywords_spotter_keywords_file ="/com/wangyonglin/qloongify/usr/etc/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/test_wavs/test_keywords.txt";
    config.keywords_spotter_feat_sample_rate=16000;

    qkeywordspotting->initialize(config);
}

QLoongifyWidget::~QLoongifyWidget()
{
    qsoundspeaker->stop();
    qkeywordspotting->cleanup();
}
