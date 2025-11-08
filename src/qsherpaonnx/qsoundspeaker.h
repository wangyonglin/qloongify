#ifndef QSOUNDSPEAKER_H
#define QSOUNDSPEAKER_H

#include <QObject>
#include <QIODevice>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>

class QSoundSpeaker : public QIODevice
{
    Q_OBJECT

public:
    explicit QSoundSpeaker(QObject *parent = nullptr);
    ~QSoundSpeaker();

    bool initialize();
    void cleanup();

    void start();
    void stop();
    bool isPlaying() const;

    void setAudioFormat(const QAudioFormat &format);
    QAudioFormat getAudioFormat() const;

    qint64 readData(char *data, qint64 maxSize) override;
    qint64 writeData(const char *data, qint64 maxSize) override;

signals:
    void stateChanged(QAudio::State state);
    void errorOccurred(QAudio::Error error);
    void playbackStarted();
    void playbackStopped();
public slots:
 void slotSpeaker(const QByteArray &data);
private slots:
    void handleStateChanged(QAudio::State newState);

private:
    QAudioOutput *qaudiooutput;
    QAudioFormat qaudioformat;
    QAudioDeviceInfo qaudiodeviceinfo;
    bool isInitialized;
};

#endif // QSOUNDSPEAKER_H
