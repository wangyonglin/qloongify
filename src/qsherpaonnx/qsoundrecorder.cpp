#include "qsoundrecorder.h"
#include <QDebug>

QSoundRecorder::QSoundRecorder(QObject *parent)
    : QIODevice(parent)
    , qaudioinput(nullptr)  // 修复：变量名拼写错误
    , isInitialized(false)
{
    // 设置默认音频格式
    // qaudioformat.setSampleRate(44100);       // 采样率 44.1kHz
    // qaudioformat.setChannelCount(1);         // 单声道
    // qaudioformat.setSampleSize(16);          // 样本大小 16位
    // qaudioformat.setCodec("audio/pcm");      // 编码格式 PCM
    // qaudioformat.setByteOrder(QAudioFormat::LittleEndian); // 字节序 小端
    // qaudioformat.setSampleType(QAudioFormat::UnSignedInt);   // 样本类型 有符号整数
    initialize();
}

QSoundRecorder::~QSoundRecorder()
{
    cleanup();
}

// 必须重写的 QIODevice 纯虚函数
qint64 QSoundRecorder::readData(char *data, qint64 maxSize)
{
    Q_UNUSED(data);
    Q_UNUSED(maxSize);
    return 0; // 这个设备只用于写入，不用于读取
}

qint64 QSoundRecorder::writeData(const char *data, qint64 maxSize)
{
    // 当音频数据写入时发出信号
    QByteArray audioData(data, maxSize);
    emit signalRecorder(audioData);
    return maxSize;
}

void QSoundRecorder::initialize()
{
    cleanup();
    // 检查音频格式是否支持
    // if (!qaudiodeviceinfo.isFormatSupported(qaudioformat)) {
    //     qWarning() << "默认格式不支持，使用最接近的格式。";
    //     qaudioformat = qaudiodeviceinfo.nearestFormat(qaudioformat);
    //     qDebug() << "使用格式 - 采样率:" << qaudioformat.sampleRate()
    //              << "声道数:" << qaudioformat.channelCount()
    //              << "样本大小:" << qaudioformat.sampleSize();
    // }
    // 创建自定义音频设备
    if (!QIODevice::open(QIODevice::ReadWrite)) {
        qWarning() << "无法打开音频设备进行写入";
        return;
    }
    // 创建音频输入
    qaudioinput = new QAudioInput(qaudiodeviceinfo, qaudioformat, this);  // 修复：变量名拼写错误
    // 连接信号
    connect(qaudioinput, &QAudioInput::stateChanged,  // 修复：变量名拼写错误
            this, &QSoundRecorder::handleStateChanged);
    isInitialized = true;
    qDebug() << "音频录制器初始化完成";
    qDebug() << "麦克风设备名称:" << qaudiodeviceinfo.deviceName();
}

void QSoundRecorder::cleanup()
{
    if (qaudioinput) {  // 修复：变量名拼写错误
        qaudioinput->stop();  // 修复：变量名拼写错误
        qaudioinput->disconnect();  // 修复：变量名拼写错误
        delete qaudioinput;  // 修复：变量名拼写错误
        qaudioinput = nullptr;  // 修复：变量名拼写错误
    }
    if (QIODevice::isOpen()) {
        QIODevice::close();
    }
    isInitialized = false;
}

void QSoundRecorder::start()
{
    if (!isInitialized) {
        initialize();
        if (!isInitialized) {
            qWarning() << "音频录制器初始化失败，无法开始录制";
            return;
        }
    }
    if (qaudioinput) {  // 修复：变量名拼写错误
        qaudioinput->start(this);  // 修复：变量名拼写错误
        qDebug() << "开始录制音频...";
    } else {
        qWarning() << "音频输入或设备未正确初始化";
    }
}

void QSoundRecorder::stop()
{
    if (qaudioinput) {  // 修复：变量名拼写错误
        qaudioinput->stop();  // 修复：变量名拼写错误
        qDebug() << "停止录制音频";
    }
}

bool QSoundRecorder::isRecording() const
{
    return qaudioinput &&  // 修复：变量名拼写错误
           qaudioinput->state() == QAudio::ActiveState;  // 修复：变量名拼写错误
}

void QSoundRecorder::setAudioFormat(const QAudioFormat &format)
{
    if (qaudioformat != format) {
        qaudioformat = format;
        // 如果正在运行，重新初始化
        if (isInitialized) {
            QAudio::State oldState = qaudioinput ? qaudioinput->state() : QAudio::StoppedState;  // 修复：变量名拼写错误
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

void QSoundRecorder::handleStateChanged(QAudio::State newState)
{
    qDebug() << "音频输入状态变化:" << newState;
    switch (newState) {
    case QAudio::StoppedState:
        // 录制停止时，检查是否有错误
        if (qaudioinput && qaudioinput->error() != QAudio::NoError) {  // 修复：变量名拼写错误
            qWarning() << "音频输入错误:" << qaudioinput->error();  // 修复：变量名拼写错误
            emit errorOccurred(qaudioinput->error());  // 修复：变量名拼写错误
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
