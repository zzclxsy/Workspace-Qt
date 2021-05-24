#include "dataacquire.h"
#include<QDebug>
#include<QRegExp>
#include <QFile>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QSqlRecord>
DataAcquire::DataAcquire(QObject *parent) : QObject(parent)
{

}

void DataAcquire::StartAcquire()
{
    //创建数据库
    database.CreateDatabase();
    QByteArray replydata;
    QString url ="https://voice.baidu.com/act/newpneumonia/newpneumonia/?from=osari_pc_3";
    QMap<QString, QString> headers;
    headers.insert("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.135 Safari/537.36");
    headers.insert("QNetworkRequest::UserAgentHeader", "RT-Thread ART");

    bool isok = hp.getSyn(url, headers, replydata);
    //数据表模型
    database.db.open();
    QSqlTableModel *model = new QSqlTableModel();
    model ->setEditStrategy(QSqlTableModel::OnFieldChange);
    QString datatime;
    QString NewDataTime;
    if (isok == true)
    {
        //获取上一次更新日期
        if(!database.createTable("mapLastUpdatedTime","id integer primary key autoincrement not null,type varchar,data varchar"))
        {
            qDebug()<<"创建失败";
            return;
        }

        model->setTable("mapLastUpdatedTime");
        model->select();
        //先获取数据库更新日期
        datatime =  model->record(0).value(2).toString();

        int nRowCnt = model->rowCount();
        while (nRowCnt) {
            nRowCnt--;
            model->removeRow(nRowCnt);

        }

        //获取更新日期
        {
            QRegExp rx("\"mapLastUpdatedTime\":\"(.*)\"");
            //非贪婪模式
            rx.setMinimal(true);
            int pos = rx.indexIn(replydata);
            if (pos > -1) {
                QString value = rx.cap(1);
                qDebug()<<value;
                NewDataTime = value;
                database.insertData("mapLastUpdatedTime",QString("'%1','%2','%3'").arg(0).arg("mapLastUpdatedTime").arg(value));
                }
            }
        }
        //和数据库更新日期进行对比，如果一样就不爬取，不一样就爬取
    if(datatime == NewDataTime)
    {
        qDebug()<<"数据库是最新的";
        return;
    }
  //***************************************************************************************************************************************
        if(!database.createTable("summaryDataIn","id integer primary key autoincrement not null,type varchar,data varchar"))
        {
            qDebug()<<"创建失败";
            return;
        }

        model->setTable("summaryDataIn");
        model->select();
        int nRowCnt = model->rowCount();
        while (nRowCnt) {
            nRowCnt--;
            model->removeRow(nRowCnt);

        }
        //提取国内总体情况
        //获取summaryDataIn里面的数据
        {
            QRegExp rx("\"summaryDataIn\":[{](.*)[}]");
            //非贪婪模式
            rx.setMinimal(true);
            int pos = rx.indexIn(replydata);
            if (pos > -1) {
                QString value = rx.cap(1);
                //qDebug()<<value;
                //进一步提取数据
                QRegExp rx1("\"(.*)\":\"(.*)\"");
                rx1.setMinimal(true);
                int pos1=0;
                int count=0;
                while ((pos1=rx1.indexIn(value,pos1)) != -1) {
                    pos1 += rx1.matchedLength();
                    //qDebug()<<rx1.cap(1)<<":"<<rx1.cap(2)<<",";
                    database.insertData("summaryDataIn",QString("'%1','%2','%3'").arg(count).arg(rx1.cap(1)).arg(rx1.cap(2)));
                    count++;
                }
            }
        }
        //********************************************************************************************************************************************************
        if(!database.createTable("summaryDataOut","id integer primary key autoincrement not null,type varchar,data varchar"))
        {
            qDebug()<<"创建失败";
            return;
        }

        model->setTable("summaryDataOut");
        model->select();
        nRowCnt = model->rowCount();
        while (nRowCnt) {
            nRowCnt--;
            model->removeRow(nRowCnt);
        }

        //提取国外总体情况
        //获取summaryDataOut里面的数据
        {
            QRegExp rx("\"summaryDataOut\":[{](.*)[}]");
            //非贪婪模式
            rx.setMinimal(true);
            int pos = rx.indexIn(replydata);
            if (pos > -1) {
                QString value = rx.cap(1);
                //qDebug()<<value;
                //进一步提取数据
                QRegExp rx1("\"(.*)\":\"(.*)\"");
                rx1.setMinimal(true);
                int pos1=0;
                int count =0;
                while ((pos1=rx1.indexIn(value,pos1)) != -1) {
                    pos1 += rx1.matchedLength();
                    //qDebug()<<rx1.cap(1)<<":"<<rx1.cap(2)<<",";
                    database.insertData("summaryDataOut",QString("'%1','%2','%3'").arg(count).arg(rx1.cap(1)).arg(rx1.cap(2)));
                    count++;
                }
            }
        }
        //************************************************************************************************************************************************************
        if(!database.createTable("topAddCountry","id integer primary key autoincrement not null,type varchar,data varchar"))
        {
            qDebug()<<"创建失败";
            return;
        }

        model->setTable("topAddCountry");
        model->select();
        nRowCnt = model->rowCount();
        while (nRowCnt) {
            nRowCnt--;
            model->removeRow(nRowCnt);

        }

        //新增国家top10
        {
            QRegExp rx("\"topAddCountry\":[[](.*)[]]");
            //非贪婪模式
            rx.setMinimal(true);
            int pos = rx.indexIn(replydata);
            if (pos > -1) {
                QString value = rx.cap(1);
                //qDebug()<<value;
                //进一步提取数据
                QRegExp rx1("[{]\"(.*)\":\"(.*)\",\"(.*)\":(.*)[}]");
                rx1.setMinimal(true);
                int pos1=0;
                int count =0;
                while ((pos1=rx1.indexIn(value,pos1)) != -1) {
                    pos1 += rx1.matchedLength();

                    //将Unicode转为中文
                    QString filename = rx1.cap(2);
                    do {
                        int idx = filename.indexOf("\\u");
                        QString strHex = filename.mid(idx, 6);
                        strHex = strHex.replace("\\u", QString());
                        int nHex = strHex.toInt(0, 16);
                        filename.replace(idx, 6, QChar(nHex));
                    } while (filename.indexOf("\\u") != -1);
                    //qDebug()<<rx1.cap(1)<<":"<<filename<<","<<rx1.cap(3)<<":"<<rx1.cap(4);
                    database.insertData("topAddCountry",QString("'%1','%2','%3'").arg(count).arg(filename).arg(rx1.cap(4)));
                    count++;
                }
            }
        }
        //****************************************************************************************************************************************************************
        if(!database.createTable("topOverseasInput","id integer primary key autoincrement not null,type varchar,data varchar"))
        {
            qDebug()<<"创建失败";
            return;
        }

        model->setTable("topOverseasInput");
        model->select();
        nRowCnt = model->rowCount();
        while (nRowCnt) {
            nRowCnt--;
            model->removeRow(nRowCnt);

        }

        //境外输入省级top10
        {
            QRegExp rx("\"topOverseasInput\":[[](.*)[]]");
            //非贪婪模式
            rx.setMinimal(true);
            int pos = rx.indexIn(replydata);
            if (pos > -1) {
                QString value = rx.cap(1);
                // qDebug()<<value;
                //进一步提取数据
                QRegExp rx1("[{]\"(.*)\":\"(.*)\",\"(.*)\":(.*)[}]");
                rx1.setMinimal(true);
                int pos1=0;
                int count =0;
                while ((pos1=rx1.indexIn(value,pos1)) != -1) {
                    pos1 += rx1.matchedLength();

                    //将Unicode转为中文
                    QString filename = rx1.cap(2);
                    do {
                        int idx = filename.indexOf("\\u");
                        QString strHex = filename.mid(idx, 6);
                        strHex = strHex.replace("\\u", QString());
                        int nHex = strHex.toInt(0, 16);
                        filename.replace(idx, 6, QChar(nHex));
                    } while (filename.indexOf("\\u") != -1);
                    //qDebug()<<rx1.cap(1)<<":"<<filename<<","<<rx1.cap(3)<<":"<<rx1.cap(4);
                    database.insertData("topOverseasInput",QString("'%1','%2','%3'").arg(count).arg(filename).arg(rx1.cap(4)));
                    count++;
                }
            }
        }
        //********************************************************************************************************************************************************
        if(!database.createTable("asymptomaticTopProvince","id integer primary key autoincrement not null,type varchar,data varchar"))
        {
            qDebug()<<"创建失败";
            return;
        }

        model->setTable("asymptomaticTopProvince");
        model->select();
        nRowCnt = model->rowCount();
        while (nRowCnt) {
            nRowCnt--;
            model->removeRow(nRowCnt);

        }

        //无症状省级top10
        {
            QRegExp rx("\"asymptomaticTopProvince\":[[](.*)[]]");
            //非贪婪模式
            rx.setMinimal(true);
            int pos = rx.indexIn(replydata);
            if (pos > -1) {
                QString value = rx.cap(1);
                // qDebug()<<value;
                //进一步提取数据
                QRegExp rx1("[{]\"(.*)\":\"(.*)\",\"(.*)\":(.*)[}]");
                rx1.setMinimal(true);
                int pos1=0;
                int count =0;
                while ((pos1=rx1.indexIn(value,pos1)) != -1) {
                    pos1 += rx1.matchedLength();

                    //将Unicode转为中文
                    QString filename = rx1.cap(2);
                    do {
                        int idx = filename.indexOf("\\u");
                        QString strHex = filename.mid(idx, 6);
                        strHex = strHex.replace("\\u", QString());
                        int nHex = strHex.toInt(0, 16);
                        filename.replace(idx, 6, QChar(nHex));
                    } while (filename.indexOf("\\u") != -1);
                    //qDebug()<<rx1.cap(1)<<":"<<filename<<","<<rx1.cap(3)<<":"<<rx1.cap(4);
                    database.insertData("asymptomaticTopProvince",QString("'%1','%2','%3'").arg(count).arg(filename).arg(rx1.cap(4)));
                    count++;
                }
            }
        }
        //*****************************************************************************************************************************************
        if(!database.createTable("newAddTopProvince","id integer primary key autoincrement not null,type varchar,data varchar,overseasInput varchar"))
        {
            qDebug()<<"创建失败";
            return;
        }

        model->setTable("newAddTopProvince");
        model->select();
        nRowCnt = model->rowCount();
        while (nRowCnt) {
            nRowCnt--;
            model->removeRow(nRowCnt);

        }
        //新增确诊省级top10
        {
            QRegExp rx("\"newAddTopProvince\":[[](.*)[]]");
            //非贪婪模式
            rx.setMinimal(true);
            int pos = rx.indexIn(replydata);
            if (pos > -1) {
                QString value = rx.cap(1);
                //qDebug()<<value;
                //进一步提取数据
                QRegExp rx1("[{]\"(.*)\":\"(.*)\",\"(.*)\":(.*),\"(.*)\":(.*)[}]");
                rx1.setMinimal(true);
                int pos1=0;
                int count =0;
                while ((pos1=rx1.indexIn(value,pos1)) != -1) {
                    pos1 += rx1.matchedLength();

                    //将Unicode转为中文
                    QString filename = rx1.cap(2);
                    do {
                        int idx = filename.indexOf("\\u");
                        QString strHex = filename.mid(idx, 6);
                        strHex = strHex.replace("\\u", QString());
                        int nHex = strHex.toInt(0, 16);
                        filename.replace(idx, 6, QChar(nHex));
                    } while (filename.indexOf("\\u") != -1);
                    //qDebug()<<rx1.cap(1)<<":"<<filename<<","<<rx1.cap(3)<<":"<<rx1.cap(4)<<rx1.cap(5)<<":"<<rx1.cap(6);
                    database.insertData("newAddTopProvince",QString("'%1','%2','%3','%4'").arg(count).arg(filename).arg(rx1.cap(4)).arg(rx1.cap(6)));
                    count++;

                }
            }
        }
        //************************************************************************************************************************************************************
        if(!database.createTable("allForeignTrend","id integer primary key autoincrement not null,type varchar,data varchar"))
        {
            qDebug()<<"创建失败";
            return;
        }

        model->setTable("allForeignTrend");
        model->select();
        nRowCnt = model->rowCount();
        while (nRowCnt) {
            nRowCnt--;
            model->removeRow(nRowCnt);

        }
        //国内/国外新增确诊 趋势
        {
            QRegExp rx("\"allForeignTrend\":(.*)\"topAddCountry\"");
            //非贪婪模式
            rx.setMinimal(true);
            int pos = rx.indexIn(replydata);
            if (pos > -1) {
                QString value = rx.cap(1);
                // qDebug()<<value;
                //进一步提取数据
                QRegExp rx1("\"(updateDate)\":[[](.*)[]]");
                rx1.setMinimal(true);
                rx1.indexIn(value);
                //updateDate数据
                //qDebug()<<rx1.cap(1)<<rx1.cap(2);
                int count=0;
                database.insertData("allForeignTrend",QString("'%1','%2','%3'").arg(count).arg(rx1.cap(1)).arg(rx1.cap(2)));
                count++;

                QRegExp rx2("\"(name)\":\"(.*)\",\"(data)\":[[](.*)[]]");
                rx2.setMinimal(true);
                int pos1=0;
                while ((pos1=rx2.indexIn(value,pos1)) != -1) {
                    pos1 += rx2.matchedLength();

                    //将Unicode转为中文
                    QString filename = rx2.cap(2);
                    do {
                        int idx = filename.indexOf("\\u");
                        QString strHex = filename.mid(idx, 6);
                        strHex = strHex.replace("\\u", QString());
                        int nHex = strHex.toInt(0, 16);
                        filename.replace(idx, 6, QChar(nHex));
                    } while (filename.indexOf("\\u") != -1);
                    //list数据
                    //qDebug()<<rx2.cap(1)<<":"<<filename<<","<<rx2.cap(3)<<":"<<rx2.cap(4);
                    database.insertData("allForeignTrend",QString("'%1','%2','%3'").arg(count).arg(filename).arg(rx2.cap(4)));
                    count++;
                }

            }
        }
        //******************************************************************************************************************************************
        if(!database.createTable("trend","id integer primary key autoincrement not null,type varchar,data varchar"))
        {
            qDebug()<<"创建失败";
            return;
        }

        model->setTable("trend");
        model->select();
        nRowCnt = model->rowCount();
        while (nRowCnt) {
            nRowCnt--;
            model->removeRow(nRowCnt);

        }
        //国内 累计确诊 /现有疑似 /治愈 /死亡 /新增确诊 /累计境外输入 /新增境外输入

        {
            QRegExp rx("\"trend\":(.*)\"foreignLastUpdatedTime\"");
            //非贪婪模式
            rx.setMinimal(true);
            int pos = rx.indexIn(replydata);
            if (pos > -1) {
                QString value = rx.cap(1);
                // qDebug()<<value;
                //进一步提取数据
                QRegExp rx1("\"(updateDate)\":[[](.*)[]]");
                rx1.setMinimal(true);
                rx1.indexIn(value);
                //updateDate数据
                //qDebug()<<rx1.cap(1)<<rx1.cap(2);
                int count=0;
                database.insertData("trend",QString("'%1','%2','%3'").arg(count).arg(rx1.cap(1)).arg(rx1.cap(2)));
                count++;

                QRegExp rx2("\"(name)\":\"(.*)\",\"(data)\":[[](.*)[]]");
                rx2.setMinimal(true);
                int pos1=0;
                while ((pos1=rx2.indexIn(value,pos1)) != -1) {
                    pos1 += rx2.matchedLength();

                    //将Unicode转为中文
                    QString filename = rx2.cap(2);
                    do {
                        int idx = filename.indexOf("\\u");
                        QString strHex = filename.mid(idx, 6);
                        strHex = strHex.replace("\\u", QString());
                        int nHex = strHex.toInt(0, 16);
                        filename.replace(idx, 6, QChar(nHex));
                    } while (filename.indexOf("\\u") != -1);
                    //list数据
                    //qDebug()<<rx2.cap(1)<<":"<<filename<<","<<rx2.cap(3)<<":"<<rx2.cap(4);
                    database.insertData("trend",QString("'%1','%2','%3'").arg(count).arg(filename).arg(rx2.cap(4)));
                    count++;
                }

            }

        }
        //**********************************************************************************************************************************************************province
        //设置省份索引
        if(!database.createTable("province","id integer primary key autoincrement not null,type varchar"))
        {
            qDebug()<<"创建失败";
            return;
        }

        model->setTable("province");
        model->select();
        nRowCnt = model->rowCount();
        while (nRowCnt) {
            nRowCnt--;
            model->removeRow(nRowCnt);

        }
        int ProvinceCount=0;
        //国内各省份具体数据
        {
            QRegExp rx("\"caseList\":(.*)\"caseOutsideList\"");
            //非贪婪模式
            rx.setMinimal(true);
            int pos = rx.indexIn(replydata);
            if (pos > -1) {
                QString value = rx.cap(1);
                //qDebug()<<value;
                //进一步提取数据

                QRegExp rx1("[{](\"confirmed\".*)[}],[{]\"confirmed\"");
                rx1.setMinimal(true);
                int pos=0;
                while ((pos =  rx1.indexIn(value,pos)) != -1) {
                    pos += rx1.matchedLength()-13;
                    //qDebug()<<rx1.cap(1);qDebug()<<"   ";

                    QRegExp rx2("\"(.*)\":\"(.*)\"");
                    rx2.setMinimal(true);
                    int pos1=0;
                    bool next_is_city = false;
                    QSqlRecord rec;
                    int count=0;
                    QString jwsr;//境外输入
                    QMap<QString,QString> map;
                    while ((pos1=rx2.indexIn(rx1.cap(1),pos1)) != -1) {
                        pos1 += rx2.matchedLength();

                        //将省份的Unicode 转为中文
                        if(rx2.cap(1) == "area")
                        {
                            //将Unicode转为中文
                            QString filename = rx2.cap(2);
                            do {
                                int idx = filename.indexOf("\\u");
                                QString strHex = filename.mid(idx, 6);
                                strHex = strHex.replace("\\u", QString());
                                int nHex = strHex.toInt(0, 16);
                                filename.replace(idx, 6, QChar(nHex));
                            } while (filename.indexOf("\\u") != -1);
                            // qDebug()<<rx2.cap(1)<<"==="<<filename;
                            //一个省一张表
                             database.insertData("province",QString("'%1','%2'").arg(ProvinceCount).arg(filename));
                             ProvinceCount++;

                            if(!database.createTable(QString("%1").arg(filename),"id int,area varchar,confirmed varchar,"
                                                     "died varchar,crued varchar,relativeTime varchar,confirmedRelative varchar,diedRelative varchar,curedRelative varchar,asymptomaticRelative varchar,"
                                                     "asymptomatic varchar,curConfirm varchar,curConfirmRelative varchar,icuDisable varchar,cityCode varchar"))
                            {
                                qDebug()<<"创建失败";
                                return;
                            }

                            //每次打开软件先 清空表 ，然后重写写入新数据
                            model->setTable(QString("%1").arg(filename));
                            model->select();
                            nRowCnt = model->rowCount();
                            while (nRowCnt) {
                                nRowCnt--;
                                model->removeRow(nRowCnt);

                            }
                            count =0;
                            QMap<QString,QString>::iterator it = map.begin();
                            rec = model->record();
                            while (it!=map.end()) {
                                rec.setValue(QString("%1").arg(it.key()),QString("%1").arg(it.value()));
                                //qDebug()<<it.key()<<"...."<<it.value()<<model->tableName();
                                it++;
                            }
                            rec.setValue(QString("%1").arg("area"),QString("%1").arg(filename));
                            rec.setValue(QString("%1").arg("id"),QString("%1").arg(count));
                            //将当前行所有数据写入到rec里面，最后插入到model离
                            model->insertRecord(count, rec);
                            //不用clear这个rec，如果clear这个rec又得重新 model->record()
                            rec.clear();
                            rec = model->record();
                            //next_is_city == true意味着 下面数据都是该省份城市的数据，这么做是方便处理数据
                            next_is_city = true;

                        }else {

                            //这个if里面都是城市数据 else if 是省份数据
                            if(next_is_city){
                                //该省份第一个城市
                                if(rx2.cap(1).contains("subList",Qt::CaseSensitive)){
                                    //将城市的Unicode转为中文
                                    QString filename = rx2.cap(2);
                                    do {
                                        int idx = filename.indexOf("\\u");
                                        QString strHex = filename.mid(idx, 6);
                                        strHex = strHex.replace("\\u", QString());
                                        int nHex = strHex.toInt(0, 16);
                                        filename.replace(idx, 6, QChar(nHex));
                                    } while (filename.indexOf("\\u") != -1);
                                    // qDebug()<<"city begin"<<"==="<<filename;
                                    rec.setValue(QString("%1").arg("area"),QString("%1").arg(filename));

                                }else{

                                    //将城市的Unicode转为中文

                                    if(rx2.cap(1) == "city"){
                                        //获取下一个城市的数据前，先将前面城市的数据提交
                                        count++;
                                        rec.setValue(QString("%1").arg("id"),QString("%1").arg(count));
                                        model->insertRecord(count, rec);
                                        rec.clear();
                                        rec = model->record();
                                        QString filename = rx2.cap(2);
                                        do {
                                            int idx = filename.indexOf("\\u");
                                            QString strHex = filename.mid(idx, 6);
                                            strHex = strHex.replace("\\u", QString());
                                            int nHex = strHex.toInt(0, 16);
                                            filename.replace(idx, 6, QChar(nHex));
                                        } while (filename.indexOf("\\u") != -1);
                                        // qDebug()<<rx2.cap(1)<<"==="<<filename;
                                        jwsr = filename;
                                        rec.setValue(QString("%1").arg("area"),QString("%1").arg(filename));
                                    }else {
                                        //qDebug()<<rx2.cap(1)<<"==="<<rx2.cap(2);
                                        rec.setValue(QString("%1").arg(rx2.cap(1)),QString("%1").arg(rx2.cap(2)));
                                        if(jwsr == "境外输入"&&rx2.cap(1) == "curConfirm")
                                        {
                                            count++;
                                            rec.setValue(QString("%1").arg("id"),QString("%1").arg(count));
                                            model->insertRecord(count, rec);
                                        }
                                    }
                                }
                            }else {//这是省份数据
                                //这里得用map，因为是先有数据后有表
                                // qDebug()<<rx2.cap(1)<<"==="<<rx2.cap(2);
                                map[rx2.cap(1)] = rx2.cap(2);

                            }


                        }




                    }
                }
            }
        }
    }





