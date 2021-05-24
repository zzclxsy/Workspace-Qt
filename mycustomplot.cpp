#include "mycustomplot.h"
#include<QDebug>
MyCustomPlot::MyCustomPlot(QWidget *parent) : QCustomPlot(parent)
{

}

void MyCustomPlot::wheelEvent(QWheelEvent *event)
{
     QCustomPlot::wheelEvent(event);
}

