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
#ifndef KNOPACITYBUTTON_H
#define KNOPACITYBUTTON_H

#include "knabstractbutton.h"

class QGraphicsOpacityEffect;
class KNOpacityButton : public KNAbstractButton
{
    Q_OBJECT
public:
    explicit KNOpacityButton(QWidget *parent = 0);
    QPixmap icon() const;
    void setIcon(const QPixmap &icon);
    QString text() const
    {
        return QString();
    }
    void setText(const QString &text)
    {
        Q_UNUSED(text)
    }
    qreal opacityGap() const;
    void setOpacityGap(const qreal &opacityGap);
    qreal opacity() const;
    void setOpacity(const qreal &opacity);

signals:

public slots:

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    QPixmap m_icon;
    QGraphicsOpacityEffect *m_opacityEffect;
    qreal m_opacityGap=0.2, m_originalOpacity=1.0;
    bool m_pressed=false;
};

#endif // KNOPACITYBUTTON_H
