#include "Qt5FFmpegController.h"
#include "Qt5FrameSync.h"
#include <QDateTime>
#include <QElapsedTimer>
#include <QMutexLocker>

Qt5FFmpegController::Qt5FFmpegController(QObject *parent)
    : Qt5ThreadObject(parent)
    , context(nullptr)
{
    // 创建 FFmpeg 上下文frameObject
    context = new Qt5FFmpegContext(this);
    m_audio_frame_queue= new Qt5FrameQueue(this,100);
    m_video_frame_queue= new Qt5FrameQueue(this,100);
    connect(context,&Qt5FFmpegContext::signalFlash,[&](){
        Qt5ThreadObject::pause();
    });
}

Qt5FFmpegController::~Qt5FFmpegController()
{

}

bool Qt5FFmpegController::replay(const QString &url)
{
    if (!context)return false;
    return context->initContext(url);
}
void Qt5FFmpegController::start()
{
    if (!context)return;
    // 然后启动主工作线程
    Qt5ThreadObject::start();
    qDebug() << "PlayerWorker all started";
}

void Qt5FFmpegController::thread()
{

    QElapsedTimer timer;
    timer.start();

    Qt5PacketObject packetObject;
    if(context->isFrameFlash()){
        pause();
        qDebug() << "Qt5FFmpegController flash";
        return;
    }

    // 读取帧
    if(context->readPacketObject(packetObject)){
        // 根据流类型放入对应队列
        if (packetObject.getStreamIndex() == context->getAudioStreamIndex()) {
            Qt5FrameObject  frameObject;
            if(context->getAudioReceiveFrame(packetObject,frameObject)){
                //qDebug() << "m_audio_frame_queue:" << m_audio_frame_queue->size();
               m_audio_frame_queue->enqueue(frameObject);
            }
        } else if (packetObject.getStreamIndex() == context->getVideoStreamIndex()) {
            Qt5FrameObject  frameObject;
            if(context->getVideoReceiveFrame(packetObject,frameObject)){
                //qDebug() << "m_video_frame_queue:" << m_video_frame_queue->size();
                m_video_frame_queue->enqueue(frameObject);
            }
        }
    }
    // 控制读取速度，避免过快
    if (timer.elapsed() < 10) { // 至少10ms间隔
        QThread::usleep(1000);
    }
    timer.restart();
}




void Qt5FFmpegController::stop()
{

    // 停止主工作线程
    Qt5ThreadObject::stop();

    qDebug() << "PlayerWorker stopped";
}

void Qt5FFmpegController::pause()
{
    Qt5ThreadObject::pause();
}

void Qt5FFmpegController::resume()
{
    Qt5ThreadObject::resume();
}

Qt5FrameQueue *Qt5FFmpegController::getAudioFrameQueue()
{
    return m_audio_frame_queue;
}

Qt5FrameQueue *Qt5FFmpegController::getVideoFrameQueue()
{
    return m_video_frame_queue;
}



