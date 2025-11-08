#include "qsoundrecorder.h"
#include <QDebug>

QSoundRecorder::QSoundRecorder(QObject *parent, const QAudioFormat &format)
    : QObject(parent)
    ,qaudioformat(format)
    , qaudioinput(nullptr)
{
    initialize();
}


QSoundRecorder::~QSoundRecorder()
{
    cleanup();
}


void QSoundRecorder::initialize()
{
    cleanup();
    // 设置默认音频格式

    // qaudioformat.setSampleRate(44100);       // 采样率 44.1kHz
    // qaudioformat.setChannelCount(1);         // 单声道
    // qaudioformat.setSampleSize(16);          // 样本大小 16位
    // qaudioformat.setCodec("audio/pcm");      // 编码格式 PCM
    // qaudioformat.setByteOrder(QAudioFormat::LittleEndian); // 字节序 小端
    // qaudioformat.setSampleType(QAudioFormat::SignedInt);   // 样本类型 有符号整数

    QAudioDeviceInfo qaudiodeviceinfo = QAudioDeviceInfo::defaultInputDevice();
    // 检查音频格式是否支持
    if (!qaudiodeviceinfo.isFormatSupported(qaudioformat)) {
        qWarning() << "defaultInputDevice 默认格式不支持，使用最接近的格式。" ;
        qDebug() << qaudiodeviceinfo.deviceName();
        qaudioformat = qaudiodeviceinfo.nearestFormat(qaudioformat);
        qDebug() << "使用格式 - 采样率:" << qaudioformat.sampleRate()
                 << "声道数:" << qaudioformat.channelCount()
                 << "样本大小:" << qaudioformat.sampleSize();
    }


}

void QSoundRecorder::cleanup()
{
    if (qaudioinput) {
        qaudioinput->stop();
        qaudioinput->disconnect();
        delete qaudioinput;
        qaudioinput = nullptr;
    }
}

bool QSoundRecorder::start()
{
    qDebug() << "start 开始录制音频...";
    QAudioDeviceInfo qaudiodeviceinfo = QAudioDeviceInfo::defaultInputDevice();
    // 创建音频输入
    qaudioinput = new QAudioInput(qaudiodeviceinfo, qaudioformat, this);
    if (!qaudioinput) {
        qWarning() << "音频输入或设备未正确初始化";
        return false;
    }

    // 连接信号
    connect(qaudioinput, &QAudioInput::stateChanged,
            this, &QSoundRecorder::handleStateChanged);
    qiodevice=qaudioinput->start();
    if (!qiodevice) {
        qCritical() << "Failed to start audio input!";
        cleanup();
        return false;
    }
    // 连接输入数据可读信号
    connect(qiodevice, &QIODevice::readyRead,
            this, &QSoundRecorder::handleInputDataReady);
    qDebug() << "音频录制器初始化完成";
    qDebug() << "麦克风设备名称:" << qaudiodeviceinfo.deviceName();
    qDebug() << "开始录制音频...";
    return true;
}

void QSoundRecorder::stop()
{
    if (qaudioinput) {
        qaudioinput->stop();
        qDebug() << "停止录制音频";
    }
}

bool QSoundRecorder::isRecording() const
{
    return qaudioinput &&
           qaudioinput->state() == QAudio::ActiveState;
}

QByteArray QSoundRecorder::readAll()
{
    if(qiodevice){
        return qiodevice->readAll();
    }
    return QByteArray();
}



void QSoundRecorder::handleStateChanged(QAudio::State newState)
{
    qDebug() << "音频输入状态变化:" << newState;
    switch (newState) {
    case QAudio::StoppedState:
        // 录制停止时，检查是否有错误
        if (qaudioinput && qaudioinput->error() != QAudio::NoError) {
            qWarning() << "音频输入错误:" << qaudioinput->error();
            emit errorOccurred(qaudioinput->error());
        } else {
            qDebug() << "音频录制正常停止";
        }
        break;
    case QAudio::ActiveState:
        qDebug() << "音频输入激活，开始接收数据";
        break;
    case QAudio::SuspendedState:
        qDebug() << "音频输入暂停";
        break;
    case QAudio::IdleState:
        qDebug() << "音频输入空闲";
        break;
    }

    emit stateChanged(newState);
}

void QSoundRecorder::handleInputDataReady()
{
    if (qiodevice) {
        emit readyRead();
    }
}
