#ifndef QSHERPABUILDER_H
#define QSHERPABUILDER_H

#include <QObject>
#include <QAudioFormat>

class QSherpaBuilder : public QObject
{
    Q_OBJECT
public:
    explicit QSherpaBuilder(QObject *parent = nullptr,const QAudioFormat &format = QAudioFormat());
    QAudioFormat getAudioFormat();
    std::vector<float> convertFloatVector(const QByteArray &byteArray);
private:
    QAudioFormat qaudioformat;
};

#endif // QSHERPABUILDER_H
