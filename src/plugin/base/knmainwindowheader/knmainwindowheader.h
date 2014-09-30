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
#ifndef KNMAINWINDOWHEADER_H
#define KNMAINWINDOWHEADER_H

#include "knmainwindowheaderplugin.h"

class KNHeaderButton;
class KNHeaderContainer;
class KNHeaderSwitcher;
class KNMainWindowHeader : public KNMainWindowHeaderPlugin
{
    Q_OBJECT
public:
    explicit KNMainWindowHeader(QObject *parent = 0);
    QWidget *headerWidget();

signals:

public slots:
    void addHeaderWidget(QWidget *widget);
    void setHeaderText(const QString &text);
    void setHeaderIcon(const QPixmap &icon);
    void restoreHeaderButton();

private:
    KNHeaderContainer *m_headerContainer;
    KNHeaderSwitcher *m_headerSwitcher;
    KNHeaderButton *m_headerButton;
};

#endif // KNMAINWINDOWHEADER_H
