// 已弃置
#ifndef POINTBUTTON_H
#define POINTBUTTON_H

#include <QPushButton>
#include <QMouseEvent>
#include <QPropertyAnimation>

class pointButton : public QPushButton
{
    Q_OBJECT
public:
    explicit pointButton(QWidget *parent = nullptr);
    void animove(QPoint targetPos);

signals:
    void pointMoved(int, int);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    bool btnPressed = false;
    QPoint dragStartPos;      // 存储鼠标按下时的起始位置
    QPoint startPointPos;      // 存储鼠标按下时的起始位置

    QPropertyAnimation *moveAnimation;
};

#endif // POINTBUTTON_H
