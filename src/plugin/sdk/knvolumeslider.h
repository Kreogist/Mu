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
#ifndef KNVOLUMESLIDER_H
#define KNVOLUMESLIDER_H

#include "knabstractslider.h"

class QTimeLine;
class KNVolumeSlider : public KNAbstractSlider
{
    Q_OBJECT
public:
    explicit KNVolumeSlider(QWidget *parent = 0);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void onActionMouseInOut(const int &frame);

private:
    void configureTimeLine(QTimeLine *timeline);
    qint64 posToValue(int position);
    int m_sliderHeight=5, m_spacing=2, m_maxAlpha=255, m_minAlpha=80;
    bool m_pressed=false;
    QColor m_rectColor=QColor(255,255,255,m_minAlpha);
    QTimeLine *m_mouseIn, *m_mouseOut;
};

#endif // KNVOLUMESLIDER_H
