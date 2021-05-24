#include "openimage.h"

OpenImage::OpenImage(QWidget *parent) : QSplashScreen(parent)
{
    QPixmap  pix("C:\\Users\\ZZC\\Desktop\\Waiting\\2.png");
    this->setPixmap(pix);
    QFont font;
    font.setPixelSize(50);
    this->setFont(font);
}

void OpenImage::Message(QString mess)
{
    this->showMessage(mess,Qt::AlignBottom|Qt::AlignHCenter);
}

void OpenImage::mousePressEvent(QMouseEvent *)
{
        //过滤掉鼠标按压事件
}
