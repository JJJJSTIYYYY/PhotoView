#include "aidescrible.h"

AIDescrible::AIDescrible(QWidget *parent)
    : QWidget{parent}
{
    // 设置窗口内所有控件的字体为 "幼圆"
    this->setStyleSheet("QWidget { font-family: 'YouYuan'; }");

    describleEdit = new QTextEdit(this);
    describleEdit->resize(225, 200);
    describleEdit->setPlaceholderText("在此输入描述\n(将会使用AI润色以此提高成功率)\n\n// 一张动漫美少女\n// A colorful sunset over the mountains");
    describleEdit->move(5, 10);
    describleEdit->setStyleSheet("border-radius: 10px;"
                                 "background-color: #F8F8F8;");

    add = new LabelButton(this, "", "");
    add->move(204, 225);
    add->resize(16, 16);
    add->setPixmap(QPixmap(":/1/arrow-right-bold.png"));
    connect(add, &LabelButton::labelClicked, this, &AIDescrible::addSize);

    modolWid = new QLabel(this);
    modolWid->move(31, 225);
    modolWid->resize(84, 16);
    modolWid->setText("DALLE.2");
    modolWid->setAlignment(Qt::AlignCenter);
    sizeWid = new QLabel(this);
    sizeWid->move(120, 225);
    sizeWid->resize(84, 16);
    sizeWid->setText("256x256");
    sizeWid->setAlignment(Qt::AlignCenter);

    ebb = new LabelButton(this, "", "");
    ebb->move(15, 225);
    ebb->resize(16, 16);
    ebb->setPixmap(QPixmap(":/1/arrow-left-bold.png"));
    connect(ebb, &LabelButton::labelClicked, this, &AIDescrible::ebbSize);

    send = new QPushButton("生成", this);
    cancel = new QPushButton("关闭", this);
    cancel->move(17, 250);
    send->move(127, 250);
    cancel->resize(90, 30);
    send->resize(90, 30);
    connect(cancel, &QPushButton::clicked, this, &AIDescrible::closeWid);
    connect(send, &QPushButton::clicked, this, &AIDescrible::generate);
}

QString AIDescrible::getText()
{
    return describleEdit->toPlainText();
}

QString AIDescrible::getSize()
{
    return sizeWid->text();
}

QString AIDescrible::getModol()
{
    return modolWid->text();
}

void AIDescrible::disableSend(bool disable)
{
    if(disable) this->send->setText("生成中...");
    else this->send->setText("生成");
    this->send->setDisabled(disable);
}

void AIDescrible::setDesribeText(QString des)
{
    this->describleEdit->setText(des);
}

void AIDescrible::addSize()
{
    QString size = sizeWid->text();
    QString modol = modolWid->text();
    if(size == "256x256" && modol == "DALLE.2"){
        sizeWid->setText("512x512");
        modolWid->setText("DALLE.2");
    }
    else if(size == "512x512" && modol == "DALLE.2"){
        sizeWid->setText("1024x1024");
        modolWid->setText("DALLE.2");
    }
    else if(size == "1024x1024" && modol == "DALLE.2"){
        sizeWid->setText("1024x1024");
        modolWid->setText("DALLE.3");
    }
    else if(size == "1024x1024" && modol == "DALLE.3"){
        sizeWid->setText("1024x1792");
        modolWid->setText("DALLE.3");
    }
    else if(size == "1024x1792" && modol == "DALLE.3"){
        sizeWid->setText("1024x1024");
        modolWid->setText("DALLE.3.hd");
    }
    else if(size == "1024x1024" && modol == "DALLE.3.hd"){
        sizeWid->setText("1024x1792");
        modolWid->setText("DALLE.3.hd");
    }
    else if(size == "1024x1792" && modol == "DALLE.3.hd"){
        sizeWid->setText("256x256");
        modolWid->setText("DALLE.2");
    }
}

void AIDescrible::ebbSize()
{
    QString size = sizeWid->text();
    QString modol = modolWid->text();
    if(size == "256x256" && modol == "DALLE.2"){
        sizeWid->setText("1024x1792");
        modolWid->setText("DALLE.3.hd");
    }
    else if(size == "512x512" && modol == "DALLE.2"){
        sizeWid->setText("256x256");
        modolWid->setText("DALLE.2");
    }
    else if(size == "1024x1024" && modol == "DALLE.2"){
        sizeWid->setText("512x512");
        modolWid->setText("DALLE.2");
    }
    else if(size == "1024x1024" && modol == "DALLE.3"){
        sizeWid->setText("1024x1024");
        modolWid->setText("DALLE.2");
    }
    else if(size == "1024x1792" && modol == "DALLE.3"){
        sizeWid->setText("1024x1024");
        modolWid->setText("DALLE.3");
    }
    else if(size == "1024x1024" && modol == "DALLE.3.hd"){
        sizeWid->setText("1024x1792");
        modolWid->setText("DALLE.3");
    }
    else if(size == "1024x1792" && modol == "DALLE.3.hd"){
        sizeWid->setText("1024x1024");
        modolWid->setText("DALLE.3.hd");
    }
}
