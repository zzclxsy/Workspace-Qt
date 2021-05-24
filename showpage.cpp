#include "showpage.h"
#include "ui_showpage.h"
#include<QWheelEvent>
#include <QEvent>
#include <QDialog>

ShowPage::ShowPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ShowPage)
{
    ui->setupUi(this);

    Gui_Set();
    //第一个参数是数据库类型 ，第二个是连接名，不写会有一个默认的连接名。因为子线程已经打开了数据库，并且使用了默认连接名，所以主线程要换一个连接名来连接这个数据库。一个数据库可以有多个连接
    db = QSqlDatabase::addDatabase("QSQLITE","main");
    db.setDatabaseName("crawler.db");
    if(db.open())
    {
          qDebug()<<"打开成功";
          query = new QSqlQuery(db);

    }

    model = new QSqlTableModel(this,db);
    model ->setEditStrategy(QSqlTableModel::OnFieldChange);
    model2 = new QSqlTableModel(this,db);
    model2 ->setEditStrategy(QSqlTableModel::OnFieldChange);
    Get_SummaryDataIn_Data();
    Get_SummaryDataOut_Data();
    Get_NewAddTopProvince_Data();
    Get_AsymptomaticTopProvince_Data();
    Get_TopOverseasInput_Data();
    Get_NewImportFrom_Overseas();
    Get_AccumulationOfOverseasImports();
    Get_TopAddCountry_Data();
    Get_TheNewDiagnosis();
    Get_DiagnosisOfSuspected();
    Get_DiedLive();
    GetforeignTheNewDiagnosis();
    Get_CumulativeDiagnosisAbroad();
    Get_foreignDiedLive();
    Province();
}

ShowPage::~ShowPage()
{
    delete ui;
}

void ShowPage::Gui_Set()
{
       int width = ui->tabWidget->width();
       int tabCount = ui->tabWidget->count();
       int tabWidth = width / tabCount;
       this->setStyleSheet(QString("QTabBar::tab{width:%1px;}QTabBar::tab{height:40px}QTabBar::tab:hover{background-color: rgb(85, 255, 255);}QTabBar::tab:selected{background-color: rgb(255, 255, 127);}").arg(tabWidth));

       tabCount = ui->tabWidget_3->count();
       tabWidth = width / tabCount;
       ui->tabWidget_3->setStyleSheet(QString("QTabBar::tab{width:%1px;}").arg(tabWidth));

       tabCount = ui->tabWidget_4->count();
       tabWidth = width / tabCount;
       ui->tabWidget_4->setStyleSheet(QString("QTabBar::tab{width:%1px;}").arg(tabWidth));
       tabCount = ui->tabWidget_5->count();
       tabWidth = width / tabCount;
       ui->tabWidget_5->setStyleSheet(QString("QTabBar::tab{width:%1px;}").arg(tabWidth));

}

void ShowPage::Get_SummaryDataIn_Data()
{
    model->setTable("summaryDataIn");
    model->select();
    ui->label->setText(QString("现有确诊:%1\n\n+%2").arg(model->record(17).value(2).toString()).arg(model->record(18).value(2).toString()));
    ui->label_2->setText(ui->label_2->text()+QString("%1\n\n+%2").arg(model->record(3).value(2).toString()).arg(model->record(4).value(2).toString()));
    ui->label_3->setText(QString("现有疑似:%1\n\n+%2").arg(model->record(5).value(2).toString()).arg(model->record(8).value(2).toString()));
    ui->label_4->setText(QString("现有重症:%1\n\n+%2").arg(model->record(11).value(2).toString()).arg(model->record(12).value(2).toString()));
    ui->label_5->setText(QString("累计确诊:%1\n\n+%2").arg(model->record(0).value(2).toString()).arg(model->record(7).value(2).toString()));
    ui->label_6->setText(QString("境外输入:%1\n\n+%2").arg(model->record(13).value(2).toString()).arg(model->record(15).value(2).toString()));
    ui->label_7->setText(QString("累计治愈:%1\n\n+%2").arg(model->record(2).value(2).toString()).arg(model->record(9).value(2).toString()));
    ui->label_8->setText(QString("累计死亡:%1\n\n+%2").arg(model->record(1).value(2).toString()).arg(model->record(10).value(2).toString()));
}

void ShowPage::Get_SummaryDataOut_Data()
{
    model->setTable("summaryDataOut");
    model->select();
    ui->label_18->setText(QString("现有确诊:%1\n\n+%2").arg(model->record(2).value(2).toString()).arg(model->record(7).value(2).toString()));
    ui->label_21->setText(ui->label_21->text()+QString("%1\n\n+%2").arg(model->record(0).value(2).toString()).arg(model->record(4).value(2).toString()));
    ui->label_20->setText(QString("累计治愈:%1\n\n+%2").arg(model->record(3).value(2).toString()).arg(model->record(5).value(2).toString()));
    ui->label_23->setText(QString("累计死亡:%1\n\n+%2").arg(model->record(1).value(2).toString()).arg(model->record(6).value(2).toString()));

}

void ShowPage::Get_NewAddTopProvince_Data()
{
    model->setTable("newAddTopProvince");
    model->select();
    //轴的范围，范围小化轴显示不全
    QVector<double> ticks;
    //轴的刻度标签，正常来说，ticks有多少，labels就有多少
    QVector<QString> labels;
    //柱状条的value数据，这里有2个柱状，所以有2个数据
    QVector<double> fossilData;
    QVector<double> nuclearData;
    int Row_All = model->rowCount();
    for (int n=0;n<Row_All;n++) {
        ticks<<(n+1);
        //将数据库里的数据写入到QVector里面
        labels<<model->record(n).value(1).toString();
        fossilData<<model->record(n).value(2).toInt();
        nuclearData<<model->record(n).value(3).toInt();
    }
    //设置坐标轴
    QCPAxis *keyAxis = ui->widget_2->xAxis;
    QCPAxis *valueAxis = ui->widget_2->yAxis;
    //设置柱状条
    CustomBars  *fossil = new CustomBars (keyAxis, valueAxis);  // 使用xAxis作为柱状图的key轴，yAxis作为value轴
    CustomBars  *nuclear  = new CustomBars (keyAxis, valueAxis);  // 使用xAxis作为柱状图的key轴，yAxis作为value轴
    ui->widget_2->legend->setVisible(true);

    //2组数据的柱状 设置柱状的基本信息
    fossil->setAntialiased(false); // 为了更好的边框效果，关闭抗齿锯
    fossil->setName("本土新增"); // 设置柱状图的名字，可在图例中显示
    fossil->setPen(QPen(QColor(0, 168, 140).lighter(130))); // 设置柱状图的边框颜色
    fossil->setBrush(QColor(0, 168, 140));  // 设置柱状图的画刷颜色

    nuclear->setAntialiased(false); // 为了更好的边框效果，关闭抗齿锯
    nuclear->setName("境外输入"); // 设置柱状图的名字，可在图例中显示
    nuclear->setPen(QPen(QColor(0, 10, 140).lighter(130))); // 设置柱状图的边框颜色
    nuclear->setBrush(QColor(0, 10, 140));  // 设置柱状图的画刷颜色

    //为柱状图设置一个文字类型的key轴，ticks决定了轴的范围，而labels决定了轴的刻度文字的显示
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    keyAxis->setTicker(textTicker);        // 设置为文字轴

    keyAxis->setTickLabelRotation(60);     // 轴刻度文字旋转60度
    keyAxis->setSubTicks(false);           // 不显示子刻度
    keyAxis->setTickLength(0, 4);          // 轴内外刻度的长度分别是0,4,也就是轴内的刻度线不显示
    keyAxis->setRange(0,ticks.count()+1);               // 设置范围
    keyAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    valueAxis->setRange(0, fossilData[0]+50);
    valueAxis->setPadding(35);             // 轴的内边距，可以到QCustomPlot之开始（一）看图解
    valueAxis->setLabel("新增确诊数量(本土加境外)");
    valueAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    //设置柱状的数据value
    fossil->setData(ticks, fossilData);
    nuclear->setData(ticks, nuclearData);

    QCPBarsGroup *group = new QCPBarsGroup(ui->widget_2);

    QList<QCPBars*> bars;
    bars << fossil << nuclear;
    foreach (QCPBars *bar, bars) {
      // 设置柱状图的宽度类型为以key坐标轴计算宽度的大小，其实默认就是这种方式
      bar->setWidthType(QCPBars::wtPlotCoords);
      bar->setWidth(bar->width() / bars.size()); // 设置柱状图的宽度大小
      group->append(bar);  // 将柱状图加入柱状图分组中
    }

    group->setSpacingType(QCPBarsGroup::stAbsolute);  // 设置组内柱状图的间距，按像素
    group->setSpacing(2);     // 设置较小的间距值，这样看起来更紧凑
}

void ShowPage::Get_AsymptomaticTopProvince_Data()
{
    model->setTable("asymptomaticTopProvince");
    model->select();
    //轴的范围，范围小化轴显示不全
    QVector<double> ticks;
    //轴的刻度标签，正常来说，ticks有多少，labels就有多少
    QVector<QString> labels;
    //柱状条的value数据，这里有2个柱状，所以有2个数据
    QVector<double> fossilData;
    int Row_All = model->rowCount();
    for (int n=0;n<Row_All;n++) {
        ticks<<(n+1);
        //将数据库里的数据写入到QVector里面
        labels<<model->record(n).value(1).toString();
        fossilData<<model->record(n).value(2).toInt();
    }
    //设置坐标轴
    QCPAxis *keyAxis = ui->widget_3->xAxis;
    QCPAxis *valueAxis = ui->widget_3->yAxis;
    //设置柱状条
    CustomBars  *fossil = new CustomBars (keyAxis, valueAxis);  // 使用xAxis作为柱状图的key轴，yAxis作为value轴
    ui->widget_3->legend->setVisible(true);

    //2组数据的柱状 设置柱状的基本信息
    fossil->setAntialiased(false); // 为了更好的边框效果，关闭抗齿锯
    fossil->setName("省份"); // 设置柱状图的名字，可在图例中显示
    fossil->setPen(QPen(QColor(0, 168, 140).lighter(130))); // 设置柱状图的边框颜色
    fossil->setBrush(QColor(0, 168, 140));  // 设置柱状图的画刷颜色


    //为柱状图设置一个文字类型的key轴，ticks决定了轴的范围，而labels决定了轴的刻度文字的显示
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    keyAxis->setTicker(textTicker);        // 设置为文字轴

    keyAxis->setTickLabelRotation(60);     // 轴刻度文字旋转60度
    keyAxis->setSubTicks(false);           // 不显示子刻度
    keyAxis->setTickLength(0, 4);          // 轴内外刻度的长度分别是0,4,也就是轴内的刻度线不显示
    keyAxis->setRange(0,ticks.count()+1);               // 设置范围
    keyAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    valueAxis->setRange(0, fossilData[0]+50);
    valueAxis->setPadding(35);             // 轴的内边距，可以到QCustomPlot之开始（一）看图解
    valueAxis->setLabel("无症状数量");
    valueAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    //设置柱状的数据value
    fossil->setWidth(fossil->width()/2);
    fossil->setData(ticks, fossilData);

}

void ShowPage::Get_TopOverseasInput_Data()
{
    model->setTable("topOverseasInput");
    model->select();
    //轴的范围，范围小化轴显示不全
    QVector<double> ticks;
    //轴的刻度标签，正常来说，ticks有多少，labels就有多少
    QVector<QString> labels;
    //柱状条的value数据，这里有2个柱状，所以有2个数据
    QVector<double> fossilData;
    int Row_All = model->rowCount();
    for (int n=0;n<Row_All;n++) {
        ticks<<(n+1);
        //将数据库里的数据写入到QVector里面
        labels<<model->record(n).value(1).toString();
        fossilData<<model->record(n).value(2).toInt();
    }
    //设置坐标轴
    QCPAxis *keyAxis = ui->widget_6->xAxis;
    QCPAxis *valueAxis = ui->widget_6->yAxis;
    //设置柱状条
    CustomBars  *fossil = new CustomBars (keyAxis, valueAxis);  // 使用xAxis作为柱状图的key轴，yAxis作为value轴
    ui->widget_6->legend->setVisible(true);

    //2组数据的柱状 设置柱状的基本信息
    fossil->setAntialiased(false); // 为了更好的边框效果，关闭抗齿锯
    fossil->setName("省份"); // 设置柱状图的名字，可在图例中显示
    fossil->setPen(QPen(QColor(0, 168, 140).lighter(130))); // 设置柱状图的边框颜色
    fossil->setBrush(QColor(0, 168, 140));  // 设置柱状图的画刷颜色


    //为柱状图设置一个文字类型的key轴，ticks决定了轴的范围，而labels决定了轴的刻度文字的显示
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    keyAxis->setTicker(textTicker);        // 设置为文字轴

    keyAxis->setTickLabelRotation(60);     // 轴刻度文字旋转60度
    keyAxis->setSubTicks(false);           // 不显示子刻度
    keyAxis->setTickLength(0, 4);          // 轴内外刻度的长度分别是0,4,也就是轴内的刻度线不显示
    keyAxis->setRange(0,ticks.count()+1);               // 设置范围
    keyAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    valueAxis->setRange(0, fossilData[0]+fossilData[0]/10);
    valueAxis->setPadding(35);             // 轴的内边距，可以到QCustomPlot之开始（一）看图解
    valueAxis->setLabel("境外输入数量");
    valueAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    //设置柱状的数据value
    fossil->setWidth(fossil->width()/2);
    fossil->setData(ticks, fossilData);
}

void ShowPage::Get_NewImportFrom_Overseas()
{
    model->setTable("trend");
    model->select();

    QVector<double> date ;
    QVector<double> value;

    QString replydata = model->record(10).value(2).toString()+",";
    QRegExp rx("([0-9]*),");
    //非贪婪模式
    rx.setMinimal(true);
    int pos=0 ;
    int max=0;
    while((pos = rx.indexIn(replydata,pos)) != -1) {
       pos +=rx.matchedLength();
        //存入数据
       value<<rx.cap(1).toInt();
       if((rx.cap(1).toInt()) >max){
           max = rx.cap(1).toInt();
       }
    }

    replydata = model->record(0).value(2).toString();
    QRegExp rx1("\"(.*)\"");
    //非贪婪模式
    rx1.setMinimal(true);
    int pos1=0 ;
    int index=0;
    int year=2020;
    while((pos1 = rx1.indexIn(replydata,pos1)) != -1) {
       pos1 +=rx1.matchedLength();
       QStringList MM_dd =rx1.cap(1).split(".");

        if((MM_dd[0] == "1")&&(MM_dd[1] == "1")){
            year++;
        }
        if(MM_dd[0].toInt()<10){
            MM_dd[0] ="0"+ MM_dd[0];
        }

        if(MM_dd[1].toInt()<10){
            MM_dd[1] ="0"+ MM_dd[1];
        }

        QString curdate = QString("%1").arg(year)+"-" + MM_dd[0] +"-"+ MM_dd[1];


        //存入数据
        date<<QDateTime::fromString(curdate,"yyyy-MM-dd").toTime_t();
        index++;
    }
               //添加图形
               ui->widget_4->addGraph();
               //设置画笔
               ui->widget_4->graph(0)->setPen(QPen(Qt::red));
               ui->widget_4->legend->setVisible(true);
               ui->widget_4->graph(0)->setName("新增境外输入");
               //传入数据
               ui->widget_4->graph(0)->setData(date,value);

               ui->widget_4->xAxis->setRange(date[0],date[0]+24*3600*index);
               ui->widget_4->yAxis->setRange(0,max);

               ui->widget_4->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
               ui->widget_4->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
               ui->widget_4->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom );

               QSharedPointer<QCPAxisTickerDateTime> dateTick(new QCPAxisTickerDateTime);
               dateTick->setDateTimeFormat("yyyy-MM-dd");
               ui->widget_4->xAxis->setTicker(dateTick);

}

void ShowPage::Get_AccumulationOfOverseasImports()
{
    model->setTable("trend");
    model->select();

    QVector<double> date ;
    QVector<double> value;

    QString replydata = model->record(9).value(2).toString()+",";
    QRegExp rx("([0-9]*),");
    //非贪婪模式
    rx.setMinimal(true);
    int pos=0 ;
    int max=0;
    while((pos = rx.indexIn(replydata,pos)) != -1) {
       pos +=rx.matchedLength();
        //存入数据
       value<<rx.cap(1).toInt();
       if((rx.cap(1).toInt()) >max){
           max = rx.cap(1).toInt();
       }
    }

    replydata = model->record(0).value(2).toString();
    QRegExp rx1("\"(.*)\"");
    //非贪婪模式
    rx1.setMinimal(true);
    int pos1=0 ;
    int index=0;
    int year=2020;
    while((pos1 = rx1.indexIn(replydata,pos1)) != -1) {
       pos1 +=rx1.matchedLength();
       QStringList MM_dd =rx1.cap(1).split(".");

        if((MM_dd[0] == "1")&&(MM_dd[1] == "1")){
            year++;
        }
        if(MM_dd[0].toInt()<10){
            MM_dd[0] ="0"+ MM_dd[0];
        }

        if(MM_dd[1].toInt()<10){
            MM_dd[1] ="0"+ MM_dd[1];
        }

        QString curdate = QString("%1").arg(year)+"-" + MM_dd[0] +"-"+ MM_dd[1];


        //存入数据
        date<<QDateTime::fromString(curdate,"yyyy-MM-dd").toTime_t();
        index++;
    }
               //添加图形
               ui->widget_5->legend->setVisible(true);
               ui->widget_5->addGraph();
               //设置画笔
               ui->widget_5->graph(0)->setPen(QPen(Qt::red));
               ui->widget_5->graph(0)->setName("境外输入累计");
               //传入数据
               ui->widget_5->graph(0)->setData(date,value);

               ui->widget_5->xAxis->setRange(date[0],date[0]+24*3600*index);
               ui->widget_5->yAxis->setRange(0,max);

               ui->widget_5->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
               ui->widget_5->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
               ui->widget_5->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom );

               QSharedPointer<QCPAxisTickerDateTime> dateTick(new QCPAxisTickerDateTime);
               dateTick->setDateTimeFormat("yyyy-MM-dd");
               ui->widget_5->xAxis->setTicker(dateTick);
}

void ShowPage::Get_TopAddCountry_Data()
{
    model->setTable("topAddCountry");
    model->select();
    //轴的范围，范围小化轴显示不全
    QVector<double> ticks;
    //轴的刻度标签，正常来说，ticks有多少，labels就有多少
    QVector<QString> labels;
    //柱状条的value数据，这里有2个柱状，所以有2个数据
    QVector<double> fossilData;
    int Row_All = model->rowCount();
    for (int n=0;n<Row_All;n++) {
        ticks<<(n+1);
        //将数据库里的数据写入到QVector里面
        labels<<model->record(n).value(1).toString();
        fossilData<<model->record(n).value(2).toInt();
    }
    //设置坐标轴
    QCPAxis *keyAxis = ui->widget_7->xAxis;
    QCPAxis *valueAxis = ui->widget_7->yAxis;
    //设置柱状条
    CustomBars  *fossil = new CustomBars (keyAxis, valueAxis);  // 使用xAxis作为柱状图的key轴，yAxis作为value轴
    ui->widget_7->legend->setVisible(true);

    //2组数据的柱状 设置柱状的基本信息
    fossil->setAntialiased(false); // 为了更好的边框效果，关闭抗齿锯
    fossil->setName("国家"); // 设置柱状图的名字，可在图例中显示
    fossil->setPen(QPen(QColor(0, 168, 140).lighter(130))); // 设置柱状图的边框颜色
    fossil->setBrush(QColor(0, 168, 140));  // 设置柱状图的画刷颜色


    //为柱状图设置一个文字类型的key轴，ticks决定了轴的范围，而labels决定了轴的刻度文字的显示
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    keyAxis->setTicker(textTicker);        // 设置为文字轴

    keyAxis->setTickLabelRotation(60);     // 轴刻度文字旋转60度
    keyAxis->setSubTicks(false);           // 不显示子刻度
    keyAxis->setTickLength(0, 4);          // 轴内外刻度的长度分别是0,4,也就是轴内的刻度线不显示
    keyAxis->setRange(0,ticks.count()+1);               // 设置范围
    keyAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    valueAxis->setRange(0, fossilData[0]+fossilData[0]/10);
    valueAxis->setPadding(35);             // 轴的内边距，可以到QCustomPlot之开始（一）看图解
    valueAxis->setLabel("新增确诊数量");
    valueAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    //设置柱状的数据value
    fossil->setWidth(fossil->width()/2);
    fossil->setData(ticks, fossilData);
}

void ShowPage::Get_TheNewDiagnosis()
{
    model->setTable("trend");
    model->select();

    QVector<double> date ;
    QVector<double> value;

    QString replydata = model->record(5).value(2).toString()+",";
    QRegExp rx("([0-9]*),");
    //非贪婪模式
    rx.setMinimal(true);
    int pos=0 ;
    int max=0;
    while((pos = rx.indexIn(replydata,pos)) != -1) {
       pos +=rx.matchedLength();
        //存入数据
       value<<rx.cap(1).toInt();
       if((rx.cap(1).toInt()) >max){
           max = rx.cap(1).toInt();
       }
    }

    replydata = model->record(0).value(2).toString();
    QRegExp rx1("\"(.*)\"");
    //非贪婪模式
    rx1.setMinimal(true);
    int pos1=0 ;
    int index=0;
    int year=2020;
    while((pos1 = rx1.indexIn(replydata,pos1)) != -1) {
       pos1 +=rx1.matchedLength();
       QStringList MM_dd =rx1.cap(1).split(".");

        if((MM_dd[0] == "1")&&(MM_dd[1] == "1")){
            year++;
        }
        if(MM_dd[0].toInt()<10){
            MM_dd[0] ="0"+ MM_dd[0];
        }

        if(MM_dd[1].toInt()<10){
            MM_dd[1] ="0"+ MM_dd[1];
        }

        QString curdate = QString("%1").arg(year)+"-" + MM_dd[0] +"-"+ MM_dd[1];


        //存入数据
        date<<QDateTime::fromString(curdate,"yyyy-MM-dd").toTime_t();
        index++;
    }
               ui->widget_8->legend->setVisible(true);

               //添加图形
               ui->widget_8->addGraph();
               ui->widget_8->graph(0)->setName("新增确诊");
               //设置画笔
               ui->widget_8->graph(0)->setPen(QPen(Qt::red));

               //传入数据
               ui->widget_8->graph(0)->setData(date,value);

               ui->widget_8->xAxis->setRange(date[0],date[0]+24*3600*index);
               ui->widget_8->yAxis->setRange(0,max);

               ui->widget_8->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
               ui->widget_8->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
               ui->widget_8->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom );

               QSharedPointer<QCPAxisTickerDateTime> dateTick(new QCPAxisTickerDateTime);
               dateTick->setDateTimeFormat("yyyy-MM-dd");
               ui->widget_8->xAxis->setTicker(dateTick);
}

void ShowPage::Get_DiagnosisOfSuspected()
{
    model->setTable("trend");
    model->select();

    QVector<double> date ;
    QVector<double> value;
    QVector<double> value2;
    QString replydata = model->record(1).value(2).toString()+",";
    QRegExp rx("([0-9]*),");
    //非贪婪模式
    rx.setMinimal(true);
    int pos=0 ;
    int max=0;
    while((pos = rx.indexIn(replydata,pos)) != -1) {
       pos +=rx.matchedLength();
        //存入数据
       value<<rx.cap(1).toInt();
       if((rx.cap(1).toInt()) >max){
           max = rx.cap(1).toInt();
       }
    }

    replydata = model->record(2).value(2).toString()+",";
    QRegExp rx2("([0-9]*),");
    //非贪婪模式
    rx2.setMinimal(true);
    int pos2=0 ;
    while((pos2 = rx2.indexIn(replydata,pos2)) != -1) {
       pos2 +=rx2.matchedLength();
        //存入数据
       value2<<rx2.cap(1).toInt();
    }




    replydata = model->record(0).value(2).toString();
    QRegExp rx1("\"(.*)\"");
    //非贪婪模式
    rx1.setMinimal(true);
    int pos1=0 ;
    int index=0;
    int year=2020;
    while((pos1 = rx1.indexIn(replydata,pos1)) != -1) {
       pos1 +=rx1.matchedLength();
       QStringList MM_dd =rx1.cap(1).split(".");

        if((MM_dd[0] == "1")&&(MM_dd[1] == "1")){
            year++;
        }
        if(MM_dd[0].toInt()<10){
            MM_dd[0] ="0"+ MM_dd[0];
        }

        if(MM_dd[1].toInt()<10){
            MM_dd[1] ="0"+ MM_dd[1];
        }

        QString curdate = QString("%1").arg(year)+"-" + MM_dd[0] +"-"+ MM_dd[1];


        //存入数据
        date<<QDateTime::fromString(curdate,"yyyy-MM-dd").toTime_t();
        index++;
    }
               //添加图形
               ui->widget_9->legend->setVisible(true);
               ui->widget_9->addGraph();
               //设置画笔
               ui->widget_9->graph(0)->setPen(QPen(Qt::red));
               ui->widget_9->graph(0)->setName("累计确诊");
               //传入数据
               ui->widget_9->graph(0)->setData(date,value);
               //添加图形
               ui->widget_9->addGraph();
               ui->widget_9->graph(1)->setName("疑似");
               //设置画笔
               ui->widget_9->graph(1)->setPen(QPen(Qt::black));

               //传入数据
               ui->widget_9->graph(1)->setData(date,value2);

               ui->widget_9->xAxis->setRange(date[0],date[0]+24*3600*index);
               ui->widget_9->yAxis->setRange(0,max);

               ui->widget_9->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
               ui->widget_9->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
               ui->widget_9->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom );

               QSharedPointer<QCPAxisTickerDateTime> dateTick(new QCPAxisTickerDateTime);
               dateTick->setDateTimeFormat("yyyy-MM-dd");
               ui->widget_9->xAxis->setTicker(dateTick);



}

void ShowPage::Get_DiedLive()
{
    model->setTable("trend");
    model->select();

    QVector<double> date ;
    QVector<double> value;
    QVector<double> value2;
    QString replydata = model->record(3).value(2).toString()+",";
    QRegExp rx("([0-9]*),");
    //非贪婪模式
    rx.setMinimal(true);
    int pos=0 ;
    int max=0;
    while((pos = rx.indexIn(replydata,pos)) != -1) {
       pos +=rx.matchedLength();
        //存入数据
       value<<rx.cap(1).toInt();
       if((rx.cap(1).toInt()) >max){
           max = rx.cap(1).toInt();
       }
    }

    replydata = model->record(4).value(2).toString()+",";
    QRegExp rx2("([0-9]*),");
    //非贪婪模式
    rx2.setMinimal(true);
    int pos2=0 ;
    while((pos2 = rx2.indexIn(replydata,pos2)) != -1) {
       pos2 +=rx2.matchedLength();
        //存入数据
       value2<<rx2.cap(1).toInt();
    }




    replydata = model->record(0).value(2).toString();
    QRegExp rx1("\"(.*)\"");
    //非贪婪模式
    rx1.setMinimal(true);
    int pos1=0 ;
    int index=0;
    int year=2020;
    while((pos1 = rx1.indexIn(replydata,pos1)) != -1) {
       pos1 +=rx1.matchedLength();
       QStringList MM_dd =rx1.cap(1).split(".");

        if((MM_dd[0] == "1")&&(MM_dd[1] == "1")){
            year++;
        }
        if(MM_dd[0].toInt()<10){
            MM_dd[0] ="0"+ MM_dd[0];
        }

        if(MM_dd[1].toInt()<10){
            MM_dd[1] ="0"+ MM_dd[1];
        }

        QString curdate = QString("%1").arg(year)+"-" + MM_dd[0] +"-"+ MM_dd[1];


        //存入数据
        date<<QDateTime::fromString(curdate,"yyyy-MM-dd").toTime_t();
        index++;
    }
               //添加图形
               ui->widget_10->legend->setVisible(true);
               ui->widget_10->addGraph();
               //设置画笔
               ui->widget_10->graph(0)->setPen(QPen(Qt::red));
               ui->widget_10->graph(0)->setName("治愈");
               //传入数据
               ui->widget_10->graph(0)->setData(date,value);
               //添加图形
               ui->widget_10->addGraph();
               ui->widget_10->graph(1)->setName("死亡");
               //设置画笔
               ui->widget_10->graph(1)->setPen(QPen(Qt::black));

               //传入数据
               ui->widget_10->graph(1)->setData(date,value2);

               ui->widget_10->xAxis->setRange(date[0],date[0]+24*3600*index);
               ui->widget_10->yAxis->setRange(0,max);

               ui->widget_10->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
               ui->widget_10->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
               ui->widget_10->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom );

               QSharedPointer<QCPAxisTickerDateTime> dateTick(new QCPAxisTickerDateTime);
               dateTick->setDateTimeFormat("yyyy-MM-dd");
               ui->widget_10->xAxis->setTicker(dateTick);
}

void ShowPage::GetforeignTheNewDiagnosis()
{
    model->setTable("allForeignTrend");
    model->select();

    QVector<double> date ;
    QVector<double> value;

    QString replydata = model->record(5).value(2).toString()+",";
    QRegExp rx("([0-9]*),");
    //非贪婪模式
    rx.setMinimal(true);
    int pos=0 ;
    int max=0;
    while((pos = rx.indexIn(replydata,pos)) != -1) {
       pos +=rx.matchedLength();
        //存入数据
       value<<rx.cap(1).toInt();
       if((rx.cap(1).toInt()) >max){
           max = rx.cap(1).toInt();
       }
    }

    replydata = model->record(0).value(2).toString();
    QRegExp rx1("\"(.*)\"");
    //非贪婪模式
    rx1.setMinimal(true);
    int pos1=0 ;
    int index=0;
    int year=2020;
    while((pos1 = rx1.indexIn(replydata,pos1)) != -1) {
       pos1 +=rx1.matchedLength();
       QStringList MM_dd =rx1.cap(1).split(".");

        if((MM_dd[0] == "1")&&(MM_dd[1] == "1")){
            year++;
        }
        if(MM_dd[0].toInt()<10){
            MM_dd[0] ="0"+ MM_dd[0];
        }

        if(MM_dd[1].toInt()<10){
            MM_dd[1] ="0"+ MM_dd[1];
        }

        QString curdate = QString("%1").arg(year)+"-" + MM_dd[0] +"-"+ MM_dd[1];


        //存入数据
        date<<QDateTime::fromString(curdate,"yyyy-MM-dd").toTime_t();
        index++;
    }
               ui->widget_11->legend->setVisible(true);

               //添加图形
               ui->widget_11->addGraph();
               ui->widget_11->graph(0)->setName("新增确诊");
               //设置画笔
               ui->widget_11->graph(0)->setPen(QPen(Qt::red));

               //传入数据
               ui->widget_11->graph(0)->setData(date,value);

               ui->widget_11->xAxis->setRange(date[0],date[0]+24*3600*index);
               ui->widget_11->yAxis->setRange(0,max);

               ui->widget_11->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
               ui->widget_11->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
               ui->widget_11->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom );

               QSharedPointer<QCPAxisTickerDateTime> dateTick(new QCPAxisTickerDateTime);
               dateTick->setDateTimeFormat("yyyy-MM-dd");
               ui->widget_11 ->xAxis->setTicker(dateTick);
}

void ShowPage::Get_CumulativeDiagnosisAbroad()
{
    model->setTable("allForeignTrend");
    model->select();

    QVector<double> date ;
    QVector<double> value;
    QVector<double> value2;
    QString replydata = model->record(1).value(2).toString()+",";
    QRegExp rx("([0-9]*),");
    //非贪婪模式
    rx.setMinimal(true);
    int pos=0 ;
    int max=0;
    while((pos = rx.indexIn(replydata,pos)) != -1) {
       pos +=rx.matchedLength();
        //存入数据
       value<<rx.cap(1).toInt();
       if((rx.cap(1).toInt()) >max){
           max = rx.cap(1).toInt();
       }
    }

    replydata = model->record(4).value(2).toString()+",";
    QRegExp rx2("([0-9]*),");
    //非贪婪模式
    rx2.setMinimal(true);
    int pos2=0 ;
    while((pos2 = rx2.indexIn(replydata,pos2)) != -1) {
       pos2 +=rx2.matchedLength();
        //存入数据
       value2<<rx2.cap(1).toInt();
    }




    replydata = model->record(0).value(2).toString();
    QRegExp rx1("\"(.*)\"");
    //非贪婪模式
    rx1.setMinimal(true);
    int pos1=0 ;
    int index=0;
    int year=2020;
    while((pos1 = rx1.indexIn(replydata,pos1)) != -1) {
       pos1 +=rx1.matchedLength();
       QStringList MM_dd =rx1.cap(1).split(".");

        if((MM_dd[0] == "1")&&(MM_dd[1] == "1")){
            year++;
        }
        if(MM_dd[0].toInt()<10){
            MM_dd[0] ="0"+ MM_dd[0];
        }

        if(MM_dd[1].toInt()<10){
            MM_dd[1] ="0"+ MM_dd[1];
        }

        QString curdate = QString("%1").arg(year)+"-" + MM_dd[0] +"-"+ MM_dd[1];


        //存入数据
        date<<QDateTime::fromString(curdate,"yyyy-MM-dd").toTime_t();
        index++;
    }
               //添加图形
               ui->widget_12->legend->setVisible(true);
               ui->widget_12->addGraph();
               //设置画笔
               ui->widget_12->graph(0)->setPen(QPen(Qt::red));
               ui->widget_12->graph(0)->setName("累计确诊");
               //传入数据
               ui->widget_12->graph(0)->setData(date,value);
               //添加图形
               ui->widget_12->addGraph();
               ui->widget_12->graph(1)->setName("现有确诊");
               //设置画笔
               ui->widget_12->graph(1)->setPen(QPen(Qt::black));

               //传入数据
               ui->widget_12->graph(1)->setData(date,value2);

               ui->widget_12->xAxis->setRange(date[0],date[0]+24*3600*index);
               ui->widget_12->yAxis->setRange(0,max);

               ui->widget_12->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
               ui->widget_12->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
               ui->widget_12->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom );

               QSharedPointer<QCPAxisTickerDateTime> dateTick(new QCPAxisTickerDateTime);
               dateTick->setDateTimeFormat("yyyy-MM-dd");
               ui->widget_12->xAxis->setTicker(dateTick);

}

void ShowPage::Get_foreignDiedLive()
{
    model->setTable("allForeignTrend");
    model->select();

    QVector<double> date ;
    QVector<double> value;
    QVector<double> value2;
    QString replydata = model->record(2).value(2).toString()+",";
    QRegExp rx("([0-9]*),");
    //非贪婪模式
    rx.setMinimal(true);
    int pos=0 ;
    int max=0;
    while((pos = rx.indexIn(replydata,pos)) != -1) {
       pos +=rx.matchedLength();
        //存入数据
       value<<rx.cap(1).toInt();
       if((rx.cap(1).toInt()) >max){
           max = rx.cap(1).toInt();
       }
    }

    replydata = model->record(3).value(2).toString()+",";
    QRegExp rx2("([0-9]*),");
    //非贪婪模式
    rx2.setMinimal(true);
    int pos2=0 ;
    while((pos2 = rx2.indexIn(replydata,pos2)) != -1) {
       pos2 +=rx2.matchedLength();
        //存入数据
       value2<<rx2.cap(1).toInt();
    }




    replydata = model->record(0).value(2).toString();
    QRegExp rx1("\"(.*)\"");
    //非贪婪模式
    rx1.setMinimal(true);
    int pos1=0 ;
    int index=0;
    int year=2020;
    while((pos1 = rx1.indexIn(replydata,pos1)) != -1) {
       pos1 +=rx1.matchedLength();
       QStringList MM_dd =rx1.cap(1).split(".");

        if((MM_dd[0] == "1")&&(MM_dd[1] == "1")){
            year++;
        }
        if(MM_dd[0].toInt()<10){
            MM_dd[0] ="0"+ MM_dd[0];
        }

        if(MM_dd[1].toInt()<10){
            MM_dd[1] ="0"+ MM_dd[1];
        }

        QString curdate = QString("%1").arg(year)+"-" + MM_dd[0] +"-"+ MM_dd[1];


        //存入数据
        date<<QDateTime::fromString(curdate,"yyyy-MM-dd").toTime_t();
        index++;
    }
               //添加图形
               ui->widget_13->legend->setVisible(true);
               ui->widget_13->addGraph();
               //设置画笔
               ui->widget_13->graph(0)->setPen(QPen(Qt::red));
               ui->widget_13->graph(0)->setName("治愈");
               //传入数据
               ui->widget_13->graph(0)->setData(date,value);
               //添加图形
               ui->widget_13->addGraph();
               ui->widget_13->graph(1)->setName("死亡");
               //设置画笔
               ui->widget_13->graph(1)->setPen(QPen(Qt::black));

               //传入数据
               ui->widget_13->graph(1)->setData(date,value2);

               ui->widget_13->xAxis->setRange(date[0],date[0]+24*3600*index);
               ui->widget_13->yAxis->setRange(0,max);

               ui->widget_13->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
               ui->widget_13->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
               ui->widget_13->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom );

               QSharedPointer<QCPAxisTickerDateTime> dateTick(new QCPAxisTickerDateTime);
               dateTick->setDateTimeFormat("yyyy-MM-dd");
               ui->widget_13->xAxis->setTicker(dateTick);
}

void ShowPage::Province()
{



    model->setTable("province");
    model->select();

    int mrow = model->rowCount();
    QStringList HStrList;
    HStrList.push_back(QString("疫情地区"));
    HStrList.push_back(QString("新增"));
    HStrList.push_back(QString("现有"));
    HStrList.push_back(QString("累计"));
    HStrList.push_back(QString("治愈"));
    HStrList.push_back(QString("死亡"));
    //设置行列数(只有列存在的前提下，才可以设置列标签)
    int HlableCnt = HStrList.count();
    ui->tableWidget->setRowCount(mrow);
    ui->tableWidget->setColumnCount(HlableCnt);
    //设置列标签
    ui->tableWidget->setHorizontalHeaderLabels(HStrList);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for(int n=0;n<mrow;n++){
        qDebug()<<model->record(n).value(1).toString();
        model2->setTable(model->record(n).value(1).toString());
        model2->select();

          ui->tableWidget->setItem(n,0,new QTableWidgetItem(model2->record(0).value(1).toString()));
          ui->tableWidget->setItem(n,1,new QTableWidgetItem(model2->record(0).value(6).toString()));
          ui->tableWidget->setItem(n,2,new QTableWidgetItem(model2->record(0).value(11).toString()));
          ui->tableWidget->setItem(n,3,new QTableWidgetItem(model2->record(0).value(2).toString()));
          ui->tableWidget->setItem(n,4,new QTableWidgetItem(model2->record(0).value(4).toString()));
          ui->tableWidget->setItem(n,5,new QTableWidgetItem(model2->record(0).value(3).toString()));

    }

}


void ShowPage::paintEvent(QPaintEvent *)
{


}

void ShowPage::resizeEvent(QResizeEvent *)
{
    Gui_Set();
}








void ShowPage::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{

    QDialog w;
    w.setWindowTitle("城市数据");
    w.resize(500,500);

    QGridLayout *lay =new QGridLayout(&w);
    QTableWidget *table = new QTableWidget;
    lay->addWidget(table);

    model2->setTable(ui->tableWidget->item(item->row(),0)->text());
    model2->select();


    QStringList HStrList;
    HStrList.push_back(QString("疫情地区"));
    HStrList.push_back(QString("新增"));
    HStrList.push_back(QString("现有"));
    HStrList.push_back(QString("累计"));
    HStrList.push_back(QString("治愈"));
    HStrList.push_back(QString("死亡"));

    //设置行列数(只有列存在的前提下，才可以设置列标签)
    int HlableCnt = HStrList.count();
    table->setRowCount(model2->rowCount()-1);
    table->setColumnCount(HlableCnt);
    //设置列标签
    table->setHorizontalHeaderLabels(HStrList);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for(int n=0;n<model2->rowCount();n++){

          table->setItem(n,0,new QTableWidgetItem(model2->record(n+1).value(1).toString()));
          table->setItem(n,1,new QTableWidgetItem(model2->record(n+1).value(6).toString()));
          table->setItem(n,2,new QTableWidgetItem(model2->record(n+1).value(11).toString()));
          table->setItem(n,3,new QTableWidgetItem(model2->record(n+1).value(2).toString()));
          table->setItem(n,4,new QTableWidgetItem(model2->record(n+1).value(4).toString()));
          table->setItem(n,5,new QTableWidgetItem(model2->record(n+1).value(3).toString()));

    }


    w.exec();
}
