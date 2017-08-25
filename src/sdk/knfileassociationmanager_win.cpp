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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <ShlObj.h>

#include <QDir>
#include <QSettings>
#include <QApplication>

#include "knfileassociationmanager.h"

#include <QDebug>

#define KreogistHeader  "KreogistFileType"
#define UserClassPath   "HKEY_CURRENT_USER\\Software\\Classes"
#define OpenWithPath    "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\" \
                        "CurrentVersion\\Explorer\\FileExts"

bool KNFileAssociationManager::bindFileTypes(const QString &suffix,
                                             const QString &handler,
                                             const QString &description)
{
    Q_UNUSED(handler)
    //First, we need to write Kreogist file type to user class registry.
    QSettings userClass(UserClassPath, QSettings::NativeFormat);
    //Generate the file type.
    QString fileType=KreogistHeader+suffix;
    //Begin the file type group.
    userClass.beginGroup(fileType);
    userClass.setValue("Default", description);
    //Write DefaultIcon subgroup.
    userClass.beginGroup("DefaultIcon");
    userClass.setValue("Default", QApplication::applicationFilePath()+",1");
    userClass.endGroup(); //--DefaultIcon
    //Write the file association.
    userClass.beginGroup("shell");
    //Write the open file assocition.
    userClass.beginGroup("Open");
    userClass.beginGroup("Command");
    userClass.setValue("Default", QDir::toNativeSeparators(
                           QApplication::applicationFilePath())+" %1");
    userClass.endGroup(); //--command
    userClass.endGroup(); //--open
    userClass.endGroup(); //--shell
    userClass.endGroup();
    //Okay, now, update the user group file association to Kreogist file type.
    QSettings extUserClass(UserClassPath"\\"+suffix, QSettings::NativeFormat);
    //Set the default value to Kreogist file type.
    extUserClass.setValue("Default", fileType);
    //Set the OpenWithProgIds values.
    extUserClass.beginGroup("OpenWithProgIds");
    extUserClass.setValue(fileType, "");
    extUserClass.endGroup();
    //Basically, it should be done here.
    //But we need to check whether the open with dialog has set a value or not.
    //Check the "Open With" dialog settings.
    QSettings openWith(OpenWithPath, QSettings::NativeFormat);
    if(openWith.childGroups().contains(suffix))
    {
        //Generate the file type path.
        QString fileTypePath=OpenWithPath"\\"+suffix;
        //Find the Open With has been set a data.
        QSettings typeOpenWith(fileTypePath, QSettings::NativeFormat);
        //Get the child groups.
        QStringList openWithGroups=typeOpenWith.childGroups();
        //Check for user choice settings.
        if(openWithGroups.contains("UserChoice"))
        {
            //Remove the UserChoice group.
            typeOpenWith.beginGroup("UserChoice");
            typeOpenWith.remove("");
            typeOpenWith.endGroup();
        }
    }
    //Notify Windows for the file association changed.
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_DWORD | SHCNF_FLUSH,
                   nullptr, nullptr);
    //Mission complete.
    return true;
}

bool KNFileAssociationManager::unbindFileTypes(const QString &suffix,
                                               const QString &handler)
{
    return false;
}

bool KNFileAssociationManager::isFileTypeBinded(const QString &suffix,
                                                const QString &handler)
{
    Q_UNUSED(handler)
    //Generate the file type.
    QString fileType=KreogistHeader+suffix;
    //Generate the open with path.
    //Check the "Open With" dialog settings.
    QSettings openWith(OpenWithPath, QSettings::NativeFormat);
    if(openWith.childGroups().contains(suffix))
    {
        //Generate the file type path.
        QString fileTypePath=OpenWithPath"\\"+suffix;
        //Find the Open With has been set a data.
        QSettings typeOpenWith(fileTypePath, QSettings::NativeFormat);
        //Get the child groups.
        QStringList openWithGroups=typeOpenWith.childGroups();
        //Check for user choice settings.
        if(openWithGroups.contains("UserChoice"))
        {
            //Open the UserChoice.
            QSettings userChoice(fileTypePath+"\\UserChoice",
                                 QSettings::NativeFormat);
            //Check the value.
            if(userChoice.contains("Progid"))
            {
                //Which is not associate with us.
                return fileType==userChoice.value("Progid").toString();
            }
            //When there is no Progid, move to next settings.
        }
        if(openWithGroups.contains("OpenWithProgids"))
        {
            //Which means that user didn't choose an option for the suffix.
            //Check the OpenWithProgids.
            QSettings progid(fileTypePath+"\\OpenWithProgids",
                             QSettings::NativeFormat);
            //Get the all keys, if it contains the file type, return true.
            if(progid.childKeys().contains(fileType))
            {
                return true;
            }
        }
    }
    //Prepare the settings to read the current user data.
    QSettings userClass(UserClassPath, QSettings::NativeFormat);
    //Chech whether the user class has the group of the suffix.
    if(userClass.childGroups().contains(suffix))
    {
        //If no suffix here, then this suffix is not registered.
        //If we find the suffix, detect its progid.
        QSettings extGroup(UserClassPath"\\"+suffix, QSettings::NativeFormat);
        //Check whether the default value is the same as Kreogist settings.
        return extGroup.value("Default").toString()==fileType;
    }
    //If we cannot find any thing, it is definiately false.
    return false;
}
