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
class QT5SHERPAONNX_EXPORT QSoundRecorder : public QIODevice
{
    Q_OBJECT

public:
    explicit QSoundRecorder(QObject *parent = nullptr);
    ~QSoundRecorder();

    // 开始录制
    void start();
    // 停止录制
    void stop();
    // 获取音频格式
    QAudioFormat getAudioFormat() const { return qaudioformat; }
    // 检查是否正在录制
    bool isRecording() const;

public slots:
    // 设置音频格式
    void setAudioFormat(const QAudioFormat &format);

signals:
    // 音频数据刷新信号，传递原始音频数据
    void signalRecorder(const QByteArray &data);
    // 状态变化信号
    void stateChanged(QAudio::State state);
    // 错误信号
    void errorOccurred(QAudio::Error error);

private slots:
    // 处理音频输入状态变化
    void handleStateChanged(QAudio::State newState);

private:
    //注册麦克风设备
    // 初始化音频设备
    void initialize();
    // 清理资源
    void cleanup();
    QAudioFormat qaudioformat;
    QAudioInput *qaudioinput;
    QAudioDeviceInfo qaudiodeviceinfo;
    bool isInitialized;

    // QIODevice interface
protected:
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
};

#endif // QSOUNDRECORDER_H
