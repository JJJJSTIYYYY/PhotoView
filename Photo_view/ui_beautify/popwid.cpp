#include "popwid.h"
#include <QPainter>
#include <QFont>
#include <QGuiApplication>
#include <QScreen>

PopWid::PopWid(QWidget *parent, MessageType type, int time1, int time2)
    : QWidget(parent), m_alpha(0) {
    // 初始化透明度动画
    fadeAnimation = new QPropertyAnimation(this, "alpha");
    fadeAnimation->setDuration(time1);  // 动画持续时间为1000毫秒

    // 初始化定时器
    delayTimer = new QTimer(this);
    delayTimer->setSingleShot(true);  // 定时器只触发一次
    delayTimer->setInterval(time2);     // 延迟500毫秒

    // 设置无边框和透明背景
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlag(Qt::WindowStaysOnTopHint, true);

    t = type;
    if(type == Message){
        bg = QColor(250, 250, 250);
    }
    else if(type == Warning){
        bg = QColor(230, 230, 50);
    }
    else if(type == Error){
        bg = QColor(255, 120, 80);
    }
    // 动画完成后检查是淡入还是淡出
    connect(fadeAnimation, &QPropertyAnimation::finished, this, [this]() {
        if (fadeAnimation->endValue().toInt() == 255) {
            delayTimer->start();  // 启动延迟定时器
        } else {
            this->deleteLater();  // 淡出完成后自动析构窗口
            qDebug() << "pop wid auto delete";
        }
    });

    // 定时器超时后自动执行 fadeOut
    connect(delayTimer, &QTimer::timeout, this, &PopWid::fadeOut);
}

// 设置显示的文本，并自动调整窗口大小
void PopWid::setText(const QString &text) {
    displayText = text;

    // 设置字体
    QFont font("幼圆", 14);
    QFontMetrics metrics(font);

    // 计算文本所需的大小
    int textWidth = metrics.horizontalAdvance(displayText);
    int textHeight = metrics.height();

    // 设置窗口大小，添加一些边距
    int padding = 10;  // 左右和上下的额外边距
    this->resize(textWidth + padding, textHeight + padding);

    update();  // 更新绘制
}

// 显示窗口并开始淡入效果
void PopWid::showWithText(const QString &text, QPoint initialPos, int parentWidth) {
    // 获取窗口所在屏幕的指针
    QScreen *screen = QGuiApplication::screenAt(this->geometry().center());
    if (!screen) {
        screen = QGuiApplication::primaryScreen();  // 如果无法确定，使用主屏幕
    }
    // 获取屏幕的几何尺寸
    QRect scr = screen->geometry();

    setText(text);  // 设置文本内容
    fadeIn();       // 开始淡入动画
    if(initialPos.x()+parentWidth+this->width() > scr.width()) // 在左边显示
    {
        this->move((initialPos.x()-this->width()-10), initialPos.y());
    }
    else
    {
        this->move((initialPos.x()+parentWidth+10), initialPos.y());
    }
    this->show();   // 显示窗口
}

// 淡入动画
void PopWid::fadeIn() {
    fadeAnimation->stop();
    fadeAnimation->setStartValue(0);    // 从完全透明
    fadeAnimation->setEndValue(255);    // 到完全不透明
    fadeAnimation->start();
}

// 淡出动画
void PopWid::fadeOut() {
    fadeAnimation->stop();
    fadeAnimation->setStartValue(255);  // 从完全不透明
    fadeAnimation->setEndValue(0);      // 到完全透明
    fadeAnimation->start();
}

// 获取透明度
int PopWid::alpha() const {
    return m_alpha;
}

// 设置透明度并重绘
void PopWid::setAlpha(int alpha) {
    m_alpha = alpha;
    update();  // 每次透明度变化时重新绘制
}

// 自定义绘制事件
void PopWid::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制背景，透明度由 m_alpha 控制
    QColor backgroundColor(bg.red(), bg.green(), bg.blue(), m_alpha);
    painter.setBrush(backgroundColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(this->rect(), 14, 14);  // 绘制圆角矩形背景

    // 绘制文本，透明度同样由 m_alpha 控制
    QColor textColor;
    if(t == Error) textColor = QColor(255, 255, 255);
    else if(t == Warning) textColor = QColor(20, 20, 20);
    else if(t == Message) textColor = QColor(70, 70, 70);
    painter.setPen(textColor);
    painter.setFont(QFont("幼圆", 12));
    painter.drawText(this->rect(), Qt::AlignCenter, displayText);

    QWidget::paintEvent(event);
}
