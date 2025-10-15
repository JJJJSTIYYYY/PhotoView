// 处理图像：灰度
#ifndef IMAGEPROCESSINGTHREAD_H
#define IMAGEPROCESSINGTHREAD_H

#include <QObject>
#include <QThread>
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QColor>

class ImageProcessingThread : public QThread
{
    Q_OBJECT
public:
    explicit ImageProcessingThread(QObject *parent = nullptr);
    // 构造函数，接受一个 QPixmap 对象用于图像处理
    ImageProcessingThread(const QPixmap& pixmap, QObject* parent = nullptr);
    qint64 r = 0;
    qint64 g = 0;
    qint64 b = 0;
    // 返回处理后的灰度图像
    QPixmap getGrayscalePixmap() const;

signals:
    // 处理完成时发出的信号
    void processingFinished(qint64 cr, qint64 cg, qint64 cb);

protected:
    // 子线程的运行函数
    void run() override;

private:
    QPixmap picture;       // 原始图片
    QPixmap gray_picture;  // 转换后的灰度图片

};

#endif // IMAGEPROCESSINGTHREAD_H
