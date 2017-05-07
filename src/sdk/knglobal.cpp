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
#include <QApplication>
#include <QDir>
#include <QProcess>
#include <QStandardPaths>
#include <QThread>
#include <QNetworkReply>

#include "account/knaccount.h"
#include "knutil.h"
#include "knconfigure.h"
#include "knconfiguremanager.h"
#include "knfontmanager.h"
#include "knlocalemanager.h"
#include "knpreferenceplugin.h"
#include "knthememanager.h"
#include "knnotification.h"

#include "knglobal.h"

#include <QDebug>

//Initial the instance pointer to null.
KNGlobal *KNGlobal::m_instance=nullptr;

KNGlobal::~KNGlobal()
{
    //Stop account working thread.
    m_accountThread->quit();
    m_accountThread->wait();

    //Remove account system.
    knAccount->deleteLater();
}

KNGlobal *KNGlobal::instance()
{
    //Return the instance pointer.
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

QString KNGlobal::dirPath(int index)
{
    Q_ASSERT(index>-1 && index<DefaultDirCount);
    return m_dirPath[index];
}

QString KNGlobal::byteToString(qreal fileSize)
{
    //Set the unit pointer to Byte.
    int unitPointer=Byte;
    //Let the file size divide by 1024.0, and raise the pointer.
    while(fileSize>1024.0 && unitPointer<StorageUnitCount)
    {
        fileSize/=1024.0;
        unitPointer++;
    }
    //Return the file size and the unit string.
    return QString::number(fileSize, 'f', 2) + " " + m_storageUnit[unitPointer];
}

QString KNGlobal::connectionErrorText(int errorId)
{
    //Give back the error index.
    switch(errorId)
    {
    case QNetworkReply::ConnectionRefusedError:
        return tr("The remote server refused the connection, it is not "
                  "accepting requests.");
    case QNetworkReply::RemoteHostClosedError:
        return tr("The remote server closed the connection prematurely, before "
                  "the entire reply was received and processed.");
    case QNetworkReply::HostNotFoundError:
        return tr("The remote host name was not found.");
    case QNetworkReply::TimeoutError:
        return tr("The connection timed out.");
    case QNetworkReply::OperationCanceledError:
        return tr("The operation was canceled before it was finished.");
    case QNetworkReply::SslHandshakeFailedError:
        return tr("The SSL/TLS handshake failed and the encrypted channel "
                  "could not be established.");
    case QNetworkReply::TemporaryNetworkFailureError:
        return tr("The connection was broken due to disconnection from the "
                  "network, however the system has initiated roaming to "
                  "another access point.");
    case QNetworkReply::NetworkSessionFailedError:
        return tr("The connection was broken due to disconnection from the "
                  "network or failure to start the network.");
    case QNetworkReply::BackgroundRequestNotAllowedError:
        return tr("The background request is not currently allowed due to "
                  "platform policy.");
    case QNetworkReply::ProxyConnectionRefusedError:
        return tr("The connection to the proxy server was refused, it is not "
                  "accepting requests.");
    case QNetworkReply::ProxyConnectionClosedError:
        return tr("The proxy server closed the connection prematurely, before "
                  "the entire reply was received and processed.");
    case QNetworkReply::ProxyNotFoundError:
        return tr("The proxy host name was not found, The proxy hostname is "
                  "invalid.");
    case QNetworkReply::ProxyTimeoutError:
        return tr("The connection to the proxy timed out or the proxy did not "
                  "reply in time to the request sent.");
    case QNetworkReply::ProxyAuthenticationRequiredError:
        return tr("The proxy requires authentication in order to honour the "
                  "request but did not accept any credentials offered.");
    case QNetworkReply::UnknownProxyError:
        return tr("An unknown proxy-related error was detected.");;
    case QNetworkReply::ContentAccessDenied:
        return tr("The access to the remote content was denied "
                  "(HTTP error 401).");
    case QNetworkReply::ContentOperationNotPermittedError:
        return tr("The operation requested on the remote content is not "
                  "permitted.");
    case QNetworkReply::ContentNotFoundError:
        return tr("The remote content was not found at the server "
                  "(HTTP error 404).");
    case QNetworkReply::AuthenticationRequiredError:
        return tr("The remote server requires authentication to serve the "
                  "content but the credentials provided were not accepted (if "
                  "any).");
    case QNetworkReply::ContentConflictError:
        return tr("The request could not be completed due to a conflict with "
                  "the current state of the resource.");;
    case QNetworkReply::ContentGoneError:
        return tr("The requested resource is no longer available at the "
                  "server.");
    case QNetworkReply::UnknownContentError:
        return tr("An unknown error related to the remote content was "
                  "detected.");
    case QNetworkReply::ProtocolUnknownError:
        return tr("The Network Access API cannot honor the request because the "
                  "protocol is not known.");
    case QNetworkReply::ProtocolInvalidOperationError:
        return tr("The requested operation is invalid for this protocol.");
    case QNetworkReply::ProtocolFailure:
        return tr("A breakdown in protocol was detected (parsing error, "
                  "invalid or unexpected responses, etc.).");
    case QNetworkReply::InternalServerError:
        return tr("The server encountered an unexpected condition which "
                  "prevented it from fulfilling the request. (HTTP error 500)");
    case QNetworkReply::ServiceUnavailableError:
        return tr("The server is unable to handle the request at this time. "
                  "(HTTP error 503)");
    case QNetworkReply::UnknownServerError:
        return tr("An unknown error related to the server response was "
                  "detected.");
    default:
        //Treat all the other error as unknown error.
        return tr("An unknown network-related error was detected.");
    }
}

QBrush KNGlobal::textureBursh(const int &index) const
{
    Q_ASSERT(index>-1 && index<TextureBrushCount);
    return m_brushes[index];
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

KNConfigure *KNGlobal::accountConfigure()
{
    return knConf->configure(KNConfigureManager::Account);
}

void KNGlobal::retranslate()
{
    //Update storeage unit.
    m_storageUnit[Byte]=tr("Byte");

    //Update image filter.
    QString readWriteImageFilter;
    readWriteImageFilter.append(tr("Portable Network Graphics"));
    readWriteImageFilter.append(" (*.png);;");
    readWriteImageFilter.append(tr("Joint Photographic Experts Group"));
    readWriteImageFilter.append(" (*.jpg *.jpeg);;");
    readWriteImageFilter.append(tr("Windows Bitmap"));
    readWriteImageFilter.append(" (*.bmp);;");
    readWriteImageFilter.append(tr("Portable Pixmap"));
    readWriteImageFilter.append(" (*.ppm);;");
    readWriteImageFilter.append(tr("X11 Bitmap"));
    readWriteImageFilter.append(" (*.xbm);;");
    readWriteImageFilter.append(tr("X11 Pixmap"));
    readWriteImageFilter.append(" (*.xpm)");
    //Get all supported files text.
    QString allSupportedFiles=tr("All supported files");
    //Compose write image filter.
    m_writeImageFilter.clear();
    m_writeImageFilter.append(allSupportedFiles);
    m_writeImageFilter.append(" (*.png *.jpg *.jpeg *.bmp *.ppm *.xbm "
                                "*.xpm);;");
    m_writeImageFilter.append(readWriteImageFilter);
    //Compose the read image filter.
    m_readImageFilter.clear();
    m_readImageFilter.append(allSupportedFiles);
    m_readImageFilter.append(" (*.png *.jpg *.jpeg *.bmp *.ppm *.xbm "
                               "*.xpm *.gif *.pbm *.pgm);;");
    m_readImageFilter.append(readWriteImageFilter + ";;");
    m_readImageFilter.append(tr("Graphic Interchange Format"));
    m_readImageFilter.append(" (*.gif);;");
    m_readImageFilter.append(tr("Portable Bitmap"));
    m_readImageFilter.append(" (*.pbm);;");
    m_readImageFilter.append(tr("Portable Graymap"));
    m_readImageFilter.append(" (*.pgm)");
}

KNGlobal::KNGlobal(QObject *parent) :
    QObject(parent),
    #ifdef Q_OS_UNIX
    m_desktopEnviroment(NullShell),
    #endif
    m_writeImageFilter(QString()),
    m_mainWindow(nullptr),
    m_preference(nullptr),
    m_globalConfigure(nullptr),
    m_accountThread(new QThread(this))
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
    //Generate the notification manager.
    KNNotification::initial(this);
    //Generate the account system.
    KNAccount::initial();

    //Initial the infrastructure.
    initialInfrastrcture();
    //Update the infrastructure.
    // Load the language.
    knI18n->setLanguage(m_globalConfigure->data("Language").toString());

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
     * Kreogist Dir:
     *     Windows: My documents/Kreogist/
     *    Mac OS X: Documents/Kreogist/
     *       Linux: ~/.kreogist/
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
#ifdef Q_OS_WIN //No matter Win32/Win64
    m_dirPath[KreogistDir]=
            KNUtil::simplifiedPath(QStandardPaths::writableLocation(
                                       QStandardPaths::DocumentsLocation)
                                   +"/Kreogist");
    m_dirPath[UserDataDir]=m_dirPath[KreogistDir]+"/Mu";
    m_dirPath[ResourceDir]=qApp->applicationDirPath();
#endif
#ifdef Q_OS_MACX
    m_dirPath[KreogistDir]=
                KNUtil::simplifiedPath(QStandardPaths::writableLocation(
                                           QStandardPaths::DocumentsLocation)
                                       +"/Kreogist");
    m_dirPath[UserDataDir]=m_dirPath[KreogistDir]+"/Mu";
    m_dirPath[ResourceDir]=
            KNUtil::simplifiedPath(qApp->applicationDirPath()+"/../Resources");
#endif
#ifdef Q_OS_LINUX
    m_dirPath[KreogistDir]=
            KNUtil::simplifiedPath(QStandardPaths::writableLocation(
                                       QStandardPaths::HomeLocation))
            + "/.kreogist";
    m_dirPath[UserDataDir]=m_dirPath[KreogistDir]+"/mu";
    m_dirPath[ResourceDir]=m_dirPath[UserDataDir];
#endif
    m_dirPath[AccountDir]=
            KNUtil::simplifiedPath(m_dirPath[KreogistDir]+"/Account");
    m_dirPath[LibraryDir]=
            KNUtil::simplifiedPath(m_dirPath[UserDataDir]+"/Library");
    m_dirPath[PluginDir]=
            KNUtil::simplifiedPath(m_dirPath[UserDataDir]+"/Plugins");
}

inline void KNGlobal::initialInfrastrcture()
{
    //Initial storage unit.
    initialStorageUnit();
    //Initial the default path.
    initialDefaultDirPath();
    //Initial the global pixmaps.
    initialBrushes();
#ifdef Q_OS_UNIX
    //Check out the desktop environment.
    initialDesktopEnvironment();
#endif

    //Initial the configure manager.
    //Set the configure folder path.
    //-- Q: Why set configure path here?
    //   A: Because we won't change the configure path.
    //Because KNConfigureManager will automatically reload the configure, so we
    //don't need to load the configure here.
    knConf->setFolderPath(m_dirPath[UserDataDir]+"/Configure",
                          m_dirPath[AccountDir]);
    //Get the global configure.
    m_globalConfigure=userConfigure()->getConfigure("Global");

    //Initial the font manager.
    //Set the font resource folder.
    knFont->loadCustomFontFolder(m_dirPath[ResourceDir]+"/Fonts");
    //Set the default font.
    knFont->loadDefaultFont();

    //Initial the locale manager.
    //Load the language in language folder.
#ifdef Q_OS_LINUX
    //Thanks for Sou Bunnbu, amazingfate:
    //For Linux, we should also find langauges at /usr/share/Kreogist/mu.
    knI18n->addLanguageDirectory("/usr/share/Kreogist/mu/Language");
    //And the user installed languages, thanks for 1dot75cm.
    knI18n->addLanguageDirectory(m_dirPath[UserDataDir]+"/Language");
    knI18n->loadLanguageFiles();
#else
    //For Windows and Mac OS X, we can simply check the resource folder and
    //application dir.
    knI18n->addLanguageDirectory(m_dirPath[ResourceDir]+"/Language");
    knI18n->addLanguageDirectory(qApp->applicationDirPath()+"/Language");
    knI18n->loadLanguageFiles();
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

    //Set the configure.
    knAccount->setCacheConfigure(accountConfigure());
    //Initial the account system.
    knAccount->setWorkingThread(m_accountThread);
    //Link the account.
    connect(this, &KNGlobal::startWorking, knAccount, &KNAccount::startToWork,
            Qt::QueuedConnection);
    //Start account working thread.
    m_accountThread->start();

    //Update infrastructure, update the path of the library directory.
    updateInfrastructure();
}

inline void KNGlobal::initialBrushes()
{
    //Set the texture file path.
    QString textures[TextureBrushCount];
    textures[DullPolish]="://public/dull_polish_texture.png";

    //Initial all kinds of pixmaps.
    for(int i=0; i<TextureBrushCount; i++)
    {
        //Iniital the brush.
        m_brushes[i]=QBrush();
        //Set the texture of the brush.
        m_brushes[i].setTexture(QPixmap(textures[i]));
    }
}

inline QString KNGlobal::getMonthString(int month)
{
    switch(month)
    {
    case 1:
        return tr("Jan.");
    case 2:
        return tr("Feb.");
    case 3:
        return tr("Mar.");
    case 4:
        return tr("Apr.");
    case 5:
        return tr("May");
    case 6:
        return tr("Jun.");
    case 7:
        return tr("Jul.");
    case 8:
        return tr("Aug.");
    case 9:
        return tr("Sept.");
    case 10:
        return tr("Oct.");
    case 11:
        return tr("Nov.");
    case 12:
        return tr("Dec.");
    default:
        return QString();
    }
}

QString KNGlobal::readImageFilter() const
{
    return m_readImageFilter;
}

QString KNGlobal::writeImageFilter() const
{
    return m_writeImageFilter;
}

#ifdef Q_OS_UNIX
inline void KNGlobal::initialDesktopEnvironment()
{
#ifdef Q_OS_MACX
    return;
#endif
#ifdef Q_OS_LINUX
    //Check Gnome Shell.
    //Get system environment.
    QStringList &&systemEnvironment=QProcess::systemEnvironment();
    //If the desktop is gnome shell, then it should contains a DESKTOP_SESSION
    //which is gnome.
    for(auto item : systemEnvironment)
    {
        //Check out the line.
        if(item.startsWith("DESKTOP_SESSION="))
        {
            //Get the data of the item line.
            QString &&itemValue=item.mid(16).toLower();
            //Chekc the item value.
            if(itemValue=="gnome")
            {
                //Save the gnome shell.
                m_desktopEnviroment=GnomeShell;
                //Mission compelte.
                return;
            }
            //We find nothing, but mission complete.
            break;
        }
    }
#endif
}
#endif

QWidget *KNGlobal::mainWindow() const
{
    return m_mainWindow;
}

void KNGlobal::setMainWindow(QWidget *mainWindow)
{
    m_mainWindow = mainWindow;
}

QString KNGlobal::localeDateString(const QDate &date)
{
    //Check the date.
    return tr("%2 %3, %1").arg(QString::number(date.year()),
                               getMonthString(date.month()),
                               QString::number(date.day()));
}

#ifdef Q_OS_UNIX
int KNGlobal::desktopEnvironment() const
{
    return m_desktopEnviroment;
}
#endif

void KNGlobal::setPreference(KNPreferencePlugin *preference)
{
    //Check the preference has been set before or not.
    if(m_preference==nullptr)
    {
        return;
    }
    //Save the preference pointer.
    m_preference = preference;
}

void KNGlobal::updateInfrastructure()
{
    //Update the library directory path.
    QString oldLibraryPath=m_dirPath[LibraryDir];
    //Get the directory path from the configure, use the old library path as the
    //default value.
    QString savedLibraryDir=
            systemConfigure()->data("LibraryPath", oldLibraryPath).toString();
    //Construct the saved library directory path.
    QDir savedDirectory(savedLibraryDir);
    //Check the library directory is exist or not.
    if((!savedDirectory.exists()) &&
            //Try to build the directory.
            KNUtil::ensurePathValid(savedLibraryDir).isEmpty())
    {
        //! FIXME: report the update error.
        return;
    }
    //Save the new library directory path.
    m_dirPath[LibraryDir]=savedDirectory.absolutePath();
    //Check if the previous directory is just the same as the current one.
    //If they are different, emit the library moved signal to update.
    if(oldLibraryPath!=m_dirPath[LibraryDir])
    {
        emit libraryPathChanged(oldLibraryPath, m_dirPath[LibraryDir]);
    }
}
