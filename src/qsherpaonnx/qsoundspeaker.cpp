#include "qsoundspeaker.h"
#include <QDebug>

QSoundSpeaker::QSoundSpeaker(QObject *parent)
    : QIODevice(parent)
    , qaudiooutput(nullptr)
    , isInitialized(false)
{
    // 设置默认音频格式（与录音器匹配）
    qaudioformat.setSampleRate(44100);       // 采样率 44.1kHz
    qaudioformat.setChannelCount(1);         // 单声道
    qaudioformat.setSampleSize(16);          // 样本大小 16位
    qaudioformat.setCodec("audio/pcm");      // 编码格式 PCM
    qaudioformat.setByteOrder(QAudioFormat::LittleEndian); // 字节序 小端
    qaudioformat.setSampleType(QAudioFormat::SignedInt);   // 样本类型 有符号整数

    // 获取默认音频输出设备
    qaudiodeviceinfo = QAudioDeviceInfo::defaultOutputDevice();
    initialize();
}

QSoundSpeaker::~QSoundSpeaker()
{
    cleanup();
}

bool QSoundSpeaker::initialize()
{
    cleanup();

    // 检查音频格式是否支持
    if (!qaudiodeviceinfo.isFormatSupported(qaudioformat)) {
        qWarning() << "默认格式不支持，使用最接近的格式。";
        qaudioformat = qaudiodeviceinfo.nearestFormat(qaudioformat);
        qDebug() << "使用格式 - 采样率:" << qaudioformat.sampleRate()
                 << "声道数:" << qaudioformat.channelCount()
                 << "样本大小:" << qaudioformat.sampleSize();
    }

    // 创建自定义音频设备
    if (!QIODevice::open(QIODevice::ReadWrite)) {
        qWarning() << "无法打开音频设备进行读取";
        return false;
    }

    // 创建音频输出
    qaudiooutput = new QAudioOutput(qaudiodeviceinfo, qaudioformat, this);

    // 连接信号
    connect(qaudiooutput, &QAudioOutput::stateChanged,
            this, &QSoundSpeaker::handleStateChanged);

    isInitialized = true;
    qDebug() << "音频播放器初始化完成";
    qDebug() << "播放器设备名称:" << qaudiodeviceinfo.deviceName();
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

    if (QIODevice::isOpen()) {
        QIODevice::close();
    }
    isInitialized = false;
}

void QSoundSpeaker::start()
{
    if (!isInitialized) {
        if (!initialize()) {
            qWarning() << "音频播放器初始化失败，无法开始播放";
            return;
        }
    }

    if (qaudiooutput) {
        qaudiooutput->start(this);
        qDebug() << "开始播放音频...";
        emit playbackStarted();
    } else {
        qWarning() << "音频输出设备未正确初始化";
    }
}

void QSoundSpeaker::stop()
{
    if (qaudiooutput) {
        qaudiooutput->stop();
        qDebug() << "停止播放音频";
        emit playbackStopped();
    }
}

bool QSoundSpeaker::isPlaying() const
{
    return qaudiooutput &&
           qaudiooutput->state() == QAudio::ActiveState;
}

void QSoundSpeaker::setAudioFormat(const QAudioFormat &format)
{
    if (qaudioformat != format) {
        qaudioformat = format;

        // 如果正在运行，重新初始化
        if (isInitialized) {
            QAudio::State oldState = qaudiooutput ? qaudiooutput->state() : QAudio::StoppedState;
            if (oldState == QAudio::ActiveState) {
                this->stop();
            }
            initialize();
            if (oldState == QAudio::ActiveState) {
                this->start();
            }
        }
    }
}

QAudioFormat QSoundSpeaker::getAudioFormat() const
{
    return qaudioformat;
}

qint64 QSoundSpeaker::readData(char *data, qint64 maxSize)
{
    // 这个函数由 QAudioOutput 调用以获取音频数据
    // 在实际应用中，您可能需要从缓冲区或文件提供数据
    // 这里我们只是返回0，表示没有数据可用
    // 实际使用时，您需要重写这个函数来提供音频数据

    Q_UNUSED(data);
    Q_UNUSED(maxSize);

    // 在实际实现中，您应该：
    // 1. 从内部缓冲区复制数据到 data
    // 2. 返回实际复制的字节数
    // 3. 如果没有足够的数据，可能返回小于 maxSize 的值

    return 0;
}

qint64 QSoundSpeaker::writeData(const char *data, qint64 maxSize)
{
    // 扬声器主要是读取设备，但也可以接收数据写入内部缓冲区
    // 在实际实现中，您可以将数据存储到缓冲区中供 readData 使用

    Q_UNUSED(data);
    Q_UNUSED(maxSize);

    // 实际使用时，您可能需要：
    // 1. 将数据添加到内部缓冲区
    // 2. 发出信号通知有新数据可用

    return 0;
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

void QSoundSpeaker::slotSpeaker(const QByteArray &data)
{
    if (qaudiooutput && qaudiooutput->state() == QAudio::ActiveState) {
        // 将数据写入缓冲区，QAudioOutput 会自动读取并播放
        seek(size()); // 移动到末尾
        write(data);
    }
}
