#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);

public:
    QSqlDatabase db;
    QSqlQuery *query;
    //创建数据库SQLIT
    bool CreateDatabase();
    //关闭数据库
    void Close();
    //创建表
    bool createTable(QString table_name,QString table_header);
    //插入数据
    bool insertData(QString table_name,QString data);

};

#endif // DATABASE_H
