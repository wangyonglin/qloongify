#ifndef QT5FFMPEGUTILS_H
#define QT5FFMPEGUTILS_H
extern "C" {
#include "libavutil/avstring.h"
#include "libavutil/channel_layout.h"
#include "libavutil/eval.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/imgutils.h"
#include "libavutil/dict.h"
#include "libavutil/fifo.h"
#include "libavutil/parseutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/time.h"
#include "libavutil/bprint.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
#include "libavutil/tx.h"
#include "libswresample/swresample.h"
#include "libavfilter/avfilter.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
}
#if defined(QT5FFMPEG_LIBRARY)
#define QT5FFMPEG_EXPORT Q_DECL_EXPORT
#else
#define QT5FFMPEG_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include <QString>

namespace Qt5FFmpegUtils{
    QString errorString(int ret);
}
#endif // QT5FFMPEGUTILS_H
