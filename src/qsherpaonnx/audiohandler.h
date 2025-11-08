#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include <QObject>
#include <QAudioInput>
#include <QAudioOutput>
#include <QIODevice>
#include <QAudioFormat>
#include <QDebug>

class AudioHandler : public QObject
{
    Q_OBJECT

public:
    explicit AudioHandler(QObject *parent = nullptr);
    ~AudioHandler();

    bool startAudio();
    void stopAudio();

private slots:
    void handleInputDataReady();
    void handleOutputStateChanged(QAudio::State state);

private:
    bool setupAudioFormat();
    void cleanup();

    QAudioFormat m_audioFormat;
    QAudioInput *m_audioInput;
    QAudioOutput *m_audioOutput;
    QIODevice *m_inputDevice;
    QIODevice *m_outputDevice;
    bool m_isRunning;
};

#endif // AUDIOHANDLER_H
