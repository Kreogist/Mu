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
     * \brief Construct a KNFontManager class.
     * \param parent The parent object of the font manager.
     */
    explicit KNFontManager(QObject *parent = 0);

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
     * \brief A shortcut function to set global font.
     * \param fontName The global font family name.
     * \param pointSize The point size of the global font. The default value of
     * this parameter is 12.
     */
    void setGlobalFont(const QString &fontName,
                       const qreal &pointSize=12.00);

signals:

public slots:

private:
    static KNFontManager *m_instance;
};

#endif // KNFONTMANAGER_H
