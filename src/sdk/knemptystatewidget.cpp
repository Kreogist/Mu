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

#include "knemptystatewidget.h"

KNEmptyStateWidget::KNEmptyStateWidget(QWidget *parent) :
    QWidget(parent),
    m_emptyWidget(nullptr),
    m_contentWidget(nullptr),
    m_isEmptyWidget(true)
{
}

void KNEmptyStateWidget::resizeEvent(QResizeEvent *event)
{
    //Resize the container.
    QWidget::resizeEvent(event);
    //Resize the content widget if it's not null.
    if(m_isEmptyWidget)
    {
        //Check the empty widget is null or not.
        if(m_emptyWidget!=nullptr)
        {
            //Resize the empty to the same size as the container.
            m_emptyWidget->resize(size());
        }
    }
    else
    {
        //Check the content widget is null or not.
        if(m_contentWidget!=nullptr)
        {
            //Resize the content widget to the same size as the container.
            m_contentWidget->resize(size());
        }
    }
}
QWidget *KNEmptyStateWidget::emptyWidget() const
{
    return m_emptyWidget;
}

void KNEmptyStateWidget::setEmptyWidget(QWidget *emptyWidget)
{
    //Save the empty widget pointer.
    m_emptyWidget = emptyWidget;
    //Check whether the widget is null.
    if(m_emptyWidget!=nullptr)
    {
        //Change the relationship of the widget.
        m_emptyWidget->setParent(this);
        //Change the visible of the widget.
        m_emptyWidget->setVisible(m_isEmptyWidget);
        //Resize the empty widget.
        m_emptyWidget->resize(size());
    }
}

void KNEmptyStateWidget::showEmptyWidget()
{
    //Change the state flag.
    m_isEmptyWidget=true;
    //Check the empty widget pointer.
    if(m_emptyWidget!=nullptr)
    {
        //Change the visible of the widget.
        m_emptyWidget->setVisible(true);
        //Resize the widget.
        m_emptyWidget->resize(size());
    }
    //Check the content widget.
    if(m_contentWidget!=nullptr)
    {
        //Change the visible of the widget.
        m_contentWidget->setVisible(false);
    }
}

void KNEmptyStateWidget::showContentWidget()
{
    //Change the state flag.
    m_isEmptyWidget=false;
    //Check the content widget pointer.
    if(m_contentWidget!=nullptr)
    {
        //Change the visible of the widget.
        m_contentWidget->setVisible(true);
        //Resize the widget.
        m_contentWidget->resize(size());
    }
    //Check the content widget.
    if(m_emptyWidget!=nullptr)
    {
        //Change the visible of the widget.
        m_emptyWidget->setVisible(false);
    }
}

QWidget *KNEmptyStateWidget::contentWidget() const
{
    return m_contentWidget;
}

void KNEmptyStateWidget::setContentWidget(QWidget *contentWidget)
{
    //Save the content widget.
    m_contentWidget = contentWidget;
    //Check whether the widget is null.
    if(m_contentWidget!=nullptr)
    {
        //Change the relationship of the widget.
        m_contentWidget->setParent(this);
        //Change the visible of the widget.
        m_contentWidget->setVisible(!m_isEmptyWidget);
        //Resize the empty widget.
        m_contentWidget->resize(size());
    }
}

