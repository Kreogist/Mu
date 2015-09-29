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
#include <QUrl>

#include "knmusicglobal.h"
#include <QStandardItemModel>

#include <QAbstractTableModel>

using namespace MusicUtil;

#define ModelMimeType "application/org.kreogist.mu.musicmodel"
#define ModelType "application/org.kreogist.mu.musicmodeltype"
#define ModelRowData "application/org.kreogist.mu.musicmodelrowdata"
#define ModelRowList "application/org.kreogist.mu.musicmodelrows"

/*!
 * \brief The KNMusicModel class is the basic model of all the other models
 * which will be used in the music plugin. This provides a basic foundation for
 * the playlist, music library and current playing. It's has fixed column size.
 * You should display a KNMusicModel's implement with an implementation of
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
    virtual void appendRow(const KNMusicDetailInfo &detailInfo);

    /*!
     * \brief Append several musics to the end of the model.
     * \param detailInfos The KNMusicDetailInfo list of the music.
     */
    virtual void appendRows(const QList<KNMusicDetailInfo> &detailInfos);

    /*!
     * \brief Insert a music to the specific position of the model.
     * \param row The position of the music.
     * \param detailInfo The KNMusicDetailInfo class of the music.
     * \return If insert the music successful, it will be true.
     */
    virtual bool insertRow(int row, const KNMusicDetailInfo &detailInfo);

    /*!
     * \brief Insert several music detail infos to the specific position of the
     * model.
     * \param row The position of the music.
     * \param detailInfos The KNMusicDetailInfo list of the music.
     * \return If insert the music list successful, it will be true.
     */
    virtual bool insertMusicRows(int row,
                                 const QList<KNMusicDetailInfo> &detailInfos);

    /*!
     * \brief Update one music row into a new one. Notice that this will update
     * everything but except the following things: Rating, AlbumRating, Date
     * Added, Plays.
     * \param row The specific row index of the model.
     * \param detailInfo The new detail info which is used to replace the old
     * one.
     * \return If the row update successfully, it will be true.
     */
    virtual bool updateRow(int row, KNMusicAnalysisItem analysisItem);

    /*!
     * \brief Replace one music row into a new one. Different from update row,
     * this will simply replace the data.
     * \param row The specific row index of the model.
     * \param detailInfo The new detail info which is used to replace the old
     * one.
     * \return If the row replace successfully, it will be true.
     */
    virtual bool replaceRow(int row, const KNMusicDetailInfo &detailInfo);

    /*!
     * \brief Remove several rows of music from the model.
     * \param position The position of the start row which the row will be
     * removed.
     * \param rows The row count will be removed from the position.
     * \param index The parent index. Leave it to QModelIndex().
     * \return If remove those rows successfully, it will be true.
     */
    bool removeRows(int position,
                    int rows,
                    const QModelIndex &index = QModelIndex()) Q_DECL_OVERRIDE;

    /*!
     * \brief Remove several rows from the model at once.
     * \param rows The rows list.
     */
    void removeRowList(QList<int> rows);

    /*!
     * \brief Remove all the music rows in the model.
     */
    virtual void clear();

    /*!
     * \brief Reimplemented from QAbstractTableModel::rowCount().
     */
    int rowCount(const QModelIndex &parent=QModelIndex()) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractTableModel::columnCount().
     */
    int columnCount(const QModelIndex &parent=QModelIndex()) const
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractTableModel::headerData().
     */
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractTableModel::flags().
     */
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractTableModel::canDropMimeData().
     */
    bool canDropMimeData(const QMimeData *data,
                         Qt::DropAction action,
                         int row,
                         int column,
                         const QModelIndex &parent) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractTableModel::data().
     */
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractTableModel::mimeData().
     */
    QMimeData *mimeData(const QModelIndexList &indexes) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractTableModel::setData().
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
     * \param playedRow Any valid index of the row.
     */
    void addPlayingTimes(const QPersistentModelIndex &playedRow);

    /*!
     * \brief Get the current playing index.
     * \return The playing index.
     */
    QPersistentModelIndex playingIndex() const;

    /*!
     * \brief Get row music detail info.
     * \param row The row index.
     * \return The detail info structure of the song.
     */
    KNMusicDetailInfo rowDetailInfo(const int &row);

    /*!
     * \brief Get the row of one detail info, it will return the first row where
     * the detail info appears.
     * \param detailInfo The detail info.
     * \return The row of the detail info first appear. If we cannot find the
     * detail info, it will return -1.
     */
    int detailInfoRow(const KNMusicDetailInfo &detailInfo);

    /*!
     * \brief Reimplemented from QAbstractTableModel::supportedDropActions().
     */
    Qt::DropActions supportedDropActions() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractTableModel::dropMimeData().
     */
    bool dropMimeData(const QMimeData *data,
                      Qt::DropAction action,
                      int row,
                      int column,
                      const QModelIndex &parent) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractTableModel::mimeTypes().
     */
    QStringList mimeTypes() const Q_DECL_OVERRIDE;

    /*!
     * \brief Get a property information of the row.
     * \param i The row index.
     * \param role The role of the property.
     * \return The property value.
     */
    QVariant rowProperty(const int &i, const int &role) const
    {
        return data(index(i, 0), role);
    }

    /*!
     * \brief Get the text data of at the specific row and column.
     * \param row The specific row.
     * \param column The specific column.
     * \return The QString transfer data from the data(index(row, column)).
     */
    QString textData(const int &row, const int &column) const;

    /*!
     * \brief Set the text data of a specific row and column.
     * \param row The specific row.
     * \param column The specific column.
     * \param textData The QString data you want to set the index(row, column).
     * \return If we can set data successfully, return true.
     */
    bool setTextData(const int &row,
                     const int &column,
                     const QString &textData)
    {
        return setData(index(row, column), textData, Qt::DisplayRole);
    }

    /*!
     * \brief Reimplemented from QAbstractTableModel::moveRows().
     */
    bool moveRows(const QModelIndex &sourceParent,
                  int sourceRow,
                  int count,
                  const QModelIndex &destinationParent,
                  int destinationChild);

signals:
    /*!
     * \brief When the playing item is begin removed, this signal will be
     * emitted.
     */
    void playingItemRemoved();

    /*!
     * \brief When the row count is changed, this signal will be emitted.
     */
    void rowCountChanged();

    /*!
     * \brief When there's new file is trying to add to the music model, this
     * signal will be emitted.
     * \param fileList The file path list.
     */
    void requireAnalysisFiles(QStringList fileList);

public slots:
    /*!
     * \brief Set the playing index. It will display a playing index mark on it.
     * \param playingIndex
     */
    void setPlayingIndex(const QModelIndex &playingIndex);

    /*!
     * \brief Append several files to the music model.
     * \param filePaths The file path list.
     */
    void appendFiles(const QStringList &filePaths);

    /*!
     * \brief Append several urls to the music model.
     * \param urls The url list.
     */
    void appendUrls(const QList<QUrl> &urls);

protected:
    /*!
     * \brief This will append a KNMusicDetailInfo structure in the model data
     * list without the beginInsertRows funtion. The performance will be the
     * best. When you called this funciton, you have to handle it manually.
     * \param detailInfo The detail info data.
     */
    void appendDetailInfo(const KNMusicDetailInfo &detailInfo);

    /*!
     * \brief Initial the total duration to a specific number manually.
     * \param totalDuration The total duration number.
     */
    void initialTotalDuration(const quint64 &totalDuration);

private:
    QList<KNMusicDetailInfo> m_detailInfos;
    quint64 m_totalDuration;
    QPersistentModelIndex m_playingIndex;
    QIcon m_playingIcon, m_cannotPlayIcon;
    static QVariant m_alignLeft, m_alignCenter, m_alignRight;

    static QStringList m_dropMimeTypes;
};

#endif // KNMUSICMODEL_H
