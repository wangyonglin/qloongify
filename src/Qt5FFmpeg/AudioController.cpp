#include "AudioController.h"
#include <QAudioDeviceInfo>
#include <QDebug>
#include <QThread>
#include <QElapsedTimer>


AudioController::AudioController(QObject *parent,
                                 Qt5FFmpegSync * qt5_ffmpeg_sync,
                                 Qt5FrameQueue * framequeue)
    : Qt5ThreadObject(parent)
    ,m_iodevice(nullptr)
    ,m_frame_queue(framequeue)
    ,pauseFlag(false)
    ,m_qt5_ffmpeg_sync(qt5_ffmpeg_sync)
{
    // 设置默认音频格式
    m_format.setSampleRate(44100);
    m_format.setChannelCount(2);
    m_format.setSampleSize(16);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::SignedInt);

}

AudioController::~AudioController()
{

}

bool AudioController::initContext(const QAudioFormat &format)
{
    // 检查格式是否支持
    QAudioDeviceInfo deviceInfo = QAudioDeviceInfo::defaultOutputDevice();
    if (!deviceInfo.isFormatSupported(format)) {
        m_format = deviceInfo.nearestFormat(format);
        qWarning() << "Requested audio format not supported, using nearest format";
        return false;
    }
    return true;
}



void AudioController::start(Priority priority)
{
    output= new QAudioOutput(m_format,this);
    output->setVolume(0.1);
    m_iodevice =output->start();
    Qt5ThreadObject::start(priority);
}

void AudioController::stop()
{
    if(output){
        output->stop();
    }
    Qt5ThreadObject::stop();
}

void AudioController::setVolume(qreal volume)
{
    if(!output)return;
    output->setVolume(volume);
}



qreal AudioController::volume() const
{
    if(!output)return 0.0;
    return output->volume();
}

void AudioController::thread()
{
    int64_t result_fps=m_qt5_ffmpeg_sync->audio_diff();
    if(result_fps>=0.05){
        Qt5ThreadObject::usleep(10);
        return;
    }
    if(m_frame_queue && m_iodevice &&  m_frame_queue->size()>0){
        Qt5FrameObject frameObject;
        if( m_frame_queue->dequeue(frameObject)){
            if(frameObject.isAudioFrame()){
                //qDebug() << "audio : " << frameObject.getFrameTime();
                m_qt5_ffmpeg_sync->audio(frameObject);
                QByteArray bytes= Qt5FrameUtils::Qt5FrameByteArray(frameObject,m_format);
                if(!bytes.isEmpty()){
                        m_iodevice->write(bytes);

                }
            }
        }
    }
}

