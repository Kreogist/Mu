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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QTimeLine>
#include <QPainter>

#include "knthememanager.h"

#include "knmousesenseheader.h"

#define InBrightness 0x80
#define OutBrightness 0x40

KNMouseSenseHeader::KNMouseSenseHeader(QWidget *parent) :
    QHeaderView(Qt::Horizontal, parent),
    m_mouseIn(generateTimeLine(InBrightness)),
    m_mouseOut(generateTimeLine(OutBrightness)),
    m_lineBrightness(OutBrightness),
    m_buttonGradient(QLinearGradient(QPoint(0,0), QPoint(0, height())))
{
    setObjectName("MouseSenseHeader");
    //Set properties.
    setAutoFillBackground(true);
    setAutoScroll(true);
    setContentsMargins(0, 0, 0, 0);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setDefaultAlignment(Qt::AlignLeft);
    setFrameShape(QFrame::NoFrame);
    setFrameShadow(QFrame::Plain);
    setSectionsMovable(true);

    //Link with the theme changed signal.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNMouseSenseHeader::onThemeChanged);
    //Call the onThemeChanged() to apply the default theme.
    onThemeChanged();
}

void KNMouseSenseHeader::enterEvent(QEvent *event)
{
    //Start animation.
    startAnime(m_mouseIn);
    //Do original enter event.
    QHeaderView::enterEvent(event);
}

void KNMouseSenseHeader::leaveEvent(QEvent *event)
{
    //Start animation.
    startAnime(m_mouseOut);
    //Do original leave event.
    QHeaderView::leaveEvent(event);
}

void KNMouseSenseHeader::resizeEvent(QResizeEvent *event)
{
    //Do resize.
    QHeaderView::resizeEvent(event);
    //Resize the gradient.
    m_buttonGradient.setFinalStop(0, height());
}

void KNMouseSenseHeader::paintSection(QPainter *painter,
                                      const QRect &rect,
                                      int logicalIndex) const
{
    //Get the content rect.
    QRect contentRect=QRect(rect.x()+4,
                            rect.y()+1,
                            rect.width()-8,
                            rect.height()-2);
    //Enabled antialiasing and smooth transform.
    painter->setRenderHints(QPainter::Antialiasing |
                            QPainter::TextAntialiasing |
                            QPainter::SmoothPixmapTransform, true);
    //Draw sort indicator if it's shown, update the content rect.
    if(logicalIndex==sortIndicatorSection() && isSortIndicatorShown())
    {
        //!FIXME: Draw indicator.
        //Change the content rect.
        contentRect=QRect(rect.x()+4,
                          rect.y()+1,
                          rect.width()-28,
                          rect.height()-2);
    }
    //Draw the text of the section.
    painter->drawText(contentRect,
                      model()->headerData(logicalIndex,
                                          Qt::Horizontal,
                                          Qt::TextAlignmentRole).toInt(),
                      model()->headerData(logicalIndex,
                                          Qt::Horizontal,
                                          Qt::DisplayRole).toString());
    //Draw the separate line.
    painter->setPen(palette().color(QPalette::WindowText));
    painter->drawLine(rect.topRight(), rect.bottomRight());
}

void KNMouseSenseHeader::onThemeChanged()
{
    //Get the palette.
    QPalette pal=knTheme->getPalette(objectName());
    //Get the button color.
    QColor color=pal.color(QPalette::Window);
    //Change the brightness, use this as the gradient end color.
    color.setHsv(color.hue(), color.saturation(), OutBrightness+0x28);
    //Change the gradient color.
    m_buttonGradient.setColorAt(1, color);
    //Use the onActionMouseInOut() to set the default palette.
    onActionMouseInOut(OutBrightness);
}

void KNMouseSenseHeader::onActionMouseInOut(const int &frame)
{
    //Save the parameter to line brightness.
    m_lineBrightness=frame;
    //Get the palette.
    QPalette pal=palette();
    //Change the line brightness color.
    QColor color=pal.color(QPalette::WindowText);
    color.setHsv(color.hue(), color.saturation(), frame);
    pal.setColor(QPalette::WindowText, color);
    //Change the button gradient start color.
    color=pal.color(QPalette::Window);
    color.setHsv(color.hue(), color.saturation(), (frame>>1)+0x38);
    m_buttonGradient.setColorAt(0, color);
    pal.setBrush(QPalette::Button, m_buttonGradient);
    pal.setBrush(QPalette::Window, m_buttonGradient);
    //Change the text color.
    color=pal.color(QPalette::ButtonText);
    color.setHsv(color.hue(), color.saturation(), frame+0x7F);
    pal.setColor(QPalette::ButtonText, color);
    //Set the palette.
    setPalette(pal);
}

QTimeLine *KNMouseSenseHeader::generateTimeLine(const int &endFrame)
{
    //Generate the time line.
    QTimeLine *timeLine=new QTimeLine(200, this);
    //Set parameters.
    timeLine->setEndFrame(endFrame);
    //Configure time line.
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    timeLine->setUpdateInterval(10);
    //Link the time line.
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNMouseSenseHeader::onActionMouseInOut);
    //Give back the time line.
    return timeLine;
}

void KNMouseSenseHeader::startAnime(QTimeLine *timeLine)
{
    //Stop animations.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Configure current animation.
    timeLine->setStartFrame(m_lineBrightness);
    //Start animation.
    timeLine->start();
}

