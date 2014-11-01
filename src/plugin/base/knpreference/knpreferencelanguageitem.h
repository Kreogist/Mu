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
#ifndef KNPREFERENCELANGUAGEITEM_H
#define KNPREFERENCELANGUAGEITEM_H

#include <QPixmap>

#include "knanimecheckedbutton.h"

class QTimeLine;
class KNPreferenceLanguageItem : public KNAnimeCheckedButton
{
    Q_OBJECT
public:
    explicit KNPreferenceLanguageItem(QWidget *parent = 0);
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

private slots:
    void onActionMouseInOut(const int &frame);

private:
    void configureTimeLine(QTimeLine *timeLine);
    QPixmap m_icon;
    QString m_text;
    qreal m_backgroundOpacity=0.0;
    int m_itemHeight=40, m_iconSize=30, m_spacing=8, m_iconX=17,
        m_textX=m_iconX+m_iconSize+m_spacing, m_progress=0, m_shadowHeight=5;
    QTimeLine *m_mouseIn, *m_mouseOut, *m_mouseDown, *m_mouseUp;
    QLinearGradient m_shadowGradient;
};

#endif // KNPREFERENCELANGUAGEITEM_H
