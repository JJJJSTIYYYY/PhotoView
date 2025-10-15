#include "openaiAPI.h"

#include <QDir>
#include <QStandardPaths>



openai::openai(QObject *parent)
{
    apiKey = "";
    apiKey_pic = "";
    quest = "";
    model = "";
    modelPic = "";

    // 创建网络访问管理器
    manager = new QNetworkAccessManager();
    // 设置 API URL
    url = QUrl("https://api.chatanywhere.tech/v1/chat/completions");
    url_pic = QUrl("https://api.chatanywhere.tech/v1/images/generations");
    url_pic2 = QUrl("https://aip.baidubce.com/rpc/2.0/ernievilg/v1/txt2imgv2");
    // 获取随机数生成器的实例
    generator = QRandomGenerator::global();

    thread = new QThread;
    downloader = new ImageDownloader();
    // 连接信号和槽
    connect(thread, &QThread::started, downloader, &ImageDownloader::downloadImage);
    connect(downloader, &ImageDownloader::imageDownloaded, this, &openai::onImageDownloaded);
    connect(downloader, &ImageDownloader::finished, thread, &QThread::quit);
    connect(downloader, &ImageDownloader::imageDownloadError, this, &openai::imageDownloadError);

    request_pic = QNetworkRequest(url_pic2);
    // 设置请求头
    request_pic.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//    request_pic.setRawHeader("Authorization", "Bearer " + apiKey_pic.toUtf8());

}

openai::~openai()
{
    manager->deleteLater();
}

void openai::callOpenAI()
{
    if(apiKey == "") {
        emit this->returnError("error apiKey");
        return;
    }
    // 生成一个随机整数 [0, 100]
    int randomInt = generator->bounded(133);
    // 构建请求数据
    QJsonObject messageObject;
    messageObject["role"] = "user";
    if(quest == "") {
        qDebug() << "quest" << randomInt%6;
        if(randomInt%6 == 0)quest = "聊聊天吧";
        if(randomInt%6 == 1)quest = "聊聊天吧";
        if(randomInt%6 == 2)quest = "聊聊天吧";
        if(randomInt%6 == 3)quest = "聊聊天吧";
        if(randomInt%6 == 4)quest = "聊聊天吧";
        if(randomInt%6 == 5)quest = "聊聊天吧";
    }
    messageObject["content"] = quest;

    QJsonArray messagesArray;
    messagesArray.append(messageObject);

    QJsonObject jsonObject;
    jsonObject["model"] = model;  // 指定模型
    jsonObject["messages"] = messagesArray;

    QJsonDocument jsonDoc(jsonObject);
    QByteArray jsonData = jsonDoc.toJson();

    // 发送 POST 请求
    QNetworkReply* reply = manager->post(request, jsonData);


    // 处理响应
    QObject::connect(reply, &QNetworkReply::finished, [reply, this]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObject = jsonResponse.object();

            QString responseText = "";
            // 提取返回的文本内容
            responseText = jsonObject["choices"].toArray()[0].toObject()["message"].toObject()["content"].toString();
            qDebug() << "Response from OpenAI:" << responseText;
            emit this->returnMessage(responseText);
            reply->deleteLater();
        } else {
            qDebug() << "Error:" << reply->errorString();
            emit this->returnError("network error");
            reply->deleteLater();
        }
    });
}

void openai::callOpenAIModifyDescribe(QString descri)
{
    if(apiKey == "") {
        emit this->returnError("error apiKey");
        return;
    }
    // 画一个可爱的动漫美少女，她躺在床上，周围有许多枕头，场景充满温馨的氛围
    descri = "请你为我去掉以下这段文字的所有感情色彩：" + descri;
    QJsonObject messageObject;
    messageObject["role"] = "user";
    messageObject["content"] = descri;

    QJsonArray messagesArray;
    messagesArray.append(messageObject);

    QJsonObject jsonObject;
    jsonObject["model"] = "gpt-4o-mini";  // 指定模型
    jsonObject["messages"] = messagesArray;

    QJsonDocument jsonDoc(jsonObject);
    QByteArray jsonData = jsonDoc.toJson();

    // 发送 POST 请求
    QNetworkReply* reply = manager->post(request, jsonData);


    // 处理响应
    QObject::connect(reply, &QNetworkReply::finished, [reply, this]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObject = jsonResponse.object();

            QString responseText = "";
            // 提取返回的文本内容
            responseText = jsonObject["choices"].toArray()[0].toObject()["message"].toObject()["content"].toString();
            qDebug() << "Response from OpenAI:" << responseText;
            emit this->modifyDescribeFinish(responseText);
            reply->deleteLater();
        } else {
            qDebug() << "Error:" << reply->errorString() << reply->error();
            emit this->returnError("network error");
            reply->deleteLater();
        }
    });
}

void openai::callOpenAI_pic(QString describe, QString size, int num)
{
    // 动漫女孩在床上，周围有许多枕头
    // A colorful sunset over the mountains and a gril
    // A colorful sunset over the mountains
    if(apiKey_pic == "") {
        qDebug() << "Error: apiKey is empty.";
        emit this->returnError("error apiKey");
        return;
    }
    if(describe == "") {
        qDebug() << "Error: describe is empty.";
        emit this->returnError("network error");
        return;
    }

    QJsonObject json;
    json["prompt"] = describe;
    json["n"] = num;
    json["model"] = modelPic;
    json["size"] = size;
    if(qualityPic != "") json["quality"] = qualityPic;

    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson();

    qDebug() << jsonDoc;
    qDebug()<<url_pic;


    // 发送 POST 请求
    QNetworkReply *reply1 = manager->post(request_pic, jsonData);

    connect(reply1, &QNetworkReply::finished, [reply1, this]() {
        if (reply1->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply1->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObject = jsonResponse.object();

            // 检查 "data" 是否是一个数组，并且数组至少有一个元素
            if (jsonObject.contains("data") && jsonObject["data"].isArray()) {
                QJsonArray dataArray = jsonObject["data"].toArray();
                if (!dataArray.isEmpty()) {
                    QString imageUrl = dataArray[0].toObject()["url"].toString();

                    qDebug() << "imageUrl in callOpenAI_pic :" << imageUrl;
                    // 启动子线程下载图片
                    downloader->setUrl(imageUrl);
                    // 将下载器移动到线程中
                    downloader->moveToThread(thread);
                    // 启动线程
                    thread->start();
                } else {
                    qDebug() << "Error: The 'data' array is empty.";
                }
            } else {
                qDebug() << "Error: 'data' is not an array or is missing.";
            }

            reply1->deleteLater();
        } else {
            qDebug() << "Error generating description:" << reply1->errorString() << reply1->error();
            emit this->returnError("network error");
            reply1->deleteLater();
        }
    });
}

void openai::callOpenAI_pic2(QString describe, int w, int h, int num, QString imagePath, int cd)
{
//    // 动漫女孩在床上，周围有许多枕头
//    // A colorful sunset over the mountains and a gril
//    // A colorful sunset over the mountains
//    if(describe == "") {
//        qDebug() << "Error: describe is empty.";
//        emit this->returnError("network error");
//        return;
//    }

//    QJsonObject json;
//    json["prompt"] = describe;
//    json["width"] = w;
//    json["height"] = h;
//    json["image_num"] = num;

//    if(imagePath != "") {
//        QImage img = QImage(imagePath);
//        if(!img.isNull()) {
//            QByteArray byteArray;
//            QBuffer buffer(&byteArray);
//            buffer.open(QIODevice::WriteOnly);
//            img.save(&buffer, "PNG");
//            QByteArray base64Data = byteArray.toBase64();
//            QString PNGData = QString::fromLatin1(base64Data);

//            json["image"] = PNGData;
//            json["change_degree"] = cd;
//        }
//    }

//    QJsonDocument jsonDoc(json);
//    QByteArray jsonData = jsonDoc.toJson();

//    qDebug() << jsonDoc;
//    qDebug() << url_pic2;


//    // 发送 POST 请求
//    QNetworkReply *reply2 = manager->post(request_pic2, jsonData);

//    connect(reply2, &QNetworkReply::finished, [reply2, this]() {
//        if (reply2->error() == QNetworkReply::NoError) {
//            QByteArray responseData = reply2->readAll();
//            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
//            QJsonObject jsonObject = jsonResponse.object();

//            // 检查 "data" 是否是一个数组，并且数组至少有一个元素
//            if (jsonObject.contains("data") && jsonObject["data"].isArray()) {

//                // 获取 "data" 对象
//                QJsonObject dataObj = jsonObject.value("data").toObject();
//                if (dataObj.isEmpty()) {
//                    qWarning() << "No 'data' object found.";
//                }
//                // 获取 "sub_task_result_list" 数组
//                QJsonArray subTaskArray = dataObj.value("sub_task_result_list").toArray();
//                if (subTaskArray.isEmpty()) {
//                    qWarning() << "No 'sub_task_result_list' array found.";
//                }
//                // 获取第一个子任务的对象
//                QJsonObject subTaskObj = subTaskArray.at(0).toObject();
//                // 获取 "final_image_list" 数组
//                QJsonArray imageListArray = subTaskObj.value("final_image_list").toArray();
//                if (imageListArray.isEmpty()) {
//                    qWarning() << "No 'final_image_list' array found.";
//                }
//                // 获取第一个图片对象
//                QJsonObject imageObj = imageListArray.at(0).toObject();
//                // 提取 "img_url" 字段
//                QString imgUrl = imageObj.value("img_url").toString();

//                if (!imgUrl.isEmpty()) {
//                    qDebug() << "imageUrl in callOpenAI_pic2 :" << imgUrl;
//                    // 启动子线程下载图片
//                    downloader->setUrl(imgUrl);
//                    // 将下载器移动到线程中
//                    downloader->moveToThread(thread);
//                    // 启动线程
//                    thread->start();
//                } else {
//                    qDebug() << "Error: The 'data' array is empty.";
//                }
//            } else {
//                qDebug() << "Error: 'data' is not an array or is missing.";
//            }

//            reply2->deleteLater();
//        } else {
//            qDebug() << "Error generating description:" << reply2->errorString() << reply2->error();
//            emit this->returnError("network error");
//            reply2->deleteLater();
//        }
//    });
}

void openai::setAPIKey(QString key) // 每次倒计时结束都会调用一次
{
    this->apiKey = key;

    request = QNetworkRequest(url);
    // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + apiKey.toUtf8());
}

void openai::setAPIKeyPic(QString key)
{
    this->apiKey_pic = key;

    request_pic = QNetworkRequest(url_pic);
    // 设置请求头
    request_pic.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request_pic.setRawHeader("Authorization", "Bearer " + apiKey_pic.toUtf8());
}

void openai::setQuest(QString quest)
{
    this->quest = quest;
}

void openai::setModel(QString model)
{
    if(model == "") model = "gpt-4o-mini";
    this->model = model;
}

void openai::setModelPic(QString model)
{
    if(model == "") model = "dall-e-3";
    this->modelPic = model;
}

void openai::setQualityPic(QString quality)
{
    qualityPic = quality;
}

void openai::imageDownloadError()
{
    emit this->returnError("picture error");
    return;
}

void openai::onImageDownloaded(const QPixmap &image)
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
            emit generateFinish(savedPath);
        } else {
            qDebug() << "网页保存图片失败:" << savedPath;
            emit this->returnError("picture error");
        }
    }
}
