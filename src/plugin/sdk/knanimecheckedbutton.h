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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#ifndef KNANIMECHECKEDBUTTON_H
#define KNANIMECHECKEDBUTTON_H

#include "knabstractbutton.h"

class KNAnimeCheckedButton : public KNAbstractButton
{
    Q_OBJECT
public:
    explicit KNAnimeCheckedButton(QWidget *parent = 0);
    bool checked() const;

signals:

public slots:
    void setChecked(bool checked);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    virtual void startMouseInAnime()=0;
    virtual void startMouseOutAnime()=0;
    virtual void startMouseDownAnime()=0;
    virtual void startMouseUpAnime()=0;

protected slots:
    virtual void onCheckedChanged();

private slots:

private:
    bool m_pressed=false, m_checked=false;
};

#endif // KNANIMECHECKEDBUTTON_H
