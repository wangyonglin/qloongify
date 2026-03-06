#ifndef QT5THREADOBJECT_H
#define QT5THREADOBJECT_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <atomic>

class Qt5ThreadObject : public QThread
{
    Q_OBJECT

public:
    explicit Qt5ThreadObject(QObject *parent = nullptr);
public slots:
    // 启动线程（重写以添加自定义逻辑）
    void start(Priority priority = InheritPriority);
    // 停止线程（安全停止）
    void stop();
    // 检查线程是否正在工作（运行中且未被请求停止）
    void pause();
    void resume();
    bool state();
    // 设置工作状态，如果为false，则线程会等待
    void reset();
protected:
    // 线程运行函数（重写基类）
    void run() override;
    // 纯虚函数，子类必须实现，执行实际工作
    virtual void thread() = 0;

private:
    // 停止请求标志
    std::atomic<bool> stopFlag;
    // 工作标志，表示是否应该执行工作
    std::atomic<bool> pauseFlag;

    // 互斥锁和条件变量，用于等待工作
    QMutex mutex;
    QWaitCondition waitFlag;
    unsigned long timeout;
};

#endif // QT5THREADOBJECT_H
