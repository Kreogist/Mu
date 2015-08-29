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

#ifndef KNMUSICGLOBAL_H
#define KNMUSICGLOBAL_H

#include <QStringList>
#include <QPixmap>

#include "knmusicutil.h"

#include <QObject>

using namespace MusicUtil;

/*!
 * \def knGlobal
 * A global pointer referring to the unique music global object.
 */
#define knMusicGlobal (KNMusicGlobal::instance())

class KNMusicDetailDialog;
class KNMusicParser;
class KNMusicSearchBase;
class KNConfigure;
class KNMusicBackend;
class KNMusicNowPlayingBase;
class KNMusicDetailTooltipBase;
class KNMusicSoloMenuBase;
class KNMusicMultiMenuBase;
/*!
 * \brief The KNMusicGlobal class provides some public instance and function of
 * the official music category plugin.\n
 * It will provide a KNMusicDetailDialog for music global detail dialog editing.
 */
class KNMusicGlobal : public QObject
{
    Q_OBJECT
public:
    ~KNMusicGlobal();

    /*!
     * \brief Get the singleton instance of class KNMusicGlobal.
     * \return The global instance of KNMusicGlobal.
     */
    static KNMusicGlobal *instance();

    /*!
     * \brief Initial the music global class.
     * \param parent The parent object of the music global class.
     */
    static void initial(QObject *parent = 0);

    /*!
     * \brief Get the indexed genre text. It is the same as the index in ID3v1
     * tags.
     * \param index The genre index.
     * \return The genre text. If index is out of range, it will be "Unknown".
     */
    QString indexedGenre(const int &index);

    /*!
     * \brief Get the genre index in the list. It is the same as the index in
     * ID3v1 tag defination.
     * \param genre The genre text.
     * \return The genre index. If the genre is not in the list, it will be -1.
     */
    int genreIndex(const QString &genre);

    /*!
     * \brief Get the music detail dialog pointer.
     * \return The dialog pointer.
     */
    KNMusicDetailDialog *detailDialog()
    {
        return m_detailDialog;
    }

    /*!
     * \brief Get the music global parser.
     * \return The parser pointer.
     */
    KNMusicParser *parser()
    {
        return m_parser;
    }

    /*!
     * \brief Get the type description of a specific suffix.
     * \param suffix The file suffix.
     * \return The description of the suffix.
     */
    QString typeDescription(const QString &suffix) const;

    /*!
     * \brief Get the header text of the specific column of tree view.
     * \param index The index of the column.
     * \return The text of the column title.
     */
    QString treeViewHeaderText(const int &index) const;

    /*!
     * \brief Move the parser and searcher to their own working threads. All the
     * working threads shouhld be here.
     */
    void startThreads();

    /*!
     * \brief Check whether a suffix is in the music file suffix list.
     * \param suffix The suffix text.
     * \return If the suffix is in the list, then true.
     */
    bool isMusicFile(const QString &suffix) const
    {
        return m_suffixs.contains(suffix);
    }

    /*!
     * \brief Check whether a suffix is in the music list file suffix list.
     * \param suffix The file suffix text.
     * \return If the suffix is in the list file suffix list, then true.
     */
    bool isMusicListFile(const QString &suffix) const
    {
        return m_listSuffixs.contains(suffix);
    }

    /*!
     * \brief Get the music library path.
     * \return The music library path.
     */
    QString musicLibPath() const;

    /*!
     * \brief Get the music search object.
     * \return The music search object. Before set this object it will be
     * nullptr.
     */
    KNMusicSearchBase *search() const;

    /*!
     * \brief Set the music search object pointer.
     * \param search The object pointer.
     */
    void setSearch(KNMusicSearchBase *search);

    /*!
     * \brief Get the music configure.
     * \return The music configure pointer.
     */
    KNConfigure *configure();

    /*!
     * \brief Get the now playing object class.
     * \return The now playing object pointer.
     */
    KNMusicNowPlayingBase *nowPlaying() const;

    /*!
     * \brief Set the now playing object pointer.
     * \param nowPlaying The object pointer.
     */
    void setNowPlaying(KNMusicNowPlayingBase *nowPlaying);

    /*!
     * \brief Get the solo menu object class.
     * \return The sole menu object pointer. It will be nullptr if you never set
     * it before.
     */
    KNMusicSoloMenuBase *soloMenu() const;

    /*!
     * \brief Set the solo menu pointer.
     * \param soloMenu The solo menu object pointer.
     */
    void setSoloMenu(KNMusicSoloMenuBase *soloMenu);

    /*!
     * \brief Get all the avaliable music file suffixs.
     * \return The suffix list.
     */
    QStringList suffixList() const;

    /*!
     * \brief Get all the avaliable music list file suffixs.
     * \return The suffix list.
     */
    QStringList listSuffixList() const;

    /*!
     * \brief Get the default album art image when there's no album art.
     * \return The no album art image.
     */
    QPixmap noAlbumArt() const;

    /*!
     * \brief Get the backend object class.
     * \return The music backend class pointer.
     */
    KNMusicBackend *backend() const;

    /*!
     * \brief Set the backend class object.
     * \param backend The backend class object pointer.
     */
    void setBackend(KNMusicBackend *backend);

    /*!
     * \brief Get the detail tooltip object class.
     * \return The music tree view detail tooltip widget pointer.
     */
    KNMusicDetailTooltipBase *detailTooltip() const;

    /*!
     * \brief Set the detail tooltip widget.
     * \param detailTooltip The tooltip widget object pointer.
     */
    void setDetailTooltip(KNMusicDetailTooltipBase *detailTooltip);

    /*!
     * \brief Get the multi menu object class.
     * \return The multi menu object pointer. It will be nullptr if you never
     * set it before.
     */
    KNMusicMultiMenuBase *multiMenu() const;

    /*!
     * \brief Set the multi item menu object pointer.
     * \param multiMenu The multi menu object pointer.
     */
    void setMultiMenu(KNMusicMultiMenuBase *multiMenu);

signals:

public slots:
    /*!
     * \brief Set the path of the music library folder.
     * \param musicLibPath The folder path.
     */
    void setMusicLibPath(const QString &musicLibPath);

private slots:
    void retranslate();

private:
    static KNMusicGlobal *m_instance;
    explicit KNMusicGlobal(QObject *parent = 0);
    KNMusicGlobal(const KNMusicGlobal &);
    inline void initialFileType();
    inline void initialGenre();

    QWidget *m_parentWidget;
    KNMusicDetailDialog *m_detailDialog;
    KNMusicParser *m_parser;
    KNMusicSoloMenuBase *m_soloMenu;
    KNMusicMultiMenuBase *m_multiMenu;
    KNMusicSearchBase *m_search;
    KNMusicBackend *m_backend;
    KNMusicNowPlayingBase *m_nowPlaying;
    KNMusicDetailTooltipBase *m_detailTooltip;

    QThread *m_searcherThread, *m_analysisThread;
    KNConfigure *m_musicConfigure;
    QString m_musicLibPath;

    QStringList m_suffixs, m_listSuffixs, m_suffixDescription,
                m_listSuffixDescription, m_indexedGenres;
    QString m_treeViewHeaderText[MusicDataCount];
    QPixmap m_noAlbumArt;
};

#endif // KNMUSICGLOBAL_H
