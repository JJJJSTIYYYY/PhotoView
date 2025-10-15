#include "imageprocessingthread.h"

ImageProcessingThread::ImageProcessingThread(QObject *parent)
    : QThread {parent}
{

}

// 构造函数，传入待处理的 QPixmap
ImageProcessingThread::ImageProcessingThread(const QPixmap& pixmap, QObject* parent)
    : QThread(parent), picture(pixmap) {}

// 返回处理后的灰度 QPixmap
QPixmap ImageProcessingThread::getGrayscalePixmap() const {
    return gray_picture;
}

// 子线程中的图像处理操作
void ImageProcessingThread::run() {
    // 将 QPixmap 转换为 QImage
    QImage image = picture.toImage();

    // 创建一个新的灰度 QImage，使用 RGB32 格式来存储高质量灰度图
    QImage grayImage(image.size(), QImage::Format_RGB32);

    // 使用 QPainter 绘制时启用抗锯齿
    QPainter painter(&grayImage);
    painter.setRenderHint(QPainter::Antialiasing, true);  // 启用抗锯齿
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);  // 启用平滑图像变换

    // 遍历每个像素，手动计算灰度值
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            // 获取当前像素的颜色
            QColor color = image.pixelColor(x, y);
            r += color.red();
            g += color.green();
            b += color.blue();
            // 使用加权公式计算灰度值
            int gray = qRound(0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue());

            // 在 RGB32 格式中存储灰度值，将灰度值赋给 R、G、B 通道
            grayImage.setPixel(x, y, qRgb(gray, gray, gray));
        }
    }
    r /= (image.height()*image.width());
    r *= 1.2;
    g /= (image.height()*image.width());
    g *= 1.2;
    b /= (image.height()*image.width());
    b *= 1.2;
    if(r > 255) r = 255;
    if(g > 255) g = 255;
    if(b > 255) b = 255;
    // 完成绘制，关闭 painter
    painter.end();

    // 将灰度图像转换回 QPixmap
    gray_picture = QPixmap::fromImage(grayImage);

    // 发出信号，通知处理完成
    emit processingFinished(r, g, b);
}
