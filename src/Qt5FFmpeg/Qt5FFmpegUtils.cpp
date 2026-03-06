#include "Qt5FFmpegUtils.h"
#include <QDebug>

QString Qt5FFmpegUtils::errorString(int ret)
{
    char bufferErrorString[AV_ERROR_MAX_STRING_SIZE];
    av_make_error_string(bufferErrorString,AV_ERROR_MAX_STRING_SIZE, ret);
    return QString(bufferErrorString);
}
