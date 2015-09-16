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
 * \brief The KNMusicTagM4a class provides you a m4a format file decode file.
 */
class KNMusicTagM4a : public KNMusicTagParser
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicTagM4a object.
     * \param parent The parent object.
     */
    explicit KNMusicTagM4a(QObject *parent = 0);

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
        quint32 size;
        QString name;
        //A box is independent is means:
        //It holds its own data, the data is not shared by other box.
        bool independence;
        char *data;
        M4ABox() :
            independence(false),
            data(nullptr)
        {
        }
        ~M4ABox()
        {
            //Check if is a independence box, recover the memory
            if(independence && data!=nullptr)
            {
                delete[] data;
            }
        }
    };
    inline void clearBox(M4ABox &box);
    inline bool getBox(QDataStream &musicDataStream,
                       M4ABox &box,
                       bool ignoreContent=false);
    inline bool extractBox(const M4ABox &source,
                           QList<M4ABox> &boxes);
    inline void independent(M4ABox &box);
    inline bool findIlstBox(const M4ABox &metaBox, M4ABox &ilstBox);
    inline void writeBox(const M4ABox &source, QFile &targetFile);
    inline bool extractMetaBox(const M4ABox &metaBox, QList<M4ABox> &boxes);
    inline bool extractData(quint32 sourceSize,
                            char *dataPosition,
                            QList<M4ABox> &boxes);
    inline bool findBox(const QString &targetName,
                        M4ABox &targetBox,
                        const QList<M4ABox> &boxList);
    inline M4ABox generateItemBox(const int &column,
                                  const QString &atomName,
                                  const QByteArray &rawData);
    M4ABox generateItemBox(char *atomFlags,
                           const QString &atomName,
                           const QByteArray &rawData);
    inline M4ABox zipBox(const QString &name, const QList<M4ABox> &boxes);
    inline QByteArray combineBoxList(const QList<M4ABox> &boxes);
    inline QByteArray packBox(const M4ABox &box);

    static QHash<QString, int> m_atomIndexMap;
    static QHash<int, QString> m_indexAtomMap;
    static QHash<int, quint8> m_indexFlagMap;
};

#endif // KNMUSICTAGM4A_H
