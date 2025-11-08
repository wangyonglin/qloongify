#include "qsherpaonnxthreader.h"


QSherpaOnnxThreader::QSherpaOnnxThreader(QObject *parent)
    : QThread(parent),
    pauseFlag(false),
    stopFlag(false)
{}

QSherpaOnnxThreader::~QSherpaOnnxThreader()
{

}

QSherpaOnnxThreader::State QSherpaOnnxThreader::state() const
{
    State s = Stoped;
    if (!QThread::isRunning())
    {
        s = Stoped;
    }
    else if (QThread::isRunning() && pauseFlag)
    {
        s = Paused;
    }
    else if (QThread::isRunning() && (!pauseFlag))
    {
        s = Running;
    }
    return s;
}

void QSherpaOnnxThreader::start(Priority pri)
{
    QThread::start(pri);
}

void QSherpaOnnxThreader::stop()
{
    if (QThread::isRunning())
    {
        stopFlag = true;
        condition.wakeAll();
        QThread::quit();
        QThread::wait();
    }
}

void QSherpaOnnxThreader::pause()
{
    if (QThread::isRunning())
    {
        pauseFlag = true;
    }
}

void QSherpaOnnxThreader::resume()
{
    if (QThread::isRunning())
    {
        pauseFlag = false;
        condition.wakeAll();
    }
}

void QSherpaOnnxThreader::run()
{
    qDebug() << "enter thread : " << QThread::currentThreadId();
    while (!stopFlag)
    {

        loopRunnable(); // do something...

        if (pauseFlag)
        {
            mutex.lock();
            condition.wait(&mutex);
            mutex.unlock();
        }
    }
    pauseFlag = false;
    stopFlag = false;

    qDebug() << "exit thread : " << QThread::currentThreadId();
}
