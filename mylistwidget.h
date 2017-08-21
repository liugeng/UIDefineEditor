#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include <QListWidget>
#include "mylistitem.h"

class MyListGroupItem;
class MyListWidget : public QListWidget
{
    Q_OBJECT

    //左边的id是1，右边的是2
    Q_PROPERTY(int id READ id WRITE setId)

public:
    MyListWidget(QWidget* parent);

    void setId(int n) { _id = n; }
    int id() { return _id; }

    MyListGroupItem* addGroupItem(const QString& desc = "");
    void addUiItem(const UIData& d);
    bool isGroupItem(QListWidgetItem* qItem);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    void contextMenuEvent(QContextMenuEvent *event);

    void addItemAndSort(const QString& name, const QString& desc);
    void addItemAndPlaceUnderMouse(const QString& name, const QString& desc, const QPoint& pos);

    int getIndexByPos(const QPoint& pos);

private:
    int _id;
    QPoint _mousePos;

signals:
    void itemRemoved(QListWidgetItem* item);

private slots:
    void itemNameChanged(MyListItem* item);
    void contexMenu_addGroup();
    void contexMenu_addUi();
    void contexMenu_delete();
};

#endif // MYLISTWIDGET_H
