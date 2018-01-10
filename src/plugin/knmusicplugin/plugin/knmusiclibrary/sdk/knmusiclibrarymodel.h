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

/*
 * Code name:
 * 4.1 - BNR34.
 * 4.0 - BNR32.
 * Previous version code name (KNJsonDatabase):
 * 3.0 - Trueno.
 * 2.0 - Cheater.
 * 1.0 - Beast.
 */

class KNMusicSearcher;
class KNMusicCategoryModelBase;
class KNMusicAnalysisQueue;
class KNMusicLibraryImageManager;
class KNMusicLibraryDirMonitor;
/*!
 * \brief The KNMusicLibraryModel class is the standard library model. It can
 * holds a image manager to read cached album art from the library folder, and
 * write new album art cache to the image folder.
 */
class KNMusicLibraryModel : public KNMusicModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicLibraryModel object.
     * \param parent The parent object.
     */
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

    /*!
     * \brief Reimplemented from KNMusicModel::artwork().
     */
    QPixmap artwork(int row) Q_DECL_OVERRIDE;

    /*!
     * \brief Get the artwork via providing the hash key.
     * \param hashKey The hash key of the image.
     * \return The image from the image manager. If there's no hash key in the
     * image manager, it will return the no album art image.
     */
    QPixmap artwork(const QString &hashKey);

    /*!
     * \brief Add a category model to the library model. When the music model is
     * modified, it will call the specific function of the category model to
     * apply the modification.
     * \param model The model pointer.
     */
    void installCategoryModel(KNMusicCategoryModelBase *model);

    /*!
     * \brief Get the album art hash map pointer.
     * \return The album art hash map pointer.
     */
    QHash<QString, QVariant> *hashAlbumArt();

    /*!
     * \brief Get the image manager, the image manager could provide the
     * original quality image which is read from hard disk.
     * \return The image manager pointer.
     */
    KNMusicLibraryImageManager *imageManager() const;

    /*!
     * \brief Reimplemented from KNMusicModel::isWorking().
     */
    bool isWorking() Q_DECL_OVERRIDE;

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

    /*!
     * \brief When the monitor directories have been updated, this signal would
     * be emitted.
     * \param directories The directory list.
     */
    void requireUpdateMonitorDirs(QStringList directories);

    /*!
     * \brief When the model finished recover, this signal should be emitted to
     * ask monitor to check the entire model.
     * \param monitorCheckList The monitoring item to its row map.
     */
    void requireMonitorCheck(QHash<QString, int> monitorCheckMap);

public slots:
    /*!
     * \brief Set the database file path of the library model.
     * \param databasePath The database file path.
     */
    void setDatabase(const QString &databasePath);

    /*!
     * \brief Set the library folder path, it will set the album art cache
     * folder to the image manager.
     * \param libraryPath The library path.
     */
    void setLibraryPath(const QString &libraryPath);

    /*!
     * \brief Set the library configure to the library.
     * \param configure The configure object.
     */
    void setLibraryConfigure(KNConfigure *configure);

    /*!
     * \brief Set the system configuration to the library. This configure
     * contains the settings related to the locale system.
     * \param configure The system configure object.
     */
    void setLibrarySystemConfigure(KNConfigure *configure);

    /*!
     * \brief Recover the library model from the json database.
     */
    void recoverModel();

    /*!
     * \brief Synchronize the library model by adding and removing the list.
     * \param addList The files need to be added to the model.
     * \param addListDirHash The list of the monitor directory hash for each
     * file path.
     * \param removeList The index of needed to be removed.
     */
    void syncModel(const QStringList &addList,
                   const QList<uint> &addListDirHash,
                   const QList<int> &removeList);

private slots:
    void onAnalysisComplete(const KNMusicAnalysisItem &analysisItem);
    void onImageUpdateRow(int row,
                          const KNMusicDetailInfo &detailInfo);
    void onImageRecoverComplete();
    void onConfigureUpdate();
    void onSystemConfigureUpdate();
    void onMonitorDirUpdated(const QList<uint> &removedHash);

private:
    inline void applyConfigureUpdate();
    inline void monitorCheckEntireLibrary();
    inline void addCategoryDetailInfo(const KNMusicDetailInfo &detailInfo);
    inline bool updateModelRow(int row,
                               const KNMusicAnalysisItem &analysisItem);
    inline void updateCategoryDetailInfo(const KNMusicDetailInfo &before,
                                         const KNMusicDetailInfo &after);
    inline void removeCategoryDetailInfo(const KNMusicDetailInfo &detailInfo);
    inline void reduceHashImage(const QString &imageKey);
    inline void count(int counts=1);
    inline void writeDatabase();
    QStringList m_monitorDirs;
    QLinkedList<KNMusicCategoryModelBase *> m_categoryModels;
    QHash<QString, QVariant> m_scaledHashAlbumArt;
    QHash<QString, int> m_hashAlbumArtCounter;
    QThread m_searchThread, m_analysisThread, m_imageThread, m_dirMonitorThread;
    QString m_databasePath;
    int m_operateCounter;
    KNMusicSearcher *m_searcher;
    KNMusicAnalysisQueue *m_analysisQueue;
    KNMusicLibraryImageManager *m_imageManager;
    KNMusicLibraryDirMonitor *m_dirMonitor;
    KNConfigure *m_configure, *m_systemConfigure, *m_monitorSystemConfigure;
    bool m_databaseLoaded, m_ignoreCueData;
};

#endif // KNMUSICLIBRARYMODEL_H
