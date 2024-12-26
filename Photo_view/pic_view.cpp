#include "pic_view.h"

#include <QImage>
#include <QMenu>
#include <QPainter>
#include <QStyle>
#include <QVBoxLayout>

pic_view::pic_view(QWidget *parent)
    : QWidget{parent}
{
    label = new QLabel(this);
}

pic_view::pic_view(QWidget *parent, QString path)
{
    resizeMargin = 40;
    isResizing = false;
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
//    setMouseTracking(true);
    picDir = path;
    label = new QLabel(this);
    lock = new QLabel(this);
    colorValue = new QLabel(this);
    colorValue->setFixedSize(200,40);
    colorValue->move(4, 0);
    colorValue->setStyleSheet("color:rgb(130, 130, 150)");
    colorValue->setAttribute(Qt::WA_TranslucentBackground);
    label->setAttribute(Qt::WA_TranslucentBackground);
    lock->setAttribute(Qt::WA_TranslucentBackground);
    picture = QPixmap(path);
    state = fullColor;
    lockState = unlocked;

    // 右键菜单
    contextMenu = new CustomMenu();
    contextMenu->hide();
    LabelButton *top = new LabelButton(this, "顶置", "background-color:rgb(240, 240, 240, 100);"
                                                       "color:rgb(100, 100, 100, 200);"
                                                     "border: 0.5px dashed gray;");
    LabelButton *cancel_top = new LabelButton(this, "取消顶置", "background-color:rgb(240, 240, 240, 100);"
                                                                    "color:rgb(100, 100, 100, 200);"
                                                                "border: 0.5px dashed gray;");
    LabelButton *black_white = new LabelButton(this, "关闭颜色", "background-color:rgb(240, 240, 240, 100);"
                                                                     "color:rgb(100, 100, 100, 200);"
                                                                 "border: 0.5px dashed gray;");
    LabelButton *colorful = new LabelButton(this, "打开颜色", "background-color:rgb(240, 240, 240, 100);"
                                                                  "color:rgb(100, 100, 100, 200);"
                                                              "border: 0.5px dashed gray;");
    LabelButton *openRGB = new LabelButton(this, "rgb/hsv", "background-color:rgb(240, 240, 240, 100);"
                                                            "color:rgb(100, 100, 100, 200);"
                                                            "border: 0.5px dashed gray;");
    contextMenu->addAction(top);
    contextMenu->addAction(cancel_top);
    contextMenu->addAction(black_white);
    contextMenu->addAction(colorful);
    contextMenu->addAction(openRGB);
    connect(top, &LabelButton::labelClicked, this, &pic_view::set_window_first);
    connect(cancel_top, &LabelButton::labelClicked, this, &pic_view::cancel_window_first);
    connect(black_white, &LabelButton::labelClicked, this, &pic_view::showGrayPic);
    connect(colorful, &LabelButton::labelClicked, this, &pic_view::showColorfulPic);
    connect(openRGB, &LabelButton::labelClicked, this, &pic_view::openRGBHSV);

    if (picture.isNull()) {
        throw std::runtime_error("Invalid value: path can not be null.");
    }

    // 转换灰度图
    // 创建并启动子线程
    imageProcessingThread = new ImageProcessingThread(picture, this);
    // 连接处理完成的信号和槽
    connect(imageProcessingThread, &ImageProcessingThread::processingFinished, this, &pic_view::onProcessingFinished);
    // 启动子线程
    imageProcessingThread->start();

    QPixmap pic = picture;
    pic = pic.scaled(400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->resize(pic.size());
    label->resize(pic.size());
    label->setPixmap(pic);
    aspectRatio = (double)width() / height();

    QPixmap lockImg = QPixmap(":/1/lock.png");
    lockImg = lockImg.scaled(25, 25, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    lock->resize(lockImg.size());
    lock->setPixmap(lockImg);
    lock->move(0,0);
    lock->hide();

    this->setFocusPolicy(Qt::ClickFocus);
    //    label->hide();
    colorValue->hide();
}

pic_view::pic_view(QWidget *parent, QPixmap path)
{
    qreal pixRatio = QGuiApplication::primaryScreen()->devicePixelRatio();
    path = changePixmapSize(path, path.width()/pixRatio, path.height()/pixRatio);

    resizeMargin = 40;
    isResizing = false;
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
//    setMouseTracking(true);
    picDir = "";
    label = new QLabel(this);
    lock = new QLabel(this);
    colorValue = new QLabel(this);
    colorValue->setFixedSize(200,40);
    colorValue->move(4, 0);
    colorValue->setStyleSheet("color:rgb(130, 130, 150)");
    colorValue->setAttribute(Qt::WA_TranslucentBackground);
    label->setAttribute(Qt::WA_TranslucentBackground);
    lock->setAttribute(Qt::WA_TranslucentBackground);
    picture = QPixmap(path);
    state = fullColor;
    lockState = unlocked;

    // 右键菜单
    contextMenu = new CustomMenu();
    contextMenu->hide();
    LabelButton *top = new LabelButton(this, "顶置", "background-color:rgb(240, 240, 240, 200);"
                                                       "color:rgb(100, 100, 100, 200);"
                                                       "text-align: center;");
    LabelButton *cancel_top = new LabelButton(this, "取消顶置", "background-color:rgb(240, 240, 240, 200);"
                                                                    "color:rgb(100, 100, 100, 200);"
                                                                    "text-align: center;");
    LabelButton *black_white = new LabelButton(this, "关闭颜色", "background-color:rgb(240, 240, 240, 200);"
                                                                     "color:rgb(100, 100, 100, 200);"
                                                                     "text-align: center;");
    LabelButton *colorful = new LabelButton(this, "打开颜色", "background-color:rgb(240, 240, 240, 200);"
                                                                  "color:rgb(100, 100, 100, 200);"
                                                                  "text-align: center;");
    LabelButton *openRGB = new LabelButton(this, "rgb/hsv", "background-color:rgb(240, 240, 240, 200);"
                                                            "color:rgb(100, 100, 100, 200);"
                                                            "text-align: center;");
    contextMenu->addAction(top);
    contextMenu->addAction(cancel_top);
    contextMenu->addAction(black_white);
    contextMenu->addAction(colorful);
    contextMenu->addAction(openRGB);
    connect(top, &LabelButton::labelClicked, this, &pic_view::set_window_first);
    connect(cancel_top, &LabelButton::labelClicked, this, &pic_view::cancel_window_first);
    connect(black_white, &LabelButton::labelClicked, this, &pic_view::showGrayPic);
    connect(colorful, &LabelButton::labelClicked, this, &pic_view::showColorfulPic);
    connect(openRGB, &LabelButton::labelClicked, this, &pic_view::openRGBHSV);

    // 转换灰度图
    // 创建并启动子线程
    imageProcessingThread = new ImageProcessingThread(picture, this);
    // 连接处理完成的信号和槽
    connect(imageProcessingThread, &ImageProcessingThread::processingFinished, this, &pic_view::onProcessingFinished);
    // 启动子线程
    imageProcessingThread->start();

    QPixmap pic = picture;
//    pic = pic.scaled(400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->resize(pic.size());
    label->resize(pic.size());
    label->setPixmap(pic);
    aspectRatio = (double)width() / height();

    QPixmap lockImg = QPixmap(":/1/lock.png");
    lockImg = lockImg.scaled(25, 25, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    lock->resize(lockImg.size());
    lock->setPixmap(lockImg);
    lock->move(0,0);
    lock->hide();

    qDebug() << "widget size = " << this->size();
    qDebug() << "label size = " << label->size();
    qDebug() << "pic size = " << pic.size();
    colorValue->hide();
}

void pic_view::set_window_first()
{
    contextMenu->hide();
    qDebug() << "set_window_first execute";
    this->setWindowFlag(Qt::WindowStaysOnTopHint, true);
    this->show();
}

void pic_view::cancel_window_first()
{
    contextMenu->hide();
    qDebug() << "cancel_window_first execute";
    this->setWindowFlag(Qt::WindowStaysOnTopHint, false);
    this->show();
}

void pic_view::showContextMenu(const QPoint &pos)
{
    contextMenu->resize(0, 0);
    contextMenu->move(pos);
    // 显示菜单
    contextMenu->showMenu();
}

void pic_view::showGrayPic()
{
    contextMenu->hide();
    if(grayPicturePrepared)
    {
        QPixmap temp = label->pixmap();
        QPixmap grayTemp = gray_picture.scaled(temp.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        label->setPixmap(grayTemp);
        state = whiteAndBlack;
    }
}

void pic_view::showColorfulPic()
{
    contextMenu->hide();
    QPixmap temp = label->pixmap();
    QPixmap colorfulTemp = picture.scaled(temp.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    label->setPixmap(colorfulTemp);
    state = fullColor;
}

void pic_view::openRGBHSV()
{
    contextMenu->hide();
    if(colorValue->isHidden()) colorValue->show();
    else colorValue->hide();
}

void pic_view::switchLockState()
{
    if(lockState == unlocked)
    {
        lockState = locked;
        lock->show();
    }
    else
    {
        lockState = unlocked;
        lock->hide();
    }
}

void pic_view::hideWidget(bool ifHide)
{
    if(ifHide) this->hide();
    else return;
}

void pic_view::showWidget()
{
    qDebug()<<"Pic reshow";
    this->show();
}

void pic_view::getPicFitWidSize()
{
    QPixmap pic = picture;
    pic = pic.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    label->setPixmap(pic);
    label->move(0,0);
    label->resize(pic.size());
}

QString pic_view::getPath()
{
    return picDir;
}

void pic_view::setPicDir(QString path)
{
    picDir = path;
}

void pic_view::onProcessingFinished(qint64 cr, qint64 cg, qint64 cb)
{
    bg = QColor(cr, cg, cb, 120);
    gray_picture = imageProcessingThread->getGrayscalePixmap();
    grayPicturePrepared = true;
}

void pic_view::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){ // HSV?RGB
        QPoint pos = event->pos(); // 获取鼠标位置
        pos = pos - label->pos();
        // 获取 QLabel 的 pixmap
        QPixmap scaledPixmap = label->pixmap(Qt::ReturnByValue);
        if (scaledPixmap.isNull()) {
            return; // 如果没有图像，返回
        }

        // 将 pixmap 转换为 QImage
        QImage image = scaledPixmap.toImage();
        if (image.isNull()) {
            return; // 如果转换失败，返回
        }

        // 确保鼠标位置在缩放图像范围内
        if (pos.x() < 0 || pos.x() >= image.width() || pos.y() < 0 || pos.y() >= image.height()) {
            return; // 鼠标位置超出范围，返回
        }

        // 计算缩放比例
        double scaleX = static_cast<double>(picture.width()) / scaledPixmap.width();
        double scaleY = static_cast<double>(picture.height()) / scaledPixmap.height();

        // 计算原始图像坐标
        int originalX = static_cast<int>(pos.x() * scaleX);
        int originalY = static_cast<int>(pos.y() * scaleY);

        // 确保原始坐标在图像范围内
        if (originalX < 0 || originalX >= picture.width() || originalY < 0 || originalY >= picture.height()) {
            return; // 原始坐标超出范围，返回
        }

        // 获取像素颜色
        QRgb pixelColor = picture.toImage().pixel(originalX, originalY); // 使用原始图像
        // 将像素颜色格式化为 RGB32
        QRgb color32 = qRgba(qRed(pixelColor), qGreen(pixelColor), qBlue(pixelColor), qAlpha(pixelColor));
        qDebug() << "r:" << qRed(color32) << "g:" << qGreen(color32) << "b:" << qBlue(color32);
        // 转换为 QColor
        QColor color(color32);
        // 获取 HSV 值
        int h, s, v;
        color.getHsv(&h, &s, &v);
        qDebug() << "H:" << h << "S:" << s << "V:" << v;
        colorValue->setText("r:" + QString::number(qRed(color32)) + " g:" + QString::number(qGreen(color32)) + " b:" + QString::number(qBlue(color32))
                           + "\nH:" + QString::number(h) + " S:" + QString::number(s) + " V:" + QString::number(v));
    }




    if(lockState == locked && event->button() != Qt::RightButton)
    {
        event->ignore();
        return;
    }
    if (event->button() == Qt::LeftButton) {
        if (isMouseOnEdge(event->pos()) && !spacePressed) {
            qDebug() << "isMouseOnEdge :: true";
            isResizing = true;
            resizeStartPos = event->globalPos();
            windowStartSize = size();
        } else {
            isDragging = true;
            dragPosition = event->globalPos() - frameGeometry().topLeft();
            lastMousePos = event->globalPos();
            labelStartPos = label->pos();
        }
    }
    else if (event->button() == Qt::RightButton) {
        showContextMenu(event->pos()+this->pos());
    }
    return QWidget::mousePressEvent(event);
}

void pic_view::mouseMoveEvent(QMouseEvent *event)
{
    if(lockState == locked)
    {
        return QWidget::mouseMoveEvent(event);
    }
    if (isResizing && !spacePressed) {
        setCursor(Qt::SizeFDiagCursor);
        // 计算鼠标移动的距离
        int dx = event->globalPos().x() - resizeStartPos.x();
        int dy = event->globalPos().y() - resizeStartPos.y();

        // 根据宽高比，决定是基于 dx 还是 dy 来调整大小
        int adjustedDx = dx;
        int adjustedDy = dy;

        // 使用宽度变化来决定高度变化，保持宽高比
//        if (abs(dx) > abs(dy)) {
        adjustedDy = adjustedDx / aspectRatio;
//        } else {
//            adjustedDx = adjustedDy * aspectRatio;
//        }

        // 根据调整后的 dx 和 dy 设置窗口的新尺寸
        QSize newSize = windowStartSize + QSize(adjustedDx, adjustedDy);

        // 限制窗口最小尺寸
        if (newSize.width() >= 100 || newSize.height() >= 100) {
            resize(newSize);
        }
    } else if (isDragging && !spacePressed) {
        // 移动窗口
        setCursor(Qt::ArrowCursor);
        move(event->globalPos() - dragPosition);
    } else if (isDragging && spacePressed) {
        setCursor(Qt::ClosedHandCursor);
        QPoint delta = event->globalPos() - lastMousePos;

        QPoint newPos = labelStartPos + delta;
        if(newPos.x() > 0) newPos.setX(0);
        if(newPos.y() > 0) newPos.setY(0);
        if(newPos.x() + labelSize.width() < this->width()) newPos.setX(this->width() - labelSize.width());
        if(newPos.y() + labelSize.height() < this->height()) newPos.setY(this->height() - labelSize.height());

        label->move(newPos);

    }
    return QWidget::mouseMoveEvent(event);
}

void pic_view::mouseReleaseEvent(QMouseEvent *event)
{
    if(lockState == locked)
    {
        return QWidget::mouseReleaseEvent(event);
    }
//    qDebug()<<label->pos();
    if(isResizing)
    {
        QPixmap pic = picture;
        pic = pic.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        label->setPixmap(pic);
        label->move(0,0);
        label->resize(pic.size());
        resize(pic.size());
    }
    isDragging = false;
    isResizing = false;
    setCursor(Qt::ArrowCursor);
    return QWidget::mouseReleaseEvent(event);
}

void pic_view::keyPressEvent(QKeyEvent *event)
{
    if(true)
    {
        if (event->key() == Qt::Key_Escape) {
            // 按下 Esc 键时关闭窗口
            emit pic_view_close(picDir);
            deleteLater();
        }
        else if(event->key() == Qt::Key_Space) {
            spacePressed = true;
            labelSize = label->size();
            setCursor(Qt::OpenHandCursor);
        }
        else if(event->key() == Qt::Key_Control) {
            ctrlPressed = true;
            labelSize = label->size();
            setCursor(Qt::SizeVerCursor);
        }
        else if(event->key() == Qt::Key_Tab) {
            QPixmap pic = picture;
            pic = pic.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            label->setPixmap(pic);
            label->move(0,0);
            label->resize(pic.size());
        }
        else if(event->key() == Qt::Key_C) {
            showGrayPic();
        }
        else if(event->key() == Qt::Key_L) {
            switchLockState();
        }
    }
    // 传递给基类以处理其他按键
    return QWidget::keyPressEvent(event);
}

void pic_view::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space) {
        spacePressed = false;
        setCursor(Qt::ArrowCursor);
    }
    else if(event->key() == Qt::Key_Control) {
        ctrlPressed = false;
        setCursor(Qt::ArrowCursor);
    }
    else if(event->key() == Qt::Key_C) {
        showColorfulPic();
    }
    // 传递给基类以处理其他按键
    return QWidget::keyReleaseEvent(event);
}

void pic_view::wheelEvent(QWheelEvent *event)
{
    if(lockState == locked)
    {
        return QWidget::wheelEvent(event);
    }
    // 获取鼠标全局位置
    QPoint globalMousePos = QCursor::pos();

    // 将全局坐标转换为 QLabel 的局部坐标
    QPoint labelMousePos = label->mapFromGlobal(globalMousePos);

    // 获取 QLabel 当前的大小
    QSize currentSize = label->size();

    // 根据滚轮事件进行放大或缩小
    if (event->angleDelta().y() > 0) {
        // 放大 QLabel
        if(label->width() < screenSize.width() && label->height() < screenSize.height())
        {
            if(ctrlPressed) currentSize += QSize(label->height() * 0.2 * aspectRatio, label->height() * 0.2);
            else currentSize += QSize(20 * aspectRatio, 20); // 放大
        }
    } else {
        // 缩小 QLabel
        qDebug()<<"labelSize before reduce" << label->size();
        qDebug()<<"widget before reduce" << this->size();
        if(label->width() > this->width() && label->height() > this->height())
        {
            if(ctrlPressed) currentSize -= QSize(label->height() * 0.2 * aspectRatio, label->height() * 0.2);
            else currentSize -= QSize(20 * aspectRatio, 20); // 缩小
            qDebug()<<"labelSize after reduce" << label->size();
        }
    }

    // 计算放大或缩小后的鼠标在 QLabel 中的位置偏移比例
    double scaleX = static_cast<double>(currentSize.width()) / label->width();
    double scaleY = static_cast<double>(currentSize.height()) / label->height();

    if (label->width() >= this->width() && label->height() >= this->height())
    {
        // 重新计算 QLabel 在父窗口中的位置，使得鼠标相对位置不变
        qDebug()<<"label resize";
        QPoint newLabelPos = globalMousePos - QPoint(labelMousePos.x() * scaleX, labelMousePos.y() * scaleY);
        QPoint newPos = mapFromGlobal(newLabelPos);
        if(newPos.x() + currentSize.width() < this->width()) {newPos.setX(this->width() - currentSize.width()); qDebug()<<"1";}
        if(newPos.y() + currentSize.height() < this->height()) {newPos.setY(this->height() - currentSize.height()); qDebug()<<"2";}
        if(newPos.x() > 0) newPos.setX(0);
        if(newPos.y() > 0) newPos.setY(0);
        label->move(newPos);
        // 设置标签的新大小和位置
        if (currentSize.width() < this->width() || currentSize.height() < this->height())
        {
            currentSize = this->size();
        }
        QPixmap pic;
        if(state == fullColor) pic = picture;
        else pic = gray_picture;
        pic = pic.scaled(currentSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        label->setPixmap(pic);
        label->resize(pic.size());
    }
    else label->resize(this->size());
    return QWidget::wheelEvent(event);
}

void pic_view::paintEvent(QPaintEvent *event)
{
    // 自定义绘制边框
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // 开启抗锯齿
//    painter.setPen(QPen(QColor(230, 230, 250), 3)); // 设置边框颜色和宽度
//    painter.drawRect(rect()); // 绘制矩形边框
//    painter.setBrush(QBrush(bg));
//    painter.drawRoundedRect(rect(), 10, 10);
    painter.fillRect(rect(), bg);
    // 调用父类的绘制方法
    QWidget::paintEvent(event);
}

bool pic_view::isMouseOnEdge(const QPoint &pos)
{
    return (pos.x() >= width() - resizeMargin && pos.y() >= height() - resizeMargin);
}

QPixmap pic_view::changePixmapSize(const QPixmap &originalPixmap, int newWidth, int newHeight)
{
    QPixmap newPixmap(newWidth, newHeight); // 创建新的 QPixmap
    newPixmap.fill(Qt::white); // 填充背景颜色

    QPainter painter(&newPixmap);
    painter.drawPixmap(0, 0, originalPixmap); // 绘制原始 QPixmap
    // 这里可以根据需要修改像素的其他值

    return newPixmap; // 返回新的 QPixmap
}

void pic_view::focusOutEvent(QFocusEvent *event)
{
    qDebug()<<"pic_view focusOutEvent" ;
    spacePressed = false;
    ctrlPressed = false;
}
