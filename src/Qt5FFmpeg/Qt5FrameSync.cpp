#include "Qt5FrameSync.h"
#include <QDebug>
#include <chrono>
#include "Qt5FrameObject.h"
Qt5FrameSync::Qt5FrameSync(QObject *parent)
    : QObject{parent}
    ,m_audio_ptstime(0)
    ,m_video_ptstime(0)
{}

int64_t Qt5FrameSync::audioFrameObject(Qt5FrameObject & frameObject)
{
    return (m_audio_ptstime =frameObject.getFrameTime());
}
int64_t Qt5FrameSync::videoFrameObject(Qt5FrameObject & frameObject)
{
    return (m_video_ptstime=frameObject.getFrameTime());
}




void Qt5FrameSync::initSync()
{
    QMutexLocker locker(&mutex);
    __locktime_root_flag=false;
    __locktime_start_time_ms=0;

}
void Qt5FrameSync::startSync()
{
    QMutexLocker locker(&mutex);
    if (!__locktime_root_flag)
    {
        __locktime_start_time_ms = get_now_ms();
        __locktime_root_flag=true;
    }
}

int64_t Qt5FrameSync::diff()
{
    return m_audio_ptstime-m_video_ptstime;
}

int64_t Qt5FrameSync::get_now_ms()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

int64_t Qt5FrameSync::master()
{
    QMutexLocker locker(&mutex);
    int64_t curr_time= get_now_ms();
    if (curr_time >__locktime_start_time_ms)
    {
        return  curr_time - __locktime_start_time_ms;
    }
    return 0;
}
