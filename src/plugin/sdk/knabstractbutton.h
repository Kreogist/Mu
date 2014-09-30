/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNABSTRACTBUTTON_H
#define KNABSTRACTBUTTON_H

#include <QWidget>

class KNAbstractButton : public QWidget
{
    Q_OBJECT
public:
    KNAbstractButton(QWidget *parent = 0):QWidget(parent){}
    virtual QPixmap icon() const=0;
    virtual QString text() const=0;
    virtual void setIcon(const QPixmap &icon)=0;
    virtual void setText(const QString &text)=0;

signals:
    void clicked();
    void toggled(bool checked);

public slots:

};

#endif // KNABSTRACTBUTTON_H
