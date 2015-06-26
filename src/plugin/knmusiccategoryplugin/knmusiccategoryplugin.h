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

#ifndef KNMUSICCATEGORYPLUGIN_H
#define KNMUSICCATEGORYPLUGIN_H

#include "knabstractmusiccategoryplugin.h"

/*!
 * \brief The KNMusicCategoryPlugin class is the official music category plugin.
 * You can treat this as a example.\n
 * It will use the all the official sdk class.
 */
class KNMusicCategoryPlugin : public KNAbstractMusicCategoryPlugin
{
    Q_OBJECT
public:
    /*!
     * \brief Construct the KNMusicCategoryPlugin class.
     * \param parent
     */
    explicit KNMusicCategoryPlugin(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from KNAbstractMusicCategoryPlugin::headerWidget().
     */
    QWidget *headerWidget();

    QPixmap icon();

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
    QWidget *m_headerWidget;
};

#endif // KNMUSICCATEGORYPLUGIN_H
