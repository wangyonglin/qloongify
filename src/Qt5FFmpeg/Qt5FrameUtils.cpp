// Qt5FrameUtils.cpp
#include "Qt5FrameUtils.h"
#include <QDebug>
#include <QScopedPointer>

// FFmpeg头文件
extern "C" {
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

namespace Qt5FrameUtils {

QByteArray Qt5FrameByteArray(const Qt5FrameObject &frameObject,
                             const QAudioFormat &format)
{
    // 检查输入有效性
    if (!frameObject.isValid() || !format.isValid()) {
        qWarning() << "Invalid frame object or audio format";
        return QByteArray();
    }

    QByteArray result;
    AVFrame* frame = frameObject.frame();

    if (!frame || !frame->data[0]) {
        qWarning() << "Invalid frame data";
        return result;
    }

    // 如果是视频帧，返回空数据
    if (frame->width > 0 || frame->height > 0) {
        qWarning() << "This is a video frame, not audio";
        return result;
    }

    // 检查是否为音频帧
    if (frame->nb_samples <= 0 || frame->sample_rate <= 0) {
        qWarning() << "Invalid audio frame: no samples or sample rate";
        return result;
    }

    // 获取源音频参数
    int64_t srcChannelLayout = frame->channel_layout;
    if (!srcChannelLayout) {
        srcChannelLayout = av_get_default_channel_layout(frame->channels);
    }

    // 确定目标通道布局
    int64_t dstChannelLayout;
    switch (format.channelCount()) {
    case 1:
        dstChannelLayout = AV_CH_LAYOUT_MONO;
        break;
    case 2:
        dstChannelLayout = AV_CH_LAYOUT_STEREO;
        break;
    case 6:
        dstChannelLayout = AV_CH_LAYOUT_5POINT1;
        break;
    default:
        dstChannelLayout = av_get_default_channel_layout(format.channelCount());
        if (!dstChannelLayout) {
            dstChannelLayout = AV_CH_LAYOUT_STEREO; // 默认立体声
        }
        break;
    }

    // 确定目标采样格式
    AVSampleFormat dstSampleFormat = static_cast<AVSampleFormat>(
        audioFormatToAVSampleFormat(format));

    // 如果格式完全匹配，直接返回数据
    if (frame->format == dstSampleFormat &&
        frame->sample_rate == format.sampleRate() &&
        srcChannelLayout == dstChannelLayout) {

        int dataSize = av_samples_get_buffer_size(nullptr,
                                                  format.channelCount(),
                                                  frame->nb_samples,
                                                  dstSampleFormat, 1);
        if (dataSize > 0) {
            result = QByteArray(reinterpret_cast<const char*>(frame->data[0]), dataSize);
        }
        return result;
    }

    // 创建重采样上下文
    SwrContext* swrContext = swr_alloc();
    if (!swrContext) {
        qWarning() << "Failed to allocate SwrContext";
        return result;
    }

    // 设置重采样参数
    av_opt_set_int(swrContext, "in_channel_layout", srcChannelLayout, 0);
    av_opt_set_int(swrContext, "out_channel_layout", dstChannelLayout, 0);
    av_opt_set_int(swrContext, "in_sample_rate", frame->sample_rate, 0);
    av_opt_set_int(swrContext, "out_sample_rate", format.sampleRate(), 0);
    av_opt_set_sample_fmt(swrContext, "in_sample_fmt",
                          static_cast<AVSampleFormat>(frame->format), 0);
    av_opt_set_sample_fmt(swrContext, "out_sample_fmt", dstSampleFormat, 0);

    // 初始化重采样上下文
    if (swr_init(swrContext) < 0) {
        qWarning() << "Failed to initialize SwrContext";
        swr_free(&swrContext);
        return result;
    }

    // 计算输出样本数
    int64_t delay = swr_get_delay(swrContext, frame->sample_rate);
    int64_t outSamples = av_rescale_rnd(delay + frame->nb_samples,
                                        format.sampleRate(),
                                        frame->sample_rate,
                                        AV_ROUND_UP);

    // 分配输出缓冲区
    uint8_t** outData = nullptr;
    int outLinesize;
    int ret = av_samples_alloc_array_and_samples(&outData, &outLinesize,
                                                 format.channelCount(),
                                                 outSamples,
                                                 dstSampleFormat, 0);
    if (ret < 0) {
      //  qWarning() << "Failed to allocate output samples:" << av_err2str(ret);
        swr_free(&swrContext);
        return result;
    }

    // 执行重采样
    int convertedSamples = swr_convert(swrContext, outData, outSamples,
                                       (const uint8_t**)frame->data,
                                       frame->nb_samples);

    if (convertedSamples < 0) {
       // qWarning() << "Error during audio conversion:" << av_err2str(convertedSamples);
    } else if (convertedSamples > 0) {
        // 计算实际转换的数据大小
        int dataSize = av_samples_get_buffer_size(&outLinesize,
                                                  format.channelCount(),
                                                  convertedSamples,
                                                  dstSampleFormat, 1);
        if (dataSize > 0) {
            result = QByteArray(reinterpret_cast<const char*>(outData[0]), dataSize);
        }
    }

    // 清理资源
    if (outData) {
        av_freep(&outData[0]);
        av_freep(&outData);
    }
    swr_free(&swrContext);

    return result;
}

QImage Qt5FrameImage(const Qt5FrameObject &frameObject,
                     QImage::Format format)
{
    // 检查输入有效性
    if (!frameObject.isValid()) {
        qWarning() << "Invalid frame object";
        return QImage();
    }

    AVFrame* frame = frameObject.frame();
    if (!frame || !frame->data[0]) {
        qWarning() << "Invalid frame data";
        return QImage();
    }

    // 检查是否为视频帧
    if (frame->width <= 0 || frame->height <= 0) {
        qWarning() << "This is an audio frame, not video";
        return QImage();
    }

    int width = frame->width;
    int height = frame->height;
    AVPixelFormat srcPixelFormat = static_cast<AVPixelFormat>(frame->format);

    // 确定目标像素格式
    AVPixelFormat dstPixelFormat;
    switch (format) {
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
        dstPixelFormat = AV_PIX_FMT_BGRA;
        break;
    case QImage::Format_RGB888:
        dstPixelFormat = AV_PIX_FMT_RGB24;
        break;
    case QImage::Format_RGBA8888:
        dstPixelFormat = AV_PIX_FMT_RGBA;
        break;
    case QImage::Format_Grayscale8:
        dstPixelFormat = AV_PIX_FMT_GRAY8;
        break;
    default:
        // 默认转换为ARGB32
        dstPixelFormat = AV_PIX_FMT_BGRA;
        break;
    }

    // 如果格式匹配，直接转换
    if (srcPixelFormat == dstPixelFormat) {
        int bytesPerPixel = 0;
        switch (dstPixelFormat) {
        case AV_PIX_FMT_BGRA:
            bytesPerPixel = 4;
            break;
        case AV_PIX_FMT_RGB24:
            bytesPerPixel = 3;
            break;
        case AV_PIX_FMT_RGBA:
            bytesPerPixel = 4;
            break;
        case AV_PIX_FMT_GRAY8:
            bytesPerPixel = 1;
            break;
        default:
            bytesPerPixel = 4;
            break;
        }

        // 创建QImage（不复制数据）
        QImage image(frame->data[0], width, height,
                     frame->linesize[0], format);
        return image.copy(); // 深拷贝以确保安全
    }

    // 创建缩放上下文
    SwsContext* swsContext = sws_getContext(width, height, srcPixelFormat,
                                            width, height, dstPixelFormat,
                                            SWS_BILINEAR, nullptr, nullptr, nullptr);
    if (!swsContext) {
        qWarning() << "Failed to create SwsContext";
        return QImage();
    }

    // 分配目标帧
    AVFrame* dstFrame = av_frame_alloc();
    if (!dstFrame) {
        sws_freeContext(swsContext);
        return QImage();
    }

    dstFrame->format = dstPixelFormat;
    dstFrame->width = width;
    dstFrame->height = height;

    // 分配缓冲区
    int ret = av_frame_get_buffer(dstFrame, 0);
    if (ret < 0) {
       // qWarning() << "Failed to allocate frame buffer:" << av_err2str(ret);
        av_frame_free(&dstFrame);
        sws_freeContext(swsContext);
        return QImage();
    }

    // 执行转换
    sws_scale(swsContext,
              frame->data, frame->linesize,
              0, height,
              dstFrame->data, dstFrame->linesize);

    // 创建QImage
    QImage image;
    switch (dstPixelFormat) {
    case AV_PIX_FMT_BGRA:
        image = QImage(dstFrame->data[0], width, height,
                       dstFrame->linesize[0], QImage::Format_ARGB32);
        break;
    case AV_PIX_FMT_RGB24:
        image = QImage(dstFrame->data[0], width, height,
                       dstFrame->linesize[0], QImage::Format_RGB888);
        break;
    case AV_PIX_FMT_RGBA:
        image = QImage(dstFrame->data[0], width, height,
                       dstFrame->linesize[0], QImage::Format_RGBA8888);
        break;
    case AV_PIX_FMT_GRAY8:
        image = QImage(dstFrame->data[0], width, height,
                       dstFrame->linesize[0], QImage::Format_Grayscale8);
        break;
    default:
        // 转换为默认格式
        image = QImage(dstFrame->data[0], width, height,
                       dstFrame->linesize[0], QImage::Format_ARGB32);
        break;
    }

    QImage result = image.copy(); // 深拷贝

    // 清理资源
    av_frame_free(&dstFrame);
    sws_freeContext(swsContext);

    return result;
}

QImage::Format pixelFormatToQImageFormat(int avPixelFormat)
{
    switch (avPixelFormat) {
    case AV_PIX_FMT_RGB24:
        return QImage::Format_RGB888;
    case AV_PIX_FMT_BGRA:
        return QImage::Format_ARGB32;
    case AV_PIX_FMT_RGBA:
        return QImage::Format_RGBA8888;
    case AV_PIX_FMT_GRAY8:
        return QImage::Format_Grayscale8;
    case AV_PIX_FMT_RGB8:
    case AV_PIX_FMT_RGB4:
    case AV_PIX_FMT_RGB4_BYTE:
    case AV_PIX_FMT_BGR8:
    case AV_PIX_FMT_BGR4:
    case AV_PIX_FMT_BGR4_BYTE:
    case AV_PIX_FMT_RGB444BE:
    case AV_PIX_FMT_RGB444LE:
    case AV_PIX_FMT_BGR444BE:
    case AV_PIX_FMT_BGR444LE:
        return QImage::Format_RGB32; // 需要转换
    default:
        return QImage::Format_Invalid;
    }
}

int qImageFormatToAVPixelFormat(QImage::Format format)
{
    switch (format) {
    case QImage::Format_RGB888:
        return AV_PIX_FMT_RGB24;
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
        return AV_PIX_FMT_BGRA;
    case QImage::Format_RGBA8888:
        return AV_PIX_FMT_RGBA;
    case QImage::Format_Grayscale8:
        return AV_PIX_FMT_GRAY8;
    case QImage::Format_Indexed8:
        return AV_PIX_FMT_PAL8;
    default:
        return AV_PIX_FMT_NONE;
    }
}

int audioFormatToAVSampleFormat(const QAudioFormat &format)
{
    switch (format.sampleType()) {
    case QAudioFormat::SignedInt:
        if (format.sampleSize() == 8) {
            // 注意：FFmpeg中8位有符号通常存储为无符号
            return AV_SAMPLE_FMT_U8;
        } else if (format.sampleSize() == 16) {
            return AV_SAMPLE_FMT_S16;
        } else if (format.sampleSize() == 32) {
            return AV_SAMPLE_FMT_S32;
        } else if (format.sampleSize() == 64) {
            return AV_SAMPLE_FMT_S64;
        }
        break;
    case QAudioFormat::UnSignedInt:
        if (format.sampleSize() == 8) {
            return AV_SAMPLE_FMT_U8;
        }
        break;
    case QAudioFormat::Float:
        if (format.sampleSize() == 32) {
            return AV_SAMPLE_FMT_FLT;
        } else if (format.sampleSize() == 64) {
            return AV_SAMPLE_FMT_DBL;
        }
        break;
    default:
        break;
    }
    return AV_SAMPLE_FMT_S16; // 默认格式
}

QAudioFormat avSampleFormatToAudioFormat(int avSampleFormat)
{
    QAudioFormat format;

    switch (avSampleFormat) {
    case AV_SAMPLE_FMT_U8:
    case AV_SAMPLE_FMT_U8P:
        format.setSampleType(QAudioFormat::UnSignedInt);
        format.setSampleSize(8);
        break;
    case AV_SAMPLE_FMT_S16:
    case AV_SAMPLE_FMT_S16P:
        format.setSampleType(QAudioFormat::SignedInt);
        format.setSampleSize(16);
        break;
    case AV_SAMPLE_FMT_S32:
    case AV_SAMPLE_FMT_S32P:
        format.setSampleType(QAudioFormat::SignedInt);
        format.setSampleSize(32);
        break;
    case AV_SAMPLE_FMT_S64:
    case AV_SAMPLE_FMT_S64P:
        format.setSampleType(QAudioFormat::SignedInt);
        format.setSampleSize(64);
        break;
    case AV_SAMPLE_FMT_FLT:
    case AV_SAMPLE_FMT_FLTP:
        format.setSampleType(QAudioFormat::Float);
        format.setSampleSize(32);
        break;
    case AV_SAMPLE_FMT_DBL:
    case AV_SAMPLE_FMT_DBLP:
        format.setSampleType(QAudioFormat::Float);
        format.setSampleSize(64);
        break;
    default:
        format.setSampleType(QAudioFormat::SignedInt);
        format.setSampleSize(16);
        break;
    }

    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/pcm");

    return format;
}

bool isVideoFrame(const Qt5FrameObject &frameObject)
{
    if (!frameObject.isValid()) {
        return false;
    }

    AVFrame* frame = frameObject.frame();
    return frame && frame->width > 0 && frame->height > 0;
}

bool isAudioFrame(const Qt5FrameObject &frameObject)
{
    if (!frameObject.isValid()) {
        return false;
    }

    AVFrame* frame = frameObject.frame();
    return frame && frame->nb_samples > 0 && frame->sample_rate > 0;
}

int getFrameWidth(const Qt5FrameObject &frameObject)
{
    if (!frameObject.isValid()) {
        return -1;
    }

    AVFrame* frame = frameObject.frame();
    return frame ? frame->width : -1;
}

int getFrameHeight(const Qt5FrameObject &frameObject)
{
    if (!frameObject.isValid()) {
        return -1;
    }

    AVFrame* frame = frameObject.frame();
    return frame ? frame->height : -1;
}

QImage scaleFrameImage(const Qt5FrameObject &frameObject,
                       int width, int height,
                       QImage::Format format)
{
    if (!isVideoFrame(frameObject) || width <= 0 || height <= 0) {
        return QImage();
    }

    AVFrame* frame = frameObject.frame();
    if (!frame || !frame->data[0]) {
        return QImage();
    }

    int srcWidth = frame->width;
    int srcHeight = frame->height;
    AVPixelFormat srcPixelFormat = static_cast<AVPixelFormat>(frame->format);
    AVPixelFormat dstPixelFormat = static_cast<AVPixelFormat>(
        qImageFormatToAVPixelFormat(format));

    if (dstPixelFormat == AV_PIX_FMT_NONE) {
        dstPixelFormat = AV_PIX_FMT_BGRA; // 默认格式
    }

    // 创建缩放上下文
    SwsContext* swsContext = sws_getContext(srcWidth, srcHeight, srcPixelFormat,
                                            width, height, dstPixelFormat,
                                            SWS_BILINEAR, nullptr, nullptr, nullptr);
    if (!swsContext) {
        return QImage();
    }

    // 分配目标帧
    AVFrame* dstFrame = av_frame_alloc();
    if (!dstFrame) {
        sws_freeContext(swsContext);
        return QImage();
    }

    dstFrame->format = dstPixelFormat;
    dstFrame->width = width;
    dstFrame->height = height;

    int ret = av_frame_get_buffer(dstFrame, 0);
    if (ret < 0) {
        av_frame_free(&dstFrame);
        sws_freeContext(swsContext);
        return QImage();
    }

    // 执行缩放
    sws_scale(swsContext,
              frame->data, frame->linesize,
              0, srcHeight,
              dstFrame->data, dstFrame->linesize);

    // 创建QImage
    QImage::Format finalFormat = format;
    if (finalFormat == QImage::Format_Invalid) {
        finalFormat = pixelFormatToQImageFormat(dstPixelFormat);
    }

    QImage image(dstFrame->data[0], width, height,
                 dstFrame->linesize[0], finalFormat);

    QImage result = image.copy();

    // 清理资源
    av_frame_free(&dstFrame);
    sws_freeContext(swsContext);

    return result;
}

} // namespace Qt5FrameUtils
