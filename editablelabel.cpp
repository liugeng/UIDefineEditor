#include "editablelabel.h"

EditableLabel::EditableLabel(QWidget *parent) : QLabel(parent), _lineEdit(nullptr)
{

}

EditableLabel::~EditableLabel()
{
    if (_lineEdit) {
        delete _lineEdit;
    }
}

void EditableLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    showLineEdit();
}

void EditableLabel::editFinished()
{
    if (_lineEdit) {
        _lineEdit->hide();
        setText(_lineEdit->text());

        emit textEdited();
    }
}

void EditableLabel::showLineEdit()
{
    if (!_lineEdit) {
        _lineEdit = new QLineEdit(this);
        _lineEdit->setGeometry(QRect(0, 0, geometry().width(), geometry().height()));
        _lineEdit->raise();
        connect(_lineEdit, SIGNAL(editingFinished()), this, SLOT(editFinished()));
    }

    _lineEdit->setText(text());
    _lineEdit->setFocus();
    _lineEdit->selectAll();
    _lineEdit->show();
}


