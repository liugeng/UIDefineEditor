#ifndef MYLISTGROUPITEM_H
#define MYLISTGROUPITEM_H

#include <QWidget>
#include "mylistitem.h"

namespace Ui {
class MyListGroupItem;
}

class MyListGroupItem : public QWidget
{
    Q_OBJECT
public:
    explicit MyListGroupItem();
    ~MyListGroupItem();

    void showArrowSign(bool show);

    void setDescription(const QString& desc);
    QString description();

    QList<UIData> datas;

signals:

public slots:

private:
    Ui::MyListGroupItem* ui;
};

#endif // MYLISTGROUPITEM_H
