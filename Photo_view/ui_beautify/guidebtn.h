// 红点导航按钮
#ifndef GUIDEBTN_H
#define GUIDEBTN_H

#include <QWidget>
#include <QPushButton>

class guideBtn : public QPushButton
{
    Q_OBJECT
public:
    explicit guideBtn(QWidget *parent = nullptr, QWidget *dect = nullptr, QPoint init = QPoint(0, 0));
    void initBtn();
    void updateInitPos(QRect init);
    void showBtn(QRect initRect, QRect boardRect, QRect tableRect);
    void setTarget(QWidget *dect);

private:
    QString picPath;
    QRect rect;
    QPoint initPos, nowPos;
    QString stylesheet = "border-radius: 5px; background: rgba(250,20,20,0.4);";
    QWidget* dect = nullptr;
    int deltaX, deltaY;
    int w, h;

signals:
    void clicked(QRect rect, QWidget*);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // GUIDEBTN_H
