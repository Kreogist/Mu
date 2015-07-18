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

#include <QObject>

/*!
 * \def knGlobal
 * A global pointer referring to the unique music global object.
 */
#define knMusicGlobal (KNMusicGlobal::instance())

class KNMusicDetailDialog;
class KNMusicParser;
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
     * \brief Move the parser and searcher to their own working threads. All the
     * working threads shouhld be here.
     */
    void startThreads();

signals:

public slots:

private slots:
    void retranslate();

private:
    static KNMusicGlobal *m_instance;
    explicit KNMusicGlobal(QObject *parent = 0);
    inline void initialFileType();
    inline void initialGenre();

    QWidget *m_parentWidget;
    KNMusicDetailDialog *m_detailDialog;
    KNMusicParser *m_parser;

    QThread *m_searcherThread, *m_analysisThread;

    QStringList m_suffixs, m_listSuffixs, m_suffixDescription,
                m_listSuffixDescription, m_indexedGenres;
};

#endif // KNMUSICGLOBAL_H
