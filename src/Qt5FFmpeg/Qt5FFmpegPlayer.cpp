#include "Qt5FFmpegPlayer.h"
#include "Qt5FFmpegUtils.h"
#include <QPainter>
#include <QResizeEvent>

Qt5FFmpegPlayer::Qt5FFmpegPlayer(QWidget *parent)
    : Qt5FFmpegRender{parent}
{
    av_log_set_level(AV_LOG_ERROR);
    // 创建专门的工作对象来处理数据
    m_qt5_ffmpeg_sync=new Qt5FFmpegSync(this);
    m_controller = new Qt5FFmpegController(this);
    m_video_controller= new VideoController(this,
                                            m_qt5_ffmpeg_sync,
                                            m_controller->getVideoFrameQueue());
    m_audio_controller = new AudioController(this,
                                             m_qt5_ffmpeg_sync,
                                             m_controller->getAudioFrameQueue());
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);
    m_audio_controller->initContext(format);
    m_audio_controller->setVolume(0.5);
    // 连接信号
    connect(m_video_controller, &VideoController::signalReady,
            this, [this](const QImage &image) {
        // 显示视频帧
        displayImage(image);
        update();
    });

    m_controller->start();
    m_audio_controller->start();
    m_video_controller->start();

}

Qt5FFmpegPlayer::~Qt5FFmpegPlayer()
{
    m_controller->stop();
    m_audio_controller->stop();
    m_video_controller->stop();
}

void Qt5FFmpegPlayer::replay(const QString &url)
{
    m_video_controller->pause();
    m_audio_controller->pause();
    m_controller->pause();
    m_qt5_ffmpeg_sync->init();
    m_qt5_ffmpeg_sync->start();
    m_controller->replay(url);
    m_audio_controller->resume();
    m_controller->resume();
    m_video_controller->resume();
}

void Qt5FFmpegPlayer::stop()
{
    // 停止工作线程
    if(m_controller) {
        m_controller->stop();
    }
    qDebug() << "Player stopped";
}

void Qt5FFmpegPlayer::resizeEvent(QResizeEvent *event)
{
    Qt5FFmpegRender::resize(event->size());
}



