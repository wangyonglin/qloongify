#include "Qt5FFmpegSync.h"
#include <QDebug>

Qt5FFmpegSync::Qt5FFmpegSync(QObject *parent)
    : QObject{parent}
    ,m_audio_ptstime(0)
    ,m_video_ptstime(0)
{

}


void Qt5FFmpegSync::init()
{
    QMutexLocker locker(&mutex);
    __locktime_root_flag=false;
    __locktime_start_time_ms=0;

}
void Qt5FFmpegSync::start()
{
    QMutexLocker locker(&mutex);
    if (!__locktime_root_flag)
    {
        __locktime_start_time_ms = get_now_ms();
        __locktime_root_flag=true;
    }
}

int64_t Qt5FFmpegSync::get_now_ms()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

int64_t Qt5FFmpegSync::master()
{
    QMutexLocker locker(&mutex);
    int64_t curr_time= get_now_ms();
    if (curr_time >__locktime_start_time_ms)
    {
        return  curr_time - __locktime_start_time_ms;
    }
    return 0;
}

int64_t Qt5FFmpegSync::audio(Qt5FrameObject & frameObject)
{
    return (m_audio_ptstime =frameObject.getFrameTime());
}
int64_t Qt5FFmpegSync::video(Qt5FrameObject & frameObject)
{
    return (m_video_ptstime=frameObject.getFrameTime());
}

int64_t Qt5FFmpegSync::audio_diff()
{

    return (m_audio_ptstime-master());
}

int64_t Qt5FFmpegSync::video_diff()
{
    return (m_video_ptstime-master());
}
