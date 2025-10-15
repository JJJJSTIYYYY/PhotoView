#ifndef SETTING_H
#define SETTING_H

#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QHBoxLayout>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QLineEdit>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include "morefunc.h"

class Setting : public QWidget
{
    Q_OBJECT
public:
    explicit Setting(QWidget *parent = nullptr);

    bool get_checkBox_shotScreen();
    bool get_checkBox_reopenPic();
    bool get_checkBox_ScreenShot();
    bool get_checkBox_keepPic();
    bool get_checkBox_hidePop();
    bool get_checkBox_unableAI();
    bool get_checkBox_lockDelay();
    bool get_checkBox_defaultBoard();
    QString get_lineEdit_APIKey();
    QString get_lineEdit_APIKeyPic();
    void cancel();
    void save();
    void showMoreSetting();
    void showMoreFunc();
    void writeJsonFile(const QString &filePath);
    void readJsonFile(const QString &filePath);

private:
    bool isMoreSettingShow = false;
    bool isMoreFuncShow = false;

    QCheckBox *checkBox_shotScreen;
    QCheckBox *checkBox_reopenPic;
    QCheckBox *checkBox_ScreenShot;
    QCheckBox *checkBox_keepPic;
    QCheckBox *checkBox_hidePop;
    QCheckBox *checkBox_unableAI;
    QCheckBox *checkBox_lockDelay;
    QCheckBox *checkBox_defaultBoard;
    QLineEdit *lineEdit_APIKey;
    QLineEdit *lineEdit_APIKeyPic;
    QPushButton *cancelBtn, *finishBtn ,*moreSetting, *moreFuncBtn;

    QWidget* littleSetting, *fullSetting;
    moreFunc *moreFuncWid;

    QPropertyAnimation *animation, *animation2;
    void animeAct(int h);

signals:
    void closeSettingWid();
    void moreSettingClicked();


    // QWidget interface
protected:
    void wheelEvent(QWheelEvent *event);
};

#endif // SETTING_H
