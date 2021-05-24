#include "control.h"
#include<QDebug>
Control::Control(QObject *parent) : QObject(parent)
{
    loop = new QEventLoop;
    myt = new CrawlerThread();
    //创建子线程
    qth =new QThread(this);
    myt->moveToThread(qth);
    //启动
    connect(this,&Control::Starts,myt,&CrawlerThread::ThreadRun);
    //结束
    connect(myt,&CrawlerThread::ThreadFinally,this,
            [=]()
    {
        loop->quit();
    });
 //显示字
  connect(myt,&CrawlerThread::ThreadMess,this,
          [=](QString mess)
  {

        openimage.Message(mess);
  });

}

void Control::Begin()
{
    qth->start();//启动线程
    emit Starts();
}

void Control::Loops()
{
    loop->exec();
}

void Control::Close()
{
    qth->quit();
    qth->wait();
}
