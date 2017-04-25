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
#include <QHeaderView>
#include <QPainter>
#include <QScrollBar>
#include <QTimeLine>

#include "knthememanager.h"
#include "sao/knsaostyle.h"

#include "knmousesensetreeview.h"

#define MaxOpacity 0x20
#define FontBase 0xBF
#define ScrollBarWidth 10
#define ScrollBarSpacing 1

KNMouseSenseTreeView::KNMouseSenseTreeView(QWidget *parent) :
    QTreeView(parent),
    m_hScrollBar(new QScrollBar(Qt::Horizontal, this)),
    m_vScrollBar(new QScrollBar(Qt::Vertical, this)),
    m_mouseAnime(new QTimeLine(200, this))
{
    //Set properties.
    setAlternatingRowColors(false); //We will use our own alternating drawing.
    setContentsMargins(0, 0, 0, 0);
    setFrameShape(QFrame::NoFrame);
    setIndentation(0);
    setMouseTracking(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //Set scroll bar policies.
//    horizontalScrollBar()->setSingleStep(5);
//    horizontalScrollBar()->setPageStep(5);
//    verticalScrollBar()->setSingleStep(4);
//    verticalScrollBar()->setPageStep(4);
    //Configure the horizontal scroll bar.
    m_hScrollBar->setObjectName("MusicScrollBar");
    m_hScrollBar->setStyle(KNSaoStyle::instance());
    m_hScrollBar->hide();
    m_hScrollBar->setFixedHeight(ScrollBarWidth);
    knTheme->registerWidget(m_hScrollBar);
    setHorizontalScrollBar(m_hScrollBar);
    //Configure the vertical scroll bar.
    m_vScrollBar->setObjectName("MusicScrollBar");
    m_vScrollBar->setStyle(KNSaoStyle::instance());
    m_vScrollBar->hide();
    knTheme->registerWidget(m_vScrollBar);
    connect(verticalScrollBar(), &QScrollBar::rangeChanged,
            [=](int min, int max)
            {
                //Update the range first.
                m_vScrollBar->setRange(min, max);
                //Check whether the scroll bar is still valid.
                m_vScrollBar->setVisible(min!=max);
                //Update scrollbar state parameters.
                m_vScrollBar->setPageStep(verticalScrollBar()->pageStep());
                m_vScrollBar->setSingleStep(verticalScrollBar()->singleStep());
                //Update the geometry.
                updateVerticalScrollBarGeometry();
            });
    connect(verticalScrollBar(), &QScrollBar::valueChanged,
            [=](int value)
            {
                //Block the signal.
                m_vScrollBar->blockSignals(true);
                //Reset the value.
                m_vScrollBar->setValue(value);
                //Release the block
                m_vScrollBar->blockSignals(false);
            });
    connect(m_vScrollBar, &QScrollBar::valueChanged,
            verticalScrollBar(), &QScrollBar::setValue);

    //Configure the time line.
    m_mouseAnime->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseAnime->setUpdateInterval(16);
    //Link the time line.
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KNMouseSenseTreeView::onMouseInOut);

    //Link with theme manager.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNMouseSenseTreeView::onActionThemeUpdate);
}

void KNMouseSenseTreeView::updateObjectName(const QString &name)
{
    //Set the object name.
    setObjectName(name);
    //Update the frame.
    onActionThemeUpdate();
}

void KNMouseSenseTreeView::enterEvent(QEvent *event)
{
    //Do original enter event.
    QTreeView::enterEvent(event);
    //Start mouse in anime.
    startAnime(MaxOpacity);
}

void KNMouseSenseTreeView::leaveEvent(QEvent *event)
{
    //Do original leave event.
    QTreeView::leaveEvent(event);
    //Start mouse leave anime.
    startAnime(0);
}

void KNMouseSenseTreeView::drawRow(QPainter *painter,
                                   const QStyleOptionViewItem &options,
                                   const QModelIndex &index) const
{
    //Check if this row is odd or oven, and then take one row to have
    //alternative background.
    if(index.row() & 1)
    {
        //Draw the alternative background.
        painter->fillRect(QRect(options.rect.x(),
                                options.rect.y(),
                                width(),
                                options.rect.height()),
                          palette().color(QPalette::AlternateBase));
    }
    //Paint the other parts of the row.
    QTreeView::drawRow(painter, options, index);
}

void KNMouseSenseTreeView::resizeEvent(QResizeEvent *event)
{
    //Resize the tree view widget.
    QTreeView::resizeEvent(event);
    //Update the scroll bar position.
    updateVerticalScrollBarGeometry();
}

void KNMouseSenseTreeView::showEvent(QShowEvent *event)
{
    //Show the treeview widget.
    QTreeView::showEvent(event);
    //Update the scroll bar position.
    updateVerticalScrollBarGeometry();
}

void KNMouseSenseTreeView::onActionThemeUpdate()
{
    //Get the new palette from theme manager, and set it.
    setPalette(knTheme->getPalette(objectName()));
    //Update the palette.
    onMouseInOut(0);
}

void KNMouseSenseTreeView::onMouseInOut(int frame)
{
    //Get the palette.
    QPalette pal=palette();
    //Update the text palette color.
    QColor color=pal.color(QPalette::Text);
    color.setAlpha(FontBase+(frame<<1));
    pal.setColor(QPalette::Text, color);
    //Update the alternating base color.
    color=pal.color(QPalette::AlternateBase);
    color.setAlpha(frame);
    pal.setColor(QPalette::AlternateBase, color);
    //Update the button color.
    color=pal.color(QPalette::Button);
    color.setHsv(color.hue(), color.saturation(), frame<<1);
    pal.setColor(QPalette::Button, color);
    //Set the palette.
    setPalette(pal);
    //Calculate scroll bar alpha.
    int buttonAlpha=frame<<2,
        baseAlpha=frame*3;
    //Update the horizontal scroll bar palette.
    pal=m_hScrollBar->palette();
    color=pal.color(QPalette::Base);
    color.setAlpha(baseAlpha);
    pal.setColor(QPalette::Base, color);
    color=pal.color(QPalette::Button);
    color.setAlpha(buttonAlpha);
    pal.setColor(QPalette::Button, color);
    //Set the palette to horizontal scroll bar.
    m_hScrollBar->setPalette(pal);
    //Update the vertical scroll bar palette.
    pal=m_vScrollBar->palette();
    color=pal.color(QPalette::Base);
    color.setAlpha(baseAlpha);
    pal.setColor(QPalette::Base, color);
    color=pal.color(QPalette::Button);
    color.setAlpha(buttonAlpha);
    pal.setColor(QPalette::Button, color);
    //Set the palette to vertical scroll bar.
    m_vScrollBar->setPalette(pal);
}

inline void KNMouseSenseTreeView::startAnime(int endFrame)
{
    //Stop the mouse animations.
    m_mouseAnime->stop();
    //Set the parameter of the time line.
    m_mouseAnime->setFrameRange(
                palette().color(QPalette::AlternateBase).alpha(),
                endFrame);
    //Start the time line.
    m_mouseAnime->start();
}

inline void KNMouseSenseTreeView::updateVerticalScrollBarGeometry()
{
    //Check scroll bar visiblility.
    if(m_vScrollBar->isVisible())
    {
        //Check whether horizontal scroll is need to be visible.
        if(m_hScrollBar->isVisible())
        {
            //Both of the scroll bar is visible.
            m_vScrollBar->setGeometry(width()-ScrollBarWidth-ScrollBarSpacing,
                                      header()->height(),
                                      ScrollBarWidth,
                                      height()-ScrollBarWidth-ScrollBarSpacing-
                                      header()->height());
        }
        else
        {
            //Only vertical scroll bar is visible.
            m_vScrollBar->setGeometry(width()-ScrollBarWidth-ScrollBarSpacing,
                                      header()->height(),
                                      ScrollBarWidth,
                                      height()-header()->height());
        }
    }
}
