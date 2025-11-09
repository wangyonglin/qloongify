#include "qkeywordspotting.h"
#include <QAudioFormat>
#include <QDebug>

QKeywordSpotting::QKeywordSpotting(QObject *parent,const QAudioFormat &format)
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



void QKeywordSpotting::initialize(const QKeywordConfig &config){
    // Zipformer config
    memset(&zipformer_config, 0x00, sizeof(zipformer_config));
    zipformer_config.encoder = config.zipformerEncoder().toUtf8();
    zipformer_config.decoder = config.zipformerDecoder().toUtf8();
    zipformer_config.joiner = config.zipformerJoiner().toUtf8();

    // Online model config
    memset(&online_model_config, 0x00, sizeof(online_model_config));
    online_model_config.debug = config.onlineModelDebug();
    online_model_config.num_threads = config.onlineModelThreads();
    online_model_config.provider = config.onlineModelProvider().toUtf8();
    online_model_config.model_type=config.onlineModelType().toUtf8();
    online_model_config.tokens=config.onlineModelTokens().toUtf8();
    online_model_config.transducer = zipformer_config;

    // Keywords-spotter config
    memset(&keywords_spotter_config, 0x00, sizeof(keywords_spotter_config));
    keywords_spotter_config.max_active_paths = config.keywordsSpotterMaxActivePaths();
    keywords_spotter_config.keywords_threshold = config.keywordsSpotterKeywordsThreshold();
    keywords_spotter_config.keywords_score = config.keywordsSpotterKeywordsScore();
    keywords_spotter_config.model_config = online_model_config;
    keywords_spotter_config.keywords_file=config.keywordsSpotterKeywordsFile().toUtf8();
    keywords_spotter_config.feat_config.sample_rate=config.keywordsSpotterFeatSampleRate();
    keywords_spotter_config.feat_config.feature_dim=config.keywordsSpotterFeatFeatureDim();
    keywords_spotter= SherpaOnnxCreateKeywordSpotter(&keywords_spotter_config);
    stream=SherpaOnnxCreateKeywordStream(keywords_spotter);
    //stream=SherpaOnnxCreateKeywordStreamWithKeywords(keywords_spotter,"y ǎn y uán @演员");
}
void QKeywordSpotting::cleanup(){

}



void QKeywordSpotting::write(const QByteArray & bytes){
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

std::vector<float> QKeywordSpotting::convertFloatVector(const QByteArray &byteArray) {
    std::vector<float> floatVector;
    if (byteArray.size() % sizeof(float) == 0) {
        const float *data = reinterpret_cast<const float*>(byteArray.constData());
        size_t dataSize = byteArray.size() / sizeof(float);
        floatVector.assign(data, data + dataSize);
    }
    return floatVector;
}
