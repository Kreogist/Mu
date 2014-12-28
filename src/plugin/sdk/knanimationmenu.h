/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNANIMATIONMENU_H
#define KNANIMATIONMENU_H

#include <QMenu>

class QShowEvent;
class QPropertyAnimation;
class KNAnimationMenu : public QMenu
{
    Q_OBJECT
public:
    explicit KNAnimationMenu(QWidget *parent = 0);
    void setMouseDownPos(const QPoint &mouseDownPos);
    void setSeparatorColor(const QColor &backgroundColor);

signals:

public slots:

protected:
    void showEvent(QShowEvent *event);

private:
    QPropertyAnimation *m_showAnimation;
    QPoint m_mouseDownPos=QPoint(0,0);
};

#endif // KNANIMATIONMENU_H
