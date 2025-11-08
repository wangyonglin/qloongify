#include "qloongifywidget.h"
#include <QDebug>

QLoongifyWidget::QLoongifyWidget(QWidget *parent)
    : QWidget{parent}
{
    qvboxlayout = new QVBoxLayout(this);
    qlabel = new QLabel("RK3588 Qt EGLFS KMS&DRM Test - Success!");
    qlabel->setAlignment(Qt::AlignCenter);
    qlabel->setStyleSheet("QLabel { background-color: blue; color: white; font-size: 24px; }");
    qvboxlayout->addWidget(qlabel);

    qsoundrecorder = new QSoundRecorder(this);
    qsoundspeaker=new QSoundSpeaker(this);

    connect(qsoundrecorder, &QSoundRecorder::signalRecorder,
            qsoundspeaker, &QSoundSpeaker::slotSpeaker); // 请根据实际信号和槽名调整

    qsoundrecorder->start();
    qsoundspeaker->start();
}

QLoongifyWidget::~QLoongifyWidget()
{
    qsoundspeaker->stop();
    qsoundrecorder->stop();
}
