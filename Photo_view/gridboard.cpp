#include "gridboard.h"
#include "ui_gridboard.h"

#include <QTimer>

gridBoard::gridBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gridBoard)
{
    ui->setupUi(this);
    overlayLabel = nullptr;
    setMouseTracking(true);
    setWindowTitle("画板");
    QString sheet = "QToolButton {"
                    "   background-color: transparent;"
                    "}";
    this->lightMode = new QToolButton(this);
    this->lightMode->resize(30,30);
    this->lightMode->setStyleSheet(sheet);
    this->lightMode->setIcon(QIcon(":/1/sun.png"));
    this->lightMode->setIconSize(QSize(30, 30));
    this->windowMode = new QToolButton(this);
    this->windowMode->resize(30,30);
    this->windowMode->setStyleSheet(sheet);
    this->windowMode->setIcon(QIcon(":/1/ding_red.png"));
    this->windowMode->setIconSize(QSize(30, 30));
    this->sortBtn = new QToolButton(this);
    this->sortBtn->resize(37,37);
    this->sortBtn->setStyleSheet(sheet);
    this->sortBtn->setIcon(QIcon(":/1/sort.png"));
    this->sortBtn->setIconSize(QSize(37, 37));
    this->setStyleSheet("background-color: #f0f0f0;");

    connect(lightMode, &QToolButton::clicked, this, &gridBoard::changeLightMode);
    connect(windowMode, &QToolButton::clicked, this, &gridBoard::changeWindowMode);
    connect(sortBtn, &QToolButton::clicked, this, &gridBoard::sortWid);

    setAcceptDrops(true);

    this->setFocusPolicy(Qt::StrongFocus);
    this->setMinimumSize(400, 400);
//    this->setMaximumSize(2000, 2000);
}

gridBoard::~gridBoard()
{
    delete ui;
}

void gridBoard::changeIsAllowToClose(bool value)
{
    isAllowToClose = value;
}

void gridBoard::setLabel(table *l)
{
    label = l;
    label->move(0,0);
    labelSize = label->size();
    qDebug() << "labelSize is "<<labelSize;
    maxX = this->width()-labelSize.width();
    maxY = this->height()-labelSize.height();

    overlayLabel = new zoomLabel(label);
    overlayLabel->setStyleSheet("background-color: rgba(200, 200, 200, 80);"
                                "border: 3px dashed #5F7Fe4;");  // 半透明灰色背景
    overlayLabel->setWindowFlag(Qt::FramelessWindowHint);
    overlayLabel->hide();
    connect(overlayLabel, &zoomLabel::closePic, this, &gridBoard::getSignalClosePic);
    connect(overlayLabel, &zoomLabel::popPic, this, &gridBoard::getSignalPopPic);
}

void gridBoard::changeLightMode()
{
    // 创建渐变动画
    QPropertyAnimation *animation = new QPropertyAnimation(this, "backgroundColor");
    animation->setDuration(300);  // 动画持续时间500毫秒
    QColor startColor = isSun ? QColor("#f0f0f0") : QColor("#2E2F30");  // 当前背景颜色
    QColor endColor = isSun ? QColor("#2E2F30") : QColor("#f0f0f0");    // 目标背景颜色

    animation->setStartValue(startColor);
    animation->setEndValue(endColor);

    // 设置渐变动画效果
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    // 设置窗口背景颜色变化
    connect(animation, &QPropertyAnimation::valueChanged, this, [this](const QVariant &value) {
        QColor color = value.value<QColor>();
        this->setStyleSheet(QString("background-color: %1;").arg(color.name()));
    });

    // 设置光模式按钮图标
    if (isSun) {
        lightMode->setIcon(QIcon(":/1/moon.png"));
    } else {
        lightMode->setIcon(QIcon(":/1/sun.png"));
    }
    lightMode->setIconSize(QSize(30, 30));
    QTimer::singleShot(150, this, [this](){
        label->changeBri();
    });

    isSun = !isSun;
}

void gridBoard::changeWindowMode(bool toClose)
{
    if(toClose){
        this->setWindowFlag(Qt::WindowStaysOnTopHint, false);
    }
    if(isTop) {
        this->setWindowFlag(Qt::WindowStaysOnTopHint, false);
        bool isFull = this->isFullScreen;
        if(isFull) showNormal();
        QTimer::singleShot(100, this, [this, isFull](){
            this->show();
            if(isFull){
                this->showFullScreen();
                this->isFullScreen = true;
            }
            else{
                this->showNormal();
                this->isFullScreen = false;
            }
        });
        windowMode->setIcon(QIcon(":/1/ding_red.png"));
    }
    else{
        this->setWindowFlag(Qt::WindowStaysOnTopHint, true);
        bool isFull = this->isFullScreen;
        if(isFull) showNormal();
        QTimer::singleShot(100, this, [this, isFull](){
            this->show();
            if(isFull){
                this->showFullScreen();
                this->isFullScreen = true;
            }
            else{
                this->showNormal();
                this->isFullScreen = false;
            }
        });
        windowMode->setIcon(QIcon(":/1/ding_gray.png"));
    }
    windowMode->setIconSize(QSize(30, 30));
    isTop = !isTop;
}

void gridBoard::appendGuideBtn(guideBtn *btn)
{
//    guideBtnList.append(btn);
    btn->hide();
    connect(btn, &guideBtn::clicked, this, &gridBoard::guideToTarget);
}

void gridBoard::animationMoveLabel(QRect rect)
{
    // 获取当前label控件的位置
    QPoint startPos = label->pos();
    // 判断图片窗口相对画板的位置
    QPoint relativePos = QPoint(rect.x(),rect.y()) + label->pos();
    QPoint finalPos = QPoint(0,0);
    if(relativePos.x() < 0) {
        int deltaX = -relativePos.x();
        finalPos.setX(startPos.x()+deltaX);
    }
    else if(relativePos.x() > this->width()-rect.width()){
        int deltaX = relativePos.x() + rect.width() - this->width();
        finalPos.setX(startPos.x()-deltaX);
    }
    else {
        finalPos.setX(startPos.x());
    }
    if(relativePos.y() < 0) {
        int deltaY = -relativePos.y();
        finalPos.setY(startPos.y()+deltaY);
    }
    else if(relativePos.y() > this->height()-rect.height()){
        int deltaY = relativePos.y() + rect.height() - this->height();
        finalPos.setY(startPos.y()-deltaY);
    }
    else {
        finalPos.setY(startPos.y());
    }
    // 限制标签位置不越界
    if(finalPos.x() >= 0) finalPos.setX(0);
    if(finalPos.y() >= 0) finalPos.setY(0);
    if(finalPos.x() <= maxX) finalPos.setX(maxX);
    if(finalPos.y() <= maxY) finalPos.setY(maxY);

    // 创建动画对象，目标属性是label的"pos"
    QPropertyAnimation *animation = new QPropertyAnimation(label, "pos");
    animation->setDuration(400);  // 设置动画持续时间为400毫秒
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->setStartValue(startPos);  // 起始位置
    animation->setEndValue(finalPos);   // 目标位置

    // 启动动画
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    QTimer::singleShot(410, this, [this](){
        emit this->tableMoveFinish(QRect(this->pos(),this->size()), QRect(label->pos(),label->size()));
    });
}

void gridBoard::guideToTarget(QRect rect, QWidget *target)
{
    target->raise();
    if(overlayLabel) overlayLabel->raise();
    animationMoveLabel(rect);
}

void gridBoard::getSelectedWindows(const QRect &selectionRect)
{
    qDebug() << "gridBoard::getSelectedWindows.";
    // 获取所有子控件并检查哪些控件在选择框内
    QList<QWidget *> allWidgets = findChildren<QWidget *>();
    selectPicViewPointerList.clear();
    for (QWidget *widget : allWidgets) {
        QRect widgetRect = widget->geometry();
        if (selectionRect.intersects(widgetRect)) { // 检查空间是否相交
            if (widget->metaObject()->className() == QString("pic_view")) {
                qDebug() << "Found pic_view window:" << widget;
                selectPicViewPointerList.append(widget);
                widget->raise();
                emit selected(widget);
            }
        }
    }
}

void gridBoard::createOverlayLabel()
{
    if (!selectPicViewPointerList.isEmpty()) {
        QRect combinedRect = selectPicViewPointerList.first()->geometry();

        // 合并所有选中控件的矩形
        for (QWidget *widget : selectPicViewPointerList) {
            combinedRect = combinedRect.united(widget->geometry());
        }

        if(overlayLabel){
            overlayLabel->getSelectWidList(selectPicViewPointerList);
            overlayLabel->setGeometry(combinedRect);  // 设置 QLabel 的位置和大小
            overlayLabel->show();  // 显示覆盖的标签
            overlayLabel->raise();  // 显示覆盖的标签
        }
    }
}

void gridBoard::sortWid()
{
    // 检查overlayLabel是否隐藏
    if (overlayLabel->isHidden()) {
        selectionStart = QPoint(0, 0);
        selectionEnd = QPoint(label->width(), label->height());
        selectionActive = false;

        // 获取选择框矩形
        QRect selectionRect = QRect(selectionStart, selectionEnd).normalized();

        // 获取选中的所有窗口指针
        getSelectedWindows(selectionRect);
    }
    else overlayLabel->hide();

    int maxRowWidth = label->width()-40;  // 最大行宽
    int currentX = 0;  // 当前行的起始X坐标
    int currentY = 0;  // 当前行的起始Y坐标
    int rowMinHeight = INT_MAX;  // 当前行最小高度

    QList<QWidget *> currentRowWidgets;  // 当前行中的所有窗口
    QList<QWidget *> allWidgets = selectPicViewPointerList;  // 获取所有选中的窗口

    emit sortBegin();

    // 第一阶段：无缩放，逐行排版，直到最大宽度
    for (QWidget *widget : allWidgets) {
        int widgetWidth = widget->width();
        int widgetHeight = widget->height();

        // 检查当前窗口能否放入当前行
        if (currentX + widgetWidth <= maxRowWidth || currentX == 0) {
            currentRowWidgets.append(widget);
            currentX += widgetWidth;  // 将窗口排放到当前行，并更新X坐标

            // 更新当前行最小高度
            rowMinHeight = qMin(rowMinHeight, widgetHeight);
        } else {
            // 当前窗口超过最大宽度，需要换行
            adjustRowWidgets(currentRowWidgets, rowMinHeight, currentY);
            currentY += rowMinHeight;  // 换行，更新Y坐标
            currentRowWidgets.clear();  // 清空当前行
            currentRowWidgets.append(widget);  // 将当前窗口放入新行
            currentX = widgetWidth;  // 重置X坐标为当前窗口宽度
            rowMinHeight = widgetHeight;  // 重置最小高度为当前窗口高度
        }
    }

    // 处理最后一行的窗口
    if (!currentRowWidgets.isEmpty()) {
        adjustRowWidgets(currentRowWidgets, rowMinHeight, currentY);
    }

    // 将背景窗口移到(0, 0)位置
    animationMoveLabelPos_0_0();
}

// 第二阶段：等比缩放窗口，并将其从左到右排列
void gridBoard::adjustRowWidgets(const QList<QWidget *> &rowWidgets, int minHeight, int currentY)
{
    int rowWidth = 0;

    // 等比缩放
    for (QWidget *widget : rowWidgets) {
        int newHeight = minHeight;  // 窗口缩放后的目标高度
        int newWidth = widget->width() * newHeight / widget->height();  // 保持宽高比等比缩放

        // 创建动画效果来平滑改变窗口大小和位置
        QPropertyAnimation *resizeAnimation = new QPropertyAnimation(widget, "geometry");
        resizeAnimation->setDuration(500);  // 动画时长500ms
        resizeAnimation->setStartValue(widget->geometry());
        resizeAnimation->setEndValue(QRect(rowWidth, currentY, newWidth, newHeight));  // 设置最终目标位置和大小
        resizeAnimation->setEasingCurve(QEasingCurve::OutCubic);  // 使用缓出动画曲线

        // 启动动画
        resizeAnimation->start();

        // 更新当前行的宽度
        rowWidth += newWidth;  // 更新当前X坐标
    }
    QTimer::singleShot(510, this, [this](){
        emit sortFinish();
    });
}


//animationMoveLabelPos_0_0();
void gridBoard::animationMoveLabelPos_0_0(QPoint finalPos, int duration)
{
    // 创建动画对象，目标属性是label的"pos"
    QPropertyAnimation *animation = new QPropertyAnimation(label, "pos");
    animation->setDuration(duration);  // 设置动画持续时间为400毫秒
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->setStartValue(label->pos());  // 起始位置
    animation->setEndValue(finalPos);   // 目标位置

    // 启动动画
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    QTimer::singleShot(510, this, [this](){
        emit this->tableMoveFinish(QRect(this->pos(),this->size()), QRect(label->pos(),label->size()));
    });
}

void gridBoard::getSignalClosePic(QWidget *w)
{
    qDebug()<<"gridBoard::getSignalClosePic >> "<<w;
    emit closePicTransf(w);
    overlayLabel->hide();
}

void gridBoard::getSignalPopPic(QWidget *w)
{
    qDebug()<<"gridBoard::getSignalPopPic >> "<<w;
    emit popPicTransf(w);
    overlayLabel->hide();
}

void gridBoard::closeEvent(QCloseEvent *event)
{
    qDebug() << "Board closeEvent";
    if(!isAllowToClose){
        this->showMinimized();
        qDebug() << "Board don't allow to close!";
        event->ignore();
    }
}

void gridBoard::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if(overlayLabel&& !overlayLabel->geometry().contains(event->pos())) {
            overlayLabel->hide();
        }
        if(!ctrlPressed){
            Dragging = true;
            dragPosition = event->globalPos() - frameGeometry().topLeft();
            lastMousePos = event->globalPos();
            labelStartPos = label->pos();
        }
        else if(ctrlPressed){
            selectionStart = event->pos();
            selectionActive = true;
//            update();
        }
    }
}

void gridBoard::mouseMoveEvent(QMouseEvent *event) {
    if (!ctrlPressed && Dragging) {
        setCursor(Qt::ClosedHandCursor);
        QPoint delta = event->globalPos() - lastMousePos;

        QPoint newPos = labelStartPos + delta;

        qDebug() << "before reset >>" <<newPos <<"/nmaxX/Y is" <<maxX<<maxY;
        // 限制标签位置不越界
        if(newPos.x() >= 0) newPos.setX(0);
        if(newPos.y() >= 0) newPos.setY(0);
        if(newPos.x() <= maxX) newPos.setX(maxX);
        if(newPos.y() <= maxY) newPos.setY(maxY);

        label->move(newPos);
        qDebug() << newPos;
    }
    else if(ctrlPressed){
        if (selectionActive) {
            // 更新选择框的尺寸
            selectionEnd = event->pos();

            // 获取选择框矩形
            QRect selectionRect = QRect(selectionStart-label->pos(), selectionEnd-label->pos()).normalized();

            // 获取选中的所有窗口指针
            getSelectedWindows(selectionRect);
            // 在选中的控件上创建一个半透明的QLabel
            createOverlayLabel();
            update();
        }
    }
}

void gridBoard::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if(!ctrlPressed){
            Dragging = false; // 停止拖动
            qDebug() << "Table move finish.";
            emit tableMoveFinish(QRect(this->pos(),this->size()), QRect(label->pos(),label->size()));
            setCursor(Qt::ArrowCursor);
        }
        else if(ctrlPressed){
            // 右键释放，选择框结束，获取选中区域的窗口
            selectionEnd = event->pos();
            selectionActive = false;

            // 获取选择框矩形
            QRect selectionRect = QRect(selectionStart-label->pos(), selectionEnd-label->pos()).normalized();

            // 获取选中的所有窗口指针
            getSelectedWindows(selectionRect);
            // 在选中的控件上创建一个半透明的QLabel
            createOverlayLabel();
            update();
        }
    }
}

void gridBoard::keyPressEvent(QKeyEvent *event)
{
    //    qDebug() << "table :: key pressed is >>" << event->key();
    if(event->key() == Qt::Key_Return){
        if(this->isMaximized()) {
            this->showNormal();
        }
        else {
            this->showMaximized();
        }
    }
    else if(event->key() == Qt::Key_Control) {
        ctrlPressed = true;
        Dragging = false;
        qDebug() << "Table move finish.";
        emit tableMoveFinish(QRect(this->pos(),this->size()), QRect(label->pos(),label->size()));
        setCursor(Qt::ArrowCursor);
        labelSize = label->size();
    }
    else if(event->key() == Qt::Key_A && ctrlPressed){
        selectionStart = QPoint(0, 0);
        selectionEnd = QPoint(label->width(), label->height());
        selectionActive = false;

        // 获取选择框矩形
        QRect selectionRect = QRect(selectionStart, selectionEnd).normalized();

        // 获取选中的所有窗口指针
        getSelectedWindows(selectionRect);
        // 在选中的控件上创建一个半透明的QLabel
        createOverlayLabel();
    }
}

void gridBoard::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control) {
        ctrlPressed = false;
        setCursor(Qt::ArrowCursor);
    }
}

void gridBoard::resizeEvent(QResizeEvent *event)
{
    if(this->width() > 39500 || this->height() > 3950) {event->accept(); return;}
    qDebug() << "gridBoard::resizeEvent.";
    lightMode->move(this->width()-80, 10);
    windowMode->move(this->width()-41, 10);
    sortBtn->move(this->width()-46, 50);
    lightMode->raise();
    windowMode->raise();
    sortBtn->raise();
    maxX = event->size().width()-labelSize.width();
    maxY = event->size().height()-labelSize.height();
    QPoint newPos = label->pos();
    // 限制标签位置不越界
    if(newPos.x() >= 0) newPos.setX(0);
    if(newPos.y() >= 0) newPos.setY(0);
    if(newPos.x() <= maxX) newPos.setX(maxX);
    if(newPos.y() <= maxY) newPos.setY(maxY);
    label->move(newPos);
    emit tableMoveFinish(QRect(this->pos(),this->size()), QRect(label->pos(),label->size()));
}

void gridBoard::moveEvent(QMoveEvent *event)
{
}

void gridBoard::mouseDoubleClickEvent(QMouseEvent *event)
{
    //    qDebug() << "table :: key pressed is >>" << event->key();
    if(isFullScreen) {
        this->showNormal();
    }
    else {
        this->showFullScreen();
    }
    isFullScreen = !isFullScreen;
}

void gridBoard::paintEvent(QPaintEvent *event)
{
    if(ctrlPressed){
        QPainter painter(this);

        // 设置虚线边框
        painter.setPen(QPen(Qt::DashLine));  // 设置虚线
        painter.setBrush(QColor(200, 200, 200, 20));  // 设置半透明浅灰色填充 (RGBA, A=100表示50%透明度)

        if (selectionActive) {
            // 创建选择框矩形并绘制
            QRect selectionRect(selectionStart, selectionEnd);
            painter.drawRect(selectionRect);  // 绘制虚线框
            painter.fillRect(selectionRect, painter.brush());  // 填充半透明浅灰色
        }
    }
}

void gridBoard::wheelEvent(QWheelEvent *event)
{
    // 获取滚轮的增量
    int deltaY = event->angleDelta().y();
    int deltaX = event->angleDelta().x();

    qDebug()<<"gridBoard::wheelEvent >> ctrlPressed =" <<ctrlPressed;
    // 根据 ctrlPressed 的状态选择滚动方向
    if (ctrlPressed) {  // 横向滚动
        // 如果滚轮向右滚动
        if (deltaY > 0) {
            if (label->x() <= -40) {
                animationMoveLabelPos_0_0(QPoint(label->x() + 40, label->y()), 300);  // 向右滚动
            }
            else if (label->x() > -40 && label->x() <= 0) {
                animationMoveLabelPos_0_0(QPoint(0, label->y()), 300);  // 滚动到左边界
            }
        }
        // 如果滚轮向左滚动
        else if (deltaY < 0) {
            if (label->x() - 40 >= this->width() - label->width()) {
                animationMoveLabelPos_0_0(QPoint(label->x() - 40, label->y()), 300);  // 向左滚动
            }
            else if (label->x() - 40 < this->width() - label->width()) {
                animationMoveLabelPos_0_0(QPoint(this->width() - label->width(), label->y()), 300);  // 滚动到右边界
            }
        }
    }
    else {  // 纵向滚动
        // 如果滚轮向上滚动（y坐标增大）
        if (deltaY > 0) {
            if (label->y() <= -40) {
                animationMoveLabelPos_0_0(QPoint(label->x(), label->y() + 40), 300);  // 向下滚动
            }
            else if (label->y() > -40 && label->y() <= 0) {
                animationMoveLabelPos_0_0(QPoint(label->x(), 0), 300);  // 滚动到顶部
            }
        }
        // 如果滚轮向下滚动（y坐标减小）
        else if (deltaY < 0) {
            if (label->y() - 40 >= this->height() - label->height()) {
                animationMoveLabelPos_0_0(QPoint(label->x(), label->y() - 40), 300);  // 向上滚动
            }
            else if (label->y() - 40 < this->height() - label->height()) {
                animationMoveLabelPos_0_0(QPoint(label->x(), this->height() - label->height()), 300);  // 滚动到底部
            }
        }
    }
}

void gridBoard::showBtn()
{

}

void gridBoard::hideBtn()
{

}


