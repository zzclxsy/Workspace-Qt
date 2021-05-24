#include "crawlerthread.h"
#include<QDebug>
CrawlerThread::CrawlerThread(QObject *parent) : QObject(parent)
{

}

void CrawlerThread::ThreadRun()
{

    emit ThreadMess(QString("正在进行初始化，请耐心等待"));
    dataacquire.StartAcquire();
    emit ThreadFinally();  //发送完成信号


}
