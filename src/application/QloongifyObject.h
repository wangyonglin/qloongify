#ifndef QLOONGIFYOBJECT_H
#define QLOONGIFYOBJECT_H

#include <QWidget>
#include <QSettings>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

class QloongifyObject : public QWidget
{
    Q_OBJECT
public:
    explicit QloongifyObject(QWidget *parent = nullptr);
    QString productKey();
    QString deviceName();
    QString deviceSecret();
    QString mqttHost();
    int mqttPort();
    QSettings * getSettings();
private:
    QSettings *final;
    void mergeSettings(QSettings *target, const QSettings *source);
signals:
};

#endif // QLOONGIFYOBJECT_H
