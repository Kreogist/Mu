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

#ifndef KNGLOBAL_H
#define KNGLOBAL_H

#include <QObject>

/*!
 * \def knGlobal
 * A global pointer referring to the unique global object.
 */
#define knGlobal (KNGlobal::instance())

class KNConfigure;
/*!
 * \brief The KNGlobal class is a global instance which contains several public
 * function. \n
 * You should only construct this class once. If you construct the KNGlobal more
 * than once, the other instance will become no use.\n
 * This global instance will also initial several singleton instance, and take
 * those managers' control.
 *  * KNConfigureManager - A Json based operating system independent configure
 * system.
 *  * KNThemeManager - A Json based color theme file manager.
 *  * KNLocaleManager - International settings. Including translations and
 * locale default codec.
 *  * KNFontManager - A custom font files loader.
 */
class KNGlobal : public QObject
{
    Q_OBJECT
public:
    enum DefaultPath
    {
        UserDataDir,
        ResourceDir,
        LibraryDir,
        PluginDir,
        DefaultDirCount
    };

    /*!
     * \brief Construct a KNGlobal.
     * \param parent The parent object.
     */
    explicit KNGlobal(QObject *parent = 0);

    /*!
     * \brief Get the singleton instance of class KNGlobal.
     * \return The global instance of KNGlobal.
     */
    static KNGlobal *instance();

    /*!
     * \brief Get the specific directory path. The definition of each
     * directories.\n
     *   User Data Dir - This dir stores the configure file. This path cannot
     * be changed.\n
     * *   Windows: My documents/Kreogist/Mu\n
     * *  Mac OS X: Documents/Kreogist/Mu\n
     * *     Linux: ~/.kreogist/mu\n
     *   Resource Dir - This dir stores the default system resource. Like some
     * official plugins. This path cannot be changed.\n
     * *   Windows: Application path\n
     * *  Mac OS X: Application path/../Resources\n
     * *     Linux: ~/.kreogist/mu\n
     *   Library Dir - This dir stores the library data. This path can be
     * changed by user. The default path is the "Library" folder under the user
     * data path.\n
     *   Plugin Dir - This dir stores all the user download plugins for mu. This
     * path can be changed by user.\n
     * You should get the directory according to the DefaultPath enumerate.
     * \param index The directory enumerate number.
     * \return The path of the directory.
     */
    QString dirPath(const int &index);

    /*!
     * \brief Get the cache configure class.
     * \return The cache KNConfigure class.
     */
    KNConfigure *cacheConfigure();

    /*!
     * \brief Get the system configure class.
     * \return The system KNConfigure class.
     */
    KNConfigure *systemConfigure();

    /*!
     * \brief Get the user configure class.
     * \return The user KNConfigure class.
     */
    KNConfigure *userConfigure();

    template<typename Func2>
    /*!
     * \brief Link the language change signal with the retranslate slot which
     * should be provide by the receiver.\n
     * You should make the retranslate function as a public slot.
     * \param receiver The recevier object.
     * \param retranslate The retranslate slot.
     */
    void linkRetranslate(const typename
                         QtPrivate::FunctionPointer<Func2>::Object *receiver,
                         Func2 retranslate)
    {
        //Link the language change signal to the receiver's retranslate slot.
        connect(this, &KNGlobal::languageChange, receiver, retranslate);
    }

signals:
    /*!
     * \brief When the language file change, this signal will be emitted. All
     * the multi-locale module should connect this signal with it's
     * retranslate() slot.
     */
    void languageChange();

public slots:
    /*!
     * \brief Update all the translation of the current object.
     */
    void retranslate();

private slots:

private:
    enum StorageUnit
    {
        Byte,
        KiloByte,
        MegaByte,
        GigaByte,
        TeraByte,
        PetaByte,
        ExaByte,
        ZetaByte,
        YottaByte,
        NonaByte,
        DoggaByte,
        StorageUnitCount
    };

    static KNGlobal *m_instance;

    inline void initialStorageUnit();
    inline void initialDefaultDirPath();
    inline void initialInfrastrcture();

    QString m_dirPath[DefaultDirCount];
    QString m_storageUnit[StorageUnitCount];

    KNConfigure *m_globalConfigure;
};

#endif // KNGLOBAL_H
