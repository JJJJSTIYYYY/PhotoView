#include "ctrlbar.h"

#include <QPixmap>

ctrlBar::ctrlBar(QWidget *parent)
    : QWidget{parent}
{
    confirmBtn = new QToolButton(this);
    confirmBtn->setFixedSize(28, 28);
    confirmBtn->setIcon(QPixmap(":/1/confirm.png"));
    confirmBtn->setIconSize(QSize(24, 24));
    confirmBtn->setStyleSheet(
        "QToolButton {"
        "    border-radius: 64px;"   // 设置圆角半径
        "    background-color: transparent;"  // 设置背景色
        "    border: none;"          // 去掉默认边框
        "}"
        "QToolButton:hover {"
        "    background-color: rgba(100, 100, 100, 0.7);"
        "    border-radius: 3px;" // 悬停时改变背景色的透明度
        "}"
        );

    cancelBtn = new QToolButton(this);
    cancelBtn->setFixedSize(28, 28);
    cancelBtn->setIcon(QPixmap(":/1/cancel.png"));
    cancelBtn->setIconSize(QSize(24, 24));
    cancelBtn->setStyleSheet(
        "QToolButton {"
        "    border-radius: 64px;"   // 设置圆角半径
        "    background-color: transparent;"  // 设置背景色
        "    border: none;"          // 去掉默认边框
        "}"
        "QToolButton:hover {"
        "    background-color: rgba(100, 100, 100, 0.7);"
        "    border-radius: 3px;" // 悬停时改变背景色的透明度
        "}"
        );

    layout->addWidget(confirmBtn);
    layout->addWidget(cancelBtn);
    layout->setSpacing(5);
    this->setLayout(layout);

    connect(confirmBtn, &QToolButton::clicked, this, &ctrlBar::confirmBtnClicked);
    connect(cancelBtn, &QToolButton::clicked, this, &ctrlBar::cancelBtnClicked);
}
