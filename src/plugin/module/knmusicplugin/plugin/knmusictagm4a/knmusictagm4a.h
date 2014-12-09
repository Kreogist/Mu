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
#ifndef KNMUSICTAGM4A_H
#define KNMUSICTAGM4A_H

#include <QHash>

#include "knmusictagpraser.h"

namespace KNMusicM4A
{
struct M4ABox
{
    quint32 size;
    QString name;
    bool independence=false;
    char *data=nullptr;
    ~M4ABox()
    {
        //Check if is a independence box, recover the memory
        if(independence && data!=nullptr)
        {
            delete[] data;
        }
    }
};
}

using namespace KNMusicM4A;

class KNMusicTagM4A : public KNMusicTagParser
{
    Q_OBJECT
public:
    explicit KNMusicTagM4A(QObject *parent = 0);
    bool praseTag(QFile &musicFile,
                  QDataStream &musicDataStream,
                  KNMusicDetailInfo &detailInfo);
    bool parseAlbumArt(KNMusicDetailInfo &detailInfo);

signals:

public slots:

private:
    inline void clearBox(M4ABox &box);
    inline void toIndependence(M4ABox &box);
    inline void writeBoxListToDetailInfo(const QList<M4ABox> &expandList,
                                         KNMusicDetailInfo &detailInfo);
    inline bool parseMetaBox(M4ABox &metaBox, M4ABox &ilstBox);
    inline bool getBox(QDataStream &musicDataStream,
                       M4ABox &box,
                       bool ignoreContent=false);
    inline bool extractBox(M4ABox &source,
                           QList<M4ABox> &boxes);
    QHash<QString, int> m_metadataAtom;
};

#endif // KNMUSICTAGM4A_H
