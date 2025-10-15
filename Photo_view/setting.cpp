#include "setting.h"
#include <QStandardPaths>
#include <qlabel.h>
#include <qmessagebox.h>

const QString dirPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/Photo_View";

Setting::Setting(QWidget *parent)
    : QWidget{parent}
{
    // 设置窗口内所有控件的字体为 "幼圆"
    this->setStyleSheet("QWidget { font-family: 'YouYuan'; }");

    littleSetting = new QWidget(this);
    fullSetting = new QWidget(this);
    littleSetting->move(35, 10);
    fullSetting->move(35, 240);
    // 构建更多功能窗口
    moreFuncWid = new moreFunc(this);
    moreFuncWid->setFixedSize(200, 220);
    moreFuncWid->move(35, 10);
    moreFuncWid->hide();

    animation = new QPropertyAnimation(littleSetting, "geometry");
    animation2 = new QPropertyAnimation(fullSetting, "geometry");
    animation->setDuration(100);
    animation2->setDuration(100);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation2->setEasingCurve(QEasingCurve::OutCubic);

    QVBoxLayout *HlayOut = new QVBoxLayout(this);
//    HlayOut->setContentsMargins(5, 5, 5, 5);

    QVBoxLayout *HlayOut2 = new QVBoxLayout(this);
    HlayOut->setContentsMargins(0, 0, 0, 0);

    checkBox_hidePop = new QCheckBox("隐藏按钮提示", this);
    checkBox_hidePop->move(40,25);
    checkBox_unableAI = new QCheckBox("关闭消息气泡(建议勾选)", this);
    checkBox_unableAI->move(40,45);
    checkBox_lockDelay = new QCheckBox("固定对话时延", this);
    checkBox_lockDelay->move(40,65);
    checkBox_shotScreen = new QCheckBox("截图时隐藏参考图", this);
    checkBox_shotScreen->move(40,85);
    checkBox_reopenPic = new QCheckBox("打开时重载参考图", this);
    checkBox_reopenPic->move(40,105);
    checkBox_keepPic = new QCheckBox("退出时保留参考图", this);
    checkBox_keepPic->move(40,125);
    checkBox_ScreenShot = new QCheckBox("保存截图(关闭将无法重载截图)", this);
    checkBox_ScreenShot->move(40,145);
    checkBox_defaultBoard = new QCheckBox("默认启用画板", this);
    checkBox_defaultBoard->move(40,165);

    HlayOut->addWidget(checkBox_hidePop);
    HlayOut->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    HlayOut->addWidget(checkBox_unableAI);
    HlayOut->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    HlayOut->addWidget(checkBox_lockDelay);
    HlayOut->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    HlayOut->addWidget(checkBox_shotScreen);
    HlayOut->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    HlayOut->addWidget(checkBox_reopenPic);
    HlayOut->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    HlayOut->addWidget(checkBox_keepPic);
    HlayOut->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    HlayOut->addWidget(checkBox_ScreenShot);
    HlayOut->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    HlayOut->addWidget(checkBox_defaultBoard);

    littleSetting->setLayout(HlayOut);
    littleSetting->show();

    lineEdit_APIKey = new QLineEdit(this);
    lineEdit_APIKey->setPlaceholderText("请输入APIKEY(生成文字/可免费获取)");
    lineEdit_APIKey->setEchoMode(QLineEdit::Password);
    lineEdit_APIKey->move(40,185);
    lineEdit_APIKey->resize(160, 20);
    lineEdit_APIKey->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    lineEdit_APIKey->setStyleSheet("border-radius: 3px;"
                                 "background-color: #F0F0F0;");
    lineEdit_APIKeyPic = new QLineEdit(this);
    lineEdit_APIKeyPic->setPlaceholderText("请输入APIKEY(生成图片/需付费获取)");
    lineEdit_APIKeyPic->setEchoMode(QLineEdit::Password);
    lineEdit_APIKeyPic->move(40,206);
    lineEdit_APIKeyPic->resize(160, 20);
    lineEdit_APIKeyPic->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    lineEdit_APIKeyPic->setStyleSheet("border-radius: 3px;"
                                   "background-color: #F0F0F0;");

    HlayOut2->addWidget(lineEdit_APIKey);
//    HlayOut->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    HlayOut2->addWidget(lineEdit_APIKeyPic);

    fullSetting->setLayout(HlayOut2);

    cancelBtn = new QPushButton("取消", this);
    finishBtn = new QPushButton("确认", this);
    cancelBtn->move(20, 240);
    finishBtn->move(130, 240);
    cancelBtn->resize(90, 30);
    finishBtn->resize(90, 30);
    connect(cancelBtn, &QPushButton::clicked, this, &Setting::cancel);
    connect(finishBtn, &QPushButton::clicked, this, &Setting::save);

    moreSetting = new QPushButton("更多设置", this);
    moreSetting->move(65, 280);
    moreSetting->setStyleSheet(R"(
    QPushButton {
        color: black; /* 默认字体颜色 */
        text-decoration: none; /* 默认无下划线 */
        background-color: transparent; /* 背景透明 */
        border: none; /* 去掉按钮边框 */
    }
    QPushButton:hover {
        color: blue; /* 悬停时字体变蓝 */
        text-decoration: underline; /* 悬停时字体加下划线 */
    }
    QPushButton:pressed {
        color: deepskyblue; /* 点击时字体变为天蓝色 */
    }
    )");
    connect(moreSetting, &QPushButton::clicked, this, &Setting::showMoreSetting);

    moreFuncBtn = new QPushButton("更多功能", this);
    moreFuncBtn->move(125, 280);
    moreFuncBtn->setStyleSheet(R"(
    QPushButton {
        color: black; /* 默认字体颜色 */
        text-decoration: none; /* 默认无下划线 */
        background-color: transparent; /* 背景透明 */
        border: none; /* 去掉按钮边框 */
    }
    QPushButton:hover {
        color: blue; /* 悬停时字体变蓝 */
        text-decoration: underline; /* 悬停时字体加下划线 */
    }
    QPushButton:pressed {
        color: deepskyblue; /* 点击时字体变为天蓝色 */
    }
    )");
    connect(moreFuncBtn, &QPushButton::clicked, this, &Setting::showMoreFunc);

    littleSetting->resize(200, 220);
    fullSetting->resize(170, /*55*/0);

    qDebug() << "dir = " << dirPath;
    readJsonFile(dirPath + "/checkbox_state.json");
}

bool Setting::get_checkBox_shotScreen()
{
    return checkBox_shotScreen->isChecked();
}

bool Setting::get_checkBox_reopenPic()
{
    return checkBox_reopenPic->isChecked();
}

bool Setting::get_checkBox_ScreenShot()
{
    return checkBox_ScreenShot->isChecked();
}

bool Setting::get_checkBox_keepPic()
{
    return checkBox_keepPic->isChecked();
}

bool Setting::get_checkBox_hidePop()
{
    return checkBox_hidePop->isChecked();
}

bool Setting::get_checkBox_unableAI()
{
    return checkBox_unableAI->isChecked();
}

bool Setting::get_checkBox_lockDelay()
{
    return checkBox_lockDelay->isChecked();
}

bool Setting::get_checkBox_defaultBoard()
{
    return checkBox_defaultBoard->isChecked();
}

QString Setting::get_lineEdit_APIKey()
{
    return lineEdit_APIKey->text();
}

QString Setting::get_lineEdit_APIKeyPic()
{
    return lineEdit_APIKeyPic->text();
}

void Setting::cancel()
{
    // 生成 JSON 文件路径
    QString filePath = dirPath + "/checkbox_state.json";

    // 读取 JSON 文件
    readJsonFile(filePath);
    emit closeSettingWid();
}

void Setting::save()
{
    // 生成 JSON 文件路径
    QString filePath = dirPath + "/checkbox_state.json";

    // 保存 JSON 文件
    writeJsonFile(filePath);
    emit closeSettingWid();
}

void Setting::showMoreSetting()
{
    if(isMoreSettingShow){
        animeAct(220);
        isMoreSettingShow = false;
    }
    else{
        animeAct(170);
        isMoreSettingShow = true;
    }
}

void Setting::showMoreFunc()
{
    if(!isMoreFuncShow){
        littleSetting->hide();
        fullSetting->hide();
        moreFuncWid->show();
    }
    else{
        littleSetting->show();
        fullSetting->show();
        moreFuncWid->hide();
    }
    isMoreFuncShow = !isMoreFuncShow;
}

void Setting::writeJsonFile(const QString &filePath)
{
    // 创建 JSON 对象并保存 QCheckBox 的状态
    QJsonObject jsonObj;
    jsonObj["checkBox_shotScreen"] = checkBox_shotScreen->isChecked();
    jsonObj["checkBox_reopenPic"] = checkBox_reopenPic->isChecked();
    jsonObj["checkBox_keepPic"] = checkBox_keepPic->isChecked();
    jsonObj["checkBox_ScreenShot"] = checkBox_ScreenShot->isChecked();
    jsonObj["checkBox_hidePop"] = checkBox_hidePop->isChecked();
    jsonObj["checkBox_unableAI"] = checkBox_unableAI->isChecked();
    jsonObj["checkBox_lockDelay"] = checkBox_lockDelay->isChecked();
    jsonObj["checkBox_defaultBoard"] = checkBox_defaultBoard->isChecked();
    jsonObj["lineEdit_APIKey"] = lineEdit_APIKey->text();
    jsonObj["lineEdit_APIKeyPic"] = lineEdit_APIKeyPic->text();

    // 创建 JSON 文档
    QJsonDocument jsonDoc(jsonObj);

    // 写入 JSON 文件
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "无法打开文件";
        QMessageBox::warning(this, "Error", "Failed to open JSON file.");
        return;
    }

    file.write(jsonDoc.toJson());
    file.close();
}

void Setting::readJsonFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.exists()) {
        qDebug() << "文件不存在，无法加载状态";
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开文件";
        qDebug() << "Failed to open JSON file.";
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData);
    if (!jsonDoc.isObject()) {
        qDebug() << "JSON 数据格式错误";
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();

    // 更新 QCheckBox 的状态
    if (jsonObj.contains("checkBox_shotScreen")) {
        checkBox_shotScreen->setChecked(jsonObj["checkBox_shotScreen"].toBool());
    }
    if (jsonObj.contains("checkBox_reopenPic")) {
        checkBox_reopenPic->setChecked(jsonObj["checkBox_reopenPic"].toBool());
    }
    if (jsonObj.contains("checkBox_keepPic")) {
        checkBox_keepPic->setChecked(jsonObj["checkBox_keepPic"].toBool());
    }
    if (jsonObj.contains("checkBox_ScreenShot")) {
        checkBox_ScreenShot->setChecked(jsonObj["checkBox_ScreenShot"].toBool());
    }
    if (jsonObj.contains("checkBox_hidePop")) {
        checkBox_hidePop->setChecked(jsonObj["checkBox_hidePop"].toBool());
    }
    if (jsonObj.contains("checkBox_unableAI")) {
        checkBox_unableAI->setChecked(jsonObj["checkBox_unableAI"].toBool());
    }
    if (jsonObj.contains("checkBox_lockDelay")) {
        checkBox_lockDelay->setChecked(jsonObj["checkBox_lockDelay"].toBool());
    }
    if (jsonObj.contains("checkBox_defaultBoard")) {
        checkBox_defaultBoard->setChecked(jsonObj["checkBox_defaultBoard"].toBool());
    }
    if (jsonObj.contains("lineEdit_APIKeyPic")) {
        lineEdit_APIKeyPic->setText(jsonObj["lineEdit_APIKeyPic"].toString());
    }
    if (jsonObj.contains("lineEdit_APIKey")) {
        lineEdit_APIKey->setText(jsonObj["lineEdit_APIKey"].toString());
    }

    qDebug() << "CheckBox state loaded.";
}

void Setting::animeAct(int h)
{
    if(animation->state() == QAbstractAnimation::Running) animation->stop();
    if(animation2->state() == QAbstractAnimation::Running) animation2->stop();

    if(isMoreSettingShow) {
        animation->setStartValue(QRect(35, 10, 200, 180));  // 起始位置和大小
        animation->setEndValue(QRect(35, 10, 200, 220));    // 结束位置和大小

        animation2->setStartValue(QRect(35, 185, 170, 60));  // 起始位置和大小
        animation2->setEndValue(QRect(35, 240, 170, 0));    // 结束位置和大小

        isMoreSettingShow = false;
    }
    else {
        animation->setStartValue(QRect(35, 10, 200, 220));  // 起始位置和大小
        animation->setEndValue(QRect(35, 10, 200, 180));    // 结束位置和大小

        animation2->setStartValue(QRect(35, 240, 170, 0));  // 起始位置和大小
        animation2->setEndValue(QRect(35, 185, 170, 60));    // 结束位置和大小

        isMoreSettingShow = true;
    }

    animation->start();
    animation2->start();

}

void Setting::wheelEvent(QWheelEvent *event)
{
    QPoint relativePos = this->mapTo(static_cast<QWidget *>(this->parent()), QPoint(0, 0));
    if(relativePos.y() >= 35) return QWidget::wheelEvent(event);
    if(!isMoreSettingShow && event->angleDelta().y() < 0) {
        showMoreSetting();
        event->accept();
        return;
    }
    else if(isMoreSettingShow && event->angleDelta().y() > 0) {
        showMoreSetting();
        event->accept();
        return;
    }
    return QWidget::wheelEvent(event);
}
