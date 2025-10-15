#ifndef ANIMEBUTTON_H
#define ANIMEBUTTON_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QEnterEvent>
#include <QPaintEvent>
#include <QColor>
#include <QPaintDevice>
#include <QPainter>
#include <QBrush>
#include <QPixmap>
#include <QPainterPath>
#include <QRadialGradient>
#include <QString>
#include <QTimer>
#include <QDebug>


class animeButton : public QPushButton
{
    Q_OBJECT
public:
    explicit animeButton(QWidget *parent = nullptr);
    bool setButtonText(QString text);
    void setButtonTextColor(int r, int g, int b);
    void setRGB0(int r, int g, int b);
    void setRGB1(int r, int g, int b);
    void setV(int v1, int v2);

private:
    int v1, v2;
    int rx, ry;
    int r0, g0, b0;
    int r1, g1, b1;
    int rt, gt, bt;
    int mousex, mousey;
    QTimer timer1, timer2;
    QString text;

protected:
    void enterEvent(QEnterEvent* event);
    void leaveEvent(QEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);

signals:

public slots:
    void rAdd();
    void rReduce();
};

#endif // ANIMEBUTTON_H
