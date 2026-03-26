#include "QloongifyObject.h"

QloongifyObject::QloongifyObject(QWidget *parent)
    : QWidget{parent}
{

    QDir().mkpath(tr("%0/%1").arg(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)).arg("wangyonglin")); // 确保目录存在

    final = new QSettings(QSettings::NativeFormat, QSettings::UserScope, "wangyonglin", "qloongify"); // 最终的存储对象（可读写）
    QSettings systemSettings("/etc/qloongify.conf", QSettings::NativeFormat);
    QSettings userSettings(QDir::home().filePath(".config/qloongify.conf"), QSettings::NativeFormat);

    mergeSettings(final, &systemSettings); // 先加载系统级
    mergeSettings(final, &userSettings);   // 再加载用户级（覆盖系统级）
    // 现在 final 包含了合并后的配置，可以继续读取/写入

}

QString QloongifyObject::productKey()
{
    return   final->value("ALIIOT/productKey", "").toString();
}

QString QloongifyObject::deviceName()
{
    return final->value("ALIIOT/deviceName", "").toString();
}

QString QloongifyObject::deviceSecret()
{
    return final->value("ALIIOT/deviceSecret","").toString();
}

QString QloongifyObject::mqttHost()
{
    return final->value("ALIIOT/mqttHost").toString();
}

int QloongifyObject::mqttPort()
{
    return final->value("ALIIOT/mqttPort", 8883).toInt();
}

QSettings *QloongifyObject::getSettings()
{
    return final;
}

void QloongifyObject::mergeSettings(QSettings *target, const QSettings *source) {
    foreach (const QString &key, source->allKeys()) {
        target->setValue(key, source->value(key));
    }
}
