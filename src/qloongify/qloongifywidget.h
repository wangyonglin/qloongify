#ifndef QLOONGIFYWIDGET_H
#define QLOONGIFYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

#include <Qt5SherpaOnnx/QSoundRecorder>
#include <Qt5SherpaOnnx/QSoundSpeaker>

class QLoongifyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QLoongifyWidget(QWidget *parent = nullptr);
    ~QLoongifyWidget();
private:

    QLabel *qlabel;
    QVBoxLayout *qvboxlayout;
    QSoundRecorder *qsoundrecorder=nullptr;
    QSoundSpeaker *qsoundspeaker=nullptr;
private slots:
signals:
};

#endif // QLOONGIFYWIDGET_H
