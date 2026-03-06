#ifndef QT5FFMPEGPLAYER_H
#define QT5FFMPEGPLAYER_H

#include <QObject>
#include "Qt5FFmpegDecoder.h"
#include "Qt5FFmpegRender.h"
#include "Qt5FFmpegController.h"
#include "AudioController.h"
#include "VideoController.h"
#include "Qt5FrameSync.h"

class Qt5FFmpegPlayer : public Qt5FFmpegRender
{
    Q_OBJECT
public:
    explicit Qt5FFmpegPlayer(QWidget *parent = nullptr);
    ~Qt5FFmpegPlayer();
    void replay(const QString & url);
    void stop();
    void release();
    // ... 现有成员 ...
    Qt5FFmpegSync *m_qt5_ffmpeg_sync;
    Qt5FFmpegController* m_controller = nullptr;
    VideoController * m_video_controller;
    AudioController * m_audio_controller;
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // QT5FFMPEGPLAYER_H
