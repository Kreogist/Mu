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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QBoxLayout>
#include <QPainter>
#include <QTimeLine>

#include "kndpimanager.h"
#include "knthememanager.h"

#include "knmainwindowstatusbar.h"

#include <QDebug>

#define GradientWidth 50
#define ButtonSize 14
#define ButtonSpacing 2
#define RightSpacing 2
#define MinimumOpacity 37
#define MaximumOpacity 97

KNMainWindowStatusBar::KNMainWindowStatusBar(QWidget *parent) :
    QWidget(parent),
    m_backgroundColor(QColor(255, 255, 255)),
    m_background(QLinearGradient(QPointF(0, 0), knDpi->posF(GradientWidth, 0))),
    m_mainLayout(new QBoxLayout(QBoxLayout::RightToLeft, this)),
    m_mouseInOut(generateTimeline()),
    m_opacity(MinimumOpacity),
    m_buttonCount(0)
{
    //Set properties.
    setContentsMargins(0, 0, 0, 0);
    setFixedWidth(knDpi->width(GradientWidth));
    setFixedHeight(knDpi->height(20));
    //Initial the background.
    m_background.setColorAt(0, QColor(0, 0, 0, 0));
    //Update the gradient.
    updateGradient();

    //Configure the main layout.
    m_mainLayout->setContentsMargins(
                knDpi->margins(GradientWidth, 0, RightSpacing, 0));
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);
}

void KNMainWindowStatusBar::addWidget(QWidget *widget)
{
    //Update the size of the widget.
    widget->setFixedSize(knDpi->size(ButtonSize, ButtonSize));
    //Set the widget size.
    m_mainLayout->addWidget(widget);
    m_mainLayout->addSpacing(knDpi->width(ButtonSpacing));
    //Increase the count.
    ++m_buttonCount;
    //Update the widget size.
    setFixedWidth(knDpi->width(GradientWidth+RightSpacing+
                               m_buttonCount*(16+ButtonSpacing)));
}

void KNMainWindowStatusBar::paintEvent(QPaintEvent *event)
{
    //Initial the widget.
    QWidget::paintEvent(event);
    //Draw the background.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Draw the background.
    painter.fillRect(rect(), m_background);
}

void KNMainWindowStatusBar::enterEvent(QEvent *event)
{
    //Start the animation to range end.
    startAnime(MaximumOpacity);
    //Do the original enter event.
    QWidget::enterEvent(event);
}

void KNMainWindowStatusBar::leaveEvent(QEvent *event)
{
    //Start the animation to range start.
    startAnime(MinimumOpacity);
    //Do the original leave event.
    QWidget::leaveEvent(event);
}

void KNMainWindowStatusBar::onPaletteChanged()
{
    //Save the color.
    m_backgroundColor=
            knTheme->getPalette("MainWindowStatusBar").color(QPalette::Window);
    //Update the gradient.
    updateGradient();
}

void KNMainWindowStatusBar::changeBackgroundColor(int frame)
{
    //Save the opacity.
    m_opacity=frame;
    //Update the gradient.
    updateGradient();
}

inline void KNMainWindowStatusBar::updateGradient()
{
    //Update the opacity.
    m_backgroundColor.setAlpha(m_opacity);
    //Set the background.
    m_background.setColorAt(1.0, m_backgroundColor);
    //Update the widget.
    update();
}

inline QTimeLine *KNMainWindowStatusBar::generateTimeline()
{
    //Generate the time line.
    QTimeLine *timeline=new QTimeLine(200, this);
    timeline->setEasingCurve(QEasingCurve::OutCubic);
    timeline->setUpdateInterval(10);
    //This animation is going to change to background color, so the each frame
    //will be the parameter of a color. When frame changed, change the color.
    connect(timeline, &QTimeLine::frameChanged,
            this, &KNMainWindowStatusBar::changeBackgroundColor);
    //Return the time line.
    return timeline;
}

inline void KNMainWindowStatusBar::startAnime(int endFrame)
{
    //Stop the time line.
    m_mouseInOut->stop();
    //Set the range of the color from current to range start.
    m_mouseInOut->setFrameRange(m_mouseInOut->currentFrame(),
                                endFrame);
    //Start the time line.
    m_mouseInOut->start();
}
