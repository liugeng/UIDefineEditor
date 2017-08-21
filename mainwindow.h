#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QAbstractButton>
#include <QLabel>
#include "mylistgroupitem.h"

namespace Ui {
class MainWindow;
}


class MyListItem;
class MyListGroupItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow* instance;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    MyListGroupItem* curGroupItem() { return _curGroupItem; }

private:
    QLabel* createSelectedFrame();
    void resetGroupData();
    void loadFromJson();
    void writeJson(QJsonArray &json);
    void readConfig();
    void saveConfig();

private slots:

    void on_listWidget1_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_listWidget2_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_listWidget1_itemRemoved(QListWidgetItem* item);

    void on_open_triggered();

    void on_save_triggered();

    void on_help_triggered();

    void closeEvent(QCloseEvent* event);

    void on_newfile_triggered();

private:
    Ui::MainWindow *ui;
    MyListGroupItem *_curGroupItem;
    QLabel *_selectedFrame1;
    QLabel *_selectedFrame2;
    QString _jsonFilePath;
};

#endif // MAINWINDOW_H
