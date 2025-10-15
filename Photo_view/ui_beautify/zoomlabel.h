// 选区
#ifndef ZOOMLABEL_H
#define ZOOMLABEL_H

#include <QWidget>
#include <QLabel>
#include "menu.h"
//#include "pic_view.h"

class zoomLabel : public QLabel
{
    Q_OBJECT
public:
    enum moveDirection{
        up, down, left, right, none
    };

    explicit zoomLabel(QWidget *parent = nullptr, QList<QWidget*> selectWidList = QList<QWidget*>());
    void closeSelectedWid();
    void getSelectWidList(QList<QWidget*> selectWidList = QList<QWidget*>());
    QList<QWidget*> getAllSelectedWid();
    void showContextMenu(const QPoint &pos);
    void cancelSelect();
    void popSelectedWid();
    void resetPos();
    void moveBackground();

private:
    QList<QWidget*> selectedWidList;
    CustomMenu *contextMenu;
    QPoint dragPosition;           // 拖动窗口时鼠标的位置
    bool isDragging = false;       // 是否正在拖动窗口
    QPoint lastMousePos;
    bool continueMoving = false;
    QWidget* table = nullptr, *board = nullptr;
    QPoint beginMousePos, nowMousePos, mousePosJustNow;
    QTimer *moveTimer;
    float k_rate = 0;
    moveDirection dirX, dirY;
    bool onedge = false;

signals:
    void closePic(QWidget* w);
    void popPic(QWidget* w);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


    // QWidget interface
protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // ZOOMLABEL_H
