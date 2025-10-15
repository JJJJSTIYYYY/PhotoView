#ifndef SCREENSHOTWIDGET_H
#define SCREENSHOTWIDGET_H

#include <QMouseEvent>
#include <QWidget>
#include <QPainter>
#include <QGuiApplication>
#include <QApplication>
#include <QClipboard>
#include <QTimer>
#include <QRect>
#include <QLabel>
#include <QPushButton>
#include "ui_beautify/ctrlbar.h"
#include "ui_beautify/pointbutton.h"

class ScreenshotWidget : public QWidget
{
    Q_OBJECT

public:
    ScreenshotWidget() {
        // 设置窗口为无边框，且保持在最上层
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        setAttribute(Qt::WA_TranslucentBackground, false);
        setMouseTracking(true);

        cutWindow->setStyleSheet("border: 2px dashed #7FFFD4;"
                                 "background-color: transparent;");
        cutWindow->hide();
        ctrlB->hide();

        QString sheet = "QPushButton {"
                        "   background-color: #FFFFFF;"
                        "   border-radius: 5px;"
                        "}"
                        "QPushButton:hover {"
                        "   background-color: #CFCFCF;"
                        "   border-radius: 5px;"
                        "}";
        for(int i = 0; i < 8; i++){
            resizePoint[i] = new pointButton(this);
            resizePoint[i]->resize(10, 10);
            resizePoint[i]->setStyleSheet(sheet);
            resizePoint[i]->hide();
            connect(resizePoint[i], &pointButton::pressed, [this, i](){
                nowResizePointIndex = i;
                isResizing = true;
                isSelecting = true;
                initialPoint = resizePoint[i]->pos();
                initialRect = cutWindow->geometry();
                qDebug() << "now nowResizePointIndex is " << nowResizePointIndex;
            });
            connect(resizePoint[i], &pointButton::released, [this, i](){
                isResizing = false;
                qDebug() << "resizePointIndex release";
            });
            connect(resizePoint[i], &pointButton::pointMoved, [this, i](int deltX, int deltY){
                if(i == 0){
                    cutWindow->move(resizePoint[i]->x()+4, resizePoint[i]->y()+4);
                    deltX = resizePoint[i]->x()-initialPoint.x();
                    deltY = resizePoint[i]->y()-initialPoint.y();
                    cutWindow->resize(initialRect.width()-deltX, initialRect.height()-deltY);
                    moveResizePoint(i);
                    showCtrlBar();
                }
                else if(i == 1){
                    cutWindow->move(initialRect.x(), resizePoint[i]->y()+4);
                    deltX = resizePoint[i]->x()-initialPoint.x();
                    deltY = resizePoint[i]->y()-initialPoint.y();
                    cutWindow->resize(initialRect.width(), initialRect.height()-deltY);
                    moveResizePoint(i);
                    resizePoint[i]->move(initialPoint.x(), resizePoint[i]->y());
                    showCtrlBar();
                }
                else if(i == 2){
                    cutWindow->move(initialRect.x(), resizePoint[i]->y()+4);
                    deltX = resizePoint[i]->x()-initialPoint.x();
                    deltY = resizePoint[i]->y()-initialPoint.y();
                    cutWindow->resize(initialRect.width()+deltX, initialRect.height()-deltY);
                    moveResizePoint(i);
                    showCtrlBar();
                }
                else if(i == 3){
                    cutWindow->move(resizePoint[i]->x()+4, initialRect.y());
                    deltX = resizePoint[i]->x()-initialPoint.x();
                    deltY = resizePoint[i]->y()-initialPoint.y();
                    cutWindow->resize(initialRect.width()-deltX, initialRect.height());
                    moveResizePoint(i);
                    resizePoint[i]->move(resizePoint[i]->x(), initialPoint.y());
                    showCtrlBar();
                }
                else if(i == 4){
                    cutWindow->move(initialRect.x(), initialRect.y());
                    deltX = resizePoint[i]->x()-initialPoint.x();
                    deltY = resizePoint[i]->y()-initialPoint.y();
                    cutWindow->resize(initialRect.width()+deltX, initialRect.height());
                    moveResizePoint(i);
                    resizePoint[i]->move(resizePoint[i]->x(), initialPoint.y());
                    showCtrlBar();
                }
                else if(i == 5){
                    cutWindow->move(resizePoint[i]->x()+4, initialRect.y());
                    deltX = resizePoint[i]->x()-initialPoint.x();
                    deltY = resizePoint[i]->y()-initialPoint.y();
                    cutWindow->resize(initialRect.width()-deltX, initialRect.height()+deltY);
                    moveResizePoint(i);
                    showCtrlBar();
                }
                else if(i == 6){
                    cutWindow->move(initialRect.x(), initialRect.y());
                    deltX = resizePoint[i]->x()-initialPoint.x();
                    deltY = resizePoint[i]->y()-initialPoint.y();
                    cutWindow->resize(initialRect.width(), initialRect.height()+deltY);
                    moveResizePoint(i);
                    resizePoint[i]->move(initialPoint.x(), resizePoint[i]->y());
                    showCtrlBar();
                }
                else if(i == 7){
                    cutWindow->move(initialRect.x(), initialRect.y());
                    deltX = resizePoint[i]->x()-initialPoint.x();
                    deltY = resizePoint[i]->y()-initialPoint.y();
                    cutWindow->resize(initialRect.width()+deltX, initialRect.height()+deltY);
                    moveResizePoint(i);
                    showCtrlBar();
                }
            });
        }

        // 设置计时器，0.3秒后调用showFullScreenAndPrepare
        QTimer::singleShot(300, this, &ScreenshotWidget::showFullScreenAndPrepare);

        connect(ctrlB, &ctrlBar::confirmBtnClicked, this, &ScreenshotWidget::captureScreenshot);
        connect(ctrlB, &ctrlBar::cancelBtnClicked, [this](){
            ScreenshotWidgetExit(QPixmap());
            deleteLater();
        });
    }

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton && nowResizePointIndex == -1) {
            startPoint = event->pos(); // 使用实际鼠标坐标
            cutWindow->move(startPoint);
            cutWindow->show();
            isSelecting = true;
            ctrlB->hide();
            hideResizePoint();
            nowResizePointIndex = 8;
        }
        else if (event->button() == Qt::LeftButton && nowResizePointIndex != -1) {
            isDraging = true;
            ctrlB->hide();
            hideResizePoint();
            dragStartPos = event->pos();
            initialRect = cutWindow->geometry();
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (isSelecting && isResizing) {
            endPoint = event->pos(); // 使用实际鼠标坐标
            if(startPoint.x()-endPoint.x() > 0) cutWindow->move(endPoint.x(), cutWindow->y());
            if(startPoint.y()-endPoint.y() > 0) cutWindow->move(cutWindow->x(), endPoint.y());
            cutWindow->resize(abs(startPoint.x()-endPoint.x()), abs(startPoint.y()-endPoint.y()));
        }
        if(isDraging) cutWindow->move(initialRect.x()+event->pos().x()-dragStartPos.x(), initialRect.y()+event->pos().y()-dragStartPos.y());
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        if (isSelecting) {
            endPoint = event->pos(); // 使用实际鼠标坐标
            isSelecting = false;
        }
        isDraging = false;
        showCtrlBar();
        showResizePoint();
    }

    void keyPressEvent(QKeyEvent *event) override {
        qDebug()<<event->key();
        hideResizePoint();
        ctrlB->hide();
        if (event->key() == Qt::Key_Escape) {
            ScreenshotWidgetExit(QPixmap());
            deleteLater();
        }
        if (event->key() == Qt::Key_Control) {
            ctrlPressed = true;
        }
        else if (event->key() == Qt::Key_Space) {
            spacePressed = true;
        }
        if  (event->key() == Qt::Key_Up) {
            qDebug()<<"UpArrow";
            if(ctrlPressed) cutWindow->move(cutWindow->x(), cutWindow->y()-1);
            else if(!spacePressed){
                cutWindow->resize(cutWindow->width(), cutWindow->height()+1);
                cutWindow->move(cutWindow->x(), cutWindow->y()-1);
            }
            else{
                cutWindow->resize(cutWindow->width(), cutWindow->height()-1);
            }
        }
        else if  (event->key() == Qt::Key_Down) {
            qDebug()<<"DownArrow";
            if(ctrlPressed) cutWindow->move(cutWindow->x(), cutWindow->y()+1);
            else if(!spacePressed){
                cutWindow->resize(cutWindow->width(), cutWindow->height()+1);
            }
            else{
                cutWindow->resize(cutWindow->width(), cutWindow->height()-1);
                cutWindow->move(cutWindow->x(), cutWindow->y()+1);
            }
        }
        else if (event->key() == Qt::Key_Left) {
            qDebug()<<"LeftArrow";
            if(ctrlPressed) cutWindow->move(cutWindow->x()-1, cutWindow->y());
            else if(!spacePressed){
                cutWindow->resize(cutWindow->width()+1, cutWindow->height());
                cutWindow->move(cutWindow->x()-1, cutWindow->y());
            }
            else{
                cutWindow->resize(cutWindow->width()-1, cutWindow->height());
            }
        }
        else if (event->key() == Qt::Key_Right) {
            qDebug()<<"RightArrow";
            if(ctrlPressed) cutWindow->move(cutWindow->x()+1, cutWindow->y());
            else if(!spacePressed){
                cutWindow->resize(cutWindow->width()+1, cutWindow->height());
            }
            else{
                cutWindow->resize(cutWindow->width()-1, cutWindow->height());
                cutWindow->move(cutWindow->x()+1, cutWindow->y());
            }
        }
        moveResizePoint(-1);
    }

    void keyReleaseEvent(QKeyEvent *event) override {
        if (event->key() == Qt::Key_Escape) {
            return;
        }
        if (event->key() == Qt::Key_Control) {
            ctrlPressed = false;
        }
        showResizePoint();
        showCtrlBar();
    }

    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // 绘制整个屏幕截图
        painter.drawPixmap(0, 0, fullScreenshot);
    }

    void showResizePoint(){
        int x = cutWindow->x();
        int y = cutWindow->y();
        int w = cutWindow->width();
        int h = cutWindow->height();
        resizePoint[0]->move(x-4, y-4);
        resizePoint[1]->move(x+w/2, y-4);
        resizePoint[2]->move(x+w-4, y-4);
        resizePoint[3]->move(x-4, y+h/2);
        resizePoint[4]->move(x+w-4, y+h/2);
        resizePoint[5]->move(x-4, y+h-4);
        resizePoint[6]->move(x+w/2, y+h-4);
        resizePoint[7]->move(x+w-4, y+h-4);
        for(int i = 0; i < 8; i++){
            resizePoint[i]->show();
            resizePoint[i]->raise();
            //            qDebug() << "resizePoint index " << i << "is showing;";
        }
    }

    void moveResizePoint(int index){
        int x = cutWindow->x();
        int y = cutWindow->y();
        int w = cutWindow->width();
        int h = cutWindow->height();
        if(index !=0) resizePoint[0]->move(x-4, y-4);
        if(index !=1) resizePoint[1]->move(x+w/2, y-4);
        if(index !=2) resizePoint[2]->move(x+w-4, y-4);
        if(index !=3) resizePoint[3]->move(x-4, y+h/2);
        if(index !=4) resizePoint[4]->move(x+w-4, y+h/2);
        if(index !=5) resizePoint[5]->move(x-4, y+h-4);
        if(index !=6) resizePoint[6]->move(x+w/2, y+h-4);
        if(index !=7) resizePoint[7]->move(x+w-4, y+h-4);
    }

    void hideResizePoint(){
        for(int i = 0; i < 8; i++){
            resizePoint[i]->hide();
        }
    }

    void showCtrlBar(){
        ctrlB->move(cutWindow->x()+cutWindow->width()-46, cutWindow->y());
        ctrlB->show();
    }

private:
    QPoint startPoint;
    QPoint endPoint;
    bool isSelecting = false;
    bool isDraging = false;
    QPixmap fullScreenshot; // 存储整个屏幕截图
    QLabel *cutWindow = new QLabel(this);
    ctrlBar *ctrlB = new ctrlBar(this);
    pointButton *resizePoint[8];
    int nowResizePointIndex = -1;
    bool isResizing = true;
    QPoint initialPoint;
    QRect initialRect;
    QPoint dragStartPos;
    bool ctrlPressed = false;
    bool spacePressed = false;

    void captureScreenshot() {
        // 获取当前设备的像素比率
        qreal pixelRatio = QGuiApplication::primaryScreen()->devicePixelRatio();

        // 获取选框坐标并进行缩放调整
        QRect selectionRect(cutWindow->geometry());
        selectionRect = selectionRect.normalized(); // 确保选框是正常的

        // 使用屏幕坐标系获取选区的大小
        QRect adjustedRect(selectionRect.x() * pixelRatio, selectionRect.y() * pixelRatio,
                           selectionRect.width() * pixelRatio, selectionRect.height() * pixelRatio);

        // 从完整的屏幕截图中获取选定区域
        QPixmap selectedScreenshot = fullScreenshot.copy(adjustedRect);

        // 将截图放入剪切板
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setPixmap(selectedScreenshot);

        emit ScreenshotWidgetExit(selectedScreenshot);
        deleteLater();
    }

    void showFullScreenAndPrepare() {

        // 捕获整个屏幕的图像并保存到临时变量
        QScreen *screen = QGuiApplication::primaryScreen();
        fullScreenshot = screen->grabWindow(0);
        // 先全屏显示窗口
        showFullScreen();
    }

signals:
    void ScreenshotWidgetExit(QPixmap);

};

#endif // SCREENSHOTWIDGET_H
