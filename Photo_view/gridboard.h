// 画板窗口
#ifndef GRIDBOARD_H
#define GRIDBOARD_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QToolButton>
#include <QPropertyAnimation>
#include <QGraphicsColorizeEffect>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QApplication>
#include <QWindow>
#include "table.h"
#include "ui_beautify/guidebtn.h"
#include "ui_beautify/zoomlabel.h"

namespace Ui {
class gridBoard;
}

class gridBoard : public QWidget
{
    Q_OBJECT

public:
    explicit gridBoard(QWidget *parent = nullptr);
    ~gridBoard();
    void changeIsAllowToClose(bool value);
    void setLabel(table* l);
    void changeLightMode();
    void changeWindowMode(bool toClose = false);
    void appendGuideBtn(guideBtn *btn);
    void animationMoveLabel(QRect rect);
    void guideToTarget(QRect rect, QWidget* target);
    void getSelectedWindows(const QRect &selectionRect);
    void createOverlayLabel();
    void sortWid();
    void animationMoveLabelPos_0_0(QPoint finalPos = QPoint(0, 0), int duration = 500);

    void adjustRowWidgets(const QList<QWidget *> &rowWidgets, int minHeight, int currentY);
private:
    Ui::gridBoard *ui;
    bool isAllowToClose = false;
    bool isFullScreen = false;
    bool isSun = true;
    bool isTop = false;

    QPoint dragStartPosition; // 鼠标相对窗口的位置
    bool Dragging = false;   // 拖动状态
    int titleBarHeight;  // 窗口标题栏的高度
    QPoint dragPosition;           // 拖动窗口时鼠标的位置
    QPoint labelStartPos;          // label相对窗口的位置
    QPoint lastMousePos;
    QSize labelSize;

    QToolButton *lightMode, *windowMode, *sortBtn;
    QList<QWidget*> selectPicViewPointerList;
//    QList<guideBtn *> guideBtnList;

    zoomLabel *overlayLabel;

    int maxX = 0;
    int maxY = 0;

    table *label = nullptr;

    bool ctrlPressed = false;

    QPoint selectionStart;
    QPoint selectionEnd;
    bool selectionActive;

    void getSignalClosePic(QWidget* w);
    void getSignalPopPic(QWidget* w);

protected:
    void closeEvent(QCloseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);

signals:
    void tableMoveFinish(QRect board, QRect table);
    void closePicTransf(QWidget* w);
    void popPicTransf(QWidget* w);
    void sortFinish();
    void sortBegin();
    void selected(QWidget*);

    // QWidget interface
protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

signals:
    void dragEnterEventSignal(QDragEnterEvent *event);
    void dragLeaveEventSignal(QDragLeaveEvent *event);
    void dropEventSignal(QDropEvent *event);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
    void wheelEvent(QWheelEvent *event);

    void showBtn();
    void hideBtn();
};

inline void gridBoard::dragEnterEvent(QDragEnterEvent *event) {
    emit dragEnterEventSignal(event);  // 发射信号到主窗口
    QWidget::dragEnterEvent(event);
}

inline void gridBoard::dragLeaveEvent(QDragLeaveEvent *event) {
    emit dragLeaveEventSignal(event);  // 发射信号到主窗口
    QWidget::dragLeaveEvent(event);
}

inline void gridBoard::dropEvent(QDropEvent *event) {
    emit dropEventSignal(event);  // 发射信号到主窗口
    QWidget::dropEvent(event);
}

#endif // GRIDBOARD_H
