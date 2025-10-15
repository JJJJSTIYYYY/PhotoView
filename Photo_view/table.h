//inner gridBoard
// 画板中可移动子窗口
#ifndef TABLE_H
#define TABLE_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QTransform>
#include <QGraphicsView>

namespace Ui {
class table;
}

class table : public QWidget
{
    Q_OBJECT

public:
    explicit table(QWidget *parent = nullptr);
    ~table();
    void changeBri();
    void appendPicViewWid(QWidget* newWid);

private:
    Ui::table *ui;
    bool bri = true;
    QGraphicsView *view;
    QGraphicsScene *scene;
    qreal scaleFactor;  // 用于跟踪当前缩放比例

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event);
};

#endif // TABLE_H
