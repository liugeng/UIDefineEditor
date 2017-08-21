#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mylistitem.h"
#include "mylistgroupitem.h"
#include <QListWidgetItem>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QStandardPaths>
#include <QTextStream>
#include "helowindow.h"

MainWindow* MainWindow::instance = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _curGroupItem(nullptr)
{
    instance = this;

    ui->setupUi(this);

    _selectedFrame1 = createSelectedFrame();
    _selectedFrame2 = createSelectedFrame();

    ui->listWidget1->setId(1);
    ui->listWidget2->setId(2);

    connect(ui->listWidget1, SIGNAL(itemRemoved(QListWidgetItem*)), this, SLOT(on_listWidget1_itemRemoved(QListWidgetItem*)));
    //ui->listWidget1->setCurrentRow(0);
    ui->listWidget2->setEnabled(false);

    readConfig();
    loadFromJson();
}

MainWindow::~MainWindow()
{
    instance = nullptr;
    delete ui;
}

QLabel* MainWindow::createSelectedFrame()
{
    QLabel* label = new QLabel();
    label->setObjectName(QStringLiteral("selected"));
    label->setGeometry(QRect(0, 0, 200, 50));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
    label->setSizePolicy(sizePolicy);
    label->setStyleSheet(QLatin1String("border-width: 8px;\n"
"border-image: url(:/res/Resources/selected.png) 5 5 5 5 stretch stretch;\n"
"color: rgb(85, 170, 255);"));
    label->setAlignment(Qt::AlignCenter);
    label->setAttribute(Qt::WA_TransparentForMouseEvents);

    return label;
}

void MainWindow::resetGroupData()
{
    if (!_curGroupItem) {
        return;
    }

    _curGroupItem->datas.clear();

    for (int i = 0; i < ui->listWidget2->count()-1; i++) {
        QListWidgetItem* qItem = ui->listWidget2->item(i);
        MyListItem* item = (MyListItem*)ui->listWidget2->itemWidget(qItem);
        _curGroupItem->datas.append(item->data);
    }
}

void MainWindow::loadFromJson()
{
    ui->listWidget1->clear();
    ui->listWidget2->clear();

    _curGroupItem = nullptr;

    if (_jsonFilePath.isEmpty()) {
        return;
    }

    QFile file(_jsonFilePath);
    if (!file.open(QFile::ReadOnly)) {
        _jsonFilePath = "";
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonArray rootArr = doc.array();

    for (int i = rootArr.count()-1; i >= 0; i--) {
        QJsonObject obj = rootArr.at(i).toObject();
        if (obj.contains("name")) {
            UIData d;
            d.name = obj["name"].toString();
            d.desc = obj.contains("desc") ? obj["desc"].toString() : MyListItem::defaultDescription;
            ui->listWidget1->addUiItem(d);
        } else if (obj.contains("group")) {
            MyListGroupItem* group = ui->listWidget1->addGroupItem(obj.contains("desc") ? obj["desc"].toString() : MyListItem::defaultDescription);
            QJsonArray arr = obj["group"].toArray();
            for (int j = 0; j < arr.count(); j++) {
                QJsonObject oj = arr.at(j).toObject();
                UIData d;
                d.name = oj["name"].toString();
                d.desc = oj.contains("desc") ? oj["desc"].toString() : MyListItem::defaultDescription;
                group->datas.append(d);
            }
        }
    }
}

void MainWindow::on_listWidget1_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    _selectedFrame1->setParent(nullptr);

    resetGroupData();

    if (current) {
        if (ui->listWidget1->isGroupItem(current)) {
            ui->listWidget2->setEnabled(true);

            if (_curGroupItem) {
                MyListGroupItem* gItem = dynamic_cast<MyListGroupItem*>(_curGroupItem);
                gItem->showArrowSign(false);
            }

            MyListGroupItem* group = (MyListGroupItem*)ui->listWidget1->itemWidget(current);
            if (group != _curGroupItem) {
                _curGroupItem = group;

                //show list2 items
                ui->listWidget2->clear();
                for (auto &d : _curGroupItem->datas) {
                    ui->listWidget2->addUiItem(d);
                }
            }

            MyListGroupItem* gItem = dynamic_cast<MyListGroupItem*>(group);
            gItem->showArrowSign(true);

        }

        QWidget* item = ui->listWidget1->itemWidget(current);
        if (item) {
            _selectedFrame1->setParent(item);
            _selectedFrame1->lower();
            _selectedFrame1->show();
        }
    }
}

void MainWindow::on_listWidget2_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    _selectedFrame2->setParent(nullptr);
    if (current) {
        _selectedFrame2->setParent(ui->listWidget2->itemWidget(current));
        _selectedFrame2->lower();
        _selectedFrame2->show();
    }
}

void MainWindow::on_listWidget1_itemRemoved(QListWidgetItem *item)
{
    if (ui->listWidget1->itemWidget(item) == _curGroupItem) {
        ui->listWidget2->clear();
        _curGroupItem = nullptr;
        ui->listWidget2->setEnabled(false);
    }
}

void MainWindow::on_open_triggered()
{
    QString dir("./");
    if (!_jsonFilePath.isEmpty()) {
        QFileInfo info(_jsonFilePath);
        dir = info.path();
    }
    QString filename = QFileDialog::getOpenFileName(this, "open", dir, "*.json");
    if (filename.isEmpty()) {
        return;
    }

    _jsonFilePath = filename;
    saveConfig();
    loadFromJson();
}

void MainWindow::writeJson(QJsonArray &json)
{
    MyListGroupItem* group = nullptr;
    MyListItem* item = nullptr;

    for (int i = ui->listWidget1->count()-1; i >= 0; i--) {
        QWidget* w = ui->listWidget1->itemWidget(ui->listWidget1->item(i));
        if (item = dynamic_cast<MyListItem*>(w)) {
            if (item->data.name != MyListItem::defaultName) {
                QJsonObject obj;
                obj["name"] = item->data.name;
                if (item->data.desc != "" && item->data.desc != MyListItem::defaultDescription) {
                    obj["desc"] = item->data.desc;
                }
                json.append(obj);
            }
        } else if (group = dynamic_cast<MyListGroupItem*>(w)) {
            QJsonObject arrObj;
            QJsonArray arr;
            for (auto &d : group->datas) {
                QJsonObject obj;
                obj["name"] = d.name;
                if (d.desc != "" && d.desc != MyListItem::defaultDescription) {
                    obj["desc"] = d.desc;
                }
                arr.append(obj);
            }

            if (group->description() != "" && group->description() != MyListItem::defaultDescription) {
                arrObj["desc"] = group->description();
            }
            arrObj["group"] = arr;
            json.append(arrObj);
        }
    }
}

void MainWindow::readConfig()
{
    QString cfgPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QFile file(cfgPath + "/UIDefineConfig");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream ts(&file);
        _jsonFilePath = ts.readLine();
        file.close();
    } else {
        _jsonFilePath = "./uidefine.json";
    }
}

void MainWindow::saveConfig()
{
    QString cfgPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir dir;
    if (!dir.exists(cfgPath)) {
        dir.mkdir(cfgPath);
    }

    qDebug("save config to: %s", qPrintable(cfgPath));
    QFile file(cfgPath + "/UIDefineConfig");
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }

    QTextStream ts(&file);
    ts << _jsonFilePath;
    file.close();
}

void MainWindow::on_save_triggered()
{
    if (_jsonFilePath.isEmpty()) {
        _jsonFilePath = QFileDialog::getSaveFileName(this, "save", "./uidefine", "*.json");
        if (!_jsonFilePath.isEmpty()) {
            saveConfig();
        }
    }

    QFile file(_jsonFilePath);
    if (!file.open(QFile::WriteOnly)) {
        //QMessageBox::warning(this, "Fail", "file open failed!", QMessageBox::Ok);
        return;
    }

    resetGroupData();

    QJsonArray json;
    writeJson(json);

    QJsonDocument jsonDoc(json);
    file.write(jsonDoc.toJson());
    file.close();
}

void MainWindow::on_help_triggered()
{
    HeloWindow* help = new HeloWindow(this);
    help->show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug("close event");
    if (!(_jsonFilePath.isEmpty() && ui->listWidget1->count() == 0 && ui->listWidget2->count() == 0)) {
        on_save_triggered();
    }

    QMainWindow::closeEvent(event);
}

void MainWindow::on_newfile_triggered()
{
    on_save_triggered();
    _jsonFilePath = "";
    loadFromJson();
}
