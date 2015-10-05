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

#ifndef KNMUSICLIBRARYMODEL_H
#define KNMUSICLIBRARYMODEL_H

#include <QLinkedList>
#include <QThread>
#include <QHash>

#include "knmusicmodel.h"

class KNJsonDatabase;
class KNMusicSearcher;
class KNMusicCategoryModelBase;
class KNMusicAnalysisQueue;
class KNMusicLibraryImageManager;
class KNMusicLibraryImageSaver;
class KNMusicLibraryModel : public KNMusicModel
{
    Q_OBJECT
public:
    explicit KNMusicLibraryModel(QObject *parent = 0);
    ~KNMusicLibraryModel();

    /*!
     * \brief Reimplemented from KNMusicModel::appendRow().
     */
    void appendRow(const KNMusicDetailInfo &detailInfo) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicModel::appendRows().
     */
    void appendRows(const QList<KNMusicDetailInfo> &detailInfos)
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicModel::insertRow().
     */
    bool insertRow(int row, const KNMusicDetailInfo &detailInfo)
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicModel::insertMusicRows().
     */
    bool insertMusicRows(int row,
                         const QList<KNMusicDetailInfo> &detailInfos)
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicModel::updateRow().
     */
    bool updateRow(int row, KNMusicAnalysisItem analysisItem) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicModel::replaceRow().
     */
    bool replaceRow(int row, const KNMusicDetailInfo &detailInfo)
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicModel::removeRows().
     */
    bool removeRows(int position,
                    int rows,
                    const QModelIndex &index = QModelIndex()) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicModel::clear().
     */
    void clear() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicModel::setData().
     */
    bool setData(const QModelIndex &index,
                 const QVariant &value,
                 int role) Q_DECL_OVERRIDE;

    QPixmap artwork(const int &row);

    QPixmap artwork(const QString &hashKey);

    KNJsonDatabase *database() const;

    void installCategoryModel(KNMusicCategoryModelBase *model);

signals:
    /*!
     * \brief When the library model has the first record, this signal will be
     * emitted.
     */
    void libraryNotEmpty();

    /*!
     * \brief When the library is empty, this signal will be emitted.
     */
    void libraryEmpty();

    /*!
     * \brief When the library has already recover the data, it will emit this
     * signal to recover image from the album art folder. You won't need to use
     * this signal to do anything.
     * \param imageHashList The hash list which is cover
     */
    void requireRecoverImage(QStringList imageHashList);

public slots:
    /*!
     * \brief Set the database of the library model.
     * \param database The database model pointer.
     */
    void setDatabase(KNJsonDatabase *database);

    void setLibraryPath(const QString &libraryPath);

    /*!
     * \brief Recover the library model from the json database.
     */
    void recoverModel();

private slots:
    void onActionAnalysisComplete(const KNMusicAnalysisItem &analysisItem);
    void onActionImageUpdateRow(const int &row,
                                const KNMusicDetailInfo &detailInfo);
    void onActionImageRecoverComplete();

private:
    inline void addCategoryDetailInfo(const KNMusicDetailInfo &detailInfo);
    inline bool updateModelRow(int row,
                               const KNMusicAnalysisItem &analysisItem);
    inline void updateCategoryDetailInfo(const KNMusicDetailInfo &before,
                                         const KNMusicDetailInfo &after);
    inline void removeCategoryDetailInfo(const KNMusicDetailInfo &detailInfo);
    inline void reduceHashImage(const QString &imageKey);
    inline KNMusicDetailInfo generateDetailInfo(const QJsonArray &dataArray);
    inline QJsonArray generateDataArray(const KNMusicDetailInfo &detailInfo);
    QLinkedList<KNMusicCategoryModelBase *> m_categoryModels;
    QHash<QString, QVariant> m_hashAlbumArt;
    QHash<QString, int> m_hashAlbumArtCounter;
    QThread m_searchThread, m_analysisThread, m_imageThread;
    KNJsonDatabase *m_database;
    KNMusicSearcher *m_searcher;
    KNMusicAnalysisQueue *m_analysisQueue;
    KNMusicLibraryImageManager *m_imageManager;
    KNMusicLibraryImageSaver *m_imageSaver;
};

#endif // KNMUSICLIBRARYMODEL_H
