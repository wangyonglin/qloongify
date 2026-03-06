#ifndef QT5FFMPEGCONTROLLER_H
#define QT5FFMPEGCONTROLLER_H

#include <QObject>
#include <QAudioFormat>
#include <QImage>
#include <QDebug>
#include "Qt5ThreadObject.h"
#include "Qt5FFmpegContext.h"
#include "Qt5FrameObject.h"
#include "AudioController.h"

class Qt5FFmpegController : public Qt5ThreadObject
{
    Q_OBJECT

public:
    explicit Qt5FFmpegController(QObject *parent = nullptr);
    ~Qt5FFmpegController();

    bool replay(const QString &url);
    void start() ;
    void thread() override;
    void stop() ;
    void resume();
    void pause();
    Qt5FrameQueue * getAudioFrameQueue();
    Qt5FrameQueue * getVideoFrameQueue();
signals:
private slots:
private:
    Qt5FFmpegContext *context;
    Qt5FrameQueue *m_audio_frame_queue;
    Qt5FrameQueue *m_video_frame_queue;
    QAudioFormat format;
};

#endif // QT5FFMPEGCONTROLLER_H
