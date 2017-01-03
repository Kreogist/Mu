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
#include <QPainter>
#include <QTimeLine>

#include "knopacityanimetextbutton.h"

#define DisabledOpacity 0.1
#define BaseOpacity     400
#define HoverOpacity    800
#define RoundedRadius   4
#define SizeIncrease    8
#define DefaultIconSize 16
#define IconTextSpacing 5

KNOpacityAnimeTextButton::KNOpacityAnimeTextButton(QWidget *parent) :
    QAbstractButton(parent),
    m_mouseAnime(new QTimeLine(200, this)),
    m_textOpacity(BaseOpacity),
    m_iconSize(DefaultIconSize)
{
    //Set properties.
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred,
                              QSizePolicy::PushButton));

    //Configure the time line.
    m_mouseAnime->setUpdateInterval(33);
    m_mouseAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KNOpacityAnimeTextButton::onActionOpacityChanged);
}

QSize KNOpacityAnimeTextButton::sizeHint() const
{
    //Calculate the size and plus the size.
    return fontMetrics().boundingRect(text()).size() +
            QSize((SizeIncrease<<1) +
                  (icon().isNull()?0:(m_iconSize+IconTextSpacing)),
                  SizeIncrease);
}

void KNOpacityAnimeTextButton::paintEvent(QPaintEvent *event)
{
    //Ignore original painting.
    Q_UNUSED(event)
    //Initial painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Configure the painter.
    painter.setPen(Qt::NoPen);
    painter.setBrush(palette().brush(QPalette::Button));
    //Check whether the button is disabled and set the opacity.
    painter.setOpacity(isEnabled()?
                           imageOpacity():
                           DisabledOpacity);
    //Draw the background.
    painter.drawRoundedRect(rect(), RoundedRadius, RoundedRadius);
    //Check the icon.
    if(icon().isNull())
    {
        //Update pen.
        painter.setPen(palette().color(QPalette::ButtonText));
        //Draw the text.
        painter.drawText(rect(), Qt::AlignCenter, text());
    }
    else
    {
        //Calculate the position.
        int iconY=(height()-m_iconSize)>>1,
                contentX=(width()-(m_iconSize+IconTextSpacing+
                                  fontMetrics().width(text())))>>1;
        //Draw the icon first.
        painter.drawPixmap(contentX, iconY, icon().pixmap(m_iconSize,
                                                          m_iconSize));
        //Draw the text.
        contentX+=m_iconSize+IconTextSpacing;
        painter.setPen(palette().color(QPalette::ButtonText));
        painter.drawText(contentX, 0, width()-contentX, height(),
                         Qt::AlignVCenter | Qt::AlignLeft,
                         text());
    }
}

qreal KNOpacityAnimeTextButton::imageOpacity()
{
    return m_textOpacity/1000.0;
}

void KNOpacityAnimeTextButton::enterEvent(QEvent *event)
{
    //Start the original enter event.
    QAbstractButton::enterEvent(event);
    //Start the mouse in animation.
    startAnime(HoverOpacity);
}

void KNOpacityAnimeTextButton::leaveEvent(QEvent *event)
{
    //Start the original leave event.
    QAbstractButton::leaveEvent(event);
    //Start the mouse out animation.
    startAnime(BaseOpacity);
}

void KNOpacityAnimeTextButton::focusInEvent(QFocusEvent *event)
{
    //Start the original enter event.
    QAbstractButton::focusInEvent(event);
    //Start the mouse in animation.
    startAnime(HoverOpacity);
}

void KNOpacityAnimeTextButton::focusOutEvent(QFocusEvent *event)
{
    //Start the original enter event.
    QAbstractButton::focusOutEvent(event);
    //Start the mouse in animation.
    startAnime(BaseOpacity);
}

void KNOpacityAnimeTextButton::mousePressEvent(QMouseEvent *event)
{
    //Start the original enter event.
    QAbstractButton::mousePressEvent(event);
    //Start the mouse down animation.
    startAnime(1000);
}

void KNOpacityAnimeTextButton::mouseReleaseEvent(QMouseEvent *event)
{
    //Start the original enter event.
    QAbstractButton::mouseReleaseEvent(event);
    //Start the mouse down animation.
    startAnime(HoverOpacity);
}

void KNOpacityAnimeTextButton::onActionOpacityChanged(const int &opacity)
{
    //Save the opacity.
    m_textOpacity=opacity;
    //Check the mouse is still in the rect.
    if(!rect().contains(QWidget::mapFromGlobal(QCursor::pos())))
    {
        //Change the target.
        m_mouseAnime->setEndFrame(BaseOpacity);
    }
    //Update the widget.
    update();
}

inline void KNOpacityAnimeTextButton::startAnime(const int &endFrame)
{
    //Stop the time line.
    m_mouseAnime->stop();
    //Set the parameter of the time line.
    m_mouseAnime->setFrameRange(m_textOpacity, endFrame);
    //Start the time line.
    m_mouseAnime->start();
}
