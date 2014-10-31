/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "knmessageboxconfigure.h"

KNMessageBoxConfigure *KNMessageBoxConfigure::m_instance=nullptr;

KNMessageBoxConfigure *KNMessageBoxConfigure::instance()
{
    return m_instance==nullptr?m_instance=new KNMessageBoxConfigure:m_instance;
}

KNMessageBoxConfigure::KNMessageBoxConfigure(QObject *parent) :
    QObject(parent)
{
    m_titleFont.setFamily("sao");
    m_titleFont.setPixelSize(15);

    m_defaultTitle="Information";
}

QString KNMessageBoxConfigure::defaultTitle() const
{
    return m_defaultTitle;
}

void KNMessageBoxConfigure::setDefaultTitle(const QString &defaultTitle)
{
    m_defaultTitle = defaultTitle;
}

QFont KNMessageBoxConfigure::titleFont() const
{
    return m_titleFont;
}

void KNMessageBoxConfigure::setTitleFont(const QFont &titleFont)
{
    m_titleFont = titleFont;
}
