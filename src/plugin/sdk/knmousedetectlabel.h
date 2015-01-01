/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMOUSEDETECTLABEL_H
#define KNMOUSEDETECTLABEL_H

#include <QLabel>

class KNMouseDetectLabel : public QLabel
{
    Q_OBJECT
public:
    explicit KNMouseDetectLabel(QWidget *parent = 0);
    ~KNMouseDetectLabel();

signals:
    void mouseEntered();
    void mouseLeaved();

public slots:

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
};

#endif // KNMOUSEDETECTLABEL_H
