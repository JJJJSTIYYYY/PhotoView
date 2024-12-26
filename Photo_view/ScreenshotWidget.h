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

class ScreenshotWidget : public QWidget
{
    Q_OBJECT

public:
    ScreenshotWidget() {
        // 设置窗口为无边框，且保持在最上层
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        setAttribute(Qt::WA_TranslucentBackground, false);
        setMouseTracking(true);

        // 设置计时器，0.3秒后调用showFullScreenAndPrepare
        QTimer::singleShot(300, this, &ScreenshotWidget::showFullScreenAndPrepare);
    }

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            startPoint = event->pos(); // 使用实际鼠标坐标
            isSelecting = true;
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (isSelecting) {
            endPoint = event->pos(); // 使用实际鼠标坐标
            update(); // 重新绘制窗口以显示选框
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        if (isSelecting) {
            endPoint = event->pos(); // 使用实际鼠标坐标
            isSelecting = false;
            captureScreenshot(); // 捕获屏幕选择
            close(); // 关闭窗口
        }
    }

    void keyPressEvent(QKeyEvent *event) override {
        if (event->key() == Qt::Key_Escape) {
            close(); // 按下 Esc 键时关闭窗口
            ScreenshotWidgetExit(QPixmap());
        }
    }

    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // 绘制整个屏幕截图
        painter.drawPixmap(0, 0, fullScreenshot);

        // 绘制选框
        painter.setPen(Qt::red);
        painter.setBrush(Qt::NoBrush);
        QRect selectionRect(startPoint, endPoint);
        painter.drawRect(selectionRect.normalized());
    }

private:
    QPoint startPoint;
    QPoint endPoint;
    bool isSelecting = false;
    QPixmap fullScreenshot; // 存储整个屏幕截图

    void captureScreenshot() {
        // 获取当前设备的像素比率
        qreal pixelRatio = QGuiApplication::primaryScreen()->devicePixelRatio();

        // 获取选框坐标并进行缩放调整
        QRect selectionRect(startPoint, endPoint);
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
