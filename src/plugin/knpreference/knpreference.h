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

#ifndef KNPREFERENCE_H
#define KNPREFERENCE_H

#include "knpreferenceplugin.h"

class KNPreferenceSidebar;
class KNVWidgetSwitcher;
class KNPreferenceAbout;
class KNPreferenceLanguagePanel;
/*!
 * \brief The KNPreference class is a official preference panel. It provides the
 * standard item panels.
 */
class KNPreference : public KNPreferencePlugin
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNPreference widget with the given parent widget.
     * \param parent The parent window widget.
     */
    explicit KNPreference(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from KNPreferencePlugin::addTab().
     */
    void addTab(KNPreferenceItem *tabWidget, QWidget *content) Q_DECL_OVERRIDE;

signals:

public slots:

private slots:
    void retranslate();
    void onActionIndexChange(const int &index);

private:
    inline void addPreferenceTab(KNPreferenceItem *tabWidget, QWidget *content);
    KNPreferenceSidebar *m_sidebar;
    KNVWidgetSwitcher *m_content;
    KNPreferenceItem *m_aboutItem;
    KNPreferenceAbout *m_about;
    KNPreferenceLanguagePanel *m_languagePanel;
};

#endif // KNPREFERENCE_H
