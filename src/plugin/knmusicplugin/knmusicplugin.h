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

#ifndef KNMUSICPLUGIN_H
#define KNMUSICPLUGIN_H

#include "knabstractmusicplugin.h"

class QBoxLayout;
class KNMouseDetectHeader;
class KNHWidgetSwitcher;
class KNCategoryTabBar;
class KNMusicDetailDialogPanel;
/*!
 * \brief The KNMusicCategoryPlugin class is the official music category plugin.
 * You can treat this as a example.\n
 * It will use the all the official sdk class.
 */
class KNMusicPlugin : public KNAbstractMusicPlugin
{
    Q_OBJECT
public:
    /*!
     * \brief Construct the KNMusicCategoryPlugin class.
     * \param parent The parent widget of the plugin.
     */
    explicit KNMusicPlugin(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from KNAbstractMusicCategoryPlugin::headerWidget().
     */
    QWidget *headerWidget();

    /*!
     * \brief Reimplemented from KNAbstractMusicCategoryPlugin::icon().
     */
    QPixmap icon();

    /*!
     * \brief Reimplemented from KNAbstractMusicCategoryPlugin::title().
     */
    QString title();

signals:

public slots:
    /*!
     * \brief Reimplemented from KNAbstractMusicCategoryPlugin::saveConfigure().
     */
    void saveConfigure();

    /*!
     * \brief Reimplemented from
     * KNAbstractMusicCategoryPlugin::onArgumentsAvailable().
     */
    void onArgumentsAvailable(const QStringList &data);

private:
    inline void initialInfrastructure();
    void initialDetailDialogPanel();
    void initialParserPlugin();

    //UI infrastructure elements.
    KNMouseDetectHeader *m_headerWidget;
    QBoxLayout *m_headerLeftLayout, *m_headerRightLayout;
    KNCategoryTabBar *m_tabBar;
    KNHWidgetSwitcher *m_switcher;
};

#endif // KNMUSICPLUGIN_H
