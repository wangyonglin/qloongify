#include "qsoundspeaker.h"
#include <QDebug>

QSoundSpeaker::QSoundSpeaker(QObject *parent,const QAudioFormat &format)
    : QObject(parent)
    ,qaudioformat(format)
    ,qaudiooutput(nullptr)
{
    initialize();
}

QSoundSpeaker::~QSoundSpeaker()
{
    cleanup();
}

bool QSoundSpeaker::initialize()
{
    cleanup();
    // 设置默认音频格式（与录音器匹配）
    // qaudioformat.setSampleRate(44100);       // 采样率 44.1kHz
    // qaudioformat.setChannelCount(1);         // 单声道
    // qaudioformat.setSampleSize(16);          // 样本大小 16位
    // qaudioformat.setCodec("audio/pcm");      // 编码格式 PCM
    // qaudioformat.setByteOrder(QAudioFormat::LittleEndian); // 字节序 小端
    // qaudioformat.setSampleType(QAudioFormat::SignedInt);   // 样本类型 有符号整数
    // 获取默认音频输出设备
   QAudioDeviceInfo qaudiodeviceinfo = QAudioDeviceInfo::defaultOutputDevice();
    // 检查音频格式是否支持
    if (!qaudiodeviceinfo.isFormatSupported(qaudioformat)) {
        qWarning() << "defaultOutputDevice 默认格式不支持，使用最接近的格式。";
        qDebug() << qaudiodeviceinfo.deviceName();
        qaudioformat = qaudiodeviceinfo.nearestFormat(qaudioformat);
        qDebug() << "使用格式 - 采样率:" << qaudioformat.sampleRate()
                 << "声道数:" << qaudioformat.channelCount()
                 << "样本大小:" << qaudioformat.sampleSize();
    }
    return true;
}

void QSoundSpeaker::cleanup()
{
    if (qaudiooutput) {
        qaudiooutput->stop();
        qaudiooutput->disconnect();
        delete qaudiooutput;
        qaudiooutput = nullptr;
    }
}

bool QSoundSpeaker::start()
{
    // 创建音频输出
    QAudioDeviceInfo qaudiodeviceinfo = QAudioDeviceInfo::defaultOutputDevice();
    qaudiooutput = new QAudioOutput(qaudiodeviceinfo, qaudioformat, this);
    if (!qaudiooutput) {
        qWarning() << "音频输出设备未正确初始化";
        return false;
    }
    // 连接信号
    connect(qaudiooutput, &QAudioOutput::stateChanged,
            this, &QSoundSpeaker::handleStateChanged);

    qiodevice=qaudiooutput->start();
    if (!qiodevice) {
        qCritical() << "Failed to start audio output!";
        cleanup();
        return false;
    }
    qDebug() << "音频播放器初始化完成";
    qDebug() << "播放器设备名称:" << qaudiodeviceinfo.deviceName();
    qDebug() << "开始播放音频...";
    emit playbackStarted();
    return true;
}

void QSoundSpeaker::stop()
{
    if (qaudiooutput) {
        qaudiooutput->stop();
        qDebug() << "停止播放音频";
        emit playbackStopped();
    }
}

void QSoundSpeaker::write(const QByteArray &data)
{
    if(qiodevice){
        qint64 bytesWritten =qiodevice->write(data);
        if (bytesWritten != data.size()) {
            qWarning() << "Not all data was written to output device. Expected:"
                       << data.size() << "Actual:" << bytesWritten;
        }
    }
}

bool QSoundSpeaker::isPlaying() const
{
    return qaudiooutput &&
           qaudiooutput->state() == QAudio::ActiveState;
}


void QSoundSpeaker::handleStateChanged(QAudio::State newState)
{
    qDebug() << "音频输出状态变化:" << newState;
    switch (newState) {
    case QAudio::StoppedState:
        // 播放停止时，检查是否有错误
        if (qaudiooutput && qaudiooutput->error() != QAudio::NoError) {
            qWarning() << "音频输出错误:" << qaudiooutput->error();
            emit errorOccurred(qaudiooutput->error());
        } else {
            qDebug() << "音频播放正常停止";
        }
        break;

    case QAudio::ActiveState:
        qDebug() << "音频输出激活，开始播放数据";
        break;

    case QAudio::SuspendedState:
        qDebug() << "音频输出暂停";
        break;

    case QAudio::IdleState:
        qDebug() << "音频输出空闲（无数据可播放）";
        break;
    }

    emit stateChanged(newState);
}


