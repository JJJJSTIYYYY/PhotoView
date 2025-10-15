#include "widget.h"

#include <QApplication>
#include <QSharedMemory>
#include <QSystemSemaphore>
#include <qmessagebox.h>

// C:\Program Files



QString createPhotoViewDirectory() {
    // 创建 C:\Program Files\Photo_View 目录
    QString programFilesPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QDir dir(programFilesPath);

    if (!dir.exists("Photo_View")) {
        if (!dir.mkdir("Photo_View")) {
            qDebug() << "无法创建目录";
            QMessageBox::warning(nullptr, "Error", "Failed to create Photo_View folder.");
            exit(-1);
        }
    }

    return dir.absoluteFilePath("Photo_View");
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    createPhotoViewDirectory();

    // 创建系统信号量和共享内存，用于标识应用程序实例
    QSystemSemaphore semaphore("MyUniqueAppKeySemaphore", 1);
    semaphore.acquire(); // 请求信号量

    // 使用共享内存来检测程序是否已经运行
    QSharedMemory sharedMemory("MyUniqueAppKey");
    if (!sharedMemory.create(1)) {
        semaphore.release(); // 释放信号量
        QMessageBox::warning(nullptr, "Already Running", "The application is already running.");
        return 0; // 退出程序
    }

    semaphore.release(); // 释放信号量

    Widget w;
    w.show();
    return a.exec();
}
