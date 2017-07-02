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

#ifndef KNMUSICLIBRARYDROPPROXY_H
#define KNMUSICLIBRARYDROPPROXY_H

#include "kndropproxycontainer.h"

/*!
 * \brief The KNMusicLibraryDropProxy class provides a filter proxy container
 * for all the library models. If the proxy container detected that the drop
 * event is coming exactly from the library model, it will igonre the event.
 */
class KNMusicLibraryDropProxy : public KNDropProxyContainer
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicLibraryDropProxy widget.
     * \param parent The parent widget.
     */
    explicit KNMusicLibraryDropProxy(QWidget *parent = 0);

    /*!
     * \brief Set the library model pointer.
     * \param model The library model pointer value.
     */
    void setLibraryModelPointer(const qint64 &model);

signals:

public slots:

protected:
    /*!
     * \brief Reimpelment from KNDropProxyContainer::dropEvent().
     */
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;

private:
    qint64 m_libraryModelPointer;
};

#endif // KNMUSICLIBRARYDROPPROXY_H
