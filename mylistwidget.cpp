#include "mylistwidget.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include "mylistitem.h"
#include "mylistgroupitem.h"
#include <QMenu>
#include <QMessageBox>
#include "uidata.h"
#include "mainwindow.h"

MyListWidget::MyListWidget(QWidget* parent) : QListWidget(parent)
{
}

MyListGroupItem* MyListWidget::addGroupItem(const QString& desc)
{
    QListWidgetItem* qItem = new QListWidgetItem(this);
    qItem->setSizeHint(QSize(0, MyListItem::defaultHeight));

    MyListGroupItem* item = new MyListGroupItem();
    item->setDescription(desc);
    setItemWidget(qItem, item);

    return item;
}

void MyListWidget::addUiItem(const UIData& d)
{
    QListWidgetItem* qItem = new QListWidgetItem(this);
    qItem->setSizeHint(QSize(0, MyListItem::defaultHeight));

    MyListItem* item = new MyListItem();
    item->setName(d.name);
    item->setDesc(d.desc);
    setItemWidget(qItem, item);

    if (id() == 2) {
        connect(item, SIGNAL(nameChanged(MyListItem*)), this, SLOT(itemNameChanged(MyListItem*)));
    }
}

bool MyListWidget::isGroupItem(QListWidgetItem *qItem)
{
    if (!qItem) {
        return false;
    }
    QWidget* w = itemWidget(qItem);
    return dynamic_cast<MyListGroupItem*>(w) != nullptr;
}

void MyListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug("drag enter");
    MyListWidget* list = dynamic_cast<MyListWidget*>(event->source());
    if (list && list != this && list->isGroupItem(list->currentItem())) {
        return;
    }
    event->accept();
}

void MyListWidget::dropEvent(QDropEvent *event)
{
    qDebug("drop event");
    MyListWidget* list = dynamic_cast<MyListWidget*>(event->source());
    if (list && list != this) {
        QListWidgetItem* current = list->currentItem();
        MyListItem* mItem = (MyListItem*)list->itemWidget(current);

        if (id() == 1) {
            addItemAndPlaceUnderMouse(mItem->data.name, mItem->data.desc, event->pos());
        } else {
            addItemAndSort(mItem->data.name, mItem->data.desc);

            MyListGroupItem* group = MainWindow::instance->curGroupItem();
            if (group) {
                for (int i = 0; i < list->count(); i++) {
                    QListWidgetItem* qItem = list->item(i);
                    if (list->itemWidget(qItem) == group) {
                        list->setCurrentItem(qItem);
                        break;
                    }
                }
            }
        }

        delete current;

    } else {
        QListWidget::dropEvent(event);
    }
}

void MyListWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QListWidgetItem* qItem = itemAt(event->pos());

    QMenu* menu = new QMenu(this);

    QAction* act1 = nullptr;
    if (id() == 1) act1 = new QAction("Add Group", this);
    QAction* act2 = new QAction("Add UI", this);
    QAction* act3 = nullptr;
    if (qItem) act3 = new QAction("Delete", this);

    if (id() == 1) menu->addAction(act1);
    menu->addAction(act2);
    if (qItem) menu->addAction(act3);

    if (id() == 1) connect(act1, SIGNAL(triggered(bool)), this, SLOT(contexMenu_addGroup()));
    connect(act2, SIGNAL(triggered(bool)), this, SLOT(contexMenu_addUi()));
    if (qItem) connect(act3, SIGNAL(triggered(bool)), this, SLOT(contexMenu_delete()));

    _mousePos = event->pos();
    menu->exec(event->globalPos());

    delete menu;
    if (id() == 1) delete act1;
    delete act2;
    if (qItem) delete act3;
}

void MyListWidget::contexMenu_addGroup()
{
    QListWidgetItem* qItem = new QListWidgetItem();
    qItem->setSizeHint(QSize(0, MyListItem::defaultHeight));
    int idx = getIndexByPos(_mousePos);
    insertItem(idx, qItem);

    MyListGroupItem* item = new MyListGroupItem();
    setItemWidget(qItem, item);

    setCurrentItem(qItem);
}

void MyListWidget::contexMenu_addUi()
{
    if (id() == 1) {
        addItemAndPlaceUnderMouse(MyListItem::defaultName, MyListItem::defaultDescription, _mousePos);
    } else {
        addItemAndSort(MyListItem::defaultName, MyListItem::defaultDescription);
    }
}

void MyListWidget::contexMenu_delete()
{
    QListWidgetItem* qItem = itemAt(_mousePos);
    if (!qItem) {
        return;
    }

    QWidget* item = itemWidget(qItem);
    QString title = "Delete ";

    MyListItem* item1 = nullptr;
    MyListGroupItem* item2 = nullptr;
    if (item1 = dynamic_cast<MyListItem*>(item)) {
        title += item1->data.name;
    } else if (item2 = dynamic_cast<MyListGroupItem*>(item)) {
        title += "Group";
    }

    int ret = QMessageBox::warning(this, title, tr("Are You Sure?"), QMessageBox::Yes, QMessageBox::No);
    if (ret != QMessageBox::Yes) {
        return;
    }

    emit itemRemoved(qItem);
    delete qItem;
}

//用于id=2列表
void MyListWidget::addItemAndSort(const QString& name, const QString& desc)
{
    /* 源码里判断如果 !item->view 才会执行插入操作
     * 所以这里构造的时候不能传入 this
     */
    QListWidgetItem* qItem = new QListWidgetItem();
    qItem->setSizeHint(QSize(0, MyListItem::defaultHeight));

    if (count() > 0) {
        bool added = false;
        for (int i = count()-1; i >= 0; i--) {
            QListWidgetItem* child = item(i);
            MyListItem* childItem = (MyListItem*)itemWidget(child);
            if (name.toLower().compare(childItem->data.name.toLower()) > 0) {
                insertItem(i+1, qItem);
                added = true;
                break;
            }
        }

        if (!added) {
            insertItem(0, qItem);
        }
    } else {
        addItem(qItem);
    }

    MyListItem* newItem = new MyListItem();
    newItem->setName(name);
    newItem->setDesc(desc);

    setItemWidget(qItem, newItem);
    setCurrentItem(qItem);

    connect(newItem, SIGNAL(nameChanged(MyListItem*)), this, SLOT(itemNameChanged(MyListItem*)));
}

//用于id==1列表
void MyListWidget::addItemAndPlaceUnderMouse(const QString& name, const QString& desc, const QPoint &pos)
{
    QListWidgetItem* qItem = new QListWidgetItem();
    qItem->setSizeHint(QSize(0, MyListItem::defaultHeight));

    MyListItem* newItem = new MyListItem();
    newItem->setName(name);
    newItem->setDesc(desc);

    int idx = getIndexByPos(pos);

    insertItem(idx, qItem);
    setItemWidget(qItem, newItem);
    setCurrentItem(qItem);
}

int MyListWidget::getIndexByPos(const QPoint &pos)
{
    QListWidgetItem* qItem1 = itemAt(pos);
    QListWidgetItem* qItem2 = itemAt(pos.x(), pos.y() + MyListItem::defaultHeight*0.5);

    int idx = 0;

    //放到最后
    if (!qItem1 || !qItem2) {
        idx = count();
    }
    //顶替光标处item的位置
    else if (qItem1 == qItem2) {
        idx = row(qItem1);

    }
    //放到光标处item的下面
    else if (qItem1 != qItem2) {
        idx = row(qItem1) + 1;
    }

    return idx;
}

void MyListWidget::itemNameChanged(MyListItem* mItem)
{
    for (int i = 0; i < count(); i++) {
        QListWidgetItem* qItem = item(i);
        MyListItem* itemI = (MyListItem*)itemWidget(qItem);
        if (itemI == mItem) {
            addItemAndSort(mItem->data.name, mItem->data.desc);
            delete qItem;
            break;
        }
    }
}


