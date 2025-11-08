#include "qkeywordspotting.h"
#include <QAudioFormat>
#include <QDebug>

QKeywordSpotting::QKeywordSpotting(QObject *parent)
    : QObject{parent},feature_dim(80),sample_rate(16000),channel_count(1)
{

}

QKeywordSpotting::~QKeywordSpotting()
{
    if(stream && keywords_spotter){
       SherpaOnnxDestroyOnlineStream(stream);
         SherpaOnnxDestroyKeywordSpotter(keywords_spotter);
    }
}

void QKeywordSpotting::loadZipformer(QString encoder,
                                        QString decoder,
                                        QString joiner)
{
    // Zipformer config
    memset(&zipformer_config, 0x00, sizeof(zipformer_config));
    zipformer_config.encoder = encoder.toLocal8Bit();
    zipformer_config.decoder = decoder.toLocal8Bit();
    zipformer_config.joiner = joiner.toLocal8Bit();

}

void QKeywordSpotting::loadOnlinemodel(int debug,
                                       int num_threads,
                                        QString provider,
                                        QString tokens)
{
    // Online model config
    memset(&online_model_config, 0x00, sizeof(online_model_config));
    online_model_config.debug = 1;
    online_model_config.num_threads = 1;
    online_model_config.provider = "cpu";
    online_model_config.model_type="zipformer";
    online_model_config.tokens=tokens.toLocal8Bit();
    online_model_config.transducer = zipformer_config;
}

void QKeywordSpotting::loadKeywordSpotter(QString keywords_file)
{
    // Keywords-spotter config
    memset(&keywords_spotter_config, 0x00, sizeof(keywords_spotter_config));
    keywords_spotter_config.max_active_paths = 4;
    keywords_spotter_config.keywords_threshold = 0.25;
    keywords_spotter_config.keywords_score = 1.0;
    keywords_spotter_config.model_config = online_model_config;
    keywords_spotter_config.keywords_file=keywords_file.toLocal8Bit();
    keywords_spotter_config.feat_config.sample_rate=sample_rate;
    keywords_spotter_config.feat_config.feature_dim=feature_dim;
    keywords_spotter= SherpaOnnxCreateKeywordSpotter(&keywords_spotter_config);
    stream=SherpaOnnxCreateKeywordStream(keywords_spotter);
   //stream=SherpaOnnxCreateKeywordStreamWithKeywords(keywords_spotter,"y ǎn y uán @演员");
}



std::vector<float> QKeywordSpotting::convertFloatVector(const QByteArray &byteArray) {
    std::vector<float> floatVector;
    if (byteArray.size() % sizeof(float) == 0) {
        const float *data = reinterpret_cast<const float*>(byteArray.constData());
        size_t dataSize = byteArray.size() / sizeof(float);
        floatVector.assign(data, data + dataSize);
    }
    return floatVector;
}
void QKeywordSpotting::readSpotter(const QByteArray & bytes){
    std::vector<float> streamfloat= convertFloatVector(bytes);
    qDebug() << "streamfloat.size:" <<streamfloat.size();
    SherpaOnnxOnlineStreamAcceptWaveform(stream,sample_rate, streamfloat.data(), streamfloat.size());
    while (SherpaOnnxIsKeywordStreamReady(keywords_spotter, stream)) {
        SherpaOnnxDecodeKeywordStream(keywords_spotter, stream);
    }
    const SherpaOnnxKeywordResult *r = SherpaOnnxGetKeywordResult(keywords_spotter, stream);
    if (strlen(r->keyword)) {
        qDebug() << "spotter:" << r->json;
        emit refresh(QString::fromStdString(r->keyword));
    }

    SherpaOnnxDestroyKeywordResult(r);
}
