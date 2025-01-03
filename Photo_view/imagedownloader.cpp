#include "imagedownloader.h"

ImageDownloader::ImageDownloader(QObject *parent)
    : QObject{parent}
{
    networkManager = new QNetworkAccessManager(this);

}

ImageDownloader::ImageDownloader(const QUrl &url, QObject *parent)
    : QObject(parent), imageUrl(url) {
}

ImageDownloader::~ImageDownloader() {
    delete networkManager;
}

void ImageDownloader::setUrl(QUrl url)
{
    this->imageUrl = url;
    networkManager->clearAccessCache();
    networkManager->clearConnectionCache();
}

void ImageDownloader::downloadImage() {
    qDebug() << "Image download begin :" << imageUrl;
    QNetworkRequest request(imageUrl);
    QNetworkReply *reply = networkManager->get(request);
    disconnect(reply, &QNetworkReply::finished, nullptr, nullptr);

    // 使用 lambda 连接信号和槽
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        qDebug() << "get reply";
        this->onDownloadFinished(reply);  // 将 reply 传递给 onDownloadFinished 槽函数
    });
}

void ImageDownloader::onDownloadFinished(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray imageData = reply->readAll();  // 读取下载的数据
        qDebug() << "Image download no error :"/* << imageData*/;
        QPixmap image;
        image.loadFromData(imageData);  // 从数据中加载图片

        if (!image.isNull()) {
            qDebug() << "Image download finish";
            emit imageDownloaded(image);  // 通知主线程图片下载完成
        }
        else
        {
            qDebug() << "Incorrect image";
            emit imageDownloadError();
        }
    } else {
        qDebug() << "Image download failed: " << reply->errorString();
        emit imageDownloadError();
    }

    reply->deleteLater();
    emit finished();  // 通知主线程子线程工作完成
}
