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

#ifndef KNFONTMANAGER_H
#define KNFONTMANAGER_H

#include <QFileInfo>

#include <QObject>

#define knFont (KNFontManager::instance())

class KNConfigure;
/*!
 * \brief The KNFontManager class is a custom font file manager. It will load
 * all the font file under a folder. And it also provides you function for easy
 * global font tweak.
 */
class KNFontManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Get the singleton instance of the font manager.
     * \return The instance of the font manager.
     */
    static KNFontManager *instance();

    /*!
     * \brief Initial the manager, generate the instance with the given parent
     * object.\n
     * Only the first time will create a instance.
     */
    static void initial(QObject *parent = 0);

    /*!
     * \brief Load all the font in from a folder. Notice: this function won't
     * find file in the sub folder.
     * \param folderPath The folder path.
     */
    void loadCustomFontFolder(const QString &folderPath);

    /*!
     * \brief This is a overload function.\n
     * Load a custom font from file according to the file info. It will only use
     * the file path data of the file info.
     * \param fileInfo The file info class.
     */
    inline void loadCustomFont(const QFileInfo &fileInfo)
    {
        loadCustomFont(fileInfo.absoluteFilePath());
    }

    /*!
     * \brief Load a custom font to font database.
     * \param filePath The custom font file path.
     */
    void loadCustomFont(const QString &filePath);

    /*!
     * \brief Set the configure of the font manager.
     * \param configure The configure object pointer.
     */
    void setConfigure(KNConfigure *configure);

    /*!
     * \brief Load default font from inner configuration file for global font.
     */
    void loadDefaultFont();

    /*!
     * \brief A shortcut function to set global font.
     * \param fontName The global font family name.
     * \param pixelSize The pixel size of the global font. The default value of
     * this parameter is 12.
     */
    void setGlobalFont(const QString &fontName=QString(),
                       int pixelSize=12);

signals:

public slots:

private:
    explicit KNFontManager(QObject *parent = 0);
    //Disable the copy of the instance.
    KNFontManager(const KNFontManager &);
    KNFontManager(KNFontManager &&);
    static KNFontManager *m_instance;

    KNConfigure *m_configure;
};

#endif // KNFONTMANAGER_H
