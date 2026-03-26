#ifndef QMAILLOADER_H
#define QMAILLOADER_H

#include <QWidget>
#include <Qt5FFmpegPlayer>

class QMailloader : public QWidget
{
    Q_OBJECT
public:
    explicit QMailloader(QWidget *parent = nullptr);
private:
    Qt5FFmpegPlayer * ffmpeg;
signals:

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif // QMAILLOADER_H
