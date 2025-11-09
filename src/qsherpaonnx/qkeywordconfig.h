#ifndef QKEYWORDCONFIG_H
#define QKEYWORDCONFIG_H

#include <QObject>
#include <QString>
#include "qsherpaonnx_global.h"

class QT5SHERPAONNX_EXPORT QKeywordConfig : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString zipformerEncoder READ zipformerEncoder WRITE setZipformerEncoder NOTIFY zipformerEncoderChanged)
    Q_PROPERTY(QString zipformerDecoder READ zipformerDecoder WRITE setZipformerDecoder NOTIFY zipformerDecoderChanged)
    Q_PROPERTY(QString zipformerJoiner READ zipformerJoiner WRITE setZipformerJoiner NOTIFY zipformerJoinerChanged)
    Q_PROPERTY(int onlineModelDebug READ onlineModelDebug WRITE setOnlineModelDebug NOTIFY onlineModelDebugChanged)
    Q_PROPERTY(int onlineModelThreads READ onlineModelThreads WRITE setOnlineModelThreads NOTIFY onlineModelThreadsChanged)
    Q_PROPERTY(QString onlineModelProvider READ onlineModelProvider WRITE setOnlineModelProvider NOTIFY onlineModelProviderChanged)
    Q_PROPERTY(QString onlineModelType READ onlineModelType WRITE setOnlineModelType NOTIFY onlineModelTypeChanged)
    Q_PROPERTY(QString onlineModelTokens READ onlineModelTokens WRITE setOnlineModelTokens NOTIFY onlineModelTokensChanged)
    Q_PROPERTY(int keywordsSpotterMaxActivePaths READ keywordsSpotterMaxActivePaths WRITE setKeywordsSpotterMaxActivePaths NOTIFY keywordsSpotterMaxActivePathsChanged)
    Q_PROPERTY(int keywordsSpotterKeywordsThreshold READ keywordsSpotterKeywordsThreshold WRITE setKeywordsSpotterKeywordsThreshold NOTIFY keywordsSpotterKeywordsThresholdChanged)
    Q_PROPERTY(int keywordsSpotterKeywordsScore READ keywordsSpotterKeywordsScore WRITE setKeywordsSpotterKeywordsScore NOTIFY keywordsSpotterKeywordsScoreChanged)
    Q_PROPERTY(QString keywordsSpotterKeywordsFile READ keywordsSpotterKeywordsFile WRITE setKeywordsSpotterKeywordsFile NOTIFY keywordsSpotterKeywordsFileChanged)
    Q_PROPERTY(int keywordsSpotterFeatSampleRate READ keywordsSpotterFeatSampleRate WRITE setKeywordsSpotterFeatSampleRate NOTIFY keywordsSpotterFeatSampleRateChanged)
    Q_PROPERTY(int keywordsSpotterFeatFeatureDim READ keywordsSpotterFeatFeatureDim WRITE setKeywordsSpotterFeatFeatureDim NOTIFY keywordsSpotterFeatFeatureDimChanged)

public:
    explicit QKeywordConfig(QObject *parent = nullptr);
    ~QKeywordConfig();

    // Getters
    QString zipformerEncoder() const;
    QString zipformerDecoder() const;
    QString zipformerJoiner() const;
    int onlineModelDebug() const;
    int onlineModelThreads() const;
    QString onlineModelProvider() const;
    QString onlineModelType() const;
    QString onlineModelTokens() const;
    int keywordsSpotterMaxActivePaths() const;
    int keywordsSpotterKeywordsThreshold() const;
    int keywordsSpotterKeywordsScore() const;
    QString keywordsSpotterKeywordsFile() const;
    int keywordsSpotterFeatSampleRate() const;
    int keywordsSpotterFeatFeatureDim() const;

    // Setters
    void setZipformerEncoder(const QString &encoder);
    void setZipformerDecoder(const QString &decoder);
    void setZipformerJoiner(const QString &joiner);
    void setOnlineModelDebug(int debug);
    void setOnlineModelThreads(int threads);
    void setOnlineModelProvider(const QString &provider);
    void setOnlineModelType(const QString &type);
    void setOnlineModelTokens(const QString &tokens);
    void setKeywordsSpotterMaxActivePaths(int maxActivePaths);
    void setKeywordsSpotterKeywordsThreshold(int threshold);
    void setKeywordsSpotterKeywordsScore(int score);
    void setKeywordsSpotterKeywordsFile(const QString &file);
    void setKeywordsSpotterFeatSampleRate(int sampleRate);
    void setKeywordsSpotterFeatFeatureDim(int featureDim);

signals:
    void zipformerEncoderChanged();
    void zipformerDecoderChanged();
    void zipformerJoinerChanged();
    void onlineModelDebugChanged();
    void onlineModelThreadsChanged();
    void onlineModelProviderChanged();
    void onlineModelTypeChanged();
    void onlineModelTokensChanged();
    void keywordsSpotterMaxActivePathsChanged();
    void keywordsSpotterKeywordsThresholdChanged();
    void keywordsSpotterKeywordsScoreChanged();
    void keywordsSpotterKeywordsFileChanged();
    void keywordsSpotterFeatSampleRateChanged();
    void keywordsSpotterFeatFeatureDimChanged();

private:
    QString m_zipformerEncoder;
    QString m_zipformerDecoder;
    QString m_zipformerJoiner;
    int m_onlineModelDebug = 1;
    int m_onlineModelThreads = 1;
    QString m_onlineModelProvider = "cpu";
    QString m_onlineModelType = "zipformer";
    QString m_onlineModelTokens;
    int m_keywordsSpotterMaxActivePaths = 4;
    int m_keywordsSpotterKeywordsThreshold = 0.25;
    int m_keywordsSpotterKeywordsScore = 1.0;
    QString m_keywordsSpotterKeywordsFile;
    int m_keywordsSpotterFeatSampleRate = 16000;
    int m_keywordsSpotterFeatFeatureDim = 80;
};

#endif // QKEYWORDCONFIG_H
