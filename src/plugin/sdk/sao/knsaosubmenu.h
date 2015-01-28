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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef KNSAOSUBMENU_H
#define KNSAOSUBMENU_H

#include <QMenu>

class QLabel;
class QPropertyAnimation;
class KNSAOSubMenu : public QMenu
{
    Q_OBJECT
public:
    explicit KNSAOSubMenu(QWidget *parent = 0);
    ~KNSAOSubMenu();

signals:

public slots:

protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private:
#ifndef Q_OS_MACX
    QPropertyAnimation *m_start;
#endif
    QWidget *m_indicator;
};

#endif // KNSAOSUBMENU_H
