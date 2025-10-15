// 已弃置
#ifndef CTRLBAR_H
#define CTRLBAR_H

#include <QVBoxLayout>
#include <QToolButton>
#include <QWidget>

class ctrlBar : public QWidget
{
    Q_OBJECT
public:
    explicit ctrlBar(QWidget *parent = nullptr);

private:
    QToolButton *confirmBtn, *cancelBtn;
    QVBoxLayout *layout = new QVBoxLayout;

signals:
    void confirmBtnClicked();
    void cancelBtnClicked();
};

#endif // CTRLBAR_H
