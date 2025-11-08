#ifndef QSOUNDPLAYER_H
#define QSOUNDPLAYER_H

#include <QObject>
#include <QSoundEffect>
#include <QUrl>
#include <QDebug>

class QSoundPlayer : public QObject
{
    Q_OBJECT

public:
    explicit QSoundPlayer(QObject *parent = nullptr);
    ~QSoundPlayer();

    // 加载音频文件
    bool loadSound(const QString &filePath);
    // 播放音频
    void play();
    // 停止播放
    void stop();
    // 设置播放循环次数
    void setLoopCount(int loopCount);
    // 设置音量 (0.0 到 1.0)
    void setVolume(float volume);
    // 检查是否正在播放
    bool isPlaying() const;
    // 检查音频是否已加载
    bool isLoaded() const;

private slots:
    void handleStatusChanged();


private:
    QSoundEffect *m_soundEffect;
};

#endif // QSOUNDPLAYER_H
