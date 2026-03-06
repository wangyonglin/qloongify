#ifndef QT5FRAMEQUEUE_H
#define QT5FRAMEQUEUE_H

#include "Qt5FrameObject.h"
#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>

class Qt5FrameQueue :public QObject {
    Q_OBJECT
public:
    Qt5FrameQueue(QObject *parent = nullptr,int maxSize = 30);
    ~Qt5FrameQueue();

    // Queue operations
    bool enqueue(const Qt5FrameObject& frame);
    bool dequeue(Qt5FrameObject& result);
    bool tryDequeue(Qt5FrameObject& result, int timeoutMs = 0);

    // Queue status
    int size() const;
    bool isEmpty() const;
    bool isFull() const;
    int maxSize() const;

    // Clear queue
    void clear();

    // Wait for queue to have data or become empty
    void waitForData(int timeoutMs = -1);
    void waitForEmpty(int timeoutMs = -1);

    // Set maximum queue size
    void setMaxSize(int maxSize);

private:
    QQueue<Qt5FrameObject> m_queue;
    mutable QMutex m_mutex;
    QWaitCondition m_notEmpty;
    QWaitCondition m_notFull;
    int m_maxSize;
    bool m_abort;
};

#endif // QT5FRAMEQUEUE_H
