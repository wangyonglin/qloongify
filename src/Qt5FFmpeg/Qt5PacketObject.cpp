#include "Qt5PacketObject.h"
#include <stdexcept>

// AVPacketDeleter 实现
void AVPacketDeleter::operator()(AVPacket* packet) const {
    if (packet) {
        av_packet_free(&packet);
    }
}

// Qt5PacketObject 实现

Qt5PacketObject::Qt5PacketObject()
    : m_packet(nullptr) {
}

Qt5PacketObject::Qt5PacketObject(const AVPacket* packet)
    : m_packet(nullptr) {
    if (packet) {
        copyFrom(packet);
    }
}

Qt5PacketObject::Qt5PacketObject(Qt5PacketObject&& other) noexcept
    : m_packet(std::move(other.m_packet)) {
}

Qt5PacketObject& Qt5PacketObject::operator=(Qt5PacketObject&& other) noexcept {
    if (this != &other) {
        m_packet = std::move(other.m_packet);
    }
    return *this;
}

bool Qt5PacketObject::allocate() {
    release();

    AVPacket* packet = av_packet_alloc();
    if (!packet) {
        return false;
    }

    m_packet.reset(packet);
    return true;
}

void Qt5PacketObject::release() {
    m_packet.reset();
}

void Qt5PacketObject::unref()
{
    if (m_packet) {
        av_packet_unref(m_packet.get());
    }
}

const AVPacket* Qt5PacketObject::packet() const {
    return m_packet.get();
}

AVPacket* Qt5PacketObject::packet() {
    return m_packet.get();
}

const AVPacketPtr& Qt5PacketObject::packetPtr() const {
    return m_packet;
}

bool Qt5PacketObject::isValid() const {
    return m_packet != nullptr;
}

int Qt5PacketObject::size() const {
    return m_packet ? m_packet->size : 0;
}

const uint8_t* Qt5PacketObject::data() const {
    return m_packet ? m_packet->data : nullptr;
}

int64_t Qt5PacketObject::pts() const {
    return m_packet ? m_packet->pts : AV_NOPTS_VALUE;
}

void Qt5PacketObject::setPts(int64_t pts) {
    if (m_packet) {
        m_packet->pts = pts;
    }
}

int64_t Qt5PacketObject::dts() const {
    return m_packet ? m_packet->dts : AV_NOPTS_VALUE;
}

void Qt5PacketObject::setDts(int64_t dts) {
    if (m_packet) {
        m_packet->dts = dts;
    }
}

int64_t Qt5PacketObject::duration() const {
    return m_packet ? m_packet->duration : 0;
}

void Qt5PacketObject::setDuration(int64_t duration) {
    if (m_packet) {
        m_packet->duration = duration;
    }
}

int Qt5PacketObject::getStreamIndex() const {
    return m_packet ? m_packet->stream_index : -1;
}

void Qt5PacketObject::setStreamIndex(int index) {
    if (m_packet) {
        m_packet->stream_index = index;
    }
}

int Qt5PacketObject::flags() const {
    return m_packet ? m_packet->flags : 0;
}

bool Qt5PacketObject::isKeyFrame() const {
    return m_packet && (m_packet->flags & AV_PKT_FLAG_KEY);
}

void Qt5PacketObject::setKeyFrame(bool isKeyFrame) {
    if (m_packet) {
        if (isKeyFrame) {
            m_packet->flags |= AV_PKT_FLAG_KEY;
        } else {
            m_packet->flags &= ~AV_PKT_FLAG_KEY;
        }
    }
}

bool Qt5PacketObject::copyFrom(const AVPacket* src) {
    if (!src) {
        return false;
    }

    // 确保我们有可用的packet
    if (!m_packet && !allocate()) {
        return false;
    }

    int ret = av_packet_ref(m_packet.get(), src);
    if (ret < 0) {
        // 复制失败，清理资源
        release();
        return false;
    }

    return true;
}

bool Qt5PacketObject::copyFrom(const Qt5PacketObject& other) {
    if (!other.isValid()) {
        release();
        return true; // 复制空对象是成功的
    }

    return copyFrom(other.packet());
}

bool Qt5PacketObject::moveFrom(AVPacket* src) {
    if (!src) {
        return false;
    }

    release();

    AVPacketPtr temp(src);
    m_packet = std::move(temp);

    return true;
}

std::optional<Qt5PacketObject> Qt5PacketObject::clone() const {
    if (!isValid()) {
        return Qt5PacketObject();
    }

    Qt5PacketObject newObject;
    if (!newObject.copyFrom(m_packet.get())) {
        return std::nullopt;
    }

    return newObject;
}

void Qt5PacketObject::reset() {
    release();
}

void Qt5PacketObject::swap(Qt5PacketObject& other) noexcept {
    m_packet.swap(other.m_packet);
}
