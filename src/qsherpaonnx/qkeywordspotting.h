#ifndef QKEYWORDSPOTTING_H
#define QKEYWORDSPOTTING_H

#include <QObject>
#include "qsherpaonnx_global.h"
#include "sherpa-onnx/c-api/c-api.h"
#include <QAudioFormat>

class QT5SHERPAONNX_EXPORT QKeywordSpotting : public QObject
{
    Q_OBJECT
public:
    explicit QKeywordSpotting(QObject *parent = nullptr);
    ~QKeywordSpotting();
    void loadKeywordSpotter(QString keywords_file);
    void loadOnlinemodel(int debug, int num_threads,QString provider,QString tokens);
    void loadZipformer(QString encoder,QString decoder,QString joiner);
public slots:
    void readSpotter(const QByteArray &bytes);
private:
    std::vector<float> convertFloatVector(const QByteArray &byteArray);

    int32_t channel_count=1;
    int32_t sample_rate;
    int32_t feature_dim=80;
    SherpaOnnxOnlineTransducerModelConfig zipformer_config;
    SherpaOnnxOnlineModelConfig online_model_config;
    SherpaOnnxKeywordSpotterConfig keywords_spotter_config;
    SherpaOnnxKeywordSpotter *keywords_spotter;
    SherpaOnnxOnlineStream *stream;

signals:
    void refresh(const QString & keywrod);
};

#endif // QKEYWORDSPOTTING_H
