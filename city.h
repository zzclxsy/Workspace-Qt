#ifndef CITY_H
#define CITY_H

#include <QDialog>

namespace Ui {
class City;
}

class City : public QDialog
{
    Q_OBJECT

public:
    explicit City(QWidget *parent = nullptr);
    ~City();

private:
    Ui::City *ui;
};

#endif // CITY_H
