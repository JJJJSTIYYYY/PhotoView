// 自定义右击菜单
#ifndef CUSTOMMENU_H
#define CUSTOMMENU_H

#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include "labelbutton.h"

class CustomMenu : public QMenu
{
    Q_OBJECT
public:
    explicit CustomMenu(QWidget *parent = nullptr);
    void addAction(LabelButton *action);
    void showMenu();

protected:
    void paintEvent(QPaintEvent *event) override;
//    void mousePressEvent(QMouseEvent *event) override;


private:
    QList<LabelButton*> actionsList;
    QVBoxLayout *layout;
    QSize finalSize;
//    QPropertyAnimation *resizeAnimation;  // 用于窗口大小调整的动画
    QTimer *timer = new QTimer(this);
    double size = 0;
};

#endif // CUSTOMMENU_H
