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
#include <QPainter>
#include <QTimeLine>

#include "kndpimanager.h"

#include "kncircleiconbutton.h"

#define MinimumLightness 80
#define ButtonSize 25
#define IconSize 13

int KNCircleIconButton::m_iconPosition=(ButtonSize-IconSize)>>1;

KNCircleIconButton::KNCircleIconButton(QWidget *parent) :
    QAbstractButton(parent),
    m_selected(generateTimeLine(255)),
    m_unselected(generateTimeLine(MinimumLightness)),
    m_lightness(MinimumLightness)
{
    //Set properties.
    setFixedSize(knDpi->size(ButtonSize, ButtonSize));
    setCheckable(true);

    //Link the toggle.
    connect(this, &KNCircleIconButton::toggled,
            this, &KNCircleIconButton::onActionToggled);
}

void KNCircleIconButton::mouseReleaseEvent(QMouseEvent *event)
{
    //Ignore the original release event.
    Q_UNUSED(event)
    //Keep the checked state.
    setChecked(true);
}

void KNCircleIconButton::paintEvent(QPaintEvent *event)
{
    //Ignore the original paint event.
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    //Set the painter hints.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Paint the circle.
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(m_lightness, m_lightness, m_lightness));
    painter.drawEllipse(rect());
    //Paint the icon if the icon is not null.
    if(!icon().isNull())
    {
        painter.drawPixmap(QRect(knDpi->pos(m_iconPosition,
                                            m_iconPosition),
                                 knDpi->size(IconSize,
                                             IconSize)),
                           icon().pixmap(knDpi->size(IconSize, IconSize)));
    }
}

void KNCircleIconButton::onActionToggleChanged(const int &frame)
{
    //Save the lightness.
    m_lightness=frame;
    //Update the button.
    update();
}

void KNCircleIconButton::onActionToggled(bool checked)
{
    //Start the anime according to checked state.
    startAnime(checked?m_selected:m_unselected);
}

QTimeLine *KNCircleIconButton::generateTimeLine(const int &endFrame)
{
    QTimeLine *timeLine=new QTimeLine(200, this);
    timeLine->setUpdateInterval(16);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    timeLine->setEndFrame(endFrame);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNCircleIconButton::onActionToggleChanged);
    return timeLine;
}

void KNCircleIconButton::startAnime(QTimeLine *timeLine)
{
    //Stop all time line.
    m_selected->stop();
    m_unselected->stop();
    //Configure the time line.
    timeLine->setStartFrame(m_lightness);
    //Start time line.
    timeLine->start();
}
