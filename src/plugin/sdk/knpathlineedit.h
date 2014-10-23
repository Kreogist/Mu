/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNPATHLINEEDIT_H
#define KNPATHLINEEDIT_H

#include <QLineEdit>

class KNConnectionHandler;
class KNPathLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit KNPathLineEdit(QWidget *parent = 0);

signals:
    void pathExist();
    void pathNotExist();
    void startEditPath();

public slots:

protected:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);

private slots:
    void disableChangedCheck();
    void enableChangedCheck();
    void checkCurrentPath();

private:
    KNConnectionHandler *m_checkHandle;
};

#endif // KNPATHLINEEDIT_H
