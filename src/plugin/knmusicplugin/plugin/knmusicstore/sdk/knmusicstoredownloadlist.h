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

#include <QItemSelection>

#include <QWidget>

class QTreeView;
class QPropertyAnimation;
class KNOpacityAnimeButton;
class KNOpacityAnimeTextButton;
class KNMusicStoreDownloadManager;
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

    /*!
     * \brief Append one item in the download manager, start to download the
     * song.
     * \param url The url of the song.
     * \param directoryPath The target directory path.
     * \param fileName The target file name.
     * \param songTitle The title of the song, used as the display information.
     */
    void downloadSong(const QString &url,
                      const QString &fileName,
                      const QString &songTitle);

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
    void retranslate();
    void hideDownloadList();
    void onDoubleClickIndex(const QModelIndex &index);
    void onContainerMove(const QVariant &value);
    void onContainerMoveFinish();
    void onBackgroundClicked();
    void onEmptyStateChanged(bool isEmpty);
    void onSelectionChanged(const QItemSelection &selected,
                            const QItemSelection &deselected);

private:
    inline KNOpacityAnimeTextButton *generateButton(const QString &iconPath);
    QColor m_backgroundColor;
    QString m_downloadPath;
    KNMusicStoreDownloadManager *m_downloadModel;
    KNOpacityAnimeButton *m_stateButton, *m_closeList;
    KNOpacityAnimeTextButton *m_missionStart, *m_missionPause, *m_missionRemove,
                             *m_missionClear;
    QWidget *m_container;
    QPropertyAnimation *m_containerAnime;
    QTreeView *m_downloadView;
};

#endif // KNMUSICSTOREDOWNLOADLIST_H
