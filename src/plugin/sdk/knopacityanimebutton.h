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
#ifndef KNOPACITYANIMEBUTTON_H
#define KNOPACITYANIMEBUTTON_H

#include "knabstractbutton.h"

class QPropertyAnimation;
class QGraphicsOpacityEffect;
class KNOpacityAnimeButton : public KNAbstractButton
{
    Q_OBJECT
public:
    explicit KNOpacityAnimeButton(QWidget *parent = 0);
    QPixmap icon() const;
    QSize sizeHint() const;
    void setIcon(const QPixmap &icon);
    QString text() const
    {
        return QString();
    }
    void setText(const QString &text)
    {
        Q_UNUSED(text)
    }

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    void configureAnimation(QPropertyAnimation *animation);
    QGraphicsOpacityEffect *m_effect;
    QPropertyAnimation *m_mouseDown, *m_mouseUp, *m_mouseIn, *m_mouseOut;
    qreal m_startOpacity=0.4, m_inoutOpacity=0.8;
    bool m_isPressed=false;
    QPixmap m_icon;
};

#endif // KNOPACITYPIXMAPBUTTON_H
