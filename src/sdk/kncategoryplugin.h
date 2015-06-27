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

#ifndef KNCATEGORYPLUGIN_H
#define KNCATEGORYPLUGIN_H

#include <QWidget>

/*!
 * \brief The KNCategoryPlugin class is the main widget which contains all the
 * things your application has.\n
 * The plugin itself is the central widget. For default, it's only a blank
 * widget. You can set layout on it and add any widget.\n
 * It should also provide a header widget which will be put on the header
 * container. As well, it should provide the preference panel of the category.
 */
class KNCategoryPlugin : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct the KNCategoryPlugin with on the parent widget.
     * \param parent The parent widget.
     */
    KNCategoryPlugin(QWidget *parent = 0):QWidget(parent){}

    /*!
     * \brief The header widget of the category.
     * \return The header widget.
     */
    virtual QWidget *headerWidget()=0;

    /*!
     * \brief Provide the icon of the category.
     * \return
     */
    virtual QPixmap icon()=0;

    virtual QString title()=0;

signals:

public slots:
    /*!
     * \brief Save the category configure. All the data should be finally saved
     * to the KNConfigure class provided by KNConfigureManager.
     */
    virtual void saveConfigure()=0;

    /*!
     * \brief When there's a new instance launched, the arguments will be send
     * from KNPluginManager. The category plugin should process these arguments.
     * \param data The arguments from the other instances.
     */
    virtual void onArgumentsAvailable(const QStringList &data)=0;
};

#endif // KNCATEGORYPLUGIN_H
