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
#ifndef KNPROGRESSSLIDER_H
#define KNPROGRESSSLIDER_H

#include "knabstractslider.h"

class QTimeLine;
class KNProgressSlider : public KNAbstractSlider
{
    Q_OBJECT
public:
    explicit KNProgressSlider(QWidget *parent = 0);

signals:

public slots:

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void onActionMouseInOut(const int &frame);

private:
    void configureTimeLine(QTimeLine *timeLine);
    void updateButtonSize();
    qint64 posToValue(int position);
    int m_sliderHeight=4, m_glowWidth=5, m_buttonSize=14,
        m_spacing=2;
    bool m_pressed=false;
    QColor m_rectColor=QColor(255,255,255,70),
           m_backgroundColor=QColor(255,255,255,80),
           m_buttonColor=QColor(255,255,255,110);
    QTimeLine *m_mouseIn, *m_mouseOut;
    qreal m_mouseOutOpacity=0.65,
          m_backOpacity=m_mouseOutOpacity;
};

#endif // KNPROGRESSSLIDER_H
