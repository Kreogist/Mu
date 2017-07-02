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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNPREFERENCEPANELDATA_H
#define KNPREFERENCEPANELDATA_H

#include "knpreferenceutil.h"

#include <QObject>

using namespace PreferenceUtil;

/*!
 * \brief The KNPreferencePanelData class is a data storage class which would
 * generate the data of the panel list.\n
 * Although we could let this class to be a non-QObject class, however the tr
 * function is easy to use and let all the string to be a part of the
 * translation file, so that we will use the QObject.
 */
class KNPreferencePanelData : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNPreferencePanelData object.
     * \param parent The parent object.
     */
    explicit KNPreferencePanelData(QObject *parent = 0);

    /*!
     * \brief Get the panel option block data of one specific panel.
     * \param panelIndex The panel index data.
     * \return The panel option block.
     */
    QList<PreferencePanelBlock> getPanelData(int panelIndex);

private:
    inline PreferencePanelBlock generateBlock(const QString &title);
    inline void addItem(PreferencePanelBlock &block,
                        const QString &title,
                        const QString &path,
                        const QVariant &defaultValue,
                        PreferencePanelOptionType type,
                        const QString &explain,
                        bool isAdvanced);
    inline void addIntItem(PreferencePanelBlock &block,
                           const QString &title,
                           const QString &path,
                           const QVariant &defaultValue,
                           const QString &explain,
                           int min, int max, bool isAdvanced);
    inline void setItemData(PreferencePanelOption &option,
                            const QString &title,
                            const QString &path,
                            const QVariant &defaultValue,
                            const QString &explain,
                            PreferencePanelOptionType type,
                            bool isAdvanced);
};

#endif // KNPREFERENCEPANELDATA_H
