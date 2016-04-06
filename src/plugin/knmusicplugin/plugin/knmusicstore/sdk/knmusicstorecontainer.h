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
#ifndef KNMUSICSTORECONTAINER_H
#define KNMUSICSTORECONTAINER_H

#include <QScrollArea>

class KNSideShadowWidget;
/*!
 * \brief The KNMusicStoreContainer class provides the scroll area for the store
 * widgets.
 */
class KNMusicStoreContainer : public QScrollArea
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreContainer widget.
     * \param parent The parent widget.
     */
    explicit KNMusicStoreContainer(QWidget *parent = 0);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QScrollArea::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    KNSideShadowWidget *m_topShadow;
};

#endif // KNMUSICSTORECONTAINER_H
