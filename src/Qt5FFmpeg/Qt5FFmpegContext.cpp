#include "Qt5FFmpegContext.h"

Qt5FFmpegContext::Qt5FFmpegContext(QObject *parent)
    : Qt5FFmpegDemuxer{parent}
{
    audio = new Qt5FFmpegDecoder(this);
    video = new Qt5FFmpegDecoder(this);
}

Qt5FFmpegContext::~Qt5FFmpegContext()
{

}

bool Qt5FFmpegContext::initContext(const QString &url)
{
    if(Qt5FFmpegDemuxer::initContext(url)){
        if(!audio->initContext(getFormatContext(),AVMEDIA_TYPE_AUDIO)) return false;
        if(!video->initContext(getFormatContext(),AVMEDIA_TYPE_VIDEO)) return false;
        return true;
    }
    return false;
}

void Qt5FFmpegContext::release()
{
    audio->release();
    video->release();
    Qt5FFmpegDemuxer::release();
}

bool Qt5FFmpegContext::getAudioReceiveFrame(const Qt5PacketObject &packetObject,
                                            Qt5FrameObject &frameObject)
{
    if(!audio)return false;
    return audio->avcodec_receive_frame(packetObject,frameObject);
}

bool Qt5FFmpegContext::getVideoReceiveFrame(const Qt5PacketObject &packetObject,
                                            Qt5FrameObject &frameObject)
{
    if(!video)return false;
    return video->avcodec_receive_frame(packetObject,frameObject);
}


