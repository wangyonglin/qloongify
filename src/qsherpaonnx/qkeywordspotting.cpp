#include "qkeywordspotting.h"
#include "qkwstokens.h"
#include <QAudioFormat>
#include <QDebug>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
QKeywordSpotting::QKeywordSpotting(QObject *parent,const QAudioFormat &format)
    : QSherpaBuilder{parent,format}
{



}

QKeywordSpotting::~QKeywordSpotting()
{
    stop();

}

void QKeywordSpotting::start()
{
    initialize();
    keywords_spotter =SherpaOnnxCreateKeywordSpotter(&keywords_spotter_config);
    stream=SherpaOnnxCreateKeywordStream(keywords_spotter);
    //stream=SherpaOnnxCreateKeywordStreamWithKeywords(keywords_spotter,"y ǎn y uán @演员");

}

void QKeywordSpotting::stop()
{
    cleanup();
}


void QKeywordSpotting::slotReadSound(const QByteArray & bytes){

    if(!bytes.isEmpty()){
        std::vector<float> streamfloat= convertFloatVector(bytes);
        SherpaOnnxOnlineStreamAcceptWaveform(stream, keywords_spotter_config.feat_config.sample_rate, streamfloat.data(), streamfloat.size());
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
}

void QKeywordSpotting::initialize()
{
    const char *encoder_filename ="/usr/etc/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/encoder-epoch-12-avg-2-chunk-16-left-64.int8.onnx";
    const char *decoder_filename="/usr/etc/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/decoder-epoch-12-avg-2-chunk-16-left-64.onnx";
    const char *joiner_filename ="/usr/etc/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/joiner-epoch-12-avg-2-chunk-16-left-64.int8.onnx";
    const char *tokens_filename ="/usr/etc/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/tokens.txt";
    const char *keywords_filename ="/usr/etc/sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/keywords.txt";
    memset(&zipformer_config, 0, sizeof(zipformer_config));
    zipformer_config.encoder = encoder_filename;
    zipformer_config.decoder = decoder_filename;
    zipformer_config.joiner = joiner_filename;

    memset(&online_model_config,0, sizeof(online_model_config));
    online_model_config.debug = 1;
    online_model_config.num_threads = 1;
    online_model_config.provider = "cpu";
    online_model_config.tokens=tokens_filename;
    online_model_config.transducer = zipformer_config;

    memset(&keywords_spotter_config, 0, sizeof(keywords_spotter_config));
    keywords_spotter_config.max_active_paths = 4;
    keywords_spotter_config.keywords_threshold = 0.25;
    keywords_spotter_config.keywords_score = 1.0;
    keywords_spotter_config.feat_config.sample_rate=getAudioFormat().sampleRate();
    keywords_spotter_config.feat_config.feature_dim=80;
    keywords_spotter_config.keywords_file=keywords_filename;
    keywords_spotter_config.model_config=online_model_config;
}

void QKeywordSpotting::cleanup()
{
    if(stream && keywords_spotter){
        SherpaOnnxDestroyOnlineStream(stream);
        SherpaOnnxDestroyKeywordSpotter(keywords_spotter);
    }
}



