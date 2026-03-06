// Qt5FrameUtils.h
#ifndef QT5FRAMEUTILS_H
#define QT5FRAMEUTILS_H

#include <QObject>
#include <QImage>
#include <QAudioFormat>
#include <QByteArray>
#include "Qt5FrameObject.h"

namespace Qt5FrameUtils {

// 音频帧转换
QByteArray Qt5FrameByteArray(const Qt5FrameObject &frameObject,
                             const QAudioFormat &format);

// 视频帧转换
QImage Qt5FrameImage(const Qt5FrameObject &frameObject,
                     QImage::Format format = QImage::Format_RGBA8888);

// 像素格式转换辅助函数
QImage::Format pixelFormatToQImageFormat(int avPixelFormat);
int qImageFormatToAVPixelFormat(QImage::Format format);

// 音频格式转换辅助函数
int audioFormatToAVSampleFormat(const QAudioFormat &format);
QAudioFormat avSampleFormatToAudioFormat(int avSampleFormat);

// 帧信息获取
bool isVideoFrame(const Qt5FrameObject &frameObject);
bool isAudioFrame(const Qt5FrameObject &frameObject);
int getFrameWidth(const Qt5FrameObject &frameObject);
int getFrameHeight(const Qt5FrameObject &frameObject);

// 图像缩放
QImage scaleFrameImage(const Qt5FrameObject &frameObject,
                       int width, int height,
                       QImage::Format format = QImage::Format_RGB32);

} // namespace Qt5FrameUtils

#endif // QT5FRAMEUTILS_H
