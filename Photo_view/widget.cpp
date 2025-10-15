#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();
    ui->setupUi(this);
    setAcceptDrops(true);
    this->setWindowIcon(QIcon(":/1/pic.png"));
    this->setWindowTitle("PhotoView");

    pic_table->resize(4000, 4000);
    board->setLabel(pic_table);

    thread = new QThread;
    downloader = new ImageDownloader();
    // 连接信号和槽
    connect(thread, &QThread::started, downloader, &ImageDownloader::downloadImage);
    connect(downloader, &ImageDownloader::imageDownloaded, this, &Widget::onImageDownloaded);
    connect(downloader, &ImageDownloader::finished, thread, &QThread::quit);
    connect(downloader, &ImageDownloader::imageDownloadError, this, &Widget::imageDownloadError);

    // 设置动画持续时间和曲线类型
    resizeAnimation = new QPropertyAnimation(this, "geometry");
    resizeAnimation->setEasingCurve(QEasingCurve::OutCubic); // 使用平滑的缓出动画曲线

    // 构建设置页
    settingWid = new Setting(this);
    settingWid->hide();
    connect(settingWid, &Setting::closeSettingWid, this, &Widget::showMainWid);

    // 构建主页
    mainWid = new QWidget(this);
    mainWid->move(0, 0);

    QVBoxLayout *layOut = new QVBoxLayout(this);

    Select_Pic = new QPushButton(this);
    Select_Pic->setText("选择图片...");
    connect(Select_Pic, &QPushButton::clicked, this, &Widget::Select_Widget);

    set_proxy = new QPushButton(this);
    set_proxy->setText("顶置所有窗口");
    connect(set_proxy, &QPushButton::clicked, this, &Widget::set_proxy_clicked);

    cancel_set_proxy = new QPushButton(this);
    cancel_set_proxy->setText("取消顶置所有窗口");
    connect(cancel_set_proxy, &QPushButton::clicked, this, &Widget::cancel_set_proxy_clicked);

    clear_all = new QPushButton(this);
    clear_all->setText("关闭所有窗口");
    connect(clear_all, &QPushButton::clicked, this, &Widget::clear);

    PrtScreen = new QPushButton(this);
    PrtScreen->setText("截图");
    connect(PrtScreen, &QPushButton::clicked, this, &Widget::getScreenShot);

    get_clipboard = new QPushButton(this);
    get_clipboard->setText("打开剪切板");
    connect(get_clipboard, &QPushButton::clicked, this, &Widget::getClipboard);

    suspend = new QPushButton(this);
    suspend->setText("悬浮窗模式");
    connect(suspend, &QPushButton::clicked, this, &Widget::suspendWindow);

    LabelButton *l = new LabelButton(this, "@Justiy", "QLabel { color : black; }");
    l->setFixedHeight(15);
    l->setAlignment(Qt::AlignCenter);
    QObject::connect(l, &LabelButton::labelClicked, this, &Widget::showJustiy);

    QToolButton *settingBtn = new QToolButton(this);
    settingBtn->setIcon(QIcon(":/1/setting.png"));
    connect(settingBtn, &QToolButton::clicked, this, &Widget::showSetting);

    // 显示主页
    mainWid->resize(240, 280);
    layOut->addWidget(Select_Pic);
    layOut->addWidget(set_proxy);
    layOut->addWidget(cancel_set_proxy);
    layOut->addWidget(clear_all);
    layOut->addWidget(PrtScreen);
    layOut->addWidget(get_clipboard);
    layOut->addWidget(suspend);
    layOut->addWidget(l);
    layOut->addWidget(settingBtn, 0, Qt::AlignCenter);
    mainWid->setLayout(layOut);

    // 构建悬浮窗
    suspendWid = new QWidget(this);
    suspendWid->move(0, 4);
    suspendWid->setFixedSize(30, 300);

    QVBoxLayout *HlayOut = new QVBoxLayout(this);
    HlayOut->setContentsMargins(5, 5, 5, 5);

    QLabel *drag = new QLabel(this);
    drag->setAlignment(Qt::AlignCenter);
    drag->setFixedSize(20,20);
    drag->setPixmap(QPixmap(":/1/icon_circle.png").scaled(20,20,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
//    connect(drag, &QLabel::)

    Select_Pic_Suspend = new QToolButton(this);
    Select_Pic_Suspend->setIcon(QIcon(":/1/open.png"));
    connect(Select_Pic_Suspend, &QToolButton::clicked, this, &Widget::Select_Widget);

    set_proxy_Suspend = new QToolButton(this);
    set_proxy_Suspend->setIcon(QIcon(":/1/top.png"));
    connect(set_proxy_Suspend, &QToolButton::clicked, [this](){
        emit this->set_proxy_clicked();
        if(!settingWid->get_checkBox_hidePop()) {
            PopWid *messagePop = new PopWid;
            messagePop->showWithText("所有图片已顶置", QPoint(this->pos().x(), this->pos().y()+59), this->width());
        }
    });

    cancel_set_proxy_Suspend = new QToolButton(this);
    cancel_set_proxy_Suspend->setIcon(QIcon(":/1/cancel_top.png"));
    connect(cancel_set_proxy_Suspend, &QToolButton::clicked, [this](){
        emit this->cancel_set_proxy_clicked();
        if(!settingWid->get_checkBox_hidePop()) {
            PopWid *messagePop = new PopWid;
            messagePop->showWithText("图片已取消顶置", QPoint(this->pos().x(), this->pos().y()+85), this->width());
        }
    });

    clear_all_Suspend = new QToolButton(this);
    clear_all_Suspend->setIcon(QIcon(":/1/close_all.png"));
    connect(clear_all_Suspend, &QToolButton::clicked, [this](){
        emit this->clear();
        if(!settingWid->get_checkBox_hidePop()) {
            PopWid *messagePop = new PopWid;
            messagePop->showWithText("已关闭所有参考图", QPoint(this->pos().x(), this->pos().y()+113), this->width());
        }
    });

    PrtScreen_Suspend = new QToolButton(this);
    PrtScreen_Suspend->setIcon(QIcon(":/1/screen_shot.png"));
    connect(PrtScreen_Suspend, &QToolButton::clicked, this, &Widget::getScreenShot);

    get_clipboard_Suspend = new QToolButton(this);
    get_clipboard_Suspend->setIcon(QIcon(":/1/paste.png"));
    connect(get_clipboard_Suspend, &QToolButton::clicked, this, &Widget::getClipboard);

    AI_Pic_Suspend = new QToolButton(this);
    AI_Pic_Suspend->setIcon(QIcon(":/1/AI.png"));
    connect(AI_Pic_Suspend, &QToolButton::clicked, this, &Widget::showGenerateAIPic);

    cancel_suspend = new QToolButton(this);
//    cancel_suspend->setIcon(QIcon(":/1/return.png"));
    cancel_suspend->setIcon(QIcon(":/1/relate.png"));
//    connect(cancel_suspend, &QToolButton::clicked, this, &Widget::cancelSuspendWindow);
    connect(cancel_suspend, &QToolButton::clicked, [this](){
        this->cancelSuspendWindow();
        PopWid *messagePop = new PopWid;
        messagePop->showWithText("所有图片已关联画板", QPoint(this->pos().x(), this->pos().y()+193), this->width());
//        messagePop->showWithText("所有图片已顶置", QPoint(this->pos().x(), this->pos().y()+59), this->width());
    });

    exit_Suspend = new LabelButton(this, "", "");
    exit_Suspend->setAlignment(Qt::AlignCenter);
    exit_Suspend->setFixedSize(20,20);
    exit_Suspend->setPixmap(QPixmap(":/1/exit.png").scaled(20,20,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    connect(exit_Suspend, &LabelButton::labelClicked, this, &Widget::exit_Suspend_clicked);

    settingBtn_Suspend = new QToolButton(this);
    settingBtn_Suspend->setIcon(QIcon(":/1/setting.png"));
    connect(settingBtn_Suspend, &QToolButton::clicked, this, &Widget::showSetting);

    HlayOut->addWidget(drag);
    HlayOut->addWidget(Select_Pic_Suspend);
    HlayOut->addWidget(set_proxy_Suspend);
    HlayOut->addWidget(cancel_set_proxy_Suspend);
    HlayOut->addWidget(clear_all_Suspend);
    HlayOut->addWidget(PrtScreen_Suspend);
    HlayOut->addWidget(get_clipboard_Suspend);
    HlayOut->addWidget(cancel_suspend);
    HlayOut->addWidget(AI_Pic_Suspend);
    HlayOut->addWidget(settingBtn_Suspend);
    HlayOut->addWidget(exit_Suspend);
    suspendWid->setLayout(HlayOut);

    // 构建dragEventWidget
    dragEventWidget = new QLabel(this);
    dragEventWidget->resize(250,300);
    dragEventWidget->move(35, 5);
    dragEventWidget->hide();
    dragEventWidget->setText("请将图片拖拽至\n窗口任意位置");
    dragEventWidget->setStyleSheet(
        "font-family: 'YouYuan';"           // 设置字体为幼圆
        "font-size: 25px;"
        "color: gray;"                      // 设置字体颜色为灰色
        "border: 1.5px dashed gray;"          // 设置圆角虚线边框
        "border-radius: 10px;"              // 设置圆角半径
        "qproperty-alignment: 'AlignCenter';"  // 文本居中
        );

    // 构建ai出图的描述信息编辑窗口
    AIDescribleWid = new AIDescrible(this);
    AIDescribleWid->setFixedSize(240, 300);
    //    AIDescribleWid->move(30, 5);
    AIDescribleWid->hide();
    connect(AIDescribleWid, &AIDescrible::closeWid, this, &Widget::showGenerateAIPic);
    connect(AIDescribleWid, &AIDescrible::generate, this, &Widget::generateAIPic);

    // 页面切换动画
    moveAnimation1 = new QPropertyAnimation(settingWid, "geometry");
    moveAnimation1->setEasingCurve(QEasingCurve::OutCubic); // 使用平滑的缓动曲线
    moveAnimation2 = new QPropertyAnimation(AIDescribleWid, "geometry");
    moveAnimation2->setEasingCurve(QEasingCurve::OutCubic); // 使用平滑的缓动曲线
//    moveAnimation2 = new QPropertyAnimation(moreFuncWid, "geometry");
//    moveAnimation2->setEasingCurve(QEasingCurve::OutCubic); // 使用平滑的缓动曲线
    resizeAnimation->setDuration(500);  // 动画持续时间（毫秒）
    resizeAnimation->setEasingCurve(QEasingCurve::OutCubic);  // 设置动画缓动曲线

    // 添加系统托盘图标
    m_showMainAction = new LabelButton(this, "强制关闭", "background-color:rgb(240, 240, 240, 100);"
                                                     "color:rgb(100, 100, 100, 200);"
                                                     "border: 0.5px dashed gray;");
    m_exitAppAction = new LabelButton(this, "退出", "background-color:rgb(240, 240, 240, 100);"
                                                    "color:rgb(100, 100, 100, 200);"
                                                    "border: 0.5px dashed gray;");
    m_reShowBoard = new LabelButton(this, "画板", "background-color:rgb(240, 240, 240, 100);"
                                                    "color:rgb(100, 100, 100, 200);"
                                                    "border: 0.5px dashed gray;");

    m_menu = new CustomMenu(this);
    m_menu->addAction(m_showMainAction);
    m_menu->addAction(m_reShowBoard);
    m_menu->addAction(m_exitAppAction);

    mSysTrayIcon = new QSystemTrayIcon(this);
    QIcon icon = QIcon(":/1/favicon.ico");
    mSysTrayIcon->setIcon(icon);
    mSysTrayIcon->setToolTip(QString("PhotoView"));
    mSysTrayIcon->setContextMenu(static_cast<QMenu*>(m_menu));
    mSysTrayIcon->show();

    connect(m_showMainAction, &LabelButton::labelClicked, this, &Widget::clearAllForece);
    connect(m_exitAppAction, &LabelButton::labelClicked, this, &Widget::close);
    connect(m_reShowBoard, &LabelButton::labelClicked, [this](){
        qDebug()<< "reshow board";
        if(!board && !pic_table) {
            pic_table->deleteLater();
            board->deleteLater();
            this->board = new gridBoard(nullptr);
            this->pic_table = new table(board);
        }
        else if(!board) {
            pic_table->deleteLater();
            this->board = new gridBoard(nullptr);
            this->pic_table = new table(board);
        }
        else if(!pic_table) {
            board->deleteLater();
            this->board = new gridBoard(nullptr);
            this->pic_table = new table(board);
        }
        this->board->show();
        this->pic_table->show();
        this->board->showNormal();
    });
    connect(mSysTrayIcon, &QSystemTrayIcon::activated, [this](QSystemTrayIcon::ActivationReason reason){
        if (reason == QSystemTrayIcon::Trigger) {
            // 显示主窗口
            this->show();  // 或者使用 this->setVisible(true) 来显示窗口
            this->raise();  // 将窗口提升到最前
            this->activateWindow();  // 激活窗口
        }
    });

    test = new openai;
    connect(test, &openai::returnMessage, this, &Widget::showMsgFronGpt);
    connect(test, &openai::returnError, this, &Widget::showMsgFronGpt);
    connect(test, &openai::generateFinish, this, &Widget::showAIPic);
    connect(test, &openai::modifyDescribeFinish, [this](QString des){
        test->setAPIKeyPic(settingWid->get_lineEdit_APIKeyPic());
        if(AIDescribleWid->getModol() == "DALLE.2"){
            test->setModelPic("dall-e-2");
            test->setQualityPic("");
        }
        else if(AIDescribleWid->getModol() == "DALLE.3"){
            test->setModelPic("dall-e-3");
            test->setQualityPic("");
        }
        else if(AIDescribleWid->getModol() == "DALLE.3.hd"){
            test->setModelPic("dall-e-3");
            test->setQualityPic("hd");
        }
        AIDescribleWid->setDesribeText(des);
        test->callOpenAI_pic(des, AIDescribleWid->getSize());
    });
    chatWithGpt.setInterval(20000);
    chatWithGpt.start();
    connect(&chatWithGpt, &QTimer::timeout, this, &Widget::getRespondFronGpt);

    connect(board, &gridBoard::dragEnterEventSignal, this, &Widget::onDragEnterEvent);
    connect(board, &gridBoard::dragLeaveEventSignal, this, &Widget::onDragLeaveEvent);
    connect(board, &gridBoard::dropEventSignal, this, &Widget::onDropEvent);
    board->show();
//    pic_table->hide();
//    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    // 重载图片
    reloadPic();
}

Widget::~Widget()
{
    delete ui;
    delete resizeAnimation;
}

void Widget::reloadPic()
{
    if(loadPicPathFromJson())
    {
        for(int i = 0; i < PicPath.count(); i++)
        {
            qDebug()<<PicPath.at(i);
            if(!picViewList.contains(PicPath.at(i)))
            {
                try {
                    bool isOnT = isOnTableList.at(i);
                    guideBtn *btn = new guideBtn(board);
                    board->appendGuideBtn(btn);
                    pic_view *wid = new pic_view(isOnT?pic_table:nullptr, PicPath.at(i), pic_table, board, btn);
                    connect(this, &Widget::set_proxy_clicked, wid, &pic_view::set_window_first);
                    connect(this, &Widget::cancel_set_proxy_clicked, wid, &pic_view::cancel_window_first);
                    connect(this, &Widget::clear_all_clicked, wid, &pic_view::deleteLater);
                    connect(this, &Widget::PrtScreen_clicked, wid, &pic_view::hideWidget);
                    connect(this, &Widget::reshowAllWidget, wid, &pic_view::showWidget);
                    connect(wid, &pic_view::pic_view_close, this, &Widget::modifyPicPath);
                    wid->show();
                    wid->move(coordinatesList.at(i));
                    wid->resize(sizeList.at(i));
                    wid->getPicFitWidSize();
                    btn->setTarget(wid);
                    QTimer::singleShot(400, this, [this, wid](){
                        wid->jugdeShowGuide(QRect(this->board->pos(), this->board->size()), QRect(this->pic_table->pos(), this->pic_table->size()));
                        this->pic_table->appendPicViewWid(wid);
                    });
                    picViewList[PicPath.at(i)] = QPointer<pic_view>(wid);
                } catch (const std::exception& e) {
                    std::cerr << "Exception caught: " << e.what() << std::endl;
                    if(picViewList.contains(PicPath.at(i)))
                    {
                        picViewList[PicPath.at(i)]->deleteLater();
                        picViewList.remove(PicPath.at(i));
                    }
                }
            }
            else picViewList[PicPath.at(i)]->show();
        }
        isSuspended = state;
        if(isSuspended)
        {
            suspendWindow();
            this->repaint();
        }
        else
        {
            cancelSuspendWindow();
        }
        this->move(widPosition[0], widPosition[1]);
        if( boardPosition[0] != 0 ||
            boardPosition[1] != 0 ||
            boardPosition[2] != 0 ||
            boardPosition[3] != 0 ){
            board->move(boardPosition[0], boardPosition[1]);
            board->resize(boardPosition[2], boardPosition[3]);
        }
    }
    else
    {
        // 显示主页
        this->resize(240, 280);
        suspendWid->hide();
        this->show();
        mainWid->show();
        isSuspended = false;
    }
}

void Widget::clear()
{
    emit clear_all_clicked();
    PicPath.clear();
    picViewList.clear();
}

void Widget::clearAllForece()
{
    // 关闭所有找到的pic_view窗口
    QMap<QString, QPointer<pic_view>>::iterator it;
    for (it = picViewList.begin(); it != picViewList.end(); ++it) {
        // 检查 QPointer 是否为空，如果不为空则删除对象
        if (!it.value().isNull()) {
            it.value()->deleteLater();  // 删除 pic_view 对象
            it.value() = nullptr;  // 将 QPointer 设置为空指针
        }
    }
    qDebug() << "Close Forece";
    picViewList.clear();
    PicPath.clear();
}

void Widget::showSetting()
{
    if(!isSuspended)
    {
        settingWid->move(0, 0);
        settingWid->show();
        mainWid->hide();
    }
    else // 悬浮窗模式下打开设置
    {
        if(isSetWidShow)
        {
            this->animateResize(QSize(30, this->height()));
            isSetWidShow = false;
            settingWid->hide();
        }
        else
        {
            animateMove(1);
            if(isAIWidShow) {
                isAIWidShow = false;
            }
            this->animateResize(QSize(270, this->height()));
            settingWid->show();
            isSetWidShow = true;
        }
    }
}

void Widget::showGenerateAIPic()
{
    if(isAIWidShow)
    {
        this->animateResize(QSize(30, this->height()));
        isAIWidShow = false;
        AIDescribleWid->hide();
    }
    else
    {
        animateMove(0);
        if(isSetWidShow) {
//            settingWid->hide();
            isSetWidShow = false;
        }
        this->animateResize(QSize(270, this->height()));
        AIDescribleWid->show();
        isAIWidShow = true;
    }
}

void Widget::showMainWid() // 这个函数 只在 设置页(取消，确认)按下时 执行，  sus-》main在 cancelSuspend
{
    if(isSuspended) // 悬浮窗模式下显示主窗口
    {
        showSetting();

    }
    else // 普通模式下显示主窗口，关闭设置页时执行
    {
        settingWid->hide();
        mainWid->show();
    }
}

void Widget::savePicPathToJson()
{
    // 获取用户文档目录
    QString docPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString filePath = docPath + "/Photo_View/config.json";

    // 创建 JSON 对象
    QJsonObject jsonObj;
    QJsonArray jsonArray;
    QJsonArray jsonArray2;
    QJsonArray jsonArray3;
    QJsonArray jsonArray4;
    QJsonArray jsonArray5;

    // 将 QList<QString> 转换为 QJsonArray
    for (QString path : picViewList.keys()) {
        jsonArray.append(path);
    }

    jsonArray2.append(widPosition[0]);
    jsonArray2.append(widPosition[1]);

    for (pic_view *picViewWid : picViewList.values()) {
        const QPoint pos = picViewWid->pos();
        QJsonObject pointObj;
        pointObj["x"] = pos.x();
        pointObj["y"] = pos.y();
        pointObj["isOnTable"] = picViewWid->getIsOnTable();
        qDebug() << "isOnTable = " << pointObj["isOnTable"];
        jsonArray3.append(pointObj);
    }

    for (pic_view *picViewWid : picViewList.values()) {
        const QSize size = picViewWid->size();
        QJsonObject sizeObj;
        sizeObj["w"] = size.width();
        sizeObj["h"] = size.height();
        jsonArray4.append(sizeObj);
    }

    jsonArray5.append(boardPosition[0]);
    jsonArray5.append(boardPosition[1]);
    jsonArray5.append(boardPosition[2]);
    jsonArray5.append(boardPosition[3]);

    jsonObj["PicturePaths"] = jsonArray;
    jsonObj["WindowState"] = isSuspended;
    jsonObj["Position"] = jsonArray2; // 窗口坐标
    jsonObj["coordinates"] = jsonArray3; // pic_view坐标
    jsonObj["size"] = jsonArray4; // pic_view窗口大小
    jsonObj["boardPos"] = jsonArray5; // pic_view窗口大小

    // 创建 JSON 文档
    QJsonDocument jsonDoc(jsonObj);

    // 写入 JSON 文件
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "无法打开文件进行写入:" << filePath;
        return;
    }

    file.write(jsonDoc.toJson());
    file.close();

    qDebug() << "保存成功:" << filePath;
}

bool Widget::loadPicPathFromJson()
{
    // 获取用户文档目录
    QString docPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString filePath = docPath + "/Photo_View/config.json";

    QList<QString> picPathList;

    // 打开 JSON 文件
    QFile file(filePath);
    if (!file.exists()) {
        qDebug() << "文件不存在:" << filePath;

        return false; // 返回空的 QList
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开文件进行读取:" << filePath;
        return false; // 返回空的 QList
    }

    QByteArray fileData = file.readAll();
    file.close();

    // 解析 JSON 数据
    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData);
    if (!jsonDoc.isObject()) {
        qDebug() << "JSON 数据格式错误";
        return false;
    }

    QJsonObject jsonObj = jsonDoc.object();
    if (jsonObj.contains("PicturePaths") && jsonObj["PicturePaths"].isArray()) {
        QJsonArray jsonArray = jsonObj["PicturePaths"].toArray();

        // 将 QJsonArray 转换为 QList<QString>
        for (const QJsonValue &value : jsonArray) {
            if (value.isString()) {
                picPathList.append(value.toString());
                coordinatesList.append(QPoint(0, 0));
            }
        }
    }
    if (jsonObj.contains("WindowState")) {
        state = jsonObj["WindowState"].toBool();
    }
    if (jsonObj.contains("Position")) {
        QJsonArray jsonArray = jsonObj["Position"].toArray();

        // 将 QJsonArray 转换为 QList<QString>
        int i = 0;
        for (const QJsonValue &value : jsonArray) {
//            qDebug() << value.toInt();
            if(i == 0) widPosition[i] = value.toInt();
            if(i == 1) widPosition[i] = value.toInt();
            i++;
        }
    }
    else
    {
        widPosition[0] = 50;
        widPosition[1] = 50;
    }
    if (jsonObj.contains("coordinates") && jsonObj["coordinates"].isArray()) {
        QJsonArray coordinatesArray = jsonObj["coordinates"].toArray();
        coordinatesList.clear();
        isOnTableList.clear();
        // 将 QJsonArray 转换为 QList<QPoint>
        for (const QJsonValue &value : coordinatesArray) {
            if (value.isObject()) {
                QJsonObject pointObj = value.toObject();
                int x = pointObj["x"].toInt();
                int y = pointObj["y"].toInt();
                coordinatesList.append(QPoint(x, y));
                isOnTableList.append(pointObj["isOnTable"].toBool());
            }
        }
    }
    if (jsonObj.contains("size") && jsonObj["size"].isArray()) {
        QJsonArray sizeArray = jsonObj["size"].toArray();
        for (const QJsonValue &value : sizeArray) {
            if (value.isObject()) {
                QJsonObject pointObj = value.toObject();
                int w = pointObj["w"].toInt();
                int h = pointObj["h"].toInt();
                sizeList.append(QSize(w, h));
            }
        }
    }
    if (jsonObj.contains("boardPos")){
        QJsonArray jsonArray = jsonObj["boardPos"].toArray();
        int i = 0;
        for (const QJsonValue &value : jsonArray) {
//            qDebug() << value.toInt();
            if(i == 0) boardPosition[i] = value.toInt();
            if(i == 1) boardPosition[i] = value.toInt();
            if(i == 2) boardPosition[i] = value.toInt();
            if(i == 3) boardPosition[i] = value.toInt();
            i++;
        }
    }
    else
    {
        boardPosition[0] = 0;
        boardPosition[1] = 0;
        boardPosition[2] = 0;
        boardPosition[3] = 0;
    }

    qDebug() << "initial pos:" << widPosition[0] << "," << widPosition[1];
    qDebug() << "读取成功:" << filePath;
    PicPath = picPathList;
    return true;
}

void Widget::modifyPicPath(QString dir)
{
    PicPath.removeAll(dir);
    if(picViewList.contains(dir))
    {
        picViewList[dir]->deleteLater();
        picViewList.remove(dir);
    }
    qDebug() << "picViewList修改成功";
}

void Widget::getClipboard()
{
    // 获取剪切板
    QClipboard *clipboard = QApplication::clipboard();

    // 从剪切板获取图像
    QImage image = clipboard->image();

    // 检查是否有图像
    if (image.isNull()) {
        qDebug() << "剪切板中没有图片";
        PopWid *messagePop = new PopWid(nullptr, PopWid::Warning);
        messagePop->showWithText("剪切板中没有图片", QPoint(this->pos().x(), this->pos().y()+166), this->width());

//        test->callOpenAI_pic2("照片的山丘覆盖着粉红色的草，阳光明媚的天气，在屏幕中间的放大镜，辛烷值渲染");

        return;
    }
    else if(!settingWid->get_checkBox_hidePop()){
        PopWid *messagePop = new PopWid;
        messagePop->showWithText("已打开剪切板", QPoint(this->pos().x(), this->pos().y()+172), this->width());
    }

    // 保存图片到文件
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    filePath += "/Screenshots";
    QDir path = QDir(filePath);
    if (!path.exists()) {
        // 创建 Screenshots 目录
        if (path.mkpath(".")) { // mkpath(".") 创建所有缺失的父目录
            qDebug() << "目录创建成功:" << filePath;
        } else {
            qDebug() << "无法创建目录:" << filePath;
        }
    }

    // 保存图片
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString savedPath = filePath + "/" + currentDateTime.toString("yyyyMMddHHmmss")+".png";
    if (image.save(savedPath)) {
        qDebug() << "图片已保存:" << savedPath;
    } else {
        qDebug() << "保存图片失败:" << savedPath;
    }

    // 显示图片
    guideBtn *btn = new guideBtn(board);
    board->appendGuideBtn(btn);
    pic_view *wid = new pic_view(settingWid->get_checkBox_defaultBoard()?pic_table:nullptr, savedPath, pic_table, board, btn);
    connect(this, &Widget::set_proxy_clicked, wid, &pic_view::set_window_first);
    connect(this, &Widget::cancel_set_proxy_clicked, wid, &pic_view::cancel_window_first);
    connect(this, &Widget::clear_all_clicked, wid, &pic_view::deleteLater);
    connect(this, &Widget::PrtScreen_clicked, wid, &pic_view::hideWidget);
    connect(this, &Widget::reshowAllWidget, wid, &pic_view::showWidget);
    connect(wid, &pic_view::pic_view_close, this, &Widget::modifyPicPath);
    PicPath.append(savedPath);
    wid->show();
    btn->setTarget(wid);
    QTimer::singleShot(400, this, [this, wid](){
            wid->jugdeShowGuide(QRect(this->board->pos(), this->board->size()), QRect(this->pic_table->pos(), this->pic_table->size()));
        });
    if(settingWid->get_checkBox_defaultBoard()) this->showBoard();

    picViewList[savedPath] = QPointer<pic_view>(wid);
}

void Widget::generateAIPic()
{
    if(AIDescribleWid->getText() == "") {
        PopWid *messagePop = new PopWid;
        messagePop->showWithText("请输入描述", QPoint(this->pos().x(), this->pos().y()), this->width());
        return;
    }
    AIDescribleWid->disableSend(true);
    qDebug() << "modify describe......";
    test->setAPIKey(settingWid->get_lineEdit_APIKey());
    test->callOpenAIModifyDescribe(AIDescribleWid->getText());
}

void Widget::suspendWindow()
{
    isSuspended = true;
    this->hide();
    this->resize(30, 310);
    settingWid->resize(240, 310);
    QPoint temp = this->pos();
    int titleBarHeight = this->style()->pixelMetric(QStyle::PM_TitleBarHeight);
    temp.setY(temp.y()-titleBarHeight);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    mainWid->hide();
    suspendWid->show();
    this->move(temp);
    settingWid->move(30, 320);
    AIDescribleWid->move(30, -320);
    this->show();
    qDebug() << "suspendWindow widSize :" << this->size();
}

void Widget::cancelSuspendWindow() // 悬浮窗恢复普通窗口使用
{
    if(false){ // 返回显示普通窗口：弃置
        this->hide();
        isSuspended = false;
        // 显示主页
        this->setWindowFlags(Qt::Window);  // 设置回有边框的窗口
        this->setWindowFlag(Qt::WindowStaysOnTopHint, false);
        this->setAttribute(Qt::WA_TranslucentBackground, false);
        this->resize(240, 280);
        settingWid->resize(240, 280);
        settingWid->move(0, 0);
        AIDescribleWid->move(30, -320);
        suspendWid->hide();
        dragEventWidget->hide();
        settingWid->hide();
        AIDescribleWid->hide();
        isSetWidShow = false;
        isAIWidShow = false;
        mainWid->show();
        this->show();
    }
    if(true){ // 所有窗口关联画板：启用
        for (pic_view *picViewWid : picViewList.values()) {
            picViewWid->showOnTableAll();
        }
    }
}

void Widget::exit_Suspend_clicked()
{
    if(!wantExit)
    {
        if(!settingWid->get_checkBox_hidePop()) {
            PopWid *messagePop = new PopWid(nullptr, PopWid::Error);
            messagePop->showWithText("再按一次退出", QPoint(this->pos().x(), this->pos().y()+275), this->width());
        }
        wantExit = true;

//        board->showNormal();
        QTimer::singleShot(500, this, [this](){
            wantExit = false;
        });
    }
    else
    {
        if(board) {
            board->showNormal();
        }
        thread->quit();
        thread->wait();
        thread->deleteLater();
        QTimer::singleShot(100, this, [this](){
            this->close();
        });
    }
}

void Widget::showJustiy()
{
    QDialog *tempWid = new QDialog(this);
    tempWid->setFixedSize(150, 90);
    QLabel *l = new QLabel(this);
    LabelButton *qq = new LabelButton(tempWid, "", "QLabel { color : black; }");
    LabelButton *mail = new LabelButton(tempWid, "", "QLabel { color : black; }");
    l->setText("提供反馈");
    qq->setText("点击复制QQ号");
    mail->setText("点击复制邮箱");
    QObject::connect(qq, &LabelButton::labelClicked, [&l]() {
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText("3129424572");
        l->setText("QQ号复制成功！");
    });
    QObject::connect(mail, &LabelButton::labelClicked, [&l]() {
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText("z3129424572@outlook.com");
        l->setText("邮箱复制成功！");
    });
    QVBoxLayout *layOut = new QVBoxLayout(tempWid);
    layOut->addWidget(l, 0, Qt::AlignCenter);
    layOut->addWidget(qq, 0, Qt::AlignCenter);
    layOut->addWidget(mail, 0, Qt::AlignCenter);
    tempWid->setLayout(layOut);
    tempWid->exec();
}

void Widget::onImageDownloaded(const QPixmap &image)
{
    if (!image.isNull()) {
        // 保存图片到文件
        QString filePath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
        //        filePath += "/Screenshots";
        QDir path = QDir(filePath);
        if (!path.exists()) {
            // 创建 Screenshots 目录
            if (path.mkpath(".")) { // mkpath(".") 创建所有缺失的父目录
                qDebug() << "目录创建成功:" << filePath;
            } else {
                qDebug() << "无法创建目录:" << filePath;
            }
        }
        // 保存图片
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString savedPath = filePath + "/" + currentDateTime.toString("yyyyMMddHHmmss")+".png";
        if (image.save(savedPath)) {
            qDebug() << "网页图片已保存:" << savedPath;
            PicPath.append(savedPath);
            PopWid *messagePop = new PopWid;
            messagePop->showWithText("图片已保存", QPoint(this->pos().x(), this->pos().y()), this->width());
            guideBtn *btn = new guideBtn(board);
            board->appendGuideBtn(btn);
            pic_view *wid = new pic_view(settingWid->get_checkBox_defaultBoard()?pic_table:nullptr, savedPath, pic_table, board, btn);
            connect(this, &Widget::set_proxy_clicked, wid, &pic_view::set_window_first);
            connect(this, &Widget::cancel_set_proxy_clicked, wid, &pic_view::cancel_window_first);
            connect(this, &Widget::clear_all_clicked, wid, &pic_view::deleteLater);
            connect(this, &Widget::PrtScreen_clicked, wid, &pic_view::hideWidget);
            connect(this, &Widget::reshowAllWidget, wid, &pic_view::showWidget);
            connect(wid, &pic_view::pic_view_close, this, &Widget::modifyPicPath);
            picViewList[savedPath] = QPointer<pic_view>(wid);
            wid->show();
            btn->setTarget(wid);
            QTimer::singleShot(400, this, [this, wid](){
            wid->jugdeShowGuide(QRect(this->board->pos(), this->board->size()), QRect(this->pic_table->pos(), this->pic_table->size()));
        });
            if(settingWid->get_checkBox_defaultBoard()) this->showBoard();
        } else {
            qDebug() << "网页保存图片失败:" << savedPath;
            PopWid *messagePop = new PopWid(nullptr, PopWid::Error);
            messagePop->showWithText("图片保存失败", QPoint(this->pos().x(), this->pos().y()), this->width());
        }

    }
}

void Widget::imageDownloadError()
{
    PopWid *messagePop = new PopWid(nullptr, PopWid::Warning);
    messagePop->showWithText("超链接不包含图片", QPoint(this->pos().x(), this->pos().y()), this->width());
}

void Widget::showBoard()
{
    this->board->setWindowState(this->board->windowState() & ~Qt::WindowMinimized);
    this->board->show();
    this->board->raise();
    this->board->activateWindow();  // 激活窗口
}

void Widget::Select_Widget()
{
    fileNames.clear();
    QString picturesPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    fileNames = QFileDialog::getOpenFileNames(this, "选择图片", picturesPath);
    bool keepBoard = settingWid->get_checkBox_defaultBoard();
    for(int i = 0; i < fileNames.count(); i++){
        qDebug()<<fileNames.at(i);
        if(!picViewList.contains(fileNames.at(i)))
        {
            guideBtn *btn = new guideBtn(board);
            board->appendGuideBtn(btn);
            pic_view *wid = new pic_view(keepBoard?pic_table:nullptr, fileNames.at(i), pic_table, board, btn);
            connect(this, &Widget::set_proxy_clicked, wid, &pic_view::set_window_first);
            connect(this, &Widget::cancel_set_proxy_clicked, wid, &pic_view::cancel_window_first);
            connect(this, &Widget::clear_all_clicked, wid, &pic_view::deleteLater);
            connect(this, &Widget::PrtScreen_clicked, wid, &pic_view::hideWidget);
            connect(this, &Widget::reshowAllWidget, wid, &pic_view::showWidget);
            connect(wid, &pic_view::pic_view_close, this, &Widget::modifyPicPath);
            PicPath.append(fileNames.at(i));
            wid->show();
            btn->setTarget(wid);
            QTimer::singleShot(400, this, [this, wid](){
            wid->jugdeShowGuide(QRect(this->board->pos(), this->board->size()), QRect(this->pic_table->pos(), this->pic_table->size()));
        });
            wid->move(i*16, i*9);

            picViewList[fileNames.at(i)] = QPointer<pic_view>(wid);
        }
    }
    if(fileNames.count() > 0 && settingWid->get_checkBox_defaultBoard()) this->showBoard();
}

void Widget::getScreenShot()
{
    qDebug()<<"getScreenShot exec";
    this->hide();
    emit PrtScreen_clicked(settingWid->get_checkBox_shotScreen());
    ScreenshotWidget *SCwidget = new ScreenshotWidget();
    connect(SCwidget, &ScreenshotWidget::ScreenshotWidgetExit, this, &Widget::recoverWidget);
}

void Widget::recoverWidget(QPixmap pic) // 隐藏窗口截图 后 重新显示窗口
{
    this->show();
    if(settingWid->get_checkBox_shotScreen())
    {
        emit reshowAllWidget();
        qDebug()<<"reshowAllWidget emit";
    }
    if(pic.isNull()) return;

    // 保存图片到文件
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    filePath += "/Screenshots";
    QDir path = QDir(filePath);
    if (!path.exists()) {
        // 创建 Screenshots 目录
        if (path.mkpath(".")) { // mkpath(".") 创建所有缺失的父目录
            qDebug() << "目录创建成功:" << filePath;
        } else {
            qDebug() << "无法创建目录:" << filePath;
        }
    }
    // 保存图片
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString savedPath = filePath + "/" + currentDateTime.toString("yyyyMMddHHmmss")+".png";
    if (pic.save(savedPath)) {
        PopWid *messagePop = new PopWid;
        messagePop->showWithText("图片已保存", QPoint(this->pos().x(), this->pos().y()), this->width());
        guideBtn *btn = new guideBtn(board);
        board->appendGuideBtn(btn);
        pic_view *wid = new pic_view(settingWid->get_checkBox_defaultBoard()?pic_table:nullptr, savedPath, pic_table, board, btn);
        connect(this, &Widget::set_proxy_clicked, wid, &pic_view::set_window_first);
        connect(this, &Widget::cancel_set_proxy_clicked, wid, &pic_view::cancel_window_first);
        connect(this, &Widget::clear_all_clicked, wid, &pic_view::deleteLater);
        connect(this, &Widget::PrtScreen_clicked, wid, &pic_view::hideWidget);
        connect(this, &Widget::reshowAllWidget, wid, &pic_view::showWidget);
        wid->show();
        btn->setTarget(wid);
        QTimer::singleShot(400, this, [this, wid](){
            wid->jugdeShowGuide(QRect(this->board->pos(), this->board->size()), QRect(this->pic_table->pos(), this->pic_table->size()));
        });
        if(settingWid->get_checkBox_defaultBoard()) this->showBoard();
        qDebug() << "图片已保存:" << savedPath;
//        wid->setPicDir(savedPath);
        PicPath.append(savedPath);
        picViewList[savedPath] = QPointer<pic_view>(wid);
        connect(wid, &pic_view::pic_view_close, this, &Widget::modifyPicPath);
    } else {
        qDebug() << "保存图片失败:" << savedPath;
        PopWid *messagePop = new PopWid(nullptr, PopWid::Error);
        messagePop->showWithText("图片已保存", QPoint(this->pos().x(), this->pos().y()), this->width());
    }
    if(!settingWid->get_checkBox_ScreenShot())
    {
        QFile::remove(savedPath);
    }
}

void Widget::loadImage(const QString &filePath, QPointF pos) // 处理dropEvent
{
    // 加载图片并显示
    QPixmap pixmap(filePath);
    if (pixmap.isNull()) {
        qDebug() << "加载图片失败:" << filePath;
        PopWid *messagePop = new PopWid(nullptr, PopWid::Error);
        messagePop->showWithText("未识别的链接", QPoint(this->pos().x(), this->pos().y()), this->width());
        return;
    }
    qDebug() << "dropEvent加载图片成功:" << filePath;
    // 显示图片
    if(!picViewList.contains(filePath))
    {
        guideBtn *btn = new guideBtn(board);
        board->appendGuideBtn(btn);
        pic_view *wid = new pic_view(settingWid->get_checkBox_defaultBoard()?pic_table:nullptr, filePath, pic_table, board, btn);
        connect(this, &Widget::set_proxy_clicked, wid, &pic_view::set_window_first);
        connect(this, &Widget::cancel_set_proxy_clicked, wid, &pic_view::cancel_window_first);
        connect(this, &Widget::clear_all_clicked, wid, &pic_view::deleteLater);
        connect(this, &Widget::PrtScreen_clicked, wid, &pic_view::hideWidget);
        connect(this, &Widget::reshowAllWidget, wid, &pic_view::showWidget);
        connect(wid, &pic_view::pic_view_close, this, &Widget::modifyPicPath);
        PicPath.append(filePath);
        wid->show();
        btn->setTarget(wid);
        if(wid->getIsOnTable() && settingWid->get_checkBox_defaultBoard()) wid->move((pos).x()-pic_table->x(), (pos).y()-pic_table->y());
        else if(!settingWid->get_checkBox_defaultBoard()) wid->move(pos.x()+this->x(), pos.y()+this->y());
        QTimer::singleShot(400, this, [this, wid](){
            wid->jugdeShowGuide(QRect(this->board->pos(), this->board->size()), QRect(this->pic_table->pos(), this->pic_table->size()));
        });
        picViewList[filePath] = QPointer<pic_view>(wid);
    }
    else picViewList[filePath]->show();
    if(settingWid->get_checkBox_defaultBoard()) this->showBoard();
}

void Widget::animateResize(const QSize &newSize) {
    QRect startRect = geometry();  // 当前窗口的几何形状
    QRect endRect(startRect.topLeft(), newSize);  // 目标窗口大小

    resizeAnimation->setStartValue(startRect);  // 起始值为当前窗口大小
    resizeAnimation->setEndValue(endRect);  // 目标大小

    if(resizeAnimation->state() == QAbstractAnimation::Running) resizeAnimation->stop();
    resizeAnimation->start();  // 启动动画
}

void Widget::animateMove(int d) // 动画调整窗口位置的函数; d=1:up, d=0:down
{
    if(d == 0) // down:AIwid show
    {
        qDebug() << "animateMove : down";
        settingWid->show();
        AIDescribleWid->show();
        QRect startGeometry1 = settingWid->geometry();
        QRect startGeometry2 = AIDescribleWid->geometry();

        // 子窗口1的动画
        moveAnimation1->setDuration(800);
        moveAnimation1->setStartValue(startGeometry1);
        moveAnimation1->setEndValue(QRect(settingWid->x(), 320, settingWid->width(), settingWid->height()));

        // 子窗口2的动画
        moveAnimation2->setDuration(800);
        moveAnimation2->setStartValue(startGeometry2);
        moveAnimation2->setEndValue(QRect(AIDescribleWid->x(), 5, AIDescribleWid->width(), AIDescribleWid->height()));
    }
    else if(d == 1) // up:setWid show
    {
        qDebug() << "animateMove : up";
        settingWid->show();
        AIDescribleWid->show();
        QRect startGeometry1 = settingWid->geometry();
        QRect startGeometry2 = AIDescribleWid->geometry();

        // 子窗口1的动画
        moveAnimation1->setDuration(800);
        moveAnimation1->setStartValue(startGeometry1);
        moveAnimation1->setEndValue(QRect(settingWid->x(), 5, settingWid->width(), settingWid->height()));

        // 子窗口2的动画
        moveAnimation2->setDuration(800);
        moveAnimation2->setStartValue(startGeometry2);
        moveAnimation2->setEndValue(QRect(AIDescribleWid->x(), -320, AIDescribleWid->width(), AIDescribleWid->height()));
    }

    if (moveAnimation1->state() == QAbstractAnimation::Running){
        moveAnimation1->stop();
    }
    if (moveAnimation2->state() == QAbstractAnimation::Running){
        moveAnimation2->stop();
    }

    // 启动动画
    moveAnimation1->start();
    moveAnimation2->start();
}

void Widget::getRespondFronGpt()
{
    if(!settingWid->get_checkBox_unableAI() && isSuspended) {
        qDebug() << "getRespondFronGpt......";
        test->setAPIKey(settingWid->get_lineEdit_APIKey());
        test->setModel("");
        test->setQuest("");
        test->callOpenAI();
        if(!settingWid->get_checkBox_lockDelay())
        {
            // 生成一个随机整数 [0, 100)
            qint64 randomInt = generator->bounded(20000, 480000);
            chatWithGpt.setInterval(randomInt);
            qDebug() << "next chat in" << double(randomInt/1000) << "s";
        }
        else
        {
            chatWithGpt.setInterval(20000);
            qDebug() << "next chat in 20 s";
        }
    }
}

void Widget::showMsgFronGpt(QString msg)
{
    if(msg == "network error")
    {
        AIDescribleWid->disableSend(false);
        PopWid *messagePop = new PopWid(nullptr, PopWid::Error);
        messagePop->showWithText("请求失败", QPoint(this->pos().x(), this->pos().y()), this->width());
        qDebug() << "请求失败......";
    }
    else if(msg == "error apiKey")
    {
        AIDescribleWid->disableSend(false);
        PopWid *messagePop = new PopWid(nullptr, PopWid::Error);
        messagePop->showWithText("APIKEY为空", QPoint(this->pos().x(), this->pos().y()), this->width());
        qDebug() << "错误的APIKEY......";
    }
    else if(msg == "picture error")
    {
        AIDescribleWid->disableSend(false);
        PopWid *messagePop = new PopWid(nullptr, PopWid::Error);
        messagePop->showWithText("图片下载失败", QPoint(this->pos().x(), this->pos().y()), this->width());
    }
    else if(msg != "")
    {
        PopWid *messagePop = new PopWid(nullptr, PopWid::Message, 100, 6000);
        messagePop->showWithText(msg, QPoint(this->pos().x(), this->pos().y()), this->width());
    }
}

void Widget::showAIPic(QString dir)
{
    AIDescribleWid->disableSend(false);
    guideBtn *btn = new guideBtn(board);
    board->appendGuideBtn(btn);
    pic_view *wid = new pic_view(settingWid->get_checkBox_defaultBoard()?pic_table:nullptr, dir, pic_table, board, btn);
    connect(this, &Widget::set_proxy_clicked, wid, &pic_view::set_window_first);
    connect(this, &Widget::cancel_set_proxy_clicked, wid, &pic_view::cancel_window_first);
    connect(this, &Widget::clear_all_clicked, wid, &pic_view::deleteLater);
    connect(this, &Widget::PrtScreen_clicked, wid, &pic_view::hideWidget);
    connect(this, &Widget::reshowAllWidget, wid, &pic_view::showWidget);
    connect(wid, &pic_view::pic_view_close, this, &Widget::modifyPicPath);
    PicPath.append(dir);
    wid->show();
    btn->setTarget(wid);
    QTimer::singleShot(400, this, [this, wid](){
            wid->jugdeShowGuide(QRect(this->board->pos(), this->board->size()), QRect(this->pic_table->pos(), this->pic_table->size()));
        });
    picViewList[dir] = QPointer<pic_view>(wid);
    if(settingWid->get_checkBox_defaultBoard()) this->showBoard();
}

void Widget::closeEvent(QCloseEvent *event)
{
    if(board) {
        board->showNormal();
    }
    if(!settingWid->get_checkBox_reopenPic())
    {
        PicPath.clear();
        picViewList.clear();
    }
    widPosition[0] = this->pos().x();
    widPosition[1] = this->pos().y();
    qDebug() << "final pos:" << widPosition[0] << "," << widPosition[1];
    if(board) {
        boardPosition[0] = board->pos().x();
        boardPosition[1] = board->pos().y();
        boardPosition[2] = board->width();
        boardPosition[3] = board->height();
    }
    else{
        boardPosition[0] = 0;
        boardPosition[1] = 0;
        boardPosition[2] = 0;
        boardPosition[3] = 0;
    }
    savePicPathToJson();
    if(board) {
        board->changeIsAllowToClose(true);
        board->close();
    }
    if(settingWid->get_checkBox_keepPic())
    {
        this->close();
    }
    else
    {
        emit clear_all_clicked();
        this->close();
    }
}

void Widget::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug()<<"dragEnterEvent";
    // 检查拖入的数据类型是否是图片文件
    if (event->mimeData()->hasImage() || event->mimeData()->hasUrls()) {
        qDebug()<<"dragEnterEvent";
        event->acceptProposedAction(); // 接受拖放事件
        if(isSuspended)
        {
            if(isSetWidShow) settingWid->hide();
            if(isAIWidShow) AIDescribleWid->hide();
            dragEventWidget->show();
            animateResize(QSize(290, 310));
        }
    }
    // 检查拖拽的内容是否包含 URL 并且是图片格式
    else if (event->mimeData()->hasUrls()) {
        QList<QUrl> urls = event->mimeData()->urls();
        if (!urls.isEmpty()) {
            QString fileName = urls.first().toString();  // 获取文件名
            qDebug()<<"dragEnterEvent";
            event->acceptProposedAction();  // 如果是支持的格式，接受拖拽
            return;
        }
    }
    else {
        PopWid *messagePop = new PopWid;
        messagePop->showWithText("未识别的链接", QPoint(this->pos().x(), this->pos().y()), this->width());
    }
}

void Widget::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event);  // 忽略未使用的参数
    // 当拖拽离开时恢复窗口大小
    if(isSetWidShow)
    {
        settingWid->show();
        dragEventWidget->hide();
        animateResize(QSize(270, 310));
    }
    else if(isAIWidShow)
    {
        AIDescribleWid->show();
        dragEventWidget->hide();
        animateResize(QSize(270, 310));
    }
    else
    {
        animateResize(QSize(30, 310));
        dragEventWidget->hide();
    }
}

void Widget::dropEvent(QDropEvent *event)
{
    qDebug()<<"dropEvent";
    if(isSuspended)
    {
        if(isSetWidShow)
        {
            settingWid->hide();
            isSetWidShow = false;
        }
        else if(isAIWidShow)
        {
            AIDescribleWid->hide();
            isAIWidShow = false;
        }
        animateResize(QSize(30, 310));
        dragEventWidget->hide();
    }
    const QMimeData *mimeData = event->mimeData();
    // 处理拖放的文件
    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        for (const QUrl &url : urlList) {
            if (url.isLocalFile()) {
                // 如果是本地文件，直接加载
                QString filePath = url.toLocalFile();  // 获取本地文件路径
                loadImage(filePath, event->position());  // 调用加载图片的函数
            } else {
                qDebug() << "url in dropEvent :" << url;
                // 如果是网页图片，启动子线程下载
                downloader->setUrl(url);
                // 将下载器移动到线程中
                downloader->moveToThread(thread);
                // 启动线程
                thread->start();
            }
        }
    }
    allowToDrop = false;
}

void Widget::mousePressEvent(QMouseEvent *event) {
    if(isSuspended)
    {
        if (event->button() == Qt::LeftButton) {
            // 记录鼠标相对窗口的位置
            dragStartPosition = event->pos();
            Dragging = true;
        }
    }
    else return QWidget::mousePressEvent(event);
}

void Widget::mouseMoveEvent(QMouseEvent *event) {
    if(isSuspended)
    {
        if (Dragging) {
            // 计算新的窗口位置并移动窗口
            move(event->globalPos() - dragStartPosition);
        }
    }
    else return QWidget::mouseMoveEvent(event);
}

void Widget::mouseReleaseEvent(QMouseEvent *event) {
    if(isSuspended)
    {
        if (event->button() == Qt::LeftButton) {
            Dragging = false; // 停止拖动
        }
    }
    else return QWidget::mouseReleaseEvent(event);
}

void Widget::wheelEvent(QWheelEvent *event)
{
    if(isSetWidShow && event->angleDelta().y() > 0) showGenerateAIPic();
    else if(isAIWidShow && event->angleDelta().y() < 0) showSetting();
}

void Widget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);  // 调用基类的 paintEvent

    if (isSuspended) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true); // 开启抗锯齿
        QRectF rect0(0, 0, this->width(), this->height()); // 定义矩形区域
//        qDebug()<<"paintEvent isSuspended true :" << rect0;
        qreal radius = 15.0; // 定义圆角半径
        QPainterPath path0;
        path0.addRoundedRect(rect0, radius, radius); // 添加圆角矩形到路径中
        painter.setClipPath(path0); // 设置允许绘制范围
        QPen pen(Qt::white);
        pen.setWidth(3);
        painter.setPen(pen); // 设置画笔
        QColor color(255, 255, 255);
        painter.setBrush(color); // 设置填充颜色
        painter.drawPath(path0); // 绘制路径
    } else {
        // 恢复普通的窗口形状
        setMask(QRegion());  // 清除圆角区域的遮罩

        // 默认的绘制内容
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);  // 启用抗锯齿
        painter.setBrush(Qt::white);
        painter.setPen(Qt::NoPen);
        painter.drawRect(this->rect());
    }
}

//void Widget::focusOutEvent(QFocusEvent *event)
//{
//    QWidget *focusedWidget = QApplication::focusWidget();
//    if (focusedWidget && (focusedWidget == this || focusedWidget->parent() == this || focusedWidget->parent()->parent() == this)) {
//        return; // 如果焦点在主窗口的子控件上，不执行focusOutEvent
//    }
//    if(isSuspended && isSetWidShow) showSetting();
//    if(isSuspended && isAIWidShow) showGenerateAIPic();
//    // 正常处理focusOutEvent的逻辑
//    QWidget::focusOutEvent(event);
//}

