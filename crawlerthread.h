#ifndef CRAWLERTHREAD_H
#define CRAWLERTHREAD_H

#include <QObject>
#include <QThread>
#include"dataacquire.h"
class CrawlerThread : public QObject
{
    Q_OBJECT
public:
    explicit CrawlerThread(QObject *parent = nullptr);

    void ThreadRun();
signals:
    void ThreadFinally();
    void ThreadMess(QString);
private:
    DataAcquire dataacquire;
};

#endif // CRAWLERTHREAD_H
