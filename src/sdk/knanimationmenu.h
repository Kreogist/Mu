/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNANIMATIONMENU_H
#define KNANIMATIONMENU_H

#include <QMenu>

class QPropertyAnimation;
class KNAnimationMenu : public QMenu
{
    Q_OBJECT
public:
    explicit KNAnimationMenu(QWidget *parent = 0);

    void setMouseDownPos(const QPoint &mouseDownPos);

signals:

public slots:

protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    bool m_showContent;
    QPropertyAnimation *m_showAnime;
    QPoint m_mouseDownPos;
};

#endif // KNANIMATIONMENU_H
