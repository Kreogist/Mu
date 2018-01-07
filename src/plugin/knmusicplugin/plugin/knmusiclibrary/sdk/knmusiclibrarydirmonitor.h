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

#ifndef KNMUSICLIBRARYDIRMONITOR_H
#define KNMUSICLIBRARYDIRMONITOR_H

#include <QHash>

#include <QObject>

class KNConfigure;
/*!
 * \brief The KNMusicLibraryDirMonitor class is designed to monitor the file
 * changes from the file.
 */
class KNMusicLibraryDirMonitor : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicLibraryDirMonitor object.
     * \param parent The parent object.
     */
    explicit KNMusicLibraryDirMonitor(QObject *parent = 0);

signals:
    /*!
     * \brief When the monitor is asked to sync files with model, this signal
     * will be emitted.
     * \param addPaths The list of file path which is needed to be added.
     * \param addPathDirHash The list of the monitor directory hash for each
     * file path.
     * \param removedItems The indexes of the removed item.
     */
    void requireSync(QStringList addPaths,
                     QList<uint> addPathDirHash,
                     QList<int> removedItems);

public slots:
    /*!
     * \brief Check the entire library model.
     * \param watchFileList The monitoring item path to its raw index list.
     */
    void checkEntireLibrary(QHash<QString, int> watchFileList);

    /*!
     * \brief Set the monitor directories.
     * \param directories The monitor dir list.
     */
    void setMonitorDirs(const QStringList &directories);

private:
    struct MonitorDirectory
    {
        uint hash;
    };

    QHash<QString, MonitorDirectory> m_monitorMap;
};

#endif // KNMUSICLIBRARYDIRMONITOR_H
