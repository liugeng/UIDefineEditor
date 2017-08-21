#ifndef EDITABLELABEL_H
#define EDITABLELABEL_H

#include <QLabel>
#include <QLineEdit>

class EditableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit EditableLabel(QWidget *parent);
    ~EditableLabel();

    void mouseDoubleClickEvent(QMouseEvent *event);

signals:
    void textEdited();

public slots:
    void editFinished();

private:
    void showLineEdit();

private:
    QLineEdit *_lineEdit;
};

#endif // EDITABLELABEL_H
