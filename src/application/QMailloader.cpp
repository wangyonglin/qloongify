#include "QMailloader.h"
#include <QKeyEvent>
#include <QResizeEvent>


QMailloader::QMailloader(QWidget *parent)
    : QWidget{parent}
{

    ffmpeg= new Qt5FFmpegPlayer(this);
    ffmpeg->resize(size());
    ffmpeg->replay("/usr/etc/video/trailer.mp4");
}
 //ffmpeg->play("/usr/etc/video/trailer.mp4");
void QMailloader::keyPressEvent(QKeyEvent *event)
{
    // 特殊处理
    if (event->key() == Qt::Key_F1)
    {
        qDebug() << "Key_F1 pressed - taking screenshot";
    }else if (event->key() == Qt::Key_F2)
    {
        qDebug() << "Key_F2 pressed - taking screenshot";
    }
}


void QMailloader::resizeEvent(QResizeEvent *event)
{
    ffmpeg->resize(event->size());

}
