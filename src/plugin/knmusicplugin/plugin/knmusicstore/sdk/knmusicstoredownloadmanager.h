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

#ifndef KNMUSICSTOREDOWNLOADMANAGER_H
#define KNMUSICSTOREDOWNLOADMANAGER_H

#include <QThread>

#include <QAbstractTableModel>

class KNFileDownloadManager;
/*!
 * \brief The KNMusicStoreDownloadManager class provides a model for the music
 * store download items. The download manager will manage one download engine,
 * which is used to download all the items in the model.
 */
class KNMusicStoreDownloadManager : public QAbstractTableModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreDownloadManager object.
     * \param parent The parent object.
     */
    explicit KNMusicStoreDownloadManager(QObject *parent=0);
    ~KNMusicStoreDownloadManager();

    /*!
     * \brief Append one item in the download manager, start to download the
     * song.
     * \param url The url of the song.
     * \param directoryPath The target directory path.
     * \param fileName The target file name.
     * \param songTitle The title of the song, used as the display information.
     */
    void appendItem(const QString &url,
                    const QString &directoryPath,
                    const QString &fileName,
                    const QString &songTitle);

    /*!
     * \brief Reimplemented from QAbstractTableModel::data().
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractTableModel::rowCount().
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractTableModel::columnCount().
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const
    Q_DECL_OVERRIDE;

    /*!
     * \brief Check whether the item is paused or not. When the item is
     * downloading or the item is waiting, it means that the item is not paused.
     * \param row The specific row of the item.
     * \return The item is in the paused state or not.
     */
    bool isPaused(int row) const;

signals:
    /*!
     * \brief Require to download specific file, this signal is used internal.
     * \param url The file url.
     * \param targetFolder The file saving directory path.
     * \param rename The new name of the file.
     * \param fromStart Download the file from the first byte.
     */
    void requireDownloadFile(const QString &url, const QString &targetFolder,
                             const QString &rename, bool fromStart);

    /*!
     * \brief Require to pause the working backend.
     */
    void requirePause();

    /*!
     * \brief Require to abort the working mission.
     */
    void requireAbort();

    /*!
     * \brief When all the model is empty, or it is no more empty, this signal
     * will be emitted.
     */
    void modelEmptyStateChange(bool isEmpty);

public slots:
    /*!
     * \brief Start a single mission.
     * \param missionRow The mission row.
     */
    void startMission(int missionRow);

    /*!
     * \brief Pause a single mission.
     * \param missionRow The mission row.
     */
    void pauseMission(int missionRow);

    /*!
     * \brief Remove the missions in the list.
     * \param missionRows The selected missions. The list must be sorted from
     * the smaller number to the bigger number.
     */
    void removeMissions(QList<int> missionRows);

    /*!
     * \brief This slot is used to start all the mission in the queueing list.
     */
    void startAll();

    /*!
     * \brief This slot is used to pause all the mission in the queueing list.
     */
    void pauseAll();

private slots:
    void onDownloadProgress(const qint64 &bytesReceived,
                            const qint64 &bytesTotal);
    void onDownloadFinished();
    void onDownloadPaused(const qint64 &pausedSize);

private:
    struct DownloadItemMetadata
    {
        //Download mission info.
        QString directoryPath;
        QString fileName;
        //Download metadata.
        QString songTitle;
        quint64 totalSize;
        quint64 downSize;
        qreal totalMegaSize;
        qreal downMegaSize;
        int state;
        DownloadItemMetadata() :
            totalSize(0),
            downSize(0),
            totalMegaSize(0.0),
            downMegaSize(0.0),
            state(0)
        {
        }
    };

    enum DownloadItemColumns
    {
        ColumnIndex,
        ColumnProgress,
        ColumnFilename,
        DownloadItemColumnCount
    };
    inline bool checkModelEmpty();
    inline void startNextAvailableMission();
    inline void launchMission(int missionIndex);
    inline void updateItem(int row, const DownloadItemMetadata &updatedItem);
    QStringList m_downloadUrlList;
    QList<DownloadItemMetadata> m_downloadItemList;
    QThread m_downloaderThread;
    QPersistentModelIndex m_currentIndex;
    KNFileDownloadManager *m_downloader;
    bool m_isRunning;
};

#endif // KNMUSICSTOREDOWNLOADMANAGER_H
