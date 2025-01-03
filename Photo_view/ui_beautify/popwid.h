#ifndef POPWID_H
#define POPWID_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QTimer>

class PopWid : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int alpha READ alpha WRITE setAlpha)

public:
    enum MessageType {
        Error, Warning, Message
    };

    explicit PopWid(QWidget *parent = nullptr, MessageType type = Message, int time1 = 100, int time2 = 1000);

    // 设置显示的文本
    void setText(const QString &text);

    // 显示窗口并开始淡入效果
    void showWithText(const QString &text, QPoint initialPos, int parentWidth = 30);

public slots:
    void fadeIn();   // 淡入动画
    void fadeOut();  // 淡出动画

protected:
    void paintEvent(QPaintEvent *event) override;  // 自定义绘制事件

private:
    int alpha() const;               // 获取透明度
    void setAlpha(int alpha);        // 设置透明度并重绘

    QPropertyAnimation *fadeAnimation;
    QTimer *delayTimer;
    QString displayText;
    QColor bg;
    MessageType t;
    int m_alpha;                     // 控制窗口和文本的透明度
};

#endif // POPWID_H
