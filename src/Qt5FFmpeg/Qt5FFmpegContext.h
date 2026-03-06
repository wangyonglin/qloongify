#ifndef QT5FFMPEGCONTEXT_H
#define QT5FFMPEGCONTEXT_H

#include <QObject>
#include "Qt5ThreadObject.h"
#include "Qt5FFmpegDemuxer.h"
#include "Qt5FFmpegDecoder.h"
#include "Qt5FrameQueue.h"
#include "Qt5PacketObject.h"

class Qt5FFmpegContext : public Qt5FFmpegDemuxer
{
    Q_OBJECT
public:
    explicit Qt5FFmpegContext(QObject *parent = nullptr);
    ~Qt5FFmpegContext();
    bool initContext(const QString &url);
    void release();
    bool getAudioReceiveFrame(const Qt5PacketObject &packetObject,
                               Qt5FrameObject & frameObject);
    bool getVideoReceiveFrame(const Qt5PacketObject &packetObject,
                              Qt5FrameObject & frameObject);
private:
    Qt5FFmpegDecoder *audio;
    Qt5FFmpegDecoder *video;
signals:

    void signalFlash();
};

#endif // QT5FFMPEGCONTEXT_H
