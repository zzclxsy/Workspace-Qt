#ifndef DATAACQUIRE_H
#define DATAACQUIRE_H
#include"HttpLib/http.h"
#include"database.h"
#include <QObject>
#include <QMap>
class DataAcquire : public QObject
{
    Q_OBJECT
public:
    explicit DataAcquire(QObject *parent = nullptr);
public:
    //开始采集数据并且将数据存入数据库
    void StartAcquire();
    http hp;
private:
    Database database;
};

#endif // DATAACQUIRE_H
