#ifndef QT5FFMPEGDECODER_H
#define QT5FFMPEGDECODER_H

#include <QObject>
#include "Qt5FFmpegDemuxer.h"
#include "Qt5FFmpegUtils.h"
#include "Qt5FrameObject.h"

class Qt5FFmpegDecoder : public QObject
{
    Q_OBJECT
public:

    explicit Qt5FFmpegDecoder(QObject *parent = nullptr);

    bool initContext(AVFormatContext *ifmt_ctx ,const enum AVMediaType & mediaType);
    void release();
    bool avcodec_receive_frame(const Qt5PacketObject &packetObject,
                                         Qt5FrameObject & frameObject);
    AVStream *stream();
public:
    AVFormatContext *decifmt_ctx;
    const AVCodec * dec=nullptr;
    AVCodecContext * dec_ctx=nullptr;
    AVStream *st;
    int stream_index =-1;

};

#endif // QT5FFMPEGDECODER_H
