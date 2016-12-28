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

#ifndef KNMUSICSTOREDOWNLOADLIST_H
#define KNMUSICSTOREDOWNLOADLIST_H

#include <QWidget>

class QPropertyAnimation;
class KNOpacityAnimeButton;
/*!
 * \brief The KNMusicStoreDownloadList class provides a download list widget to
 * show the information. It will provide buttons to control the download mission
 * of the music store.\n
 * This widget is working like a container of the download list. The widget
 * itself is not the download list, but provides animations and management of
 * the real download list widget.
 */
class KNMusicStoreDownloadList : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreDownloadList widget.
     * \param parent The parent widget.
     */
    explicit KNMusicStoreDownloadList(QWidget *parent = 0);

    /*!
     * \brief Get the state button of the download list. The button could
     * activate the download list.
     * \return The download list button.
     */
    QWidget *stateButton();

signals:

public slots:
    /*!
     * \brief Show the download list widget.
     */
    void showDownloadList();

protected:
    /*!
     * \brief Reimplemented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void hideDownloadList();
    void onContainerMove(const QVariant &value);
    void onContainerMoveFinish();
    void onBackgroundClicked();

private:
    QColor m_backgroundColor;
    KNOpacityAnimeButton *m_stateButton;
    QWidget *m_container;
    QPropertyAnimation *m_containerAnime;
};

#endif // KNMUSICSTOREDOWNLOADLIST_H
