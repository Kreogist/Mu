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
#ifndef KNPREFERENCEHEADERBUTTON_H
#define KNPREFERENCEHEADERBUTTON_H

#include <QPainterPath>

#include "knabstractbutton.h"

class QTimeLine;
class KNPreferenceHeaderButton : public KNAbstractButton
{
    Q_OBJECT
public:
    explicit KNPreferenceHeaderButton(QWidget *parent = 0);
    QPixmap icon() const;
    QString text() const;
    void setIcon(const QPixmap &icon);
    void setText(const QString &text);

signals:

public slots:

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private slots:
    void onActionMouseInOut(const int &frame);

private:
    inline void configureTimeLine(QTimeLine *timeLine);
    QPainterPath m_border;
    QTimeLine *m_mouseIn, *m_mouseOut;
    QPixmap m_icon, m_closeIcon;
    qreal m_closeIconOpacity=0.0;
    QPointF m_iconPosition=QPointF(11,7);
    bool m_pressed=false;
};

#endif // KNPREFERENCEHEADERBUTTON_H
