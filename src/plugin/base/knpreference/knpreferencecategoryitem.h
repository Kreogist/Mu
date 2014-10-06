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
#ifndef KNPREFERENCECATEGORYITEM_H
#define KNPREFERENCECATEGORYITEM_H

#include <QPixmap>

#include "knanimecheckedbutton.h"

class QTimeLine;
class KNPreferenceCategoryItem : public KNAnimeCheckedButton
{
    Q_OBJECT
public:
    explicit KNPreferenceCategoryItem(QWidget *parent = 0);
    QPixmap icon() const;
    void setIcon(const QPixmap &icon);
    QString text() const;
    void setText(const QString &text);

signals:

public slots:

protected:
    void startMouseInAnime();
    void startMouseOutAnime();
    void startMouseDownAnime();
    void startMouseUpAnime();

    void paintEvent(QPaintEvent *event);

private:
    QPixmap m_icon;
    QString m_text;
    qreal m_backgroundOpacity=0.0;
    int m_itemHeight=40, m_textX=15;
    QTimeLine *m_mouseIn, *m_mouseOut, *m_mouseDown, *m_mouseUp;
};

#endif // KNPREFERENCECATEGORYITEM_H
