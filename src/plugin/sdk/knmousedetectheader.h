/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMOUSEDETECTHEADER_H
#define KNMOUSEDETECTHEADER_H

#include <QWidget>

class KNMouseDetectHeader : public QWidget
{
    Q_OBJECT
public:
    explicit KNMouseDetectHeader(QWidget *parent = 0);

signals:
    void requireActivateWidget();
    void requireInactivateWidget();

public slots:

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
};

#endif // KNMOUSEDETECTHEADER_H
