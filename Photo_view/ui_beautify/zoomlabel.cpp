#include "zoomlabel.h"

zoomLabel::zoomLabel(QWidget *parent, QList<QWidget*> selectWidList)
    : QLabel{parent}
{
    selectedWidList = selectWidList;

    // 右键菜单
    contextMenu = new CustomMenu();
    contextMenu->hide();
    LabelButton *clo = new LabelButton(this, "关闭", "background-color:rgb(240, 240, 240, 200);"
                                                     "color:rgb(100, 100, 100, 200);"
                                                     "text-align: center;");
    LabelButton *hid = new LabelButton(this, "取消", "background-color:rgb(240, 240, 240, 200);"
                                                     "color:rgb(100, 100, 100, 200);"
                                                     "text-align: center;");
    LabelButton *pop = new LabelButton(this, "弹出画板", "background-color:rgb(240, 240, 240, 200);"
                                                     "color:rgb(100, 100, 100, 200);"
                                                     "text-align: center;");
    contextMenu->addAction(clo);
    contextMenu->addAction(hid);
    contextMenu->addAction(pop);
    connect(clo, &LabelButton::labelClicked, this, &zoomLabel::closeSelectedWid);
    connect(hid, &LabelButton::labelClicked, this, &zoomLabel::cancelSelect);
    connect(pop, &LabelButton::labelClicked, this, &zoomLabel::popSelectedWid);

    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &zoomLabel::moveBackground);
    moveTimer->setInterval(150);

    if(parent->metaObject()->className() == QString("table")) this->table = parent;
    if(this->table && this->table->parentWidget()->metaObject()->className() == QString("gridBoard")) this->board = this->table->parentWidget();
    if(!table || !board) qDebug()<<"Error to get board or table!";

}

void zoomLabel::closeSelectedWid()
{
    for(QWidget* p : selectedWidList){
        qDebug()<<"zoomLabel::closeSelectedWid >> emit closePic(p).";
        emit closePic(p);
    }
}

void zoomLabel::getSelectWidList(QList<QWidget *> selectWidList)
{
    selectedWidList = selectWidList;
}

QList<QWidget *> zoomLabel::getAllSelectedWid()
{
    return selectedWidList;
}

void zoomLabel::showContextMenu(const QPoint &pos)
{
    contextMenu->resize(0, 0);
    contextMenu->move(pos);
    // 显示菜单
    contextMenu->showMenu();
}

void zoomLabel::cancelSelect()
{
    selectedWidList.clear();
    this->hide();
}

void zoomLabel::popSelectedWid()
{
    for(QWidget* p : selectedWidList){
        qDebug()<<"zoomLabel::popSelectedWid >> emit popPic(p).";
        emit popPic(p);
    }
}

void zoomLabel::resetPos()
{
    QPoint newPos = this->pos();
    // 限制标签位置不越界
    int maxX = this->parentWidget()->width()-this->width();
    int maxY = this->parentWidget()->height()-this->height();
    if(newPos.x() <= 0) {newPos.setX(0);}
    if(newPos.y() <= 0) {newPos.setY(0);}
    if(newPos.x() >= maxX) {newPos.setX(maxX);}
    if(newPos.y() >= maxY) {newPos.setY(maxY);}
    int deltaX = this->x()-newPos.x();
    int deltaY = this->y()-newPos.y();
    this->move(newPos);
    for(QWidget* p : selectedWidList){
        if(p) p->move(p->x()-deltaX, p->y()-deltaY);
    }
}

void zoomLabel::moveBackground()
{
    // 枚举值为选择框移动方向，背景移动应与之相反
    if(dirX == none){
        // 横向不移动
        if(dirY == up){ // table 下移
            if(table->y()+20 < 0){
                this->move(this->x(), this->y()-20);
                for(QWidget* p : selectedWidList){
                    if(p) p->move(p->x(), p->y()-20);
                }
                table->move(table->x(), table->y()+20);
//                resetPos();
            }
        }
        else if(dirY == down){ // table 上移
            if(table->y()-20 >board->height()-table->height()){
                this->move(this->x(), this->y()+20);
                for(QWidget* p : selectedWidList){
                    if(p) p->move(p->x(), p->y()+20);
                }
                table->move(table->x(), table->y()-20);
                //                resetPos();
            }
        }
    }
    else if(dirY == none){
        // 纵向不移动
        if(dirX == left){
            if(table->x()+20 < 0){ // table 右移， x增大
                this->move(this->x()-20, this->y());
                for(QWidget* p : selectedWidList){
                    if(p) p->move(p->x()-20, p->y());
                }
                table->move(table->x()+20, table->y());
                //                resetPos();
            }
        }
        else if(dirX == right){ // table 左移， x减小
            if(table->x()-20 >board->width()-table->width()){
                this->move(this->x()+20, this->y());
                for(QWidget* p : selectedWidList){
                    if(p) p->move(p->x()+20, p->y());
                }
                table->move(table->x()-20, table->y());
                //                resetPos();
            }
        }
    }
    else if(dirX == right && dirY == up){

    }
    else if(dirX == right && dirY == down){

    }
    else if(dirX == left && dirY == up){

    }
    else if(dirX == left && dirY == down){

    }
//    for(QWidget* p : selectedWidList){
//        if(p){
//            p->jugdeShowGuide()
//        }
//    }
}

void zoomLabel::mousePressEvent(QMouseEvent *event)
{
    this->raise();
    mousePosJustNow = event->pos();
    if (event->button() == Qt::RightButton) {
        showContextMenu(event->globalPos());
    }
    else if(event->button() == Qt::LeftButton){
        isDragging = true;
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        lastMousePos = event->globalPos();
        beginMousePos = QCursor::pos(); // 全局坐标
        qDebug() << "zoomLabel::mousePressEvent >> Get beginMousePos >>" << beginMousePos;
    }
}

void zoomLabel::mouseMoveEvent(QMouseEvent *event)
{
    if(/*!onedge*/true){
        // 移动窗口
        setCursor(Qt::ArrowCursor);
        QPoint newPos = event->globalPos() - dragPosition;
        int deltaX = this->x()-newPos.x();
        int deltaY = this->y()-newPos.y();
        move(newPos);
        for(QWidget* p : selectedWidList){
            if(p) p->move(p->x()-deltaX, p->y()-deltaY);
        }
        resetPos();
    }
//    // 判断边界
//    nowMousePos = QCursor::pos();
//    qDebug() << "zoomLabel::mouseMoveEvent >> Get nowMousePos >>" << nowMousePos;
//    onedge = false;
//    QPoint tempNow = table->mapToGlobal(this->pos());
//    tempNow = board->mapFromGlobal(tempNow);
//    if(tempNow.x() <= 1
//        || tempNow.x() >= board->width()-this->width()-1
//        || tempNow.y() <= 1
//        || tempNow.y() >= board->height()-this->height()-1){
//        onedge = true;
//    }

//    if(isDragging && onedge){
//        dragPosition = event->globalPos() - frameGeometry().topLeft();
//        if (!moveTimer->isActive()) {
//            qDebug() << "zoomLabel::mouseMoveEvent >> is on edge. Start background move timer";
//            moveTimer->start();  // 启动定时器
//        }
//        int deltaX = nowMousePos.x() - beginMousePos.x();
//        int deltaY = nowMousePos.y() - beginMousePos.y();
//        if (deltaX > 20 && tempNow.x() >= board->width()-this->width()-5) dirX = right;
//        else if (deltaX < -20 && tempNow.x() <= 5) dirX = left;
//        else dirX = none;
//        if (deltaY > 20 && tempNow.y() >= board->height()-this->height()-5) dirY = down;
//        else if (deltaY < -20 && tempNow.y() <= 5) dirY = up;
//        else dirY = none;
//        if(dirX != none && dirY != none) {
//            k_rate = deltaY/deltaX*1.0;
//        }
//        else if(dirX == none && dirY == none) {
//            moveTimer->stop();  // 停止定时器
//            onedge = false;
//            dirX = dirY = none;
//        }
//    } else {
//        if(dirX != none || dirY != none) if (moveTimer && moveTimer->isActive()) {
//            qDebug() << "zoomLabel::mouseMoveEvent >> Mouse no longer on edge. Stop background move timer";
//            moveTimer->stop();  // 停止定时器
//            onedge = false;
//            dirX = dirY = none;
//        }
//    }
//    mousePosJustNow = event->pos();
}

void zoomLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        int x = this->x();
        int y = this->y();
        if(x%20 >= 10) x = (int(x/20)+1)*20;
        else x = int(x/20)*20;
        if(y%20 >= 10) y = (int(y/20)+1)*20;
        else y = int(y/20)*20;
        int deltaX = this->x()-x;
        int deltaY = this->y()-y;
        move(x,y);
        for(QWidget* p : selectedWidList){
            if(p) p->move(p->x()-deltaX, p->y()-deltaY);
        }

        if (moveTimer && moveTimer->isActive()) {
            moveTimer->stop();
            dirX = dirY = none;
            onedge = false;
            qDebug() << "zoomLabel::mouseReleaseEvent >> Stopped background move timer";
        }
    }
}

void zoomLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    this->hide();
}
