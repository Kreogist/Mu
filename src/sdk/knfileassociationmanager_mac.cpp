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
#include <ApplicationServices/ApplicationServices.h>

#include "knversion.h"

#include "knfileassociationmanager.h"

#include <QDebug>

CFStringRef cfStringRefFromString(const QString &string)
{
    //Use the string create function.
    return CFStringCreateWithCharacters(
                0,
                reinterpret_cast<const UniChar *>(string.unicode()),
                string.length());
}

QString stringfromCFString(CFStringRef cfStringReference)
{
    QString string;
    //The string reference is actaully a pointer.
    if (!cfStringReference)
    {
        //Ignore the invalid pointer setting.
        return QString();
    }
    //Get the reference string size.
    CFIndex size = CFStringGetLength(cfStringReference);
    //Prepare the fixed size buffer, for most of the time, the content shouldn't
    //be longer than 1024 unicode chars.
    UniChar fixedSizeBuffer[1024];
    //Prepare for the size buffer pointer.
    UniChar* buffer;
    //Check for the length is larger or not.
    if (size > (CFIndex)(sizeof(fixedSizeBuffer) / sizeof(UniChar)))
    {
        //Allocate the memory for the buffer.
        buffer = (UniChar*)malloc(size* sizeof(UniChar));
    }
    else
    {
        //Use the fixed size buffer.
        buffer = fixedSizeBuffer;
    }
    //Get the content from the string.
    CFStringGetCharacters(cfStringReference, CFRangeMake(0, size), buffer);
    //Set the content to QString.
    string.setUnicode((const QChar*)buffer, (uint)size);
    //If the buffer is using the new allocated memory, free the buffer.
    if (buffer != fixedSizeBuffer)
    {
        //Free the buffer.
        free(buffer);
    }
    //Return the string.
    return string;
}

bool KNFileAssociationManager::bindFileTypes(const QString &suffix,
                                             const QString &handler)
{
    Q_UNUSED(suffix)
    //Save the result of default role.
    OSStatus status=-1;
    //Try three times.
    int tries=3;
    while(--tries && (status!=0))
    {
        //Use the Application Services to set the default open type.
        status=LSSetDefaultRoleHandlerForContentType(
                    cfStringRefFromString(handler),
                    kLSRolesViewer,
                    cfStringRefFromString(APP_HANDLER));
    }
    //Check the set result.
    return status==0;
}

bool KNFileAssociationManager::unbindFileTypes(const QString &suffix,
                                               const QString &handler)
{
    Q_UNUSED(suffix)
    //Save the result of default role.
    OSStatus status=-1;
    //Try three times.
    int tries=3;
    while(--tries && (status!=0))
    {
        //Use the Application Services to iTunes.
        status=LSSetDefaultRoleHandlerForContentType(
                    cfStringRefFromString(handler),
                    kLSRolesViewer,
                    cfStringRefFromString("com.apple.itunes"));
    }
    //Check the set result.
    return status==0;
}

bool KNFileAssociationManager::isFileTypeBinded(const QString &suffix,
                                                const QString &contentType)
{
    Q_UNUSED(suffix)
    //Recast the content type.
    CFStringRef cfContentType=cfStringRefFromString(contentType),
                cfResult=LSCopyDefaultRoleHandlerForContentType(cfContentType,
                                                                kLSRolesViewer);
    //Check the result is the same or not.
    QString resultString=stringfromCFString(cfResult);
    qDebug()<<contentType<<resultString;
    //Compare the result string.
    return APP_HANDLER==resultString;
}
