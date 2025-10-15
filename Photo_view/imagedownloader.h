// 下载网络图片
#ifndef IMAGEDOWNLOADER_H
#define IMAGEDOWNLOADER_H

#include <QObject>
#include <QPixmap>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QBuffer>
#include <QDebug>

class ImageDownloader : public QObject
{
    Q_OBJECT
public:
    explicit ImageDownloader(QObject *parent = nullptr);
    ImageDownloader(const QUrl &url, QObject *parent = nullptr);
    ~ImageDownloader();
    void setUrl(QUrl url);

signals:
    void imageDownloaded(const QPixmap &image);  // 通知主线程图片下载完成
    void imageDownloadError();  // 通知主线程图片下载失败
    void finished();  // 通知主线程工作完成

public slots:
    void downloadImage();  // 子线程执行下载

private slots:
    void onDownloadFinished(QNetworkReply *reply);  // 下载完成后的处理

private:
    QUrl imageUrl;  // 要下载的图片的URL
    QNetworkAccessManager *networkManager;

};

#endif // IMAGEDOWNLOADER_H
