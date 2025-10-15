#include "labelbutton.h"



LabelButton::LabelButton(QWidget *parent)
    : QLabel {parent}
{
    setMouseTracking(true);
}

LabelButton::LabelButton(QWidget *parent, QString text, QString style)
    : QLabel {parent}
{
//    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_Hover);
    setAlignment(Qt::AlignCenter);
    setMouseTracking(true);
    styleSheet = style;
    this->setText(text);
    this->setStyleSheet(style);
}

void LabelButton::enterEvent(QEvent *event) {
//    qDebug() << "LabelButton::enterEvent";
//    this->setStyleSheet("QLabel { color : blue; }");
}

void LabelButton::leaveEvent(QEvent *event) {
//    qDebug() << "LabelButton::leaveEvent";
//    this->setStyleSheet(styleSheet);
}

void LabelButton::mousePressEvent(QMouseEvent *event) {
    // 如果鼠标左键按下，发射 labelClicked 信号
    if (event->button() == Qt::LeftButton) {
        emit labelClicked();
    }

    // 调用父类的 mousePressEvent 以确保正常的事件传递
    QLabel::mousePressEvent(event);
}
