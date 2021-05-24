#include "database.h"
#include <QDebug>
#include <QSqlError>
#include <QSslSocket>
Database::Database(QObject *parent) : QObject(parent)
{

}

bool Database::CreateDatabase()
{
	qDebug()<<"QSslSocket="<<QSslSocket::sslLibraryBuildVersionString();
	qDebug() << "OpenSSL支持情况:" << QSslSocket::supportsSsl();
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("crawler.db");
    if(db.open())
    {
          qDebug()<<"打开成功";
          query = new QSqlQuery(db);
          return true;
    }else {
        return false;
    }


}

void Database::Close()
{
    db.close();
    db.removeDatabase("crawler.db");
}

bool Database::createTable(QString table_name,QString table_header)
{
    QString str = QString("create table if not exists %1(%2)").arg(table_name).arg(table_header);
    bool success = query->exec(str);
    if(!success){
        qDebug()<<"新建失败";
        QSqlError lastError = query->lastError();
        qDebug() <<"插入失败:"<< lastError.driverText() << lastError.databaseText();
        return false;
    }
    return true;

}

bool Database::insertData(QString table_name, QString data)
{
    QString str = QString("insert into %1 values(%2)").arg(table_name).arg(data);
    bool success = query->exec(str);
    if(!success){
        qDebug()<<"插入失败";
        QSqlError lastError = query->lastError();
        qDebug() <<"插入失败:"<< lastError.driverText() << lastError.databaseText();
        return false;
    }
    return true;
}


