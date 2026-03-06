#ifndef QT5FFMPEGDEMUXER_H
#define QT5FFMPEGDEMUXER_H

#include "Qt5ThreadObject.h"
#include "Qt5PacketObject.h"

#include <QString>
#include <QMutex>
#include <QWaitCondition>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

class Qt5FFmpegDemuxer : public QObject
{
    Q_OBJECT

public:
    explicit Qt5FFmpegDemuxer(QObject *parent = nullptr);
    ~Qt5FFmpegDemuxer() override;

    // 初始化
    bool initContext(const QString &url);
    void release();
    // 读取帧
    bool readPacketObject(Qt5PacketObject & packetObject);
    bool isFrameFlash();
    // 获取信息
    QString getUrl() const;
    double getDuration() const;                 // 获取总时长（秒）
    double getCurrentTime() const;              // 获取当前播放时间（秒）
    int getAudioStreamIndex();
    int getVideoStreamIndex();
    AVStream* getAudioStream() const;
    AVStream* getVideoStream() const;
    AVFormatContext* getFormatContext() const;

signals:
    void demuxerInitialized(bool success);
    void seekCompleted(double time);
    void positionChanged(double position);
    void eofReached();                          // 文件结束


private:
    AVFormatContext *ifmt_ctx;      // 格式上下文
    int audio_stream_index;         // 音频流索引
    int video_stream_index;         // 视频流索引


private:
    std::atomic<bool> m_flash;
    QString m_url;                  // 文件路径
    bool m_frameFinished;           // 是否结束
    bool m_isSeeking;               // 是否正在跳转
    bool m_isBuffering;             // 是否正在缓冲
    bool m_initialized;             // 是否已初始化

    // 时间管理
    double m_duration;              // 总时长（秒）
    double m_currentTime;           // 当前时间（秒）
    int64_t m_lastAudioPts;         // 最后音频PTS
    int64_t m_lastVideoPts;         // 最后视频PTS

    // 同步保护
    mutable QMutex m_mutex;
    QWaitCondition m_seekCondition;

    // 流信息
    AVStream* m_audioStream;        // 音频流
    AVStream* m_videoStream;        // 视频流
    double m_audioTimeBase;         // 音频时间基准
    double m_videoTimeBase;         // 视频时间基准

    // 性能统计
    int m_packetsRead;              // 已读取的包数
    int m_packetsDiscarded;         // 丢弃的包数
};

#endif // QT5FFMPEGDEMUXER_H
