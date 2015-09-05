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

#ifndef KNMUSICLRCPARSER_H
#define KNMUSICLRCPARSER_H

#include <QRegularExpression>

#include <QObject>

/*!
 * \brief The KNMusicLrcParser class provides you a LRC format lyrics parser. It
 * should be a bug that it can parse the LRC file which is larger than 99:99.99.
 * But no worries.\n
 */
class KNMusicLrcParser : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicLrcParser object with given parent.
     * \param parent The parent object.
     */
    explicit KNMusicLrcParser(QObject *parent = 0);

    /*!
     * \brief Parse LRC data from a file.
     * \param filePath The file path.
     * \param positionList The output position list.
     * \param textList The output text list.
     * \return If we can parse successfully, then return true.
     */
    bool parseFile(const QString &filePath,
                   QList<qint64> &positionList,
                   QStringList &textList);

    /*!
     * \brief Parse the LRC data from a text string.
     * \param lyricsTextData The lyrics text raw data.
     * \param positionList The output position list.
     * \param textList The output text list.
     * \return If we can parse successfully, then return true.
     */
    bool parseText(const QString &lyricsTextData,
                   QList<qint64> &positionList,
                   QStringList &textList);

private:
    struct LyricsLine
    {
        qint64 position;
        QString text;
    };

    inline void parseFrames(QString frame,
                            const QString &text,
                            QList<LyricsLine> &positionList);

    static bool frameLessThan(const LyricsLine &frameLeft,
                              const LyricsLine &frameRight)
    {
        return frameLeft.position<frameRight.position;
    }
    QRegularExpression m_frameCatchRegExp, m_noneNumberRegExp;
    QTextCodec *m_utf8Codec, *m_localeCodec;
};

#endif // KNMUSICLRCPARSER_H
