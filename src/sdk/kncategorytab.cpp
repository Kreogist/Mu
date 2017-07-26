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

#include "knthememanager.h"
#include "kndpimanager.h"

#include "kncategorytab.h"

#define MaxMouseInAlpha 100
#define MaxMouseDownAlpha 150

#define MouseInAlpha    100
#define MouseOutAlpha   0
#define MouseDownAlpha  200
#define MouseUpAlpha    150
#define IconSize 20
#define TextSpacing 5
#define TextY 5
#define IconY 5

KNCategoryTab::KNCategoryTab(QWidget *parent) :
    QAbstractButton(parent),
    m_mouseInOut(generateTimeLine()),
    m_mouseUpDown(generateTimeLine()),
    m_textWidth(0),
    m_textFullWidth(0),
    m_animeParameter(0),
    m_highLight(QLinearGradient(0,0,0,tabHeight())),
    m_backgroundAlpha(0.0)
{
    setObjectName("CategoryTab");
    //Set properties.
    setAutoExclusive(true);
    setCheckable(true);
    setFixedHeight(tabHeight());
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
                                  QSizePolicy::MinimumExpanding,
                                  QSizePolicy::ToolButton));

    //Configure the background.
    m_highLight.setColorAt(0, QColor(255,255,255,139));
    m_highLight.setColorAt(1, QColor(255,255,255,92));

    //Link time line and animation slot.
    connect(m_mouseInOut, &QTimeLine::frameChanged,
            this, &KNCategoryTab::onMouseInOut);
    connect(m_mouseUpDown, &QTimeLine::frameChanged,
            this, &KNCategoryTab::onActionMouseUpDown);

    //Link the toggled signal.
    connect(this, &KNCategoryTab::toggled,
            this, &KNCategoryTab::onActionToggled);

    //Register at theme manager.
    knTheme->registerWidget(this);
}

int KNCategoryTab::tabHeight()
{
    return knDpi->height(30);
}

void KNCategoryTab::setText(const QString &text)
{
    //Update the text width.
    m_textFullWidth=fontMetrics().width(text);
    //Check out whether the current one is selected.
    if(isChecked())
    {
        //Update the select width.
        m_textWidth=m_textFullWidth;
    }
    //Set the text.
    QAbstractButton::setText(text);
}

void KNCategoryTab::enterEvent(QEvent *event)
{
    //If the button is not checked, start mouse enter anime.
    if(!isChecked())
    {
        startAnime(m_mouseInOut, MouseInAlpha);
    }
    //Do original enter event.
    QAbstractButton::enterEvent(event);
}

void KNCategoryTab::leaveEvent(QEvent *event)
{
    //If the button is not checked, start mouse leave anime.
    if(!isChecked())
    {
        startAnime(m_mouseInOut, MouseOutAlpha);
    }
    //Do original leave event.
    QAbstractButton::leaveEvent(event);
}

void KNCategoryTab::mousePressEvent(QMouseEvent *event)
{
    //If the button is not checked, start mouse press event.
    if(!isChecked())
    {
        startAnime(m_mouseUpDown, MouseDownAlpha);
    }
    //Do original press event.
    QAbstractButton::mousePressEvent(event);
}

void KNCategoryTab::mouseReleaseEvent(QMouseEvent *event)
{
    //Ignore the original event
    Q_UNUSED(event);
    //Check the checked state. If the button has been checked, ignore the mouse
    //release.
    if(isChecked())
    {
        return;
    }
    //Set the checked state, lock the checked state to true.
    setChecked(true);
    //Launch the mouse up anime.
    startAnime(m_mouseUpDown, MouseUpAlpha);
}

void KNCategoryTab::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);

    //Draw background, first draw the button color, then draw the high light.
    painter.setOpacity(m_backgroundAlpha);
    painter.fillRect(rect(), palette().color(QPalette::Button));
    painter.fillRect(rect(), m_highLight);

    //Draw the top line.
    QColor topLineColor(palette().color(QPalette::ButtonText));
    topLineColor.setAlpha(m_animeParameter);
    painter.fillRect(QRect(0, 0, width(), knDpi->height(1)), topLineColor);

    //Initial the icon position.
    int iconX=(width()-knDpi->width(IconSize)-m_textWidth)/2;
    //Draw text.
    painter.setPen(palette().color(QPalette::ButtonText));
    painter.drawText(iconX+knDpi->width(IconSize+TextSpacing),
                     knDpi->height(TextY),
                     m_textWidth,
                     knDpi->height(IconSize),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     text());

    //Draw icon.
    if(!icon().isNull())
    {
        painter.setOpacity(1.0);
        painter.drawPixmap(iconX, knDpi->height(IconY),
                           knDpi->width(IconSize),
                           knDpi->height(IconSize),
                           icon().pixmap(knDpi->size(IconSize, IconSize)));
    }
}

void KNCategoryTab::onMouseInOut(int frame)
{
    //Backup the frame data.
    m_animeParameter=frame;

    //Update the text alpha.
    m_backgroundAlpha=(qreal)frame/(qreal)MaxMouseInAlpha;
    //And update the text display width.
    m_textWidth=m_backgroundAlpha*(qreal)(m_textFullWidth+TextSpacing);
    //Update the widget.
    update();
}

void KNCategoryTab::onActionMouseUpDown(int frame)
{
    //Backup the frame data.
    m_animeParameter=frame;
    //Update the widget.
    update();
}

void KNCategoryTab::onActionToggled(bool checked)
{
    //Start the anime according to checked state.
    startAnime(m_mouseInOut, checked?MouseInAlpha:MouseOutAlpha);
}

inline QTimeLine *KNCategoryTab::generateTimeLine()
{
    QTimeLine *timeLine=new QTimeLine(200, this);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    timeLine->setUpdateInterval(16);
    return timeLine;
}

inline void KNCategoryTab::startAnime(QTimeLine *timeLine, int endFrame)
{
    //Stop all animation.
    m_mouseInOut->stop();
    m_mouseUpDown->stop();
    //Set time line parameter.
    timeLine->setFrameRange(m_animeParameter, endFrame);
    timeLine->start();
}

