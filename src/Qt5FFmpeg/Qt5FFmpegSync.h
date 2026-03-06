#ifndef QT5FFMPEGSYNC_H
#define QT5FFMPEGSYNC_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <atomic>
#include <iostream>
#include <chrono>
#include "Qt5FrameObject.h"
extern "C" {
#include <libavutil/time.h>
#include <libavutil/frame.h>
}
#include "Qt5FFmpegUtils.h"
class Qt5FFmpegSync : public QObject
{
    Q_OBJECT
public:
    explicit Qt5FFmpegSync(QObject *parent = nullptr);
    int64_t audio(Qt5FrameObject & frameObject);
    int64_t video(Qt5FrameObject & frameObject);
    int64_t audio_diff();
    int64_t video_diff();
    int64_t get_now_ms();
    int64_t master();
    void start();
    void init();
private:
    int64_t m_audio_ptstime=0;
    int64_t m_video_ptstime=0;
    bool __locktime_root_flag =false;
    int64_t __locktime_start_time_ms =0;
    QMutex mutex;
    QWaitCondition waitCondition;
signals:
};

#endif // QT5FFMPEGSYNC_H
