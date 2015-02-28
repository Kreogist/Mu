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
#ifndef KNPREFERENCEGENERAL_H
#define KNPREFERENCEGENERAL_H

#include "kncategorypreference.h"

class KNPreferenceItemPathBrowser;
class KNPreferenceGeneral : public KNCategoryPreference
{
    Q_OBJECT
public:
    explicit KNPreferenceGeneral(QObject *parent = 0);
    KNPreferenceItemPanel *panel();
    KNConfigure *configure();

signals:
    void libraryDirMoved();

public slots:

private slots:
    void retranslate();
    void onLibraryMoved();

private:
    enum GeneralPreferenceTitle
    {
        GeneralLibrary,
        GeneralPreferenceTitleCount
    };
    enum GeneralPreferenceItem
    {
        LibraryPath,
        GeneralPreferenceItemCount
    };
    QLabel *m_titles[GeneralPreferenceTitleCount];
    KNPreferenceItem *m_items[GeneralPreferenceItemCount];
    KNConfigure *m_systemConfigure, *m_generalConfigure;

    KNPreferenceItemPanel *m_panel;

    KNPreferenceItemPathBrowser *m_libraryDetector;
};

#endif // KNPREFERENCEGENERAL_H
