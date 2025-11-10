#ifndef QKEYWORDSPOTTING_H
#define QKEYWORDSPOTTING_H

#include <QObject>
#include "qsherpaonnx_global.h"
#include "sherpa-onnx/c-api/c-api.h"
#include <QAudioFormat>
#include <QIODevice>
#include <QBuffer>
#include <QThread>
#include "qsoundspeaker.h"
#include "qsherpabuilder.h"

class QT_SHERPAONNX_EXPORT QKeywordSpotting : public QSherpaBuilder
{
    Q_OBJECT
public:
    explicit QKeywordSpotting(QObject *parent = nullptr,const QAudioFormat &format = QAudioFormat());
    ~QKeywordSpotting();
    void start();
    void stop();

public slots:
    void slotReadSound(const QByteArray &bytes);
private:
    void initialize();
    void cleanup();
    SherpaOnnxOnlineTransducerModelConfig zipformer_config;
    SherpaOnnxOnlineModelConfig online_model_config;
    SherpaOnnxKeywordSpotterConfig keywords_spotter_config;
    const SherpaOnnxKeywordSpotter *keywords_spotter= nullptr;
    const SherpaOnnxOnlineStream *stream= nullptr;
signals:
    void refresh(const QString & keywrod);
    // QKWSThreader interface
protected:
    void loopRunnable();
};

#endif // QKEYWORDSPOTTING_H
