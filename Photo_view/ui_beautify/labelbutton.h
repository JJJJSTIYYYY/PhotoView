// 自定义按钮类
#ifndef LABELBUTTON_H
#define LABELBUTTON_H

#include <QEvent>
#include <QLabel>
#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <qtimer.h>

class LabelButton : public QLabel
{
    Q_OBJECT
public:
    explicit LabelButton(QWidget *parent = nullptr);
    LabelButton(QWidget *parent = nullptr, QString text = "", QString style = "");


signals:
    void labelClicked(); // 点击信号

protected:
    void enterEvent(QEvent *event);    // 鼠标进入事件
    void leaveEvent(QEvent *event);    // 鼠标离开事件
    void mousePressEvent(QMouseEvent *event); // 鼠标点击事件

private:
    QString originalColor; // 保存原始的文本颜色
    QString styleSheet;

};

#endif // LABELBUTTON_H
