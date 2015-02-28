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
#ifndef KNPREFERENCE_H
#define KNPREFERENCE_H

#include <QLinkedList>

#include "knpreferenceplugin.h"

class KNPreferencePanel;
class KNPreference : public KNPreferencePlugin
{
    Q_OBJECT
public:
    explicit KNPreference(QObject *parent = 0);
    QWidget *preferencePanel();

signals:

public slots:
    int addCategory(KNCategoryPlugin *plugin);
    void setCategoryText(const int &index, const QString &title);
    void setCurrentIndex(const int &index);
    void loadConfigure();

private slots:
    void onActionHidePreference();

private:
    KNPreferencePanel *m_preferencePanel;
};

#endif // KNPREFERENCE_H
