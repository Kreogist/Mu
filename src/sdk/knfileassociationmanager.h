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

#ifndef KNFILEASSOCIATIONMANAGER_H
#define KNFILEASSOCIATIONMANAGER_H

#include <QObject>

#define knFileType (KNFileAssociationManager::instance())

/*!
 * \brief The KNFileAssociationManager class manages the association of file
 * types for the application. It will automatically use different operations for
 * different operating system platform. Currently, Windows, Mac OS X, and Linux
 * are supported.
 */
class KNFileAssociationManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Get the global instance of the file association manager.
     * \return The file association manager global instance pointer.
     */
    static KNFileAssociationManager *instance();

    /*!
     * \brief Initial the global instance.
     * \param parent The parent of the global instance;
     */
    static void initial(QObject *parent = 0);

    /*!
     * \brief Bind one type file association to the current application.
     * \param suffix The suffix of the file type.
     * \param handler The file type handler. For example, the handler of
     * MPEG-4 Audio (.m4a) is com.apple.m4a-audio.
     * \return If the operation success, return true. Or else return false.
     */
    bool bindFileTypes(const QString &suffix,
                       const QString &handler="");

    /*!
     * \brief Unbind on file association to the current application. Notice,
     * that this operation will simply remove the file assocation from the user
     * settings.
     * \param suffix The suffix of the file type.
     * \param handler The file type handler.
     * \return If the file type association binding has been removed from the
     * operating systems, return true.
     */
    bool unbindFileTypes(const QString &suffix,
                         const QString &handler="");

    /*!
     * \brief Check whether one type of files has already associated with the
     * current application.
     * \param suffix The suffix of the file type.
     * \param contentType The file content type handler.
     * \return If the file type has already associated with the application,
     * return true.
     */
    bool isFileTypeBinded(const QString &suffix,
                          const QString &contentType="");

signals:

public slots:

private:
    static KNFileAssociationManager *m_instance;
    explicit KNFileAssociationManager(QObject *parent = 0);
};

#endif // KNFILEASSOCIATIONMANAGER_H
