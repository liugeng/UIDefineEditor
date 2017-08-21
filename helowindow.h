#ifndef HELOWINDOW_H
#define HELOWINDOW_H

#include <QMainWindow>

namespace Ui {
class HeloWindow;
}

class HeloWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HeloWindow(QWidget *parent = 0);
    ~HeloWindow();

private:
    Ui::HeloWindow *ui;
};

#endif // HELOWINDOW_H
