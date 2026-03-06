#include "Qt5FFmpegDemuxer.h"
#include "Qt5FFmpegUtils.h"
#include <QDebug>
#include <QElapsedTimer>
#include <cmath>

Qt5FFmpegDemuxer::Qt5FFmpegDemuxer(QObject *parent)
    : QObject(parent)
    , ifmt_ctx(nullptr)
    , audio_stream_index(-1)
    , video_stream_index(-1)
    , m_frameFinished(false)
    , m_isSeeking(false)
    , m_isBuffering(false)
    , m_initialized(false)
    , m_duration(0.0)
    , m_currentTime(0.0)
    , m_lastAudioPts(AV_NOPTS_VALUE)
    , m_lastVideoPts(AV_NOPTS_VALUE)
    , m_audioStream(nullptr)
    , m_videoStream(nullptr)
    , m_audioTimeBase(0.0)
    , m_videoTimeBase(0.0)
    , m_packetsRead(0)
    , m_packetsDiscarded(0)
    ,m_flash(false)
{

    qDebug() << "Qt5FFmpegDemuxer created";
}

Qt5FFmpegDemuxer::~Qt5FFmpegDemuxer()
{
    release();
    qDebug() << "Qt5FFmpegDemuxer destroyed";
}

bool Qt5FFmpegDemuxer::initContext(const QString &url)
{
    QMutexLocker locker(&m_mutex);
    m_url = url;
    m_flash=false;
    if (url.isEmpty()) {
        qDebug() << "ffmpeg initload failed: empty url";
        return false;
    }
    // 打开输入文件
    int ret = avformat_open_input(&ifmt_ctx, url.toLocal8Bit().data(), nullptr, nullptr);
    if (ret < 0) {
        QString error = QString("avformat_open_input failed: %1").arg(Qt5FFmpegUtils::errorString(ret));
        qDebug() << error;
        return false;
    }

    // 获取流信息
    ret = avformat_find_stream_info(ifmt_ctx, nullptr);
    if (ret < 0) {
        QString error = QString("avformat_find_stream_info failed: %1").arg(Qt5FFmpegUtils::errorString(ret));
        qDebug() << error;
        avformat_close_input(&ifmt_ctx);
        ifmt_ctx = nullptr;
        return false;
    }

    // 输出格式信息（调试用）
    av_dump_format(ifmt_ctx, 0, url.toLocal8Bit().data(), 0);

    // 查找最佳音频流和视频流
    audio_stream_index = av_find_best_stream(ifmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    video_stream_index = av_find_best_stream(ifmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);

    // 获取流指针和时间基准
    if (audio_stream_index >= 0) {
        m_audioStream = ifmt_ctx->streams[audio_stream_index];
        m_audioTimeBase = av_q2d(m_audioStream->time_base);
        qDebug() << "Audio stream found: index=" << audio_stream_index
                 << "time_base=" << m_audioTimeBase;
    } else {
        qDebug() << "No audio stream found";
    }

    if (video_stream_index >= 0) {
        m_videoStream = ifmt_ctx->streams[video_stream_index];
        m_videoTimeBase = av_q2d(m_videoStream->time_base);
        qDebug() << "Video stream found: index=" << video_stream_index
                 << "time_base=" << m_videoTimeBase;
    } else {
        qDebug() << "No video stream found";
    }

    // 计算总时长
    if (ifmt_ctx->duration != AV_NOPTS_VALUE) {
        m_duration = ifmt_ctx->duration / (double)AV_TIME_BASE;
        qDebug() << "Duration:" << m_duration << "seconds";
    } else if (m_videoStream && m_videoStream->duration != AV_NOPTS_VALUE) {
        m_duration = m_videoStream->duration * m_videoTimeBase;
        qDebug() << "Duration from video stream:" << m_duration << "seconds";
    } else if (m_audioStream && m_audioStream->duration != AV_NOPTS_VALUE) {
        m_duration = m_audioStream->duration * m_audioTimeBase;
        qDebug() << "Duration from audio stream:" << m_duration << "seconds";
    } else {
        m_duration = 0.0;
        qDebug() << "Unknown duration";
    }

    m_frameFinished = false;
    m_currentTime = 0.0;
    m_lastAudioPts = AV_NOPTS_VALUE;
    m_lastVideoPts = AV_NOPTS_VALUE;
    m_packetsRead = 0;
    m_packetsDiscarded = 0;
    m_initialized = true;

    emit demuxerInitialized(true);
    qDebug() << "demuxer initialized successfully";
    return true;
}

void Qt5FFmpegDemuxer::release()
{
    QMutexLocker locker(&m_mutex);
    // 关闭格式上下文
    if (ifmt_ctx) {
        avformat_close_input(&ifmt_ctx);
        ifmt_ctx = nullptr;
    }
    qDebug() << "Qt5FFmpegDemuxer released";
}

bool Qt5FFmpegDemuxer::readPacketObject(Qt5PacketObject & packetObject)
{
    if (!ifmt_ctx) {
        return false;
    }
    // 确保 packetObject 内部有分配 AVPacket
    if (!packetObject.isValid()) {
        if (!packetObject.allocate()) {
            return false;
        }
    } else {
        // 如果已经分配，则重置（unref）它
        packetObject.unref();
    }
    int ret = av_read_frame(ifmt_ctx, packetObject.packet());
    if (ret < 0) {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg, AV_ERROR_MAX_STRING_SIZE, ret);
        if (ret == AVERROR_EOF) {
            m_flash = true;
            emit eofReached();
        } else {
            qDebug() << "Error while reading frames" << errmsg;
        }
        return false;
    }
    return true;
}

bool Qt5FFmpegDemuxer::isFrameFlash()
{
   return m_flash.load();
}




// 获取信息的方法实现
QString Qt5FFmpegDemuxer::getUrl() const
{
    return m_url;
}

double Qt5FFmpegDemuxer::getDuration() const
{
    return m_duration;
}

double Qt5FFmpegDemuxer::getCurrentTime() const
{
    return m_currentTime;
}

int Qt5FFmpegDemuxer::getAudioStreamIndex()
{
    return audio_stream_index;         // 音频流索引
}

int Qt5FFmpegDemuxer::getVideoStreamIndex()
{
    return video_stream_index;         // 视频流索引
}



AVStream* Qt5FFmpegDemuxer::getAudioStream() const
{
    return m_audioStream;
}

AVStream* Qt5FFmpegDemuxer::getVideoStream() const
{
    return m_videoStream;
}

AVFormatContext* Qt5FFmpegDemuxer::getFormatContext() const
{
    return ifmt_ctx;
}


