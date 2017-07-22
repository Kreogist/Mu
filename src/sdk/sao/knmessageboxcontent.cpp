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
#include <QBoxLayout>
#include <QPainter>

#include "knmessageboxcontent.h"

#include <QDebug>

#define ShadowHeight 9
#define ShadowOpacity 33

KNMessageBoxContent::KNMessageBoxContent(QWidget *parent) :
    QWidget(parent),
    m_upShadowGradient(QLinearGradient(0, 0, 0, ShadowHeight)),
    m_downShadowGradient(QLinearGradient(0, 0, 0, ShadowHeight)),
    m_mainLayout(new QBoxLayout(QBoxLayout::TopToBottom, this)),
    m_content(nullptr),
    m_autoDelete(true)
{
    //Set properties.
    setFocusPolicy(Qt::StrongFocus);

    //Configure the shadow gradient.
    m_upShadowGradient.setColorAt(0, QColor(0, 0, 0, ShadowOpacity));
    m_upShadowGradient.setColorAt(1, QColor(0, 0, 0, 0));

    m_downShadowGradient.setColorAt(0, QColor(0, 0, 0, 0));
    m_downShadowGradient.setColorAt(1, QColor(0, 0, 0, ShadowOpacity));

    //Configure and set the layout.
    m_mainLayout->setContentsMargins(0, ShadowHeight, 0, ShadowHeight);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);
}

QSize KNMessageBoxContent::targetSize() const
{
    //If the content is empty, return the minimum size.
    if(m_content==nullptr)
    {
        return QSize(0, ShadowHeight<<1);
    }
    //Get the size hint of the content, add two shadow size.
    return m_content->size()+QSize(0, ShadowHeight<<1);
}

QWidget *KNMessageBoxContent::content()
{
    return m_content;
}

void KNMessageBoxContent::setContent(QWidget *content, bool autoDelete)
{
    //Check the content widget is null.
    if(m_content==nullptr)
    {
        //Save the content widget.
        m_content=content;
        //Set the focus proxy to content widget.
        setFocusProxy(m_content);
        //Add the content to layout.
        m_mainLayout->addWidget(m_content);
        //Save the auto delete flag.
        m_autoDelete=autoDelete;
    }
}

void KNMessageBoxContent::showContent()
{
    //Check the content widget has been set or not first.
    if(m_content==nullptr)
    {
        return;
    }
    //Show and give the focus to m_content.
    m_content->show();
    m_content->setFocus(Qt::MouseFocusReason);
}

void KNMessageBoxContent::hideContent()
{
    //Check the content widget has been set or not first.
    if(m_content==nullptr)
    {
        return;
    }
    //Simply hide the content widget.
    m_content->hide();
}

void KNMessageBoxContent::checkAutoDelete()
{
    //Check out the auto delete flag.
    if(!m_autoDelete)
    {
        //Reset the parent of the content widget.
        m_content->setParent(nullptr);
    }
}

void KNMessageBoxContent::paintEvent(QPaintEvent *event)
{
    //Do the original painting.
    QWidget::paintEvent(event);
    //Initial painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Draw shadows.
    painter.setPen(Qt::NoPen);
    //Draw up bound shadow.
    painter.fillRect(QRect(0, 0, width(), ShadowHeight),
                     m_upShadowGradient);
    //Draw down bound shadow.
    //--Why using translate?
    //Because you need to change the start and stop position of the gradient
    //if you don't want to use translate.
    painter.translate(0, height()-ShadowHeight);
    painter.fillRect(QRect(0, 0, width(), ShadowHeight),
                     m_downShadowGradient);
}
