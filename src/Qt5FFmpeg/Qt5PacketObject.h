#ifndef QT5PACKETOBJECT_H
#define QT5PACKETOBJECT_H

#include <stdint.h>
#include <memory>
#include <optional>

extern "C" {
#include <libavcodec/packet.h>
#include <libavutil/avutil.h>
}

// AVFrame 智能指针删除器
struct AVPacketDeleter {
    void operator()(AVPacket* packet) const;
};
using AVPacketPtr = std::unique_ptr<AVPacket, AVPacketDeleter>;

class Qt5PacketObject
{
public:
    /**
     * @brief 默认构造函数，创建空的AVPacket
     */
    explicit Qt5PacketObject();

    /**
     * @brief 从现有AVPacket创建（复制）
     * @param packet 源AVPacket，会被复制
     */
    explicit Qt5PacketObject(const AVPacket* packet);

    /**
     * @brief 移动构造函数
     */
    Qt5PacketObject(Qt5PacketObject&& other) noexcept;

    /**
     * @brief 移动赋值运算符
     */
    Qt5PacketObject& operator=(Qt5PacketObject&& other) noexcept;

    // 禁用拷贝构造和拷贝赋值
    Qt5PacketObject(const Qt5PacketObject&) = delete;
    Qt5PacketObject& operator=(const Qt5PacketObject&) = delete;

    /**
     * @brief 析构函数
     */
    ~Qt5PacketObject() = default;

    /**
     * @brief 分配新的AVPacket
     * @return 是否分配成功
     */
    bool allocate();

    /**
     * @brief 释放当前AVPacket
     */
    void release();
    /**
     * Wipe the packet.
     *
     * Unreference the buffer referenced by the packet and reset the
     * remaining packet fields to their default values.
     *
     * @param pkt The packet to be unreferenced.
     */
    void unref();
    /**
     * @brief 获取内部AVPacket指针（只读）
     * @return const AVPacket* 内部AVPacket指针，可能为nullptr
     */
    const AVPacket* packet() const;

    /**
     * @brief 获取内部AVPacket指针
     * @return AVPacket* 内部AVPacket指针，可能为nullptr
     */
    AVPacket* packet();

    /**
     * @brief 获取智能指针
     * @return const AVPacketPtr& 智能指针引用
     */
    const AVPacketPtr& packetPtr() const;

    /**
     * @brief 判断是否有效
     * @return bool 是否有有效的AVPacket
     */
    bool isValid() const;

    /**
     * @brief 获取数据大小
     * @return int 数据大小，如果没有数据返回0
     */
    int size() const;

    /**
     * @brief 获取数据指针
     * @return const uint8_t* 数据指针，如果没有数据返回nullptr
     */
    const uint8_t* data() const;

    /**
     * @brief 获取显示时间戳
     * @return int64_t 显示时间戳
     */
    int64_t pts() const;

    /**
     * @brief 设置显示时间戳
     * @param pts 新的显示时间戳
     */
    void setPts(int64_t pts);

    /**
     * @brief 获取解码时间戳
     * @return int64_t 解码时间戳
     */
    int64_t dts() const;

    /**
     * @brief 设置解码时间戳
     * @param dts 新的解码时间戳
     */
    void setDts(int64_t dts);

    /**
     * @brief 获取时长
     * @return int64_t 时长
     */
    int64_t duration() const;

    /**
     * @brief 设置时长
     * @param duration 新的时长
     */
    void setDuration(int64_t duration);

    /**
     * @brief 获取流索引
     * @return int 流索引
     */
    int getStreamIndex() const;

    /**
     * @brief 设置流索引
     * @param index 新的流索引
     */
    void setStreamIndex(int index);

    /**
     * @brief 获取标志
     * @return int 标志位
     */
    int flags() const;

    /**
     * @brief 检查是否有关键帧标志
     * @return bool 是否有关键帧
     */
    bool isKeyFrame() const;

    /**
     * @brief 设置关键帧标志
     * @param isKeyFrame 是否为关键帧
     */
    void setKeyFrame(bool isKeyFrame);

    /**
     * @brief 从另一个AVPacket复制数据
     * @param src 源AVPacket
     * @return bool 是否复制成功
     */
    bool copyFrom(const AVPacket* src);

    /**
     * @brief 从另一个Qt5PacketObject复制数据
     * @param other 源Qt5PacketObject
     * @return bool 是否复制成功
     */
    bool copyFrom(const Qt5PacketObject& other);

    /**
     * @brief 移动数据从另一个AVPacket
     * @param src 源AVPacket（移动后会被置空）
     * @return bool 是否移动成功
     */
    bool moveFrom(AVPacket* src);

    /**
     * @brief 创建深度拷贝
     * @return std::optional<Qt5PacketObject> 如果成功返回新对象，失败返回std::nullopt
     */
    std::optional<Qt5PacketObject> clone() const;

    /**
     * @brief 重置为默认状态
     */
    void reset();

    /**
     * @brief 交换两个对象的内容
     * @param other 另一个Qt5PacketObject
     */
    void swap(Qt5PacketObject& other) noexcept;

private:
    AVPacketPtr m_packet;
};

#endif // QT5PACKETOBJECT_H
