#ifndef QSOUNDSPEAKER_H
#define QSOUNDSPEAKER_H

#include <QObject>
#include <QIODevice>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include "qsherpaonnx_global.h"

class QT_SHERPAONNX_EXPORT QSoundSpeaker : public QObject
{
    Q_OBJECT
public:
    explicit QSoundSpeaker(QObject *parent = nullptr,const QAudioFormat &format = QAudioFormat());
    ~QSoundSpeaker();
    bool start();
    void stop();
    void write(const QByteArray &data);
    bool isPlaying() const;
signals:
    void stateChanged(QAudio::State state);
    void errorOccurred(QAudio::Error error);
    void playbackStarted();
    void playbackStopped();
private slots:
    void handleStateChanged(QAudio::State newState);
private:
    bool initialize();
    void cleanup();
    QAudioOutput *qaudiooutput = nullptr;
    QIODevice *qiodevice = nullptr;
    QAudioFormat qaudioformat ;
};

#endif // QSOUNDSPEAKER_H
