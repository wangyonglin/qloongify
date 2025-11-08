#ifndef QKEYWORDCONFIG_H
#define QKEYWORDCONFIG_H

#include <QObject>
#include "qsherpaonnx_global.h"

class QT5SHERPAONNX_EXPORT QKeywordConfig
{

public:
    QKeywordConfig();
    ~QKeywordConfig();


    QString zipformer_encoder;
    QString zipformer_decoder;
    QString zipformer_joiner;
    int online_model_debug = 1;
    int online_model_threads = 1;
    QString online_model_provider= "cpu";
    QString online_model_type="zipformer";
    QString online_model_tokens;
    int keywords_spotter_max_active_paths = 4;
    int keywords_spotter_keywords_threshold = 0.25;
    int keywords_spotter_keywords_score = 1.0;
    QString keywords_spotter_keywords_file;
    int keywords_spotter_feat_sample_rate=16000;
    int keywords_spotter_feat_feature_dim=80;
};

#endif // QKEYWORDCONFIG_H
