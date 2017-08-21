#include "mylistitem.h"
#include "ui_mylistitem.h"

const char* MyListItem::defaultName = "a_new_ui";
const char* MyListItem::defaultDescription = "description";
int MyListItem::defaultHeight = 50;

MyListItem::MyListItem() : ui(new Ui::MyListItem)
{
    ui->setupUi(this);
    connect(ui->name, SIGNAL(textEdited()), this, SLOT(onNameChanged()));
    connect(ui->description, SIGNAL(textEdited()), this, SLOT(onDescChanged()));
}

MyListItem::~MyListItem()
{
    qDebug("~MyListItem");
    delete ui;
}

void MyListItem::setName(const QString &name)
{
    ui->name->setText(name);
    data.name=  name;
}

void MyListItem::setDesc(const QString &desc)
{
    ui->description->setText(desc);
    data.desc = desc;
}

void MyListItem::onNameChanged()
{
    if (ui->name->text() == "") {
        setName(defaultName);
    }
    data.name = ui->name->text();
    emit nameChanged(this);
}

void MyListItem::onDescChanged()
{
    data.desc = ui->description->text();
}
