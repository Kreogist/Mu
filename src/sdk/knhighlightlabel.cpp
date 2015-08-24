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

#include "knhighlightlabel.h"

KNHighLightLabel::KNHighLightLabel(QWidget *parent) :
    QWidget(parent),
    m_highlight(QLinearGradient(QPointF(0,0), QPointF(0, height()))),
    m_contentPosition(QPoint(0,0)),
    m_rawContent(QPixmap()),
    m_scaledContent(QPixmap())
{
    //Initial the highlight gradient.
    m_highlight.setColorAt(0, QColor(0xff,0xff,0xff,80));
    m_highlight.setColorAt(1, QColor(0xff,0xff,0xff,0));
    //Initial the highlight area.
    //The first point of the high light area is fixed (0,0).
    m_highLightArea[0]=QPointF(0,0);
    updateHighLightArea();
}

void KNHighLightLabel::setPixmap(const QPixmap &pixmap)
{
    //Save the pixmap.
    m_rawContent=pixmap;
    //Update the cached pixmap content.
    updateScaledContent();
    //Update the widget.
    update();
}

QSize KNHighLightLabel::sizeHint() const
{
    return m_scaledContent.isNull()?QSize():m_scaledContent.size();
}

void KNHighLightLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    //Set the rendering hints.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Fill the black background.
    painter.fillRect(0,0,width(),height(),QColor(0,0,0));
    //Draw the contents.
    if(!m_scaledContent.isNull())
    {
        painter.drawPixmap(m_contentPosition, m_scaledContent);
    }
    //Set pen and brush.
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_highlight);
    //Draw the high light.
    painter.drawPolygon(m_highLightArea, 3);
}

void KNHighLightLabel::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QWidget::resizeEvent(event);
    //Update the high light area.
    updateHighLightArea();
    //Update the scaled content.
    updateScaledContent();
}

inline void KNHighLightLabel::updateHighLightArea()
{
    //Update the high light gradient end stop.
    m_highlight.setFinalStop(0, height());
    //Update the high light area.
    m_highLightArea[1]=QPointF((qreal)width()*0.7, 0              );
    m_highLightArea[2]=QPointF(0                 , (qreal)height());
}

inline void KNHighLightLabel::updateScaledContent()
{
    //Check the content data first.
    if(m_rawContent.isNull())
    {
        //Clear the scaled content.
        m_scaledContent=QPixmap();
        //Ignore the null content data scaled.
        return;
    }
    //Scaled the content.
    m_scaledContent=m_rawContent.scaled(size(),
                                        Qt::KeepAspectRatio,
                                        Qt::SmoothTransformation);
    //Update the content position.
    m_contentPosition=QPoint((width()-m_scaledContent.width())>>1,
                             (height()-m_scaledContent.height())>>1);
}
