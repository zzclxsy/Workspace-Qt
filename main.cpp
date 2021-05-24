#include "showpage.h"
#include "control.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    Control control;
    control.Begin();
    control.openimage.show();
    control.Loops();
    control.Close();

    ShowPage w;
    w.show();
    control.openimage.finish(&w);

    return a.exec();
}
