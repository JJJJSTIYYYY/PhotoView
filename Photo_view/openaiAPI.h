// ai嘲讽
#ifndef OPENAIAPI_H
#define OPENAIAPI_H

#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QByteArray>
#include <QRandomGenerator>
#include <QDebug>
#include <QThread>
#include "imagedownloader.h"

class openai : public QObject
{
    Q_OBJECT
public:
    explicit openai(QObject *parent = nullptr);
    ~openai();
    void callOpenAI();
    void callOpenAIModifyDescribe(QString descri);
    void callOpenAI_pic(QString describe = "", QString size = "256x256", int num = 1);
    void callOpenAI_pic2(QString describe = "", int w = 1024, int h = 1024, int num = 1, QString imagePath = "", int cd = 5); // 暂时弃置
    void setAPIKey(QString key);
    void setAPIKeyPic(QString key);
    void setQuest(QString quest); // 设置提示词
    void setModel(QString model); // 设置模型
    void setModelPic(QString model); // 设置模型
    void setQualityPic(QString quality); // 设置模型出图质量
    void imageDownloadError();
    void onImageDownloaded(const QPixmap &image);

private:
    QNetworkAccessManager* manager;
    QNetworkRequest request;
    QNetworkRequest request_pic;
    QNetworkRequest request_pic2;
    QUrl url;
    QUrl url_pic;
    QUrl url_pic2;
    QString apiKey;
    QString apiKey_pic;
    QString quest;
    QString model;
    QString modelPic;
    QString qualityPic;
    QRandomGenerator *generator;
    ImageDownloader *downloader; // 图片下载器
    QThread *thread; // 下载图片线程

signals:
    void returnMessage(QString msg);
    void returnError(QString msg);
    void generateFinish(QString imgDir);
    void modifyDescribeFinish(QString des);
};

// callOpenAI();调用

#endif // OPENAIAPI_H
