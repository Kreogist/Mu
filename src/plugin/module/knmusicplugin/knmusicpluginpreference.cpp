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
#include <QLabel>

#include "preference/knpreferenceitemnumber.h"

#include "knglobal.h"
#include "knconfigure.h"
#include "knpreferenceitempanel.h"
#include "knpreferenceitemfactory.h"

#include "knmusicpluginpreference.h"

KNMusicPluginPreference::KNMusicPluginPreference(QObject *parent) :
    KNCategoryPreference(parent)
{
    //Initial the music configure.
    m_musicConfigure=new KNConfigure(this);
    m_musicConfigure->setCaption("Music");
    //Add the user configure.
    KNGlobal::instance()->userConfigure()->addSubConfigure(m_musicConfigure);
    //Initial the preference panel.
    m_panel=new KNPreferenceItemPanel;

    //Add control items to panel.
    initialItems();

    //Link retranslate request, and retranslate.
    connect(KNGlobal::instance(), &KNGlobal::requireRetranslate,
            this, &KNMusicPluginPreference::retranslate);
    retranslate();
}

KNPreferenceItemPanel *KNMusicPluginPreference::panel()
{
    return m_panel;
}

KNConfigure *KNMusicPluginPreference::configure()
{
    return m_musicConfigure;
}

void KNMusicPluginPreference::retranslate()
{
    //Update titles.
    m_titles[MusicLyrics]->setText(tr("Lyrics"));

    //Update items.
    m_items[LyricsFolder]->setCaption(tr("Lyrics Folder"));
    m_items[LyricsDownloadOnline]->setCaption(tr("Download Lyrics"));
    m_items[LyricsFont]->setCaption(tr("Lyrics Font"));
    m_items[LyricsTextSpacing]->setCaption("Text Spacing");
}

inline void KNMusicPluginPreference::initialItems()
{
    //Generate title.
    for(int i=0; i<MusicPreferenceTitleCount; i++)
    {
        m_titles[i]=KNPreferenceItemFactory::createTitle();
    }
    //Generate lyrics item.
    m_items[LyricsFolder]=KNPreferenceItemFactory::create(PathEditItem, "LyricsFolder", m_musicConfigure);
    m_items[LyricsDownloadOnline]=KNPreferenceItemFactory::create(SwitcherItem, "LyricsDownloadOnline", m_musicConfigure);
    m_items[LyricsFont]=KNPreferenceItemFactory::create(FontItem, "LyricsFont", m_musicConfigure);
    m_items[LyricsTextSpacing]=KNPreferenceItemFactory::create(NumberItem, "LyricsTextSpacing", m_musicConfigure);
    ((KNPreferenceItemNumber *)m_items[LyricsTextSpacing])->setRange(0, 15);

    //Initial panel.
    m_panel->addTitle(m_titles[MusicLyrics]);
    m_panel->addItem(m_items[LyricsFolder]);
    m_panel->addItem(m_items[LyricsDownloadOnline]);
    m_panel->addItem(m_items[LyricsFont]);
    m_panel->addItem(m_items[LyricsTextSpacing], true);
}
