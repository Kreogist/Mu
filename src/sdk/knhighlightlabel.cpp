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

#include <QDebug>

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
    //Draw the content data.
    painter.drawPixmap(0,0,m_scaledContent);
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
    //Get the image size.
    int contentWidth=size().width(), contentHeight=size().height();
    //Clear the scaled content.
    m_scaledContent=QPixmap(contentWidth, contentHeight);
    //Fill up transparent to clear up the data.
    m_scaledContent.fill(QColor(0,0,0,0));
    //Generate the painter.
    QPainter contentPainter(&m_scaledContent);
    //Configure up the painter.
    contentPainter.setRenderHints(QPainter::Antialiasing |
                                  QPainter::SmoothPixmapTransform);
    //Check the content data first.
    if(m_rawContent.isNull())
    {
        //Fill up the scaled content use black color.
        contentPainter.fillRect(m_scaledContent.rect(), QColor(0,0,0));
    }
    else
    {
        QPixmap scaledAlbumArt=m_rawContent.scaled(contentWidth,
                                                   contentHeight,
                                                   Qt::KeepAspectRatio,
                                                   Qt::SmoothTransformation);
        //Draw the raw content on the scaled content.
        contentPainter.drawPixmap((contentWidth-scaledAlbumArt.width())>>1,
                                  (contentHeight-scaledAlbumArt.height())>>1,
                                  scaledAlbumArt);
    }
    //Set pen and brush.
    contentPainter.setPen(Qt::NoPen);
    contentPainter.setBrush(m_highlight);
    contentPainter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
    //Draw the high light.
    contentPainter.drawPolygon(m_highLightArea, 3);
}
