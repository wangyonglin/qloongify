#ifndef QSOUNDRECORDER_H
#define QSOUNDRECORDER_H
#include "qsherpaonnx_global.h"
#include <QObject>
#include <QAudioInput>
#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QIODevice>
#include <QByteArray>



// 主录音器类:
class QT5SHERPAONNX_EXPORT QSoundRecorder : public QObject
{
    Q_OBJECT

public:
    explicit QSoundRecorder(QObject *parent = nullptr,const QAudioFormat &format = QAudioFormat());
    ~QSoundRecorder();
    bool start();
    void stop();
    QByteArray readAll();
    bool isRecording() const;
signals:
    void readyRead();
    void stateChanged(QAudio::State state);
    void errorOccurred(QAudio::Error error);

private slots:
    // 处理音频输入状态变化
    void handleStateChanged(QAudio::State newState);
    void handleInputDataReady();
private:
    void initialize();
    void cleanup();

    QAudioInput *qaudioinput= nullptr;
    QIODevice* qiodevice= nullptr;
    QAudioFormat qaudioformat;
};

#endif // QSOUNDRECORDER_H
