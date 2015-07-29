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

#ifndef KNMUSICSEARCHSYNTAXENGINE_H
#define KNMUSICSEARCHSYNTAXENGINE_H

#include <QHash>
#include <QRegularExpression>

#include "knmusicutil.h"

#include <QObject>

using namespace MusicUtil;

/*!
 * \brief The KNMusicSearchSyntaxEngine class is the core of the user syntax
 * engine. It can parse a string to a pair list search command.\n
 * The syntax of the module is:\n
 * Column(Property)|Keyword, Column(Property)|Keyword, ...\n
 * The parse process is split the string with the Split Text(e.g. in English it
 * will be ', ') in the specfic languange into several syntax block. And then
 * match the column or property. And then get the key word.\n
 * To match the column, first find the char '|', if we cannot find '|', then
 * this cannot be a syntax block. And tried to find the previous part in a hash
 * list.
 */
class KNMusicSearchSyntaxEngine : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicSearchSyntaxEngine class.
     * \param parent The parent object.
     */
    explicit KNMusicSearchSyntaxEngine(QObject *parent = 0);

    /*!
     * \brief Parse the text of search box into several search request block.
     * \param text The text of the search box.
     * \return A search block list parsed from the text.
     */
    QList<KNMusicSearchBlock> parseSearch(const QString &text);

signals:

public slots:

private slots:
    void retranslate();

private:
    QVariant textToVariant(const QString &text);
    QHash<QString, int> m_columnMap;
    QRegularExpression m_splitter;
};

#endif // KNMUSICSEARCHSYNTAXENGINE_H
