#include "qsoundiodevice.h"

#include <QDebug>
#include <QAudioDeviceInfo>

// AudioIODevice 实现
QSoundIODevice::QSoundIODevice(QObject *parent)
    : QIODevice(parent)
{
    // 设备初始化为关闭状态
}

qint64 QSoundIODevice::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data);
    Q_UNUSED(maxlen);
    // 这是一个只写设备，不需要实现读取功能
    return 0;
}

qint64 QSoundIODevice::writeData(const char *data, qint64 len)
{
    // 当音频数据写入时，立即通过信号发出
    QByteArray audioData(data, len);
    emit dataReady(audioData);

    // 返回写入的字节数
    return len;
}

bool QSoundIODevice::isSequential() const
{
    // 音频流是顺序设备，不支持 seek
    return true;
}
