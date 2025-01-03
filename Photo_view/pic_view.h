#ifndef PIC_VIEW_H
#define PIC_VIEW_H

#include "imageprocessingthread.h"

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QMouseEvent>
#include <QGuiApplication>
#include <qpainter.h>
#include <ui_beautify/menu.h>


class pic_view : public QWidget
{
    Q_OBJECT
public:
    enum drection{
        Up, Down, Right, Left, Neither
    };
    enum color{
        fullColor, whiteAndBlack
    };
    enum lock_state{
        locked, unlocked
    };

    explicit pic_view(QWidget *parent = nullptr);
    pic_view(QWidget *parent = nullptr, QString path = "");
    pic_view(QWidget *parent = nullptr, QPixmap path = QPixmap());
    void set_window_first();
    void cancel_window_first();
    void showContextMenu(const QPoint &pos);
    void showGrayPic();
    void showColorfulPic();
    void openRGBHSV();
    void switchLockState();
    void hideWidget(bool ifHide);
    void showWidget();
    void getPicFitWidSize();
    QString getPath();
    void setPicDir(QString path);
    void onProcessingFinished(qint64 cr, qint64 cg, qint64 cb); // 灰度图计算完成

private:
    QString picDir;
    QPixmap picture;
    QPixmap gray_picture;
    bool grayPicturePrepared = false;
    color state;
    lock_state lockState;
    QLabel *label;
    QLabel *colorValue;
    QLabel *lock;
    CustomMenu *contextMenu;
    QColor bg = QColor(230, 230, 250, 150);
    QColor bg_border = QColor(25, 25, 5, 150);
    QLabel *border;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;


private:
    QPoint dragPosition;           // 拖动窗口时鼠标的位置
    QPoint labelStartPos;          // label相对窗口的位置
    bool isDragging = false;       // 是否正在拖动窗口
    bool ctrlPressed = false;      // ctrl是否按下
    bool spacePressed = false;
    QSize screenSize = QGuiApplication::primaryScreen()->size();
    QPoint resizeStartPos;         // 开始调整大小时鼠标的位置
    QSize windowStartSize;         // 调整大小时窗口的初始大小
    QSize labelSize;
    bool isResizing = false;       // 是否正在调整窗口大小
    int resizeMargin;              // 调整窗口大小的边缘宽度
    float aspectRatio;             // 宽高比
    ImageProcessingThread* imageProcessingThread; // 计算灰度图线程

    // 判断鼠标是否在窗口右下角的可调整大小区域
    bool isMouseOnEdge(const QPoint &pos);

    bool resize_pic();

    QPoint lastMousePos;

    QPixmap changePixmapSize(const QPixmap &originalPixmap, int newWidth, int newHeight);
signals:
    void pic_view_close(QString);
//    void isThereaWidShowOnTop(bool);

};

#endif // PIC_VIEW_H
