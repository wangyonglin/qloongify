#ifndef QLOONGIFYWIDGET_H
#define QLOONGIFYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

#include <Qt5SherpaOnnx/QSoundRecorder>
#include <Qt5SherpaOnnx/QSoundSpeaker>
#include <Qt5SherpaOnnx/QKeywordSpotting>

class QLoongifyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QLoongifyWidget(QWidget *parent = nullptr);
    ~QLoongifyWidget();
private:

    QLabel *qlabel=nullptr;
    QVBoxLayout *qvboxlayout=nullptr;
    QSoundRecorder *qsoundrecorder=nullptr;
    QSoundSpeaker *qsoundspeaker=nullptr;
    QKeywordSpotting *qkeywordspotting=nullptr;
private slots:
signals:
};

#endif // QLOONGIFYWIDGET_H
