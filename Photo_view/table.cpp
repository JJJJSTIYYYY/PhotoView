#include "table.h"
#include "ui_table.h"

table::table(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::table)
{
    ui->setupUi(this);
    setWindowTitle("画板");
    this->setStyleSheet("background-color: transparent;");
//    // 创建 QGraphicsScene 和 QGraphicsView
//    scene = new QGraphicsScene(this);
//    view = new QGraphicsView(scene, this);
//    view->setRenderHint(QPainter::Antialiasing);
//    view->setRenderHint(QPainter::SmoothPixmapTransform);
//    // 设置 QGraphicsView 的视口背景
//    view->setStyleSheet("border: 20px;");

//    // 布局
//    QVBoxLayout *layout = new QVBoxLayout;
//    layout->addWidget(view);
//    setLayout(layout);
}

table::~table()
{
    delete ui;
}

void table::changeBri()
{
    bri = !bri;
    this->update();
}

void table::appendPicViewWid(QWidget *newWid)
{
//    scene->addWidget(newWid);
}

void table::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if(bri) painter.setPen(QPen(Qt::lightGray, 0.6, Qt::DotLine));  // 设置浅灰色虚线
    else painter.setPen(QPen(Qt::black, 0.6, Qt::DotLine));

    int gridSize = 20;  // 固定网格大小
    // 绘制水平和垂直网格线
    for (int i = 0; i < width(); i += gridSize) {
        painter.drawLine(i, 0, i, height());
    }
    for (int i = 0; i < height(); i += gridSize) {
        painter.drawLine(0, i, width(), i);
    }
    if(bri) painter.setPen(QPen(Qt::lightGray, 1, Qt::DotLine));  // 设置浅灰色虚线
    else painter.setPen(QPen(Qt::black, 1, Qt::DotLine));
    gridSize = 100;  // 固定网格大小
    // 绘制水平和垂直网格线
    for (int i = 0; i < width(); i += gridSize) {
        painter.drawLine(i, 0, i, height());
    }
    for (int i = 0; i < height(); i += gridSize) {
        painter.drawLine(0, i, width(), i);
    }

    QWidget::paintEvent(event);
}

void table::wheelEvent(QWheelEvent *event)
{
//    qDebug() << "table::wheelEvent.";
//    qDebug() << "scene has wid >> " << scene->items();

//    // 获取滚轮的滚动方向
//    if (event->angleDelta().y() > 0) {
//        // 滚轮向上滚动，放大
//        scaleFactor *= 1.1;
//    } else {
//        // 滚轮向下滚动，缩小
//        scaleFactor /= 1.1;
//    }

//    // 确保缩放比例保持在合理范围内
//    scaleFactor = qBound(0.1, scaleFactor, 3.0);  // 设置最小缩放为0.1，最大缩放为3.0

//    // 应用缩放
//    view->resetTransform();  // 重置现有变换
//    view->scale(scaleFactor, scaleFactor);  // 进行缩放

    QWidget::wheelEvent(event);  // 调用父类的 wheelEvent 处理方法
}

