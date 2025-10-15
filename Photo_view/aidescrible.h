// ai出图窗口
#ifndef AIDESCRIBLE_H
#define AIDESCRIBLE_H

#include <QPushButton>
#include <QTextEdit>
#include <QWidget>
#include "ui_beautify/labelbutton.h"

class AIDescrible : public QWidget
{
    Q_OBJECT
public:
    explicit AIDescrible(QWidget *parent = nullptr);
    QString getText();
    QString getSize();
    QString getModol();
    void disableSend(bool disable);
    void setDesribeText(QString des);
    void addSize();
    void ebbSize();

private:
    QTextEdit *describleEdit;
    QPushButton *send;
    QPushButton *cancel;
    LabelButton *add, *ebb;
    QLabel *sizeWid;
    QLabel *modolWid;

signals:
    void generate();
    void closeWid();

};

#endif // AIDESCRIBLE_H
