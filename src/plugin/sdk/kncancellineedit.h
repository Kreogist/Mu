#ifndef KNCANCELLINEEDIT_H
#define KNCANCELLINEEDIT_H

#include <QLineEdit>

class KNCancelLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit KNCancelLineEdit(QWidget *parent = 0);

signals:
    void editingEnsure();
    void editingCancel();

public slots:
    void focusOutEvent(QFocusEvent *event);

protected:
    void keyReleaseEvent(QKeyEvent *event);

};

#endif // KNCANCELLINEEDIT_H
