/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNLABELBUTTON_H
#define KNLABELBUTTON_H

#include <QLabel>

class KNLabelButton : public QLabel
{
    Q_OBJECT
public:
    explicit KNLabelButton(QWidget *parent = 0);
    ~KNLabelButton();

signals:
    void clicked();

public slots:

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    bool m_pressed=false;
};

#endif // KNLABELBUTTON_H
