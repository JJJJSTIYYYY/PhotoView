#ifndef WIDGET_H
#define WIDGET_H

#include <iostream>
#include <QWidget>
#include <QPushButton>
#include <QJsonArray>
#include <QToolButton>
#include <QFileDialog>
#include <QStandardPaths>
#include <QTabWidget>
#include <QCheckBox>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QPointer>
#include <QPoint>
#include <QStyle>
#include <QClipboard>
#include <QDate>
#include <QLabel>
#include <QMessageBox>
#include <qboxlayout.h>
#include <QPropertyAnimation>
#include <QMap>
#include <QPainterPath>
#include <QThread>
#include "imagedownloader.h"
#include "ui_beautify/labelbutton.h"
#include "pic_view.h"
#include "ScreenshotWidget.h"
#include "setting.h"
#include "aidescrible.h"
#include "ui_beautify/popwid.h"
#include "openaiAPI.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    enum windowState{
        Suspend, MainWid
    };

    Widget(QWidget *parent = nullptr);
    ~Widget();

    void reloadPic();
    void clear();
    void clearAllForece();
    void showSetting();
    void showGenerateAIPic();
    void showMainWid();
    void savePicPathToJson();
    bool loadPicPathFromJson();
    void modifyPicPath(QString dir);
    void getClipboard();
    void generateAIPic();
    void suspendWindow();
    void cancelSuspendWindow();
    void exit_Suspend_clicked();
    void showJustiy();
    void onImageDownloaded(const QPixmap &image);
    void imageDownloadError();

private:
    Ui::Widget *ui;

    QPushButton *Select_Pic;
    QPushButton *set_proxy;
    QPushButton *cancel_set_proxy;
    QPushButton *clear_all;
    QPushButton *PrtScreen;
    QPushButton *get_clipboard;
    QPushButton *suspend;
    QToolButton *settingBtn;

    QToolButton *Select_Pic_Suspend;
    QToolButton *set_proxy_Suspend;
    QToolButton *cancel_set_proxy_Suspend;
    QToolButton *clear_all_Suspend;
    QToolButton *PrtScreen_Suspend;
    QToolButton *get_clipboard_Suspend;
    QToolButton *AI_Pic_Suspend;
    QToolButton *cancel_suspend;
    QToolButton *settingBtn_Suspend;
    LabelButton *exit_Suspend;
    bool isSuspended = false;
    bool wantExit = false;
    bool isSetWidShow = false;
    bool isAIWidShow = false;

    ImageDownloader *downloader; // 图片下载器
    QThread *thread; // 下载图片线程

    bool state; // 同 isSuspended
    int widPosition[2]; // 窗口全局坐标
    QStringList fileNames;
    QList<QString> PicPath;
    QMap<QString,QPointer<pic_view>> picViewList;
    QList<QPoint> coordinatesList;
    QList<QSize> sizeList;
    Setting *settingWid;
    QLabel *dragEventWidget;
    AIDescrible *AIDescribleWid;
    QWidget *mainWid;
    QWidget *suspendWid;
    void Select_Widget();
    void getScreenShot();
    void recoverWidget(QPixmap pic);
    void loadImage(const QString &url);
    LabelButton *m_showMainAction;
    LabelButton *m_exitAppAction;
    CustomMenu *m_menu;
    QSystemTrayIcon *mSysTrayIcon;
    QPropertyAnimation *resizeAnimation;  // 用于窗口大小调整的动画
    void animateResize(const QSize &newSize);  // 动画调整窗口大小的函数
    QPropertyAnimation *moveAnimation1;  // 用于窗口位置调整的动画
    QPropertyAnimation *moveAnimation2;  // 用于窗口位置调整的动画
    void animateMove(int d);  // 动画调整窗口位置的函数; d=1:up, d=0:down
    QTimer chatWithGpt;
    QRandomGenerator *generator = QRandomGenerator::global();
    openai *test;
    int remiantTimes; // ai的剩余响应次数
    void getRespondFronGpt();
    void showMsgFronGpt(QString msg);
    void showAIPic(QString dir);

signals:
    void set_proxy_clicked();
    void cancel_set_proxy_clicked();
    void clear_all_clicked();
    void PrtScreen_clicked(bool);
    void reshowAllWidget();

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    void paintEvent(QPaintEvent *event);

private:
    QPoint dragStartPosition; // 鼠标相对窗口的位置
    bool Dragging = false;   // 拖动状态


    // QWidget interface
//protected:
//    void focusOutEvent(QFocusEvent *event);
};
#endif // WIDGET_H
