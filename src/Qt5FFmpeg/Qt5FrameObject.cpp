#include "Qt5FrameObject.h"
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <cmath>
#include <algorithm>

// 构造函数
Qt5FrameObject::Qt5FrameObject()
    : m_frame(nullptr)
    , m_startTime(0)
    , m_timeBase({0, 0}) {
}

Qt5FrameObject::Qt5FrameObject(AVFrame* frame, int64_t startTime, AVRational timeBase)
    : m_frame(nullptr)
    , m_startTime(startTime)
    , m_timeBase(timeBase) {

    if (frame) {
        m_frame.reset(av_frame_alloc());
        if (m_frame && av_frame_ref(m_frame.get(), frame) >= 0) {
            // 引用成功
        } else {
            m_frame.reset(nullptr);
            qWarning() << "Failed to reference AVFrame";
        }
    }
}

Qt5FrameObject::Qt5FrameObject(AVFramePtr&& frame, int64_t startTime, AVRational timeBase)
    : m_frame(std::move(frame))
    , m_startTime(startTime)
    , m_timeBase(timeBase) {
}

// 拷贝构造函数
Qt5FrameObject::Qt5FrameObject(const Qt5FrameObject& other)
    : m_frame(nullptr)
    , m_startTime(other.m_startTime)
    , m_timeBase(other.m_timeBase) {

    if (other.m_frame) {
        m_frame.reset(av_frame_alloc());
        if (m_frame && av_frame_ref(m_frame.get(), other.m_frame.get()) >= 0) {
            // 引用成功
        } else {
            m_frame.reset(nullptr);
        }
    }
}

// 移动构造函数
Qt5FrameObject::Qt5FrameObject(Qt5FrameObject&& other) noexcept
    : m_frame(std::move(other.m_frame))
    , m_startTime(other.m_startTime)
    , m_timeBase(other.m_timeBase) {

    other.m_startTime = 0;
    other.m_timeBase = {0, 0};
}

// 拷贝赋值运算符
Qt5FrameObject& Qt5FrameObject::operator=(const Qt5FrameObject& other) {
    if (this != &other) {
        m_frame.reset(nullptr);
        m_startTime = other.m_startTime;
        m_timeBase = other.m_timeBase;

        if (other.m_frame) {
            m_frame.reset(av_frame_alloc());
            if (m_frame && av_frame_ref(m_frame.get(), other.m_frame.get()) >= 0) {
                // 引用成功
            } else {
                m_frame.reset(nullptr);
            }
        }
    }
    return *this;
}

// 移动赋值运算符
Qt5FrameObject& Qt5FrameObject::operator=(Qt5FrameObject&& other) noexcept {
    if (this != &other) {
        m_frame = std::move(other.m_frame);
        m_startTime = other.m_startTime;
        m_timeBase = other.m_timeBase;

        other.m_startTime = 0;
        other.m_timeBase = {0, 0};
    }
    return *this;
}

// 析构函数
Qt5FrameObject::~Qt5FrameObject() {
    // 智能指针会自动清理
}

bool Qt5FrameObject::create(AVFrame *frame, int64_t  startTime, AVRational timeBase)
{
    if (frame) {
        m_frame.reset(av_frame_alloc());
        if (m_frame && av_frame_ref(m_frame.get(), frame) >= 0) {
            // 引用成功
            m_startTime=startTime;
            m_timeBase=timeBase;
            return true;
        } else {
            m_frame.reset(nullptr);
            qWarning() << "Failed to reference AVFrame";
        }
    }
    return false;
}




// 基础信息
bool Qt5FrameObject::isValid() const {
    return m_frame != nullptr;
}

bool Qt5FrameObject::isVideoFrame() const {
    if (!isValid() || !m_frame) {
        return false;
    }
    return m_frame->width > 0 && m_frame->height > 0;
}

bool Qt5FrameObject::isAudioFrame() const {
    if (!isValid() || !m_frame) {
        return false;
    }
    return m_frame->nb_samples > 0 &&
           m_frame->sample_rate > 0 &&
           m_frame->channels > 0;
}

int64_t Qt5FrameObject::getFrameTime()
{
    int64_t pts_time=m_frame ? m_frame->pts : AV_NOPTS_VALUE;
    return (pts_time- m_startTime) * av_q2d(m_timeBase) * 1000;
}

// 获取帧数据
AVFrame* Qt5FrameObject::frame() const {
    return m_frame.get();
}

int64_t Qt5FrameObject::startTime() const {
    return m_startTime;
}

AVRational Qt5FrameObject::timeBase() const {
    return m_timeBase;
}

// 视频帧信息
int Qt5FrameObject::width() const {
    return m_frame ? m_frame->width : 0;
}

int Qt5FrameObject::height() const {
    return m_frame ? m_frame->height : 0;
}

AVPixelFormat Qt5FrameObject::pixelFormat() const {
    return m_frame ? static_cast<AVPixelFormat>(m_frame->format) : AV_PIX_FMT_NONE;
}

int64_t Qt5FrameObject::ptsTime() const {
    return m_frame ? m_frame->pts : AV_NOPTS_VALUE;
}

double Qt5FrameObject::pts() const
{
    // 计算视频帧的显示时间
    return av_frame_get_best_effort_timestamp(m_frame.get()) *
                      av_q2d(m_timeBase);
}

double Qt5FrameObject::duration() const {
    if (!m_frame) return 0.0;
    if (m_frame->pkt_duration <= 0) return 0.0;
    return m_frame->pkt_duration * av_q2d(m_timeBase);
}




int Qt5FrameObject::channels() const {
    return m_frame ? m_frame->channels : 0;
}

int Qt5FrameObject::nbSamples() const {
    return m_frame ? m_frame->nb_samples : 0;
}

int64_t Qt5FrameObject::channelLayout() const {
    return m_frame ? m_frame->channel_layout : 0;
}

// 时间计算
double Qt5FrameObject::timestamp() const {
    if (!m_frame || m_frame->pts == AV_NOPTS_VALUE) {
        return 0.0;
    }
    return m_frame->pts * av_q2d(m_timeBase);
}

double Qt5FrameObject::presentationTimestamp() const {
    return timestamp();
}
// 资源管理
void Qt5FrameObject::release() {
    m_frame.reset(nullptr);
    m_startTime = 0;
    m_timeBase = {0, 0};
}

void Qt5FrameObject::detach() {
    m_frame.release(); // 释放所有权，但不删除指针
}

