#include "pointbutton.h"

pointButton::pointButton(QWidget *parent)
    : QPushButton{parent}
{
    moveAnimation = new QPropertyAnimation(this, "geometry");
    moveAnimation->setEasingCurve(QEasingCurve::OutCubic); // 使用平滑的缓动曲线

}

void pointButton::animove(QPoint targetPos)
{
    moveAnimation->setDuration(300);
    moveAnimation->setStartValue(this->geometry());
    moveAnimation->setEndValue(QRect(targetPos.x(), targetPos.y(), this->width(), this->height()));
    if (moveAnimation->state() == QAbstractAnimation::Running){
        moveAnimation->stop();
    }

    // 启动动画
    moveAnimation->start();
}

void pointButton::mousePressEvent(QMouseEvent *event)
{
    // 记录鼠标按下的位置，准备开始拖拽
    if (event->button() == Qt::LeftButton) { // 确保是左键点击
        btnPressed = true;
        dragStartPos = event->pos();  // 保存当前鼠标的位置
        startPointPos = this->pos();
    }
    return QPushButton::mousePressEvent(event);
}

void pointButton::mouseReleaseEvent(QMouseEvent *event)
{
    // 释放拖拽标志
    if (event->button() == Qt::LeftButton) { // 确保是左键释放
        btnPressed = false;
    }
    return QPushButton::mouseReleaseEvent(event);
}

void pointButton::mouseMoveEvent(QMouseEvent *event)
{
    // 如果正在拖拽，计算差值并移动控件
    if (btnPressed) {
        move(event->globalPos() - dragStartPos);
        emit pointMoved(this->pos().x() - startPointPos.x(), this->pos().y() - startPointPos.y());
//        qDebug() << "deltX is " << event->globalPos().x() - dragStartPos.x();
//        qDebug() << "deltY is " << event->globalPos().y() - dragStartPos.y();
    }
    return QPushButton::mouseMoveEvent(event);
}
