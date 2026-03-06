#ifndef QT5FRAMEOBJECT_H
#define QT5FRAMEOBJECT_H

#include <QImage>
#include <QByteArray>
#include <QAudioFormat>
#include <QDebug>
#include <stdint.h>
#include <memory>

extern "C" {
#include <libavutil/frame.h>
#include <libavutil/rational.h>
#include <libavutil/pixdesc.h>
#include <libavutil/samplefmt.h>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

// AVFrame 智能指针删除器
struct AVFrameDeleter {
    void operator()(AVFrame* frame) const {
        if (frame) {
            av_frame_free(&frame);
        }
    }
};
using AVFramePtr = std::unique_ptr<AVFrame, AVFrameDeleter>;

class Qt5FrameObject {
public:
    // 构造函数和析构函数
    Qt5FrameObject();
    Qt5FrameObject(AVFrame* frame, int64_t startTime, AVRational timeBase);
    Qt5FrameObject(const Qt5FrameObject& other);
    Qt5FrameObject(Qt5FrameObject&& other) noexcept;
    Qt5FrameObject& operator=(const Qt5FrameObject& other);
    Qt5FrameObject& operator=(Qt5FrameObject&& other) noexcept;
    ~Qt5FrameObject();
    bool create(AVFrame* frame, int64_t startTime, AVRational timeBase);
    // 基础信息获取
    bool isValid() const;
    bool isVideoFrame() const;
    bool isAudioFrame() const;
    //
    int64_t getFrameTime();
    // 帧数据获取
    AVFrame* frame() const;
    int64_t startTime() const;
    AVRational timeBase() const;

    // 视频帧信息
    int width() const;
    int height() const;
    AVPixelFormat pixelFormat() const;
    int64_t ptsTime() const;
    double pts() const;
    double duration() const;

    int channels() const;
    int nbSamples() const;
    int64_t channelLayout() const;

    // 时间计算
    double timestamp() const;
    double presentationTimestamp() const;
    // 获取音频格式
    // 资源管理
    void release();
    void detach();  // 分离帧，不再管理内存

private:
    // 私有构造函数辅助
    Qt5FrameObject(AVFramePtr&& frame, int64_t startTime, AVRational timeBase);


private:
    AVFramePtr m_frame;
    int64_t m_startTime;
    AVRational m_timeBase;
};

// 智能指针别名
using Qt5FrameObjectPtr = std::shared_ptr<Qt5FrameObject>;
using Qt5FrameObjectWeakPtr = std::weak_ptr<Qt5FrameObject>;

#endif // QT5FRAMEOBJECT_H
