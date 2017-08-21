#ifndef MYLISTITEM_H
#define MYLISTITEM_H

#include <QWidget>

namespace Ui {
    class MyListItem;
}

struct UIData {
    QString name;
    QString desc;
};

class MyListItem : public QWidget
{
    Q_OBJECT
public:
    static const char* defaultName;
    static const char* defaultDescription;
    static int defaultHeight;

    explicit MyListItem();
    ~MyListItem();

    void setName(const QString &name);
    void setDesc(const QString &desc);

    UIData data;

private:
    Ui::MyListItem* ui;

signals:
    void nameChanged(MyListItem* item);

public slots:
    void onNameChanged();
    void onDescChanged();
};

#endif // MYLISTITEM_H
