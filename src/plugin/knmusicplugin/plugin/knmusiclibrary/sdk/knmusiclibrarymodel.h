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

#include "knmusicmodel.h"

class KNJsonDatabase;
class KNMusicSearcher;
class KNMusicAnalysisQueue;
class KNMusicLibraryModel : public KNMusicModel
{
    Q_OBJECT
public:
    explicit KNMusicLibraryModel(QThread *workingThread, QObject *parent = 0);
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
    bool updateRow(int row, KNMusicDetailInfo detailInfo) Q_DECL_OVERRIDE;

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

    bool setData(const QModelIndex &index,
                 const QVariant &value,
                 int role);


    KNJsonDatabase *database() const;

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

public slots:
    /*!
     * \brief Set the database of the library model.
     * \param database The database model pointer.
     */
    void setDatabase(KNJsonDatabase *database);

    /*!
     * \brief Recover the library model from the json database.
     */
    void recoverModel();

private slots:
    void onActionAnalysisComplete(const KNMusicAnalysisItem &analysisItem);

private:
    inline KNMusicDetailInfo generateDetailInfo(const QJsonArray &dataArray);
    inline QJsonArray generateDataArray(const KNMusicDetailInfo &detailInfo);
    KNJsonDatabase *m_database;
    KNMusicSearcher *m_searcher;
    KNMusicAnalysisQueue *m_analysisQueue;
};

#endif // KNMUSICLIBRARYMODEL_H
