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

#ifndef KNMUSICTAGM4A_H
#define KNMUSICTAGM4A_H

#include "knmusictagparser.h"

/*!
 * \brief The KNMusicTagM4a class provides you a m4a format file decode file. It
 * will decode the ilst box of the m4a format.
 */
class KNMusicTagM4a : public KNMusicTagParser
{
    Q_OBJECT
public:
    /*!
     * \brief Construct KNMusicTagM4a object with given parent.
     * \param parent The parent object.
     */
    explicit KNMusicTagM4a(QObject *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicTagParser::tagParserName().
     */
    QString tagParserName() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicTagParser::parseTag.
     */
    bool parseTag(QFile &musicFile,
                  QDataStream &musicDataStream,
                  KNMusicAnalysisItem &analysisItem) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicTagParser::writeTag.
     */
    bool writeTag(const KNMusicAnalysisItem &analysisItem) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicTagParser::parseAlbumArt.
     */
    bool parseAlbumArt(KNMusicAnalysisItem &analysisItem) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicTagParser::writable().
     */
    bool writable() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicTagParser::writeCoverImage().
     */
    bool writeCoverImage() const Q_DECL_OVERRIDE;

signals:

public slots:

private:
    struct M4ABox
    {
        QString name;
        QByteArray data;
    };
    inline bool getBox(QDataStream &musicDataStream,
                       M4ABox &box,
                       bool ignoreContent=false);
    inline bool parseBox(const M4ABox &source,
                         QHash<QString, QByteArray> &boxes);
    inline bool parseData(quint32 sourceSize,
                          char *dataPosition,
                          QHash<QString, QByteArray> &boxes);
    inline bool extractBox(const M4ABox &source,
                           QHash<QString, QByteArray> &boxes);
    inline bool extractData(quint32 sourceSize,
                            char *dataPosition,
                            QHash<QString, QByteArray> &boxes);
    inline bool findIlstBox(const QByteArray &metaBox, M4ABox &ilstBox);

    static QHash<QString, int> m_atomIndexMap;
    static QHash<int, QString> m_indexAtomMap;
    static QHash<int, quint8> m_indexFlagMap;
};

#endif // KNMUSICTAGM4A_H
