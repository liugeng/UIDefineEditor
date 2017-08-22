#include "helpwindow.h"
#include "ui_helowindow.h"

HelpWindow::HelpWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HeloWindow)
{
    ui->setupUi(this);

    Qt::WindowFlags f = Qt::Dialog;
    setWindowFlags(f);

    setFixedSize(geometry().width(), geometry().height());
}

HelpWindow::~HelpWindow()
{
    delete ui;
}
