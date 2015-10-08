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

#ifndef KNMUSICPLAYLISTMODEL_H
#define KNMUSICPLAYLISTMODEL_H

#include <QJsonArray>

#include "knmusicmodel.h"

class KNMusicSearcher;
class KNMusicAnalysisQueue;
/*!
 * \brief The KNMusicPlaylistModel class is used to describe a playlist. It
 * contains title and file path. It can be built from a json array. You can set
 * the json array and called the buildModel() function to build a playlist.
 */
class KNMusicPlaylistModel : public KNMusicModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicPlaylistModel class.
     * \param workingThread The working thread for the searcher and the analysis
     * cache.
     * \param parent The parent object.
     */
    explicit KNMusicPlaylistModel(QThread *workingThread, QObject *parent = 0);
    ~KNMusicPlaylistModel();

    /*!
     * \brief Get the title of the playlist.
     * \return The title string.
     */
    QString title() const;

    /*!
     * \brief Set the title of the playlist model.
     * \param title The new title of the playlist.
     */
    void setTitle(const QString &title);

    /*!
     * \brief Get the file path of the playlist. If the path is not set before,
     * it's QString().
     * \return The file path of the playlist.
     */
    QString filePath() const;

    /*!
     * \brief Set the file path of the playlist.
     * \param filePath The file path of the playlist.
     */
    void setFilePath(const QString &filePath);

    /*!
     * \brief Allcate a useable file path in the playlist diretory.
     */
    void allcateFilePath()
    {
        //Set the file path, allocate a new file path.
        setFilePath(generateFilePath());
    }

    /*!
     * \brief Get whether this playlist has been built or it's still data in
     * json array.
     * \return If there's no content data in the json array, and the data has
     * all loaded to the model, it will return true.
     */
    bool isBuilt() const;

    /*!
     * \brief Build model from the content data first.
     */
    void buildModel();

    /*!
     * \brief Get the content data array.
     * \return The json array which storage the data of the whole playlist.
     */
    QJsonArray contentData() const;

    /*!
     * \brief Set the content data of the whole playlist model.
     * \param contentData The content data of the model.
     */
    void setContentData(const QJsonArray &contentData);

    /*!
     * \brief Get the changed state of the model.
     * \return If the model has been changed from the last saving, this will be
     * true.
     */
    bool changed() const;

    /*!
     * \brief Set the changed state manually.
     * \param changed The changed state you prefer.
     */
    void setChanged(bool changed);

    /*!
     * \brief Reimplemented from KNMusicModel::data().
     */
    QVariant data(const QModelIndex &index, int role) const;

    /*!
     * \brief Set the playlist directory path for the whole playlist model. This
     * directory is used to generate the file path.
     * \param playlistDirPath The directory path.
     */
    static void setPlaylistDirPath(const QString &playlistDirPath);

    /*!
     * \brief Reimplemented from KNMusicModel::mimeData().
     */
    QMimeData *mimeData(const QModelIndexList &indexes) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicModel::dropMimeData().
     */
    bool dropMimeData(const QMimeData *data,
                      Qt::DropAction action,
                      int row,
                      int column,
                      const QModelIndex &parent) Q_DECL_OVERRIDE;

    /*!
     * \brief Get the playlist model detail info list.
     * \return The detail info list.
     */
    QList<KNMusicDetailInfo> playlistDetailInfos() const;

signals:
    /*!
     * \brief When the playlist title changed, this signal will be emitted.
     * \param title The new playlist title.
     */
    void titleChanged(const QString &title);

public slots:

private slots:
    void onActionModelChanged();
    void onActionAnalysisComplete(const KNMusicAnalysisItem &analysisItem);

private:
    static QString generateFilePath();
    static QString m_playlistDirPath;
    KNMusicSearcher *m_searcher;
    KNMusicAnalysisQueue *m_analysisQueue;
    QString m_title, m_filePath;
    QJsonArray m_contentData;
    bool m_built, m_changed;
};

#endif // KNMUSICPLAYLISTMODEL_H
