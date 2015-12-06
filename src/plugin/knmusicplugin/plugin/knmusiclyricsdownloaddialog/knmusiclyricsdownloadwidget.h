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

#ifndef KNMUSICLYRICSDOWNLOADWIDGET_H
#define KNMUSICLYRICSDOWNLOADWIDGET_H

#include "knmusicutil.h"

#include <QWidget>

using namespace MusicUtil;

class QBoxLayout;
class QPushButton;
class KNLabelLineEdit;
class KNMusicLyricsDownloadList;
class KNMusicOnlineLyricsDownloader;
/*!
 * \brief The KNMusicLyricsDownloadWidget class is the content of the official
 * lyrics download dialog.
 */
class KNMusicLyricsDownloadWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct KNMusicLyricsDownloadWidget widget.
     * \param parent The parent widget.
     */
    explicit KNMusicLyricsDownloadWidget(QWidget *parent = 0);

signals:
    /*!
     * \brief When user clicked the search, this signal will be emitted for
     * requiring the message box to expand.
     */
    void requireExpand();

    /*!
     * \brief When user clicked cancel, this signal will be emitted for the
     * message box to launch cancel slot.
     */
    void requireCancel();

    /*!
     * \brief When there's any lyrics valid, this signal will be emitted to show
     * the okay button.
     */
    void requireShowOkayButton();

    /*!
     * \brief requireHideOkayButton
     */
    void requireHideOkayButton();

    /*!
     * \brief requireDownloadLyrics
     * \param detailInfo
     */
    void requireDownloadLyrics(KNMusicDetailInfo detailInfo);

public slots:
    /*!
     * \brief setDetailInfo
     * \param detailInfo
     */
    void setDetailInfo(const KNMusicDetailInfo &detailInfo);

    /*!
     * \brief saveSelectLyrics
     */
    void saveSelectLyrics();

private slots:
    void retranslate();
    void onActionSearch();

protected:
    /*!
     * \brief Reimplemented from QWidget::hideEvent().
     */
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionDownloadCancel();
    void onActionDownloadComplete();

private:
    inline void enableSearchEdit();
    inline KNLabelLineEdit *generateLineEdit(const QString &pixmapPath);
    KNMusicDetailInfo m_detailInfo;
    KNLabelLineEdit *m_title, *m_artist;
    QPushButton *m_searchLyrics;
    QBoxLayout *m_searchTextLayout;
    KNMusicLyricsDownloadList *m_downloadedLyrics;
    KNMusicOnlineLyricsDownloader *m_onlineDownloader;
};

#endif // KNMUSICLYRICSDOWNLOADWIDGET_H
