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

#ifndef KNCONFIGUREMANAGER_H
#define KNCONFIGUREMANAGER_H

#include <QDateTime>
#include <QStringList>

#include <QObject>

#define knConf (KNConfigureManager::instance())

class KNConfigure;
/*!
 * \brief The KNConfigureManager class is a manager of three kind of configure:
 * cache, system and user. \n
 * Cache configure is some temporary data generate from the last session or
 * runtime libraries. Data like the window geometry should be stored in this
 * configure. The file name should be: cache.json\n
 * System configure is some data dependent on the operating system. Like the
 * file path. The file name should be: system.json\n
 * User configure is some configure like binary switchers or customized settings
 * like the color of the button or text font of the label. The file name should
 * be: user.json\n
 * All of these file should be stored as UTF-8, plain text format.\n
 * You can get the KNConfigure class from the configure manager. It will provide
 * those three configure class.
 */
class KNConfigureManager : public QObject
{
    Q_OBJECT
public:
    enum ConfigureType
    {
        Cache,
        System,
        User,
        Account,
        ConfigureTypeCount
    };

    /*!
     * \brief Get the singleton instance of the configure manager. Before the
     * initial() has been called, this function will return nullptr.
     * \return The instance of configure manager.
     */
    static KNConfigureManager *instance();

    /*!
     * \brief Initial the manager, generate the instance with the given parent
     * object.\n
     * Only the first time will create a instance.
     */
    static void initial(QObject *parent = 0);

    /*!
     * \brief Get the current configure folder path.
     * \return The folder which should contains the files of three kinds of
     * configure.
     */
    QString folderPath() const;

    /*!
     * \brief Get the configure class.
     * \param index The configure index in ConfigureType.
     * \return The specific configure.
     */
    KNConfigure *configure(int index);

    /*!
     * \brief Get a configure value from the configures.
     * \param path The path of the configure.
     * \param defaultValue The default value when the value is not existed.
     * \return The configure value data.
     */
    QVariant configureValue(QStringList path,
                            const QVariant &defaultValue);

    /*!
     * \brief Get the user configure update time.
     * \return The time of user configure updated.
     */
    QDateTime userConfigureUpdateTime() const;

signals:
    /*!
     * \brief Emit when user configure update time is changed.
     */
    void userConfigureUpdateTimeChanged();

public slots:
    /*!
     * \brief Set the folder path which contains the configure files. \n
     * If the folder path is not exist, the path won't be set.\n
     * If you set the folder path to empty, it will load the configure under the
     * application path.
     * \param folderPath The configure file folder path.
     * \param accountFolderPath The account configure file folder path.
     */
    void setFolderPath(const QString &folderPath,
                       const QString &accountFolderPath);

    /*!
     * \brief Set the configure value.
     * \param path The value saved path.
     * \param value The specific value.
     */
    void setConfigureValue(QStringList path,
                           const QVariant &value);

    /*!
     * \brief Reload the configure from the folder path.
     */
    void reloadConfigure();

    /*!
     * \brief Save the configure to the configure file.
     */
    void saveConfigure();

private:
    explicit KNConfigureManager(QObject *parent = 0);
    //Disable the copy of the instance.
    KNConfigureManager(const KNConfigureManager &);
    KNConfigureManager(KNConfigureManager &&);
    inline void loadConfigureFile(const QString &filePath, int type);
    inline void saveConfigureFile(const QString &filePath, int type);
    inline KNConfigure *getTopLevelConfigure(const QString &topPath);

    static KNConfigureManager *m_instance;

    QString m_folderPath, m_accountFolderPath;
    QDateTime m_userConfigureUpdateTime;
    KNConfigure *m_configures[ConfigureTypeCount];
};

#endif // KNCONFIGUREMANAGER_H
