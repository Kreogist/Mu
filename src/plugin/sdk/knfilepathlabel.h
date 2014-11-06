/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNFILEPATHLABEL_H
#define KNFILEPATHLABEL_H

#include <QLabel>

class KNFilePathLabel : public QLabel
{
    Q_OBJECT
public:
    explicit KNFilePathLabel(QWidget *parent = 0);

signals:

public slots:

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    bool m_pressed=false;
};

#endif // KNFILEPATHLABEL_H
