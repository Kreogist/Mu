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
#include <QLabel>
#include <QResizeEvent>

#include "knpreferenceheaderbutton.h"

#include "knpreferencetitle.h"

KNPreferenceTitle::KNPreferenceTitle(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setContentsMargins(30,0,0,0);
    //Set the height to fit the button.
    setFixedHeight(64);

    //Initial header button.
    m_headerButton=new KNPreferenceHeaderButton(this);
    //Set header button.
    QPalette buttonPalette=m_headerButton->palette();
    buttonPalette.setColor(QPalette::Window, QColor(20,20,20));
    m_headerButton->setPalette(buttonPalette);
    connect(m_headerButton, &KNPreferenceHeaderButton::clicked,
            this, &KNPreferenceTitle::requireHidePreference);

    //Initial the title label.
    m_title=new QLabel(this);
    m_title->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_title->move(m_titleX, 0);
    //Set the palette.
    QPalette titlePalette=m_title->palette();
    titlePalette.setColor(QPalette::WindowText, QColor(255,255,255));
    m_title->setPalette(titlePalette);
    //Set the title font.
    QFont titleFont=m_title->font();
    titleFont.setPixelSize(20);
    m_title->setFont(titleFont);
}

void KNPreferenceTitle::setTitleText(const QString &caption)
{
    m_title->setText(caption);
}

void KNPreferenceTitle::setTitleIcon(const QPixmap &icon)
{
    m_headerButton->setIcon(icon);
}

void KNPreferenceTitle::resizeEvent(QResizeEvent *event)
{
    //Do the resize first.
    QWidget::resizeEvent(event);
    //Resize the title.
    m_title->resize(width()-m_titleX, height());
}
