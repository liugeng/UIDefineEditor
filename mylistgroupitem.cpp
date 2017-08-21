#include "mylistgroupitem.h"
#include "ui_mylistgroupitem.h"

MyListGroupItem::MyListGroupItem() : ui(new Ui::MyListGroupItem)
{
    ui->setupUi(this);
    ui->arrow->hide();
}

MyListGroupItem::~MyListGroupItem()
{
    delete ui;
}

void MyListGroupItem::showArrowSign(bool show)
{
    if (show) {
        ui->arrow->show();
    } else {
        ui->arrow->hide();
    }
}

void MyListGroupItem::setDescription(const QString &desc)
{
    ui->description->setText(desc);
}

QString MyListGroupItem::description()
{
    return ui->description->text();
}

