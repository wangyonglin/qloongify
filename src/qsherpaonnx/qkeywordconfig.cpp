#include "qkeywordconfig.h"


QKeywordConfig::QKeywordConfig(QObject *parent)
    : QObject(parent){}
QKeywordConfig::~QKeywordConfig(){}

// Getters implementation
QString QKeywordConfig::zipformerEncoder() const { return m_zipformerEncoder; }
QString QKeywordConfig::zipformerDecoder() const { return m_zipformerDecoder; }
QString QKeywordConfig::zipformerJoiner() const { return m_zipformerJoiner; }
int QKeywordConfig::onlineModelDebug() const { return m_onlineModelDebug; }
int QKeywordConfig::onlineModelThreads() const { return m_onlineModelThreads; }
QString QKeywordConfig::onlineModelProvider() const { return m_onlineModelProvider; }
QString QKeywordConfig::onlineModelType() const { return m_onlineModelType; }
QString QKeywordConfig::onlineModelTokens() const { return m_onlineModelTokens; }
int QKeywordConfig::keywordsSpotterMaxActivePaths() const { return m_keywordsSpotterMaxActivePaths; }
int QKeywordConfig::keywordsSpotterKeywordsThreshold() const { return m_keywordsSpotterKeywordsThreshold; }
int QKeywordConfig::keywordsSpotterKeywordsScore() const { return m_keywordsSpotterKeywordsScore; }
QString QKeywordConfig::keywordsSpotterKeywordsFile() const { return m_keywordsSpotterKeywordsFile; }
int QKeywordConfig::keywordsSpotterFeatSampleRate() const { return m_keywordsSpotterFeatSampleRate; }
int QKeywordConfig::keywordsSpotterFeatFeatureDim() const { return m_keywordsSpotterFeatFeatureDim; }

// Setters implementation
void QKeywordConfig::setZipformerEncoder(const QString &encoder)
{
    if (m_zipformerEncoder != encoder) {
        m_zipformerEncoder = encoder;
        emit zipformerEncoderChanged();
    }
}

void QKeywordConfig::setZipformerDecoder(const QString &decoder)
{
    if (m_zipformerDecoder != decoder) {
        m_zipformerDecoder = decoder;
        emit zipformerDecoderChanged();
    }
}

void QKeywordConfig::setZipformerJoiner(const QString &joiner)
{
    if (m_zipformerJoiner != joiner) {
        m_zipformerJoiner = joiner;
        emit zipformerJoinerChanged();
    }
}

void QKeywordConfig::setOnlineModelDebug(int debug)
{
    if (m_onlineModelDebug != debug) {
        m_onlineModelDebug = debug;
        emit onlineModelDebugChanged();
    }
}

void QKeywordConfig::setOnlineModelThreads(int threads)
{
    if (m_onlineModelThreads != threads) {
        m_onlineModelThreads = threads;
        emit onlineModelThreadsChanged();
    }
}

void QKeywordConfig::setOnlineModelProvider(const QString &provider)
{
    if (m_onlineModelProvider != provider) {
        m_onlineModelProvider = provider;
        emit onlineModelProviderChanged();
    }
}

void QKeywordConfig::setOnlineModelType(const QString &type)
{
    if (m_onlineModelType != type) {
        m_onlineModelType = type;
        emit onlineModelTypeChanged();
    }
}

void QKeywordConfig::setOnlineModelTokens(const QString &tokens)
{
    if (m_onlineModelTokens != tokens) {
        m_onlineModelTokens = tokens;
        emit onlineModelTokensChanged();
    }
}

void QKeywordConfig::setKeywordsSpotterMaxActivePaths(int maxActivePaths)
{
    if (m_keywordsSpotterMaxActivePaths != maxActivePaths) {
        m_keywordsSpotterMaxActivePaths = maxActivePaths;
        emit keywordsSpotterMaxActivePathsChanged();
    }
}

void QKeywordConfig::setKeywordsSpotterKeywordsThreshold(int threshold)
{
    if (m_keywordsSpotterKeywordsThreshold != threshold) {
        m_keywordsSpotterKeywordsThreshold = threshold;
        emit keywordsSpotterKeywordsThresholdChanged();
    }
}

void QKeywordConfig::setKeywordsSpotterKeywordsScore(int score)
{
    if (m_keywordsSpotterKeywordsScore != score) {
        m_keywordsSpotterKeywordsScore = score;
        emit keywordsSpotterKeywordsScoreChanged();
    }
}

void QKeywordConfig::setKeywordsSpotterKeywordsFile(const QString &file)
{
    if (m_keywordsSpotterKeywordsFile != file) {
        m_keywordsSpotterKeywordsFile = file;
        emit keywordsSpotterKeywordsFileChanged();
    }
}

void QKeywordConfig::setKeywordsSpotterFeatSampleRate(int sampleRate)
{
    if (m_keywordsSpotterFeatSampleRate != sampleRate) {
        m_keywordsSpotterFeatSampleRate = sampleRate;
        emit keywordsSpotterFeatSampleRateChanged();
    }
}

void QKeywordConfig::setKeywordsSpotterFeatFeatureDim(int featureDim)
{
    if (m_keywordsSpotterFeatFeatureDim != featureDim) {
        m_keywordsSpotterFeatFeatureDim = featureDim;
        emit keywordsSpotterFeatFeatureDimChanged();
    }
}
