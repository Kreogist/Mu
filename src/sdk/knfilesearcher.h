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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNFILESEARCHER_H
#define KNFILESEARCHER_H

#include <QFileInfo>
#include <QStringList>

#include <QObject>

/*!
 * \brief The KNFileSearcher class provide a simple file searcher. It can find
 * out specific valid suffix file from the file. You can set the suffix list via
 * setSuffixList() function.
 */
class KNFileSearcher : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNFileSearcher class.
     * \param parent The parent object pointer.
     */
    explicit KNFileSearcher(QObject *parent = 0);

    /*!
     * \brief Get the suffix filter list.
     * \return The general suffix filter list.
     */
    static QStringList suffixList();

signals:
    /*!
     * \brief This signal is used privately. It's only used for avoid a deep
     * function recursive.
     */
    void requireAnalysisNext();

    /*!
     * \brief When the searcher find out there's a file meets the requirements
     * of the suffix list, this will be emitted.
     * \param fileInfo The information about the file.
     */
    void findFile(QFileInfo fileInfo);

public slots:
    /*!
     * \brief Set the suffix filter list.
     * \param suffixList All the valid suffix. It should all be lower case.
     */
    static void setSuffixList(const QStringList &suffixList);

    /*!
     * \brief Analysis several paths. You can add both folder and file path. The
     * file path will be analysis straightly. The folder will find all the file
     * inside recursive.
     * \param paths The path list.
     */
    void analysisPaths(const QStringList &paths);

private slots:
    void analysisNext();
    void analysisFolder(const QFileInfo &folderInfo);

private:
    inline void analysisFile(const QFileInfo &fileInfo);
    static QStringList m_suffixList;
    QStringList m_queue;
};

#endif // KNFILESEARCHER_H
