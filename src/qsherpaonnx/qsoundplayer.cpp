#include "qsoundplayer.h"

QSoundPlayer::QSoundPlayer(QObject *parent)
    : QObject(parent)
    , m_soundEffect(new QSoundEffect(this))
{
    // 连接状态改变信号，以便处理错误或状态更新
    connect(m_soundEffect, &QSoundEffect::statusChanged, this, &QSoundPlayer::handleStatusChanged);
    // 你也可以连接其他信号，例如 playingChanged
    // connect(m_soundEffect, &QSoundEffect::playingChanged, this, [this]() {
    //     qDebug() << "Playing state changed:" << m_soundEffect->isPlaying();
    // });
}

QSoundPlayer::~QSoundPlayer()
{
    stop(); // 析构时停止播放
    // QSoundEffect 由父对象管理，无需手动删除
}

bool QSoundPlayer::loadSound(const QString &filePath)
{
    if (filePath.isEmpty()) {
        qWarning() << "File path is empty!";
        return false;
    }

    QUrl sourceUrl = QUrl::fromLocalFile(filePath);
    if (!sourceUrl.isValid()) {
        qWarning() << "Invalid file URL:" << filePath;
        return false;
    }

    m_soundEffect->setSource(sourceUrl);

    // 检查音频是否成功开始加载
    if (m_soundEffect->status() == QSoundEffect::Error) {
        qWarning() << "Failed to load sound from:" << filePath;
        return false;
    }

    qDebug() << "Sound loaded successfully from:" << filePath;
    return true;
}

void QSoundPlayer::play()
{
    if (m_soundEffect->status() != QSoundEffect::Ready) {
        qWarning() << "Sound is not ready to play. Status:" << m_soundEffect->status();
        return;
    }
    m_soundEffect->play();
}

void QSoundPlayer::stop()
{
    m_soundEffect->stop();
}

void QSoundPlayer::setLoopCount(int loopCount)
{
    m_soundEffect->setLoopCount(loopCount);
}

void QSoundPlayer::setVolume(float volume)
{
    // 确保音量在合理范围内
    float clampedVolume = qBound(0.0f, volume, 1.0f);
    m_soundEffect->setVolume(clampedVolume);
}

bool QSoundPlayer::isPlaying() const
{
    return m_soundEffect->isPlaying();
}

bool QSoundPlayer::isLoaded() const
{
    return m_soundEffect->isLoaded();
}

void QSoundPlayer::handleStatusChanged()
{
    switch (m_soundEffect->status()) {
    case QSoundEffect::Null:
        qDebug() << "Sound status: Null - no source set.";
        break;
    case QSoundEffect::Loading:
        qDebug() << "Sound status: Loading...";
        break;
    case QSoundEffect::Ready:
        qDebug() << "Sound status: Ready to play.";
        break;
    case QSoundEffect::Error:
        qWarning() << "Sound status: Error occurred while loading sound.";
        break;
    default:
        break;
    }
}
