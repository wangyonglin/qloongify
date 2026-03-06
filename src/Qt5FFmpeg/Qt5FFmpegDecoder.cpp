#include "Qt5FFmpegDecoder.h"
#include "Qt5FFmpegUtils.h"

#include <QDebug>

Qt5FFmpegDecoder::Qt5FFmpegDecoder(QObject *parent)
    : QObject{parent},
    decifmt_ctx(nullptr)
{
    qDebug() << "Qt5FFmpegDecoder created";
}

bool Qt5FFmpegDecoder::initContext(AVFormatContext *ifmt_ctx,const enum AVMediaType & mediaType)
{
    int ret=-1;
    if(!ifmt_ctx)return false;
    decifmt_ctx=ifmt_ctx;

    ret = av_find_best_stream(decifmt_ctx, mediaType, -1, -1, NULL, 0);
    if (ret < 0) {
        qDebug() << "Could not find " << Qt5FFmpegUtils::errorString(ret)
        << "stream in input file"
                 << av_get_media_type_string(mediaType);
        return false;
    } else {
        this->st = decifmt_ctx->streams[ret];
        /* find decoder for the stream */
        this->dec = avcodec_find_decoder(this->st->codecpar->codec_id);
        if (!this->dec) {
            qDebug() << "Failed to find codec"
            << av_get_media_type_string(mediaType);
            return false;    qDebug() << "";
        }

        /* Allocate a codec context for the decoder */
        this->dec_ctx = avcodec_alloc_context3(this->dec);
        if (!this->dec_ctx) {
            qDebug() << "Failed to allocate the %s codec context"
            << av_get_media_type_string(mediaType);
            return false;
        }
        /* Copy codec parameters from input stream to output codec context */
        if ((ret = avcodec_parameters_to_context(this->dec_ctx, this->st->codecpar)) < 0) {
            qDebug() << "Failed to copy  codec parameters to decoder context"
            << av_get_media_type_string(mediaType);
            return false;
        }

        /* Init the decoders */
        if ((ret = avcodec_open2(this->dec_ctx, this->dec, NULL)) < 0) {
            qDebug() << "Failed to open %s codec\n"
            << av_get_media_type_string(mediaType);
            return false;
        }
    }
    qDebug() << av_get_media_type_string(mediaType) << "initialized successfully";
    return true;
}


void Qt5FFmpegDecoder::release(){

}
bool Qt5FFmpegDecoder::avcodec_receive_frame(
    const Qt5PacketObject &packetObject,
    Qt5FrameObject & frameObject)
{

    if(!packetObject.isValid())return false;
    int ret=-1;
    AVFrame *frame = av_frame_alloc();
    if(!dec_ctx && !frame)return false;
    ret = avcodec_send_packet(this->dec_ctx, packetObject.packet());
    //av_packet_free(&pkt);
    if (ret < 0)
    {
        qDebug() << "avcodec_send_packet failed" << Qt5FFmpegUtils::errorString(ret);
        return false;
    }
    av_frame_unref(frame);
    ret = ::avcodec_receive_frame(this->dec_ctx, frame);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
        qDebug() << "avcodec_receive_frame" <<  Qt5FFmpegUtils::errorString(ret);
        return false;
    }else if (ret < 0) {
        qDebug() << "avcodec_receive_frame failed" << Qt5FFmpegUtils::errorString(ret);
        return false;
    }
    frameObject.create(frame,st->start_time,st->time_base);
    av_frame_free(&frame);
    return true;
}

AVStream *Qt5FFmpegDecoder::stream()
{
    return st;
}

