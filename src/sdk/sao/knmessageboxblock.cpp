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

#include "knmessageboxblock.h"

KNMessageBoxBlock::KNMessageBoxBlock(QWidget *parent) :
    QLabel(parent),
    m_backgroundGradient(QLinearGradient(0,0,0,0))
{
    //Set properties.
    setAutoFillBackground(true);
    setAlignment(Qt::AlignCenter);
    setFocusPolicy(Qt::StrongFocus);

    //Update palette.
    QPalette pal=palette();
    pal.setColor(QPalette::WindowText, QColor(0,0,0,104));
    setPalette(pal);

    //Update the parameter and palette.
    updateSizeParameters();
    updatePalette();
}

void KNMessageBoxBlock::setGradientColor(const QColor &topColor,
                                         const QColor &bottomColor)
{
    //Update the gradient color.
    m_backgroundGradient.setColorAt(0, topColor);
    m_backgroundGradient.setColorAt(1, bottomColor);
    //Update the palette.
    updatePalette();
}

int KNMessageBoxBlock::widthHint()
{
    return qMax(332, fontMetrics().width(text())+150);
}

void KNMessageBoxBlock::resizeEvent(QResizeEvent *event)
{
    //Resize the block.
    QLabel::resizeEvent(event);
    //Update the parameter and palette.
    updateSizeParameters();
    updatePalette();
}

void KNMessageBoxBlock::updateSizeParameters()
{
    //Update the gradient final stop.
    m_backgroundGradient.setFinalStop(0, height());

    //Update the font size.
    QFont labelFont=font();
    //Calculate the prefer size.
    int preferSize=(height()>>1)-9;
    labelFont.setPixelSize(preferSize<2?2:preferSize);
    //Apply the font.
    setFont(labelFont);
}

inline void KNMessageBoxBlock::updatePalette()
{
    //Set the gradient to palette.
    QPalette pal=palette();
    pal.setBrush(QPalette::Window, m_backgroundGradient);
    //Apply the palete.
    setPalette(pal);
}

