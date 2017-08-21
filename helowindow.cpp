#include "helowindow.h"
#include "ui_helowindow.h"

HeloWindow::HeloWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HeloWindow)
{
    ui->setupUi(this);

    Qt::WindowFlags f = Qt::Dialog;
    setWindowFlags(f);

    setFixedSize(geometry().width(), geometry().height());
}

HeloWindow::~HeloWindow()
{
    delete ui;
}
