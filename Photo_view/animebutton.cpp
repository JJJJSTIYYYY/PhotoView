#include "animebutton.h"
#include <QtCore/qmath.h>

bool isIn = false;
bool isPress = false;

animeButton::animeButton(QWidget *parent)
    : QPushButton{parent}
{
    this->setMouseTracking(true);
    rx = ry = 0;
    r0 = g0 = b0 = r1 = g1 = b1 = rt = gt = bt = 0;
    v1 = 7;
    v2 = 5;
    mousex = mousey = -1;
    connect(&this->timer1, &QTimer::timeout, this, &animeButton::rAdd);
    connect(&this->timer2, &QTimer::timeout, this, &animeButton::rReduce);
}

bool animeButton::setButtonText(QString text)
{
    this->text = text;
    this->setText(text);
}

void animeButton::setButtonTextColor(int r, int g, int b)
{
    rt = r;
    gt = g;
    bt = b;
}

void animeButton::setRGB0(int r, int g, int b)
{
    r0 = r;
    g0 = g;
    b0 = b;
}

void animeButton::setRGB1(int r, int g, int b)
{
    r1 = r;
    g1 = g;
    b1 = b;
}

void animeButton::setV(int v1, int v2)
{
    this->v1 = v1;
    this->v2 = v2;
}

void animeButton::enterEvent(QEnterEvent *event)
{
    isIn = true;
    qDebug() << "in";
    timer1.start(v1);
}

void animeButton::leaveEvent(QEvent *event)
{
    isIn = false;
    qDebug() << "out";
    timer2.start(v2);
}

void animeButton::mouseMoveEvent(QMouseEvent *event)
{
    mousex = event->x();
    mousey = event->y();
    update();
}

void animeButton::paintEvent(QPaintEvent *event)
{
    QPixmap map(this->width(), this->height());
    QPainterPath path;
    path.addEllipse(QPoint(mousex, mousey), rx, ry);
    QRadialGradient gradient(QPointF(mousex, mousey), rx);
    gradient.setColorAt(0, QColor(r0, g0, b0));
    gradient.setColorAt(1, QColor(r1, g1, b1));
    QBrush brush(gradient);
    QPainter painter(this);
    QPen pen;
    if(isIn)
    {
        pen.setColor(QColor(50, 100, 200));
        pen.setWidth(4);
        painter.setPen(pen);
        painter.drawRect(0, 0, this->width(), this->height());
    }
    pen.setColor(QColor(r1, g1, b1));
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.setOpacity(0.5);
    if(isPress) painter.setOpacity(1);
    painter.drawPath(path);
    pen.setColor(QColor(rt, gt, bt));
    painter.setPen(pen);
    painter.setOpacity(1);
    painter.drawText(0, 0, this->width(), this->height(),
                     Qt::AlignHCenter|Qt::AlignVCenter, this->text);
}

void animeButton::rAdd()
{
    if(isIn)
    {
        timer2.stop();
//        qDebug() << "add isIn";
        if(rx < 1.5*this->width() || ry < 1.5*this->height())
        {
            if(isIn) rx+=8;
            if(isIn) ry+=5;
            if(isIn) update();
        }
        else
        {
            if(isIn) timer1.stop();
        }
    }
}

void animeButton::rReduce()
{
    if(!isIn)
    {
        timer1.stop();
//        qDebug() << "reduce isIn";
        if(rx > 0 || rx > 0)
        {
            if(!isIn) rx-=8;
            if(!isIn) ry-=5;
            if(!isIn) update();
        }
        else
        {
            if(!isIn) timer2.stop();
            rx = 0;
            ry = 0;
        }
    }
}
