#ifndef VIDEOCONTROLLER_H
#define VIDEOCONTROLLER_H

#include <QObject>
#include "Qt5ThreadObject.h"
#include "Qt5FrameQueue.h"
#include "Qt5FrameUtils.h"
#include "Qt5FFmpegSync.h"

class VideoController : public Qt5ThreadObject
{
    Q_OBJECT
public:
    explicit VideoController(QObject *parent = nullptr,
                             Qt5FFmpegSync * qt5_ffmpeg_sync=nullptr,
                             Qt5FrameQueue * framequeue=nullptr);

signals:
    void signalReady(const QImage &image);
    // Qt5ThreadObject interface
protected:
    void thread() override;
private:
     Qt5FrameQueue * m_frame_queue;
     Qt5FFmpegSync * m_qt5_ffmpeg_sync;
};

#endif // VIDEOCONTROLLER_H
