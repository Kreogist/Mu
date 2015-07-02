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

#include "knglobal.h"

#include "knlinearsensewidget.h"

#define maximumBrightness 0x57
#define minimalBrightness 0x37

KNLinearSenseWidget::KNLinearSenseWidget(QWidget *parent) :
    QWidget(parent),
    m_highLight(QLinearGradient(QPoint(0,0), QPoint(0, height()))),
    m_highLightColor(QColor(255,255,255,minimalBrightness)),
    m_mouseIn(generateTimeLine(maximumBrightness)),
    m_mouseOut(generateTimeLine(minimalBrightness))
{
    //Set properties.
    setAutoFillBackground(true);

    //Initial the gradient background color and mouse timeline.
    m_highLightColor.setAlpha(0);
    m_highLight.setColorAt(0, m_highLightColor);
    m_highLightColor.setAlpha(minimalBrightness);
    m_highLight.setColorAt(1, m_highLightColor);
}

void KNLinearSenseWidget::enterEvent(QEvent *event)
{
    //Start mouse in animation.
    startAnime(m_mouseIn);
    //Do original enter event.
    QWidget::enterEvent(event);
}

void KNLinearSenseWidget::leaveEvent(QEvent *event)
{
    //Start mouse out animation.
    startAnime(m_mouseOut);
    //Do original leave event.
    QWidget::leaveEvent(event);
}

void KNLinearSenseWidget::resizeEvent(QResizeEvent *event)
{
    //Do original resize.
    QWidget::resizeEvent(event);
    //Change the background.
    m_highLight.setFinalStop(QPoint(0, height()));
}

void KNLinearSenseWidget::paintEvent(QPaintEvent *event)
{
    //Do original paint.
    QWidget::paintEvent(event);
    //Initial painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);

    //Paint the highlight.
    painter.fillRect(rect(), m_highLight);
    painter.fillRect(rect(), knGlobal->textureBursh(KNGlobal::DullPolish));
}

void KNLinearSenseWidget::onActionMouseInOut(const int &frame)
{
    //Update the high light color.
    m_highLightColor.setAlpha(frame);
    //Set palette.
    m_highLight.setColorAt(1, m_highLightColor);
    //Update the widget.
    update();
}

inline QTimeLine *KNLinearSenseWidget::generateTimeLine(
        const int &endBrightness)
{
    QTimeLine *timeLine=new QTimeLine(200, this);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    timeLine->setUpdateInterval(10);
    timeLine->setEndFrame(endBrightness);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNLinearSenseWidget::onActionMouseInOut);
    return timeLine;
}

inline void KNLinearSenseWidget::startAnime(QTimeLine *timeLine)
{
    //Stop time lines.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set time line parameter.
    timeLine->setStartFrame(m_highLightColor.alpha());
    //Start time line.
    timeLine->start();
}
