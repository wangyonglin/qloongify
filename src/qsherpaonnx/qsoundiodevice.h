#ifndef QSOUNDIODEVICE_H
#define QSOUNDIODEVICE_H
#include "qsherpaonnx_global.h"
#include <QObject>
#include <QAudioInput>
#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QIODevice>
#include <QByteArray>

// 自定义音频IO设备，用于实时处理音频数据
class QT5SHERPAONNX_EXPORT QSoundIODevice : public QIODevice
{
    Q_OBJECT

public:
    explicit QSoundIODevice(QObject *parent = nullptr);

    // QIODevice 接口实现
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;
    bool isSequential() const override;

signals:
    // 当有新的音频数据到达时发出此信号
    void dataReady(const QByteArray &data);

private:
    QByteArray m_buffer;
};




#endif // QSOUNDIODEVICE_H
