#ifndef OPENIMAGE_H
#define OPENIMAGE_H

#include <QObject>
#include <QSplashScreen>
#include <QWidget>
class OpenImage : public QSplashScreen
{
    Q_OBJECT
public:
    explicit OpenImage(QWidget *parent = nullptr);
    void Message(QString);
protected:
    void mousePressEvent(QMouseEvent *) override;
signals:

};

#endif // OPENIMAGE_H
