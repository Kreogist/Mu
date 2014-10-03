#include <QKeyEvent>

#include "kncancellineedit.h"

KNCancelLineEdit::KNCancelLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
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
