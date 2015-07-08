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

#ifndef KNMUSICDETAILPANELOVERVIEW_H
#define KNMUSICDETAILPANELOVERVIEW_H

#include "knmusicdetaildialogpanel.h"

class QLabel;
class KNCircleIconButton;
class KNMusicDetailPanelOverview : public KNMusicDetailDialogPanel
{
    Q_OBJECT
public:
    explicit KNMusicDetailPanelOverview(QWidget *parent = 0);

    QAbstractButton *tabButton();

signals:

public slots:
    void setFilePath(const QString &filePath,
                     const QString &indexFilePath=QString(),
                     const int &index=-1);

private slots:
    void retranslate();

private:
    enum DetailInformation
    {
        DetailYear,
        DetailGenre,
        DetailKind,
        DetailSize,
        DetailBitRate,
        DetailSampleRate,
        DetailDateModified,
        DetailInformationCount
    };

    QLabel *m_caption[DetailInformationCount],
           *m_information[DetailInformationCount];
    KNCircleIconButton *m_button;

};

#endif // KNMUSICDETAILPANELOVERVIEW_H
