#ifndef MYCUSTOMPLOT_H
#define MYCUSTOMPLOT_H
#include "QCustomPlot/qcustomplot.h"
#include <QWidget>
class MyCustomPlot : public QCustomPlot
{
    Q_OBJECT
public:
    explicit MyCustomPlot(QWidget *parent = nullptr);
public:

protected:
    void wheelEvent(QWheelEvent *event) override;
signals:

};




#endif // MYCUSTOMPLOT_H
