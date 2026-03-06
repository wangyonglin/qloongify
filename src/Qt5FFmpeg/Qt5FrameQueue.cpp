#include "Qt5FrameQueue.h"
#include <QThread>

Qt5FrameQueue::Qt5FrameQueue(QObject *parent,int maxSize)
    : QObject{parent}
    ,m_maxSize(maxSize)
    , m_abort(false) {
    if (m_maxSize <= 0) {
        m_maxSize = 30; // Default size
    }
}

Qt5FrameQueue::~Qt5FrameQueue() {
    clear();
}

bool Qt5FrameQueue::enqueue(const Qt5FrameObject& frame) {
    QMutexLocker locker(&m_mutex);

    // Wait if queue is full
    while (m_queue.size() >= m_maxSize && !m_abort) {
        m_notFull.wait(&m_mutex);
    }

    if (m_abort) {
        return false;
    }

    m_queue.enqueue(frame);

    // Signal that queue is not empty
    m_notEmpty.wakeOne();

    return true;
}



bool Qt5FrameQueue::dequeue(Qt5FrameObject& result) {
    QMutexLocker locker(&m_mutex);

    // Wait if queue is empty
    while (m_queue.isEmpty() && !m_abort) {
        m_notEmpty.wait(&m_mutex);
    }

    if (m_abort || m_queue.isEmpty()) {
        return false;
    }

    result = m_queue.dequeue();

    // Signal that queue is not full
    m_notFull.wakeOne();

    return true;
}

bool Qt5FrameQueue::tryDequeue(Qt5FrameObject& result, int timeoutMs) {
    QMutexLocker locker(&m_mutex);

    // Wait with timeout if queue is empty
    if (m_queue.isEmpty()) {
        if (timeoutMs == 0) {
            return false; // Non-blocking
        } else if (timeoutMs > 0) {
            if (!m_notEmpty.wait(&m_mutex, static_cast<unsigned long>(timeoutMs))) {
                return false; // Timeout
            }
        } else {
            // timeoutMs < 0 means wait indefinitely
            while (m_queue.isEmpty() && !m_abort) {
                m_notEmpty.wait(&m_mutex);
            }
        }
    }

    if (m_abort || m_queue.isEmpty()) {
        return false;
    }

    result = m_queue.dequeue();

    // Signal that queue is not full
    m_notFull.wakeOne();

    return true;
}

int Qt5FrameQueue::size() const {
    QMutexLocker locker(&m_mutex);
    return m_queue.size();
}

bool Qt5FrameQueue::isEmpty() const {
    QMutexLocker locker(&m_mutex);
    return m_queue.isEmpty();
}

bool Qt5FrameQueue::isFull() const {
    QMutexLocker locker(&m_mutex);
    return m_queue.size() >= m_maxSize;
}

int Qt5FrameQueue::maxSize() const {
    QMutexLocker locker(&m_mutex);
    return m_maxSize;
}

void Qt5FrameQueue::clear() {
    QMutexLocker locker(&m_mutex);
    m_abort = true;

    // Wake all waiting threads
    m_notEmpty.wakeAll();
    m_notFull.wakeAll();

    // Clear queue
    while (!m_queue.isEmpty()) {
        Qt5FrameObject frame = m_queue.dequeue();
        frame.release();
    }
}

void Qt5FrameQueue::waitForData(int timeoutMs) {
    QMutexLocker locker(&m_mutex);
    if (m_queue.isEmpty() && !m_abort) {
        if (timeoutMs >= 0) {
            m_notEmpty.wait(&m_mutex, static_cast<unsigned long>(timeoutMs));
        } else {
            while (m_queue.isEmpty() && !m_abort) {
                m_notEmpty.wait(&m_mutex);
            }
        }
    }
}

void Qt5FrameQueue::waitForEmpty(int timeoutMs) {
    QMutexLocker locker(&m_mutex);
    if (!m_queue.isEmpty() && !m_abort) {
        if (timeoutMs >= 0) {
            // Calculate end time
            QDeadlineTimer deadline(timeoutMs);
            while (!m_queue.isEmpty() && !m_abort && !deadline.hasExpired()) {
                m_notFull.wait(&m_mutex, 100); // Check every 100ms
            }
        } else {
            while (!m_queue.isEmpty() && !m_abort) {
                m_notFull.wait(&m_mutex);
            }
        }
    }
}

void Qt5FrameQueue::setMaxSize(int maxSize) {
    QMutexLocker locker(&m_mutex);
    if (maxSize > 0) {
        m_maxSize = maxSize;
        // Wake threads if queue is no longer full
        if (m_queue.size() < m_maxSize) {
            m_notFull.wakeAll();
        }
    }
}
