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

#ifndef KNMUSICLYRICSMANAGER_H
#define KNMUSICLYRICSMANAGER_H

#include "knmusicutil.h"

#include <QObject>

using namespace KNMusicUtil;

class KNMusicLyricsBackend;
/*!
 * \brief The KNMusicLyricsManager class
 */
class KNMusicLyricsManager : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicLyricsManager(QObject *parent = 0);

    KNMusicLyricsBackend *managerBackend();

signals:

public slots:
    void loadLyrics(const KNMusicAnalysisItem &analysisItem);

private:
    KNMusicLyricsBackend *m_backend;
};

#endif // KNMUSICLYRICSMANAGER_H
