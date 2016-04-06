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

#ifndef KNMUSICSTORESEARCHRESULTWIDGET_H
#define KNMUSICSTORESEARCHRESULTWIDGET_H

#include <QWidget>

class QLabel;
class KNAnimeLabelButton;
class KNMusicStoreAlbumTreeView;
class KNMusicStoreBackend;
/*!
 * \brief The KNMusicStoreSearchResultWidget class provide a list to display all
 * the search result from the backend.
 */
class KNMusicStoreSearchResultWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreSearchResultWidget widget.
     * \param parent The parent widget.
     */
    explicit KNMusicStoreSearchResultWidget(QWidget *parent = 0);

    /*!
     * \brief Get the title label of the search result.
     * \return The search result.
     */
    KNAnimeLabelButton *headerLabel();

    /*!
     * \brief setBullectWidget
     * \param bullectWidget
     */
    void setBulletWidget(QLabel *bulletWidget);

signals:

public slots:
    /*!
     * \brief Set backend to search result widget.
     * \param backend The music store backend.
     */
    void setBackend(KNMusicStoreBackend *backend);

private slots:
    void retranslate();
    void onActionSearchComplete();

private:
    QLabel *m_bulletWidget;
    KNMusicStoreBackend *m_backend;
    KNAnimeLabelButton *m_headerLabel;
    KNMusicStoreAlbumTreeView *m_songTreeView;
};

#endif // KNMUSICSTORESEARCHRESULTWIDGET_H
