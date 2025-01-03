#include "menu.h"
#include <QPainter>
#include <QVBoxLayout>

CustomMenu::CustomMenu(QWidget *parent) : QMenu(parent), layout(new QVBoxLayout(this)) {
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::Popup);
    setAttribute(Qt::WA_TranslucentBackground);  // 设置背景透明

    finalSize = QSize(70,0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);  // 菜单项间隔
    this->resize(70,0);
    setLayout(layout);
//    resizeAnimation = new QPropertyAnimation(this, "geometry");
//    resizeAnimation->setEasingCurve(QEasingCurve::OutCubic); // 使用平滑的缓出动画曲线
}

void CustomMenu::addAction(LabelButton *action) {
    actionsList.append(action);
    action->setFixedSize(70, 20);
    finalSize.setHeight(finalSize.height()+20);
    layout->addWidget(action);
}

void CustomMenu::showMenu() {
    qDebug() << "CustomMenu::showMenu ::" << finalSize;
    this->resize(finalSize);
    this->show();
}

// 自定义绘制事件
void CustomMenu::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    QRectF rect = QRectF(0, 0, width(), height());
    painter.setBrush(QColor(240, 240, 240, 150));  // 设置背景颜色
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect);

    QWidget::paintEvent(event);
}
