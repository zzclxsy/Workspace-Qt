#ifndef SHOWPAGE_H
#define SHOWPAGE_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlTableModel>
#include<QSqlRecord>
#include"custombars.h"
QT_BEGIN_NAMESPACE
namespace Ui { class ShowPage; }
QT_END_NAMESPACE

class ShowPage : public QWidget
{
    Q_OBJECT

public:
    ShowPage(QWidget *parent = nullptr);
    ~ShowPage();

public:
    //ui美化
    void Gui_Set();
    //国内总体数据
    void Get_SummaryDataIn_Data();
    //国外总体数据
    void Get_SummaryDataOut_Data();
    //新增确诊分布 柱形图
    void Get_NewAddTopProvince_Data();
    //现有无症状分布
    void Get_AsymptomaticTopProvince_Data();
    //境外输入省级TOP10
    void Get_TopOverseasInput_Data();
    //境外输入新增趋势
    void Get_NewImportFrom_Overseas();
    //境外输入累计趋势
    void Get_AccumulationOfOverseasImports();
    //新增国家TOP10
    void Get_TopAddCountry_Data();
    //全国疫情新增趋势
    void Get_TheNewDiagnosis();
    //全国确诊疑似趋势
    void Get_DiagnosisOfSuspected();
    //全国累计治愈死亡
    void Get_DiedLive();
    //国外新增确诊
    void GetforeignTheNewDiagnosis();
    //国外累计确诊
    void Get_CumulativeDiagnosisAbroad();
    //国外治愈死亡
    void Get_foreignDiedLive();
    //省级数据
    void Province();
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
private slots:
    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

private:
    Ui::ShowPage *ui;
    QSqlDatabase db;
    QSqlQuery *query;
    QSqlTableModel *model;
    QSqlTableModel *model2;
};
#endif // SHOWPAGE_H
