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
#include <QStandardPaths>
#include <QApplication>

#include "knutil.h"
#include "knconfigure.h"
#include "knconfiguremanager.h"
#include "knfontmanager.h"
#include "knlocalemanager.h"
#include "knthememanager.h"

#include "knglobal.h"

//Initial the instance pointer to null.
KNGlobal *KNGlobal::m_instance=nullptr;

KNGlobal *KNGlobal::instance()
{
    //Initial the global instance if it's still null.
    return m_instance;
}

void KNGlobal::initial(QObject *parent)
{
    //Check if the singleton instance variable is null. Set the pointer to this
    //object if this is the first constructed object.
    if(m_instance==nullptr)
    {
        m_instance=new KNGlobal(parent);
    }
}

QString KNGlobal::dirPath(const int &index)
{
    Q_ASSERT(index>-1 && index<DefaultDirCount);
    return m_dirPath[index];
}

KNConfigure *KNGlobal::cacheConfigure()
{
    return knConf->configure(KNConfigureManager::Cache);
}

KNConfigure *KNGlobal::systemConfigure()
{
    return knConf->configure(KNConfigureManager::System);
}

KNConfigure *KNGlobal::userConfigure()
{
    return knConf->configure(KNConfigureManager::User);
}

void KNGlobal::retranslate()
{
    m_storageUnit[Byte]=tr("Byte");
}

KNGlobal::KNGlobal(QObject *parent) :
    QObject(parent),
    m_globalConfigure(nullptr)
{
    //Initial the managers.
    //Gerenate the configure manager.
    KNConfigureManager::initial(this);
    //Generate the font manager.
    KNFontManager::initial(this);
    //Generate the locale manager.
    KNLocaleManager::initial(this);
    //Generate the theme manager.
    KNThemeManager::initial(this);

    //Initial the infrastructure.
    initialInfrastrcture();
    //Update the infrastructure.

    //Link the retranslate.
    knI18n->link(this, &KNGlobal::retranslate);
    retranslate();
}

inline void KNGlobal::initialStorageUnit()
{
    //Simply save the storage unit text.
    //For byte, it should be update in retranslate() function.
    m_storageUnit[KiloByte]="KB";
    m_storageUnit[MegaByte]="MB";
    m_storageUnit[GigaByte]="GB";
    m_storageUnit[TeraByte]="TB";
    m_storageUnit[PetaByte]="PB";
    m_storageUnit[ExaByte] ="EB";
    m_storageUnit[ZetaByte]="ZB";
    m_storageUnit[YottaByte]="YB";
    m_storageUnit[NonaByte]="NB";
    m_storageUnit[DoggaByte]="DB";
}

inline void KNGlobal::initialDefaultDirPath()
{
    /*
     * Initial the default path of the dirs.
     * User Data Dir:
     *     Windows: My documents/Kreogist/Mu
     *    Mac OS X: Documents/Kreogist/Mu
     *       Linux: ~/.kreogist/mu
     * Resource Dir:
     *     Windows: Application path
     *    Mac OS X: Application path/../Resources
     *       Linux: ~/.kreogist/mu
     * Library Dir:
     *      $UserDataDir$/Library
     * Plugin Dir:
     *      $UserDataDir$/Plugins
     */
#ifdef Q_OS_MACX
    m_dirPath[UserDataDir]=
            KNUtil::simplifiedPath(
                QStandardPaths::writableLocation(
                    QStandardPaths::DocumentsLocation)
                +"/Kreogist/Mu");
    m_dirPath[ResourceDir]=
            KNUtil::simplifiedPath(qApp->applicationDirPath()+"/../Resources");
#endif
    m_dirPath[LibraryDir]=
            KNUtil::simplifiedPath(m_dirPath[UserDataDir]+"/Library");
    m_dirPath[PluginDir]=
            KNUtil::simplifiedPath(m_dirPath[UserDataDir]+"/Plugins");
}

void KNGlobal::initialInfrastrcture()
{
    //Initial storage unit.
    initialStorageUnit();
    //Initial the default path.
    initialDefaultDirPath();

    //Initial the configure manager.
    //Set the configure folder path.
    //-- Q: Why set configure path here?
    //   A: Because we won't change the configure path.
    //Because KNConfigureManager will automatically reload the configure, so we
    //don't need to load the configure here.
    knConf->setFolderPath(m_dirPath[UserDataDir]+"/Configure");
    //Get the global configure.
    m_globalConfigure=userConfigure()->getConfigure("Global");

    //Initial the font manager.
    //Set the font resource folder.
    knFont->loadCustomFontFolder(m_dirPath[ResourceDir]+"/Fonts");
    //Set the default font.
    knFont->setGlobalFont("WenQuanYi Micro Hei");

    //Initial the locale manager.
    //Load the language in language folder.
#ifdef Q_OS_LINUX
    //Thanks for Sou Bunnbu:
    //For Linux, we should also find langauges at /usr/share/Kreogist/mu, here's
    //the default package resource provide place.
    knI18n->loadLanguageFiles("/usr/share/Kreogist/mu/Language");
#else
    knI18n->loadLanguageFiles(m_dirPath[ResourceDir]+"/Language");
#endif
    //Load the current language file.
    //We will load the langauge file twice, for the first time, we have to load
    //the default language according to the system locale settings.
    knI18n->setDefaultLanguage();
    //Load the language stored in the configure file.
    knI18n->setLanguage(m_globalConfigure->data("Language").toString());

    //Initial the theme manager.
    //Load the theme in theme folder. It's familiar with the language folder.
#ifdef Q_OS_LINUX
    knTheme->loadThemeFiles("/usr/share/Kreogist/mu/Theme");
#else
    knTheme->loadThemeFiles(m_dirPath[ResourceDir]+"/Theme");
#endif
    //Load the current theme file.
    //Like the language, we will load theme twice. Default first, user next.
    knTheme->setTheme(0);
    //Load the theme in the configure file.
    knTheme->setTheme(m_globalConfigure->data("Theme").toString());
}
