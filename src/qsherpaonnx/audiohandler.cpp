#include "audiohandler.h"

AudioHandler::AudioHandler(QObject *parent)
    : QObject(parent)
    , m_audioInput(nullptr)
    , m_audioOutput(nullptr)
    , m_inputDevice(nullptr)
    , m_outputDevice(nullptr)
    , m_isRunning(false)
{
}

AudioHandler::~AudioHandler()
{
    cleanup();
}

bool AudioHandler::setupAudioFormat()
{
    m_audioFormat.setSampleRate(44100);       // 采样率 44.1kHz
    m_audioFormat.setChannelCount(1);         // 单声道
    m_audioFormat.setSampleSize(16);          // 样本大小 16位
    m_audioFormat.setCodec("audio/pcm");      // 编码格式 PCM
    m_audioFormat.setByteOrder(QAudioFormat::LittleEndian); // 字节序 小端
    m_audioFormat.setSampleType(QAudioFormat::SignedInt);   // 样本类型 有符号整数

    // 检查格式是否支持
    QAudioDeviceInfo inputInfo = QAudioDeviceInfo::defaultInputDevice();
    if (!inputInfo.isFormatSupported(m_audioFormat)) {
        qWarning() << "Default input format not supported, trying nearest supported format.";
        m_audioFormat = inputInfo.nearestFormat(m_audioFormat);
    }

    QAudioDeviceInfo outputInfo = QAudioDeviceInfo::defaultOutputDevice();
    if (!outputInfo.isFormatSupported(m_audioFormat)) {
        qWarning() << "Default output format not supported, trying nearest supported format.";
        m_audioFormat = outputInfo.nearestFormat(m_audioFormat);
    }

    return true;
}

bool AudioHandler::startAudio()
{
    if (m_isRunning) {
        qWarning() << "Audio is already running!";
        return false;
    }

    if (!setupAudioFormat()) {
        qCritical() << "Failed to setup audio format!";
        return false;
    }

    // 获取音频设备信息
    QAudioDeviceInfo inputInfo = QAudioDeviceInfo::defaultInputDevice();
    QAudioDeviceInfo outputInfo = QAudioDeviceInfo::defaultOutputDevice();

    qDebug() << "Input device:" << inputInfo.deviceName();
    qDebug() << "Output device:" << outputInfo.deviceName();

    // 创建音频输入
    m_audioInput = new QAudioInput(inputInfo, m_audioFormat, this);

    // 创建音频输出
    m_audioOutput = new QAudioOutput(outputInfo, m_audioFormat, this);

    // 连接输出状态变化信号
    connect(m_audioOutput, &QAudioOutput::stateChanged,
            this, &AudioHandler::handleOutputStateChanged);

    // 打开输入设备
    m_inputDevice = m_audioInput->start();
    if (!m_inputDevice) {
        qCritical() << "Failed to start audio input!";
        cleanup();
        return false;
    }

    // 打开输出设备
    m_outputDevice = m_audioOutput->start();
    if (!m_outputDevice) {
        qCritical() << "Failed to start audio output!";
        cleanup();
        return false;
    }

    // 连接输入数据可读信号
    connect(m_inputDevice, &QIODevice::readyRead,
            this, &AudioHandler::handleInputDataReady);

    m_isRunning = true;
    qDebug() << "Audio streaming started successfully!";
    return true;
}

void AudioHandler::stopAudio()
{
    if (!m_isRunning) {
        return;
    }

    cleanup();
    m_isRunning = false;
    qDebug() << "Audio streaming stopped.";
}

void AudioHandler::handleInputDataReady()
{
    if (!m_inputDevice || !m_outputDevice) {
        return;
    }

    // 读取所有可用的音频数据
    QByteArray data = m_inputDevice->readAll();

    if (data.size() > 0) {
        // 将数据写入输出设备
        qint64 bytesWritten = m_outputDevice->write(data);

        if (bytesWritten != data.size()) {
            qWarning() << "Not all data was written to output device. Expected:"
                       << data.size() << "Actual:" << bytesWritten;
        }

        // 调试信息：每秒打印一次数据流量
        static int counter = 0;
        static qint64 totalBytes = 0;
        totalBytes += data.size();

        if (++counter % 100 == 0) {
            qDebug() << "Audio data flow:" << totalBytes / 1024 << "KB";
            totalBytes = 0;
        }
    }
}

void AudioHandler::handleOutputStateChanged(QAudio::State state)
{
    switch (state) {
    case QAudio::ActiveState:
        qDebug() << "Audio output is active";
        break;
    case QAudio::SuspendedState:
        qDebug() << "Audio output is suspended";
        break;
    case QAudio::StoppedState:
        qDebug() << "Audio output is stopped";
        // 检查错误
        if (m_audioOutput && m_audioOutput->error() != QAudio::NoError) {
            qCritical() << "Audio output error:" << m_audioOutput->error();
        }
        break;
    case QAudio::IdleState:
        qDebug() << "Audio output is idle (no data)";
        break;
    }
}

void AudioHandler::cleanup()
{
    if (m_audioInput) {
        m_audioInput->stop();
        delete m_audioInput;
        m_audioInput = nullptr;
    }

    if (m_audioOutput) {
        m_audioOutput->stop();
        delete m_audioOutput;
        m_audioOutput = nullptr;
    }

    m_inputDevice = nullptr;
    m_outputDevice = nullptr;
}
