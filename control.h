#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include"crawlerthread.h"
#include"openimage.h"
#include <QEventLoop>
class Control : public QObject
{
    Q_OBJECT
public:
    explicit Control(QObject *parent = nullptr);

    CrawlerThread *myt;
    QThread *qth;
    QEventLoop *loop;
    void Begin();
    void Loops();
    void Close();
    OpenImage openimage;
signals:
    void Starts();
    void Breaks();

};

#endif // CONTROL_H
