#ifndef HELOWINDOW_H
#define HELOWINDOW_H

#include <QMainWindow>

namespace Ui {
class HeloWindow;
}

class HelpWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HelpWindow(QWidget *parent = 0);
    ~HelpWindow();

private:
    Ui::HeloWindow *ui;
};

#endif // HELOWINDOW_H
