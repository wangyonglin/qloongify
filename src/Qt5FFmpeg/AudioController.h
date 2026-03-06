#ifndef AUDIOCONTROLLER_H
#define AUDIOCONTROLLER_H

#include <QObject>
#include <QByteArray>
#include <QMutex>
#include <QWaitCondition>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QIODevice>
#include <QTimer>
#include <memory>
#include "Qt5ThreadObject.h"
#include "Qt5FrameQueue.h"
#include "Qt5FrameUtils.h"
#include "Qt5FFmpegSync.h"
#include "Qt5FrameSync.h"

class AudioController : public Qt5ThreadObject
{
    Q_OBJECT
public:
    explicit AudioController(QObject *parent = nullptr,
                             Qt5FFmpegSync * qt5_ffmpeg_sync=nullptr,
                             Qt5FrameQueue * framequeue=nullptr);
    ~AudioController();
    bool initContext(const QAudioFormat &format);
    void start(Priority priority = InheritPriority);
    void stop();
    void setVolume(qreal volume=0.5);
    qreal volume() const;
private:
    Qt5FFmpegSync * m_qt5_ffmpeg_sync;
    Qt5FrameQueue * m_frame_queue;
    QAudioOutput * output;
    QAudioFormat m_format;
    QIODevice *m_iodevice;
    std::atomic<bool> pauseFlag;
    // Qt5ThreadObject interface
protected:
    void thread() override;
};

#endif // AUDIOCONTROLLER_H
