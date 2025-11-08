#ifndef QKEYWORDSPOTTINGCONTROLLER_H
#define QKEYWORDSPOTTINGCONTROLLER_H

#include <QObject>
#include "qsherpaonnxthreader.h"
#include "qsoundrecorder.h"
#include "qkeywordspotting.h"
class QKeywordSpottingController : public QSherpaOnnxThreader
{
    Q_OBJECT
public:
    explicit QKeywordSpottingController(QObject *parent = nullptr);
    ~QKeywordSpottingController();
    void start(Priority pri = InheritPriority);
    void stop();
    QSoundRecorder *recorder= nullptr;
    QKeywordSpotting *spotting= nullptr;
    QString encoder="/com/wangyonglin/qloongify/usr/etc/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/encoder-epoch-12-avg-2-chunk-16-left-64.int8.onnx";
    QString decoder="/com/wangyonglin/qloongify/usr/etc/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/decoder-epoch-12-avg-2-chunk-16-left-64.onnx";
    QString joiner="/com/wangyonglin/qloongify/usr/etc/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/joiner-epoch-12-avg-2-chunk-16-left-64.int8.onnx";
    QString tokens="/com/wangyonglin/qloongify/usr/etc/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/tokens.txt";
    QString keywords="/com/wangyonglin/qloongify/usr/etc/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/test_wavs/test_keywords.txt";
signals:

    // QSherpaOnnxThreader interface
protected:
    void loopRunnable();
public slots:
     void slotRecoreder(const QByteArray &bytes);
};

#endif // QKEYWORDSPOTTINGCONTROLLER_H
