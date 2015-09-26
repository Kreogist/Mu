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
#include <QTimeLine>
#include <QPainter>
#include <QMouseEvent>

#include "knglassanimebutton.h"

#define MouseInTarget 50
#define MouseOutTarget 0
#define MouseDownTarget 25

KNGlassAnimeButton::KNGlassAnimeButton(QWidget *parent) :
    QAbstractButton(parent),
    m_lineGradient(QLinearGradient(QPointF(0,0), QPointF(0,0))),
    m_backGradient(QLinearGradient(QPointF(0,0), QPointF(0,0))),
    m_mouseAnime(new QTimeLine(100, this)),
    m_iconSize(0),
    m_currentHighlight(0),
    m_leftLine(false)
{
    //Set properties.
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

    //Initial linear gradients.
    m_lineGradient.setColorAt(0, QColor(255,255,255,0));
    m_lineGradient.setColorAt(1, QColor(255,255,255,140));
    m_backGradient.setColorAt(0, QColor(255,255,255,0));
    m_backGradient.setColorAt(1, QColor(255,255,255,0));

    //Configure the time line.
    m_mouseAnime->setUpdateInterval(10);
    m_mouseAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KNGlassAnimeButton::onActionMouseAnime);
}

void KNGlassAnimeButton::onActionMouseAnime(const int &frame)
{
    //Save the high light parameter.
    m_currentHighlight=frame;
    //Update the gradient.
    m_backGradient.setColorAt(1, QColor(255,255,255,m_currentHighlight));
    //Update the widget.
    update();
}

inline void KNGlassAnimeButton::startAnimation(const int &endFrame)
{
    //Stop the anime.
    m_mouseAnime->stop();
    //Change the highlight.
    m_mouseAnime->setFrameRange(m_currentHighlight, endFrame);
    //Start the anime.
    m_mouseAnime->start();
}

bool KNGlassAnimeButton::showLeftLine() const
{
    return m_leftLine;
}

void KNGlassAnimeButton::setShowLeftLine(bool leftline)
{
    m_leftLine = leftline;
}

void KNGlassAnimeButton::enterEvent(QEvent *event)
{
    //Do the original event.
    QAbstractButton::enterEvent(event);
    //Stop and start the mouse in animation.
    startAnimation(MouseInTarget);
}

void KNGlassAnimeButton::leaveEvent(QEvent *event)
{
    //Do the original event.
    QAbstractButton::leaveEvent(event);
    //Stop and start the mouse out animation.
    startAnimation(MouseOutTarget);
}

void KNGlassAnimeButton::focusInEvent(QFocusEvent *event)
{
    //Do the original event.
    QAbstractButton::focusInEvent(event);
    //Stop and start the mouse in animation.
    startAnimation(MouseInTarget);
}

void KNGlassAnimeButton::focusOutEvent(QFocusEvent *event)
{
    //Do the original event.
    QAbstractButton::focusOutEvent(event);
    //Stop and start the mouse out animation.
    startAnimation(MouseOutTarget);
}

void KNGlassAnimeButton::mousePressEvent(QMouseEvent *event)
{
    //Stop the animation.
    if(event->button()==Qt::LeftButton)
    {
        //Start the animation.
        startAnimation(MouseDownTarget);
    }
    //Do original event.
    QAbstractButton::mousePressEvent(event);
}

void KNGlassAnimeButton::mouseReleaseEvent(QMouseEvent *event)
{
    //Ensure this button has been mouse down.
    if(event->button()==Qt::LeftButton &&
            rect().contains(event->pos()) && isDown())
    {
        //Start mouse up animation.
        startAnimation(MouseInTarget);
    }
    //Do original event.
    QAbstractButton::mouseReleaseEvent(event);
}

void KNGlassAnimeButton::paintEvent(QPaintEvent *event)
{
    //Ignore the event.
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    //Configure the painter.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Draw the back gradient.
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_backGradient);
    painter.drawRect(rect());
    //Draw the side line.
    painter.setBrush(m_lineGradient);
    painter.drawRect(width()-1,0,1,height());
    //If the user wants a left line, then draw it.
    if(m_leftLine)
    {
        painter.drawRect(0,0,1,height());
    }
    //Draw the icon.
    if(!icon().isNull())
    {
        QPixmap renderedIcon=icon().pixmap(m_iconSize, m_iconSize);
        //Draw the icon to the center of the button.
        painter.drawPixmap((width()-renderedIcon.width())>>1,
                           (height()-renderedIcon.height())>>1,
                           renderedIcon);
    }
}

void KNGlassAnimeButton::resizeEvent(QResizeEvent *event)
{
    //Do the original resize.
    QAbstractButton::resizeEvent(event);
    //Update the line gradient.
    m_lineGradient.setFinalStop(0,height());
    m_backGradient.setFinalStop(0,height());
    //Update the icon size.
    m_iconSize=(qMin(width(), height())>>2)*3;
}
