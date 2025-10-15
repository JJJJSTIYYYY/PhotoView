#include "guidebtn.h"

#include <QPropertyAnimation>

guideBtn::guideBtn(QWidget *parent, QWidget *dect, QPoint init)
    : QPushButton{parent}
{
    this->hide();
    this->resize(10,10);
    this->setStyleSheet(stylesheet);
    this->dect = dect;
    this->initPos = init;
}

void guideBtn::updateInitPos(QRect init)
{
    this->rect = init;
    this->initPos = QPoint(init.x(), init.y());
    this->w = init.width();
    this->h = init.height();
}

void guideBtn::showBtn(QRect initRect, QRect boardRect, QRect tableRect)
{
    int x = tableRect.x()+initRect.x()+initRect.width()/2;
    int y = tableRect.y()+initRect.y()+initRect.height()/2;
    qDebug() << "Guide btn show. pic_vew is " << initRect;
    qDebug() << "Guide btn show. boardRect is " << boardRect;
    qDebug() << "Guide btn show. tableRect is " << tableRect;

    qDebug() << "Guide btn show. Relitive position is " << x << y;
    if(x < initRect.width()/2){
        x = 0;
    }
    if(x > boardRect.width() - initRect.width()/2){
        x = boardRect.width()-10;
    }
    if(y < initRect.height()/2){
        y = 0;
    }
    if(y > boardRect.height() - initRect.height()/2){
        y = boardRect.height()-10;
    }

    qDebug() << "Guide btn show. Position is " << x << y;
    this->show();
    // 获取当前控件的位置
    QPoint startPos = this->pos();

    // 创建动画对象，目标属性是"pos"
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(300);  // 设置动画持续时间为500毫秒
    animation->setStartValue(startPos);  // 起始位置
    animation->setEndValue(QPoint(x, y));   // 目标位置
    animation->setEasingCurve(QEasingCurve::OutCubic);  // 设置动画缓动曲线
    // 启动动画
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void guideBtn::setTarget(QWidget *dect)
{
    this->dect = dect;
}

void guideBtn::mousePressEvent(QMouseEvent *event)
{
    emit clicked(this->rect, this->dect);
    this->hide();
}
