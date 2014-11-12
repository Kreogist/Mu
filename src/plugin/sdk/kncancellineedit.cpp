#include <QKeyEvent>

#include "kncancellineedit.h"

KNCancelLineEdit::KNCancelLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
}

void KNCancelLineEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    //When lost the focus, it means the user wants to cancel the edit.
    emit editingCancel();
}

void KNCancelLineEdit::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        emit editingCancel();
        return;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        emit editingEnsure();
        return;
    default:
        QLineEdit::keyReleaseEvent(event);
    }
}
