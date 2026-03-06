#ifndef QT5FRAMESYNC_H
#define QT5FRAMESYNC_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <atomic>
#include <iostream>
#include <chrono>

extern "C" {
#include <libavutil/time.h>
#include <libavutil/frame.h>
}

class Qt5FFmpegDemuxer;
class Qt5FFmpegDecoder;
class Qt5FFmpegSpeaker;

/**
 * @brief 音视频同步器
 *
 * 负责管理音频和视频的同步，支持多种同步模式：
 * 1. 音频为主时钟 (SYNC_AUDIO_MASTER)
 * 2. 视频为主时钟 (SYNC_VIDEO_MASTER)
 * 3. 外部时钟为主 (SYNC_EXTERNAL_CLOCK)
 */
class Qt5FrameObject;
class Qt5FrameSync : public QObject
{
    Q_OBJECT
public:
    explicit Qt5FrameSync(QObject *parent = nullptr);


    void initSync();
    int64_t master();
    void startSync();
    int64_t diff();
    int64_t audioFrameObject(Qt5FrameObject &frameObject);
    int64_t videoFrameObject(Qt5FrameObject &frameObject);
private:
    int64_t m_audio_ptstime=0;
    int64_t m_video_ptstime=0;
    bool __locktime_root_flag =false;
    int64_t __locktime_start_time_ms =0;
    QMutex mutex;
    QWaitCondition waitCondition;
    int64_t get_now_ms();
};

#endif // QT5FRAMESYNC_H
