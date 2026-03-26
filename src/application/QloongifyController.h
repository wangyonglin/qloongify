#ifndef QLOONGIFYCONTROLLER_H
#define QLOONGIFYCONTROLLER_H

#include <QWidget>
#include <Qt5FFmpegPlayer>
#include <Qt5AliIOT/AliIOTConnect>
#include <Qt5AliIOT/AliIOTClient>
class QloongifyController : public QWidget
{
    Q_OBJECT
public:
    explicit QloongifyController(QWidget *parent = nullptr);
private:
  //  Qt5FFmpegPlayer * ffmpeg;
    AliIOTClient *client;
    AliIOTConnect *m_aliiot_connect=nullptr;
signals:

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif // QLOONGIFYCONTROLLER_H
