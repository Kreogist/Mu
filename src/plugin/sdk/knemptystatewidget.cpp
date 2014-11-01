/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "knemptystatewidget.h"

KNEmptyStateWidget::KNEmptyStateWidget(QWidget *parent) :
    QStackedWidget(parent)
{
    ;
}

QWidget *KNEmptyStateWidget::emptyWidget() const
{
    return m_emptyWidget;
}

void KNEmptyStateWidget::setEmptyWidget(QWidget *emptyWidget)
{
    //Set and add the widget.
    m_emptyWidget=emptyWidget;
    addWidget(m_emptyWidget);
}

QWidget *KNEmptyStateWidget::contentWidget() const
{
    return m_contentWidget;
}

void KNEmptyStateWidget::setContentWidget(QWidget *contentWidget)
{
    //Set and add the widget.
    m_contentWidget=contentWidget;
    addWidget(m_contentWidget);
}

void KNEmptyStateWidget::showEmptyWidget()
{
    //Display the empty state widget.
    setCurrentWidget(m_emptyWidget);
}

void KNEmptyStateWidget::showContentWidget()
{
    //Display the content widget.
    setCurrentWidget(m_contentWidget);
}

void KNEmptyStateWidget::showEvent(QShowEvent *event)
{
    //Emit the show signal.
    emit aboutToBeShown();
    //Do the original show event.
    QStackedWidget::showEvent(event);
}
