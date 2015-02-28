/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMOUSEUNCLICKABLEWIDGET_H
#define KNMOUSEUNCLICKABLEWIDGET_H

#include <QWidget>

class KNMouseUnclickableWidget : public QWidget
{
    Q_OBJECT
public:
    KNMouseUnclickableWidget(QWidget *parent = 0):QWidget(parent){}

signals:

public slots:

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // KNMOUSEUNCLICKABLEWIDGET_H
