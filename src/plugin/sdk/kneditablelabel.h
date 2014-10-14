/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNEDITABLELABEL_H
#define KNEDITABLELABEL_H

#include <QLabel>

class KNCancelLineEdit;
class KNEditableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit KNEditableLabel(QWidget *parent = 0);
    bool editing() const;
    void setEditing(bool editing);

signals:
    void editingFinished();

public slots:

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void onActionEditFinished();
    void onActionEditCancel();

private:
    void showEdit();
    void hideEdit();
    bool m_pressed=false, m_editing=false;
    KNCancelLineEdit *m_edit;
};

#endif // KNEDITABLELABEL_H
