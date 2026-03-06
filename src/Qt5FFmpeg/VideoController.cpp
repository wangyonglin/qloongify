#include "VideoController.h"

VideoController::VideoController(QObject *parent,
                                 Qt5FFmpegSync * qt5_ffmpeg_sync,
                                 Qt5FrameQueue * framequeue)
    : Qt5ThreadObject{parent}
    ,m_frame_queue(framequeue),
    m_qt5_ffmpeg_sync(qt5_ffmpeg_sync)
{}

void VideoController::thread()
{
    int64_t result_fps=m_qt5_ffmpeg_sync->video_diff();
    if(result_fps>=0.05){
        Qt5ThreadObject::usleep(10);
        return;
    }
    Qt5FrameObject frameObject;
    if(m_frame_queue && m_frame_queue->size()>0){

        if( m_frame_queue->dequeue(frameObject)){
            if(frameObject.isVideoFrame()){
                //qDebug() << "video : " << frameObject.getFrameTime();
                m_qt5_ffmpeg_sync->video(frameObject);
                QImage image = Qt5FrameUtils::Qt5FrameImage(frameObject);
                if(!image.isNull()){
                    emit signalReady(image);
                }
            }
        }
    }
}
