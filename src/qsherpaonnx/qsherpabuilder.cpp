#include "qsherpabuilder.h"
#include <QDebug>



QSherpaBuilder::QSherpaBuilder(QObject *parent, const QAudioFormat &format)
    :QObject(),qaudioformat(format)
{

}

QAudioFormat QSherpaBuilder::getAudioFormat()
{
    return qaudioformat;
}

std::vector<float> QSherpaBuilder::convertFloatVector(const QByteArray &byteArray) {
    std::vector<float> floatVector;

    if (byteArray.isEmpty()) {
        return floatVector;
    }

    // 根据音频格式确定转换方式
    const int bytesPerSample = qaudioformat.sampleSize() / 8;
    const int sampleCount = byteArray.size() / bytesPerSample;

    if (sampleCount == 0) {
        return floatVector;
    }

    // 预分配内存
    floatVector.reserve(sampleCount);

    bool success = false;

    switch (qaudioformat.sampleSize()) {
    case 16: {
        if (qaudioformat.sampleType() == QAudioFormat::SignedInt) {
            const int16_t *pcmData = reinterpret_cast<const int16_t*>(byteArray.constData());

            // 检查字节序
            if (qaudioformat.byteOrder() == QAudioFormat::LittleEndian) {
                // 小端序，直接转换
                for (int i = 0; i < sampleCount; ++i) {
                    floatVector.push_back(pcmData[i] / 32768.0f);
                }
            } else {
                // 大端序，需要字节交换
                for (int i = 0; i < sampleCount; ++i) {
                    int16_t sample = (pcmData[i] << 8) | (pcmData[i] >> 8);
                    floatVector.push_back(sample / 32768.0f);
                }
            }
            success = true;
        }
        break;
    }

    case 8: {
        if (qaudioformat.sampleType() == QAudioFormat::UnSignedInt) {
            const uint8_t *pcmData = reinterpret_cast<const uint8_t*>(byteArray.constData());
            for (int i = 0; i < sampleCount; ++i) {
                floatVector.push_back((pcmData[i] - 128) / 128.0f);
            }
            success = true;
        } else if (qaudioformat.sampleType() == QAudioFormat::SignedInt) {
            const int8_t *pcmData = reinterpret_cast<const int8_t*>(byteArray.constData());
            for (int i = 0; i < sampleCount; ++i) {
                floatVector.push_back(pcmData[i] / 128.0f);
            }
            success = true;
        }
        break;
    }

    case 32: {
        if (qaudioformat.sampleType() == QAudioFormat::Float) {
            const float *floatData = reinterpret_cast<const float*>(byteArray.constData());
            floatVector.assign(floatData, floatData + sampleCount);
            success = true;
        } else if (qaudioformat.sampleType() == QAudioFormat::SignedInt) {
            const int32_t *pcmData = reinterpret_cast<const int32_t*>(byteArray.constData());
            for (int i = 0; i < sampleCount; ++i) {
                floatVector.push_back(pcmData[i] / 2147483648.0f);
            }
            success = true;
        }
        break;
    }

    case 24: {
        // 24-bit PCM 处理
        const uint8_t *data = reinterpret_cast<const uint8_t*>(byteArray.constData());
        for (int i = 0; i < sampleCount; ++i) {
            int32_t sample;
            if (qaudioformat.byteOrder() == QAudioFormat::LittleEndian) {
                sample = (data[i*3] << 8) | (data[i*3+1] << 16) | (data[i*3+2] << 24);
            } else {
                sample = (data[i*3] << 24) | (data[i*3+1] << 16) | (data[i*3+2] << 8);
            }
            sample >>= 8; // 算术右移进行符号扩展
            floatVector.push_back(sample / 8388608.0f);
        }
        success = true;
        break;
    }

    default:
        qWarning() << "Unsupported sample size:" << qaudioformat.sampleSize();
        break;
    }

    if (!success) {
        qWarning() << "Failed to convert audio data to float vector";
        floatVector.clear();
    }

    return floatVector;
}
