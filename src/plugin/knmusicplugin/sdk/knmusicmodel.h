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

#ifndef KNMUSICMODEL_H
#define KNMUSICMODEL_H

#include <QList>

#include "knmusicglobal.h"
#include <QStandardItemModel>

#include <QAbstractTableModel>

using namespace MusicUtil;

/*!
 * \brief The KNMusicModel class is the basic model of all the other models
 * which will be used in the music plugin. This provides a basic foundation for
 * the playlist, music library and current playing. It's has fixed column size.
 * You should display a KNMusicModel's implement with an implement of
 * KNMusicTreeViewBase.
 */
class KNMusicModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicModel class with given parent.
     * \param parent The parent object.
     */
    explicit KNMusicModel(QObject *parent = 0);

    /*!
     * \brief Append a music to the end of the model.
     * \param detailInfo The KNMusicDetailInfo class of the music.
     */
    void appendRow(const KNMusicDetailInfo &detailInfo);

    /*!
     * \brief Append several musics to the end of the model.
     * \param detailInfos The KNMusicDetailInfo list of the music.
     */
    void appendRows(const QList<KNMusicDetailInfo> &detailInfos);

    /*!
     * \brief Insert a music to the specific position of the model.
     * \param row The position of the music.
     * \param detailInfo The KNMusicDetailInfo class of the music.
     * \return If insert the music successful, it will be true.
     */
    bool insertRow(int row, const KNMusicDetailInfo &detailInfo);

    /*!
     * \brief Update one music row into a new one. Notice that this will update
     * everything but except the following things: Rating, AlbumRating, Date
     * Added, Plays.
     * \param row The specific row index of the model.
     * \param detailInfo The new detail info which is used to replace the old
     * one.
     * \return If the row update successfully, it will be true.
     */
    bool updateRow(int row, KNMusicDetailInfo detailInfo);

    /*!
     * \brief Replace one music row into a new one. Different from update row,
     * this will simply replace the data.
     * \param row The specific row index of the model.
     * \param detailInfo The new detail info which is used to replace the old
     * one.
     * \return If the row replace successfully, it will be true.
     */
    bool replaceRow(int row, const KNMusicDetailInfo &detailInfo);

    /*!
     * \brief Remove several rows of music from the model.
     * \param position The position of the start row which the row will be
     * removed.
     * \param rows The row count will be removed from the position.
     * \param index The parent index. Leave it to QModelIndex().
     * \return If remove those rows successful, it will be true.
     */
    bool removeRows(int position,
                    int rows,
                    const QModelIndex &index = QModelIndex()) Q_DECL_OVERRIDE;

    /*!
     * \brief Remove all the music rows in the model.
     */
    void clear();

    /*!
     * \brief Reimplement from QAbstractTableModel::rowCount().
     */
    int rowCount(const QModelIndex &parent=QModelIndex()) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplement from QAbstractTableModel::columnCount().
     */
    int columnCount(const QModelIndex &parent=QModelIndex()) const
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplement from QAbstractTableModel::headerData().
     */
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplement from QAbstractTableModel::flags().
     */
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplement from QAbstractTableModel::data().
     */
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplement from QAbstractTableModel::setData().
     */
    bool setData(const QModelIndex &index,
                 const QVariant &value,
                 int role) Q_DECL_OVERRIDE;

    /*!
     * \brief Get the sum of all the duration inside the model.
     * \return The sum duration of the songs in quint64.
     */
    quint64 totalDuration() const;

    /*!
     * \brief Increase the playing times of one music row.
     * \param index Any index of the row.
     */
    void addPlayingTimes(const QPersistentModelIndex &index);

    QPersistentModelIndex playingIndex() const;

    KNMusicDetailInfo rowDetailInfo(const int &row);

signals:
    /*!
     * \brief When the row count is changed, this signal will be emitted.
     */
    void rowCountChanged();

public slots:
    void setPlayingIndex(const QPersistentModelIndex &playingIndex);

private:
    QList<KNMusicDetailInfo> m_detailInfos;
    quint64 m_totalDuration;
    QPersistentModelIndex m_playingIndex;
    QIcon m_playingIcon, m_cannotPlayIcon;
};

#endif // KNMUSICMODEL_H
