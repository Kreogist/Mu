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

signals:
    /*!
     * \brief Require to download specific file, this signal is used internal.
     * \param url The file url.
     * \param targetFolder The file saving directory path.
     * \param rename The new name of the file.
     */
    void requireDownloadFile(const QString &url, const QString &targetFolder,
                             const QString &rename);

public slots:

private:
    struct DownloadItemMetadata
    {
        QString songTitle;
        quint64 totalSize;
        quint64 downSize;
        qreal totalMegaSize;
        qreal downMegaSize;
        bool isDownloading;
        DownloadItemMetadata() :
            totalSize(0),
            downSize(0),
            totalMegaSize(0.0),
            downMegaSize(0.0),
            isDownloading(false)
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

    QList<DownloadItemMetadata> m_downloadItemList;
    QThread m_downloaderThread;
    KNFileDownloadManager *m_downloader;
};

#endif // KNMUSICSTOREDOWNLOADMANAGER_H
