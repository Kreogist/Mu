/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNFOCUSLINEEDIT_H
#define KNFOCUSLINEEDIT_H

#include <QLineEdit>

class KNFocusLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    KNFocusLineEdit(QWidget *parent = 0):QLineEdit(parent){}

signals:
    void focusIn();
    void focusOut();

protected:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
};

#endif // KNFOCUSLINEEDIT_H
