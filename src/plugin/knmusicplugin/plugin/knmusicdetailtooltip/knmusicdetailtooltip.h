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

#ifndef KNMUSICDETAILTOOLTIP_H
#define KNMUSICDETAILTOOLTIP_H

#include <QIcon>

#include "knmusicutil.h"
#include "knconnectionhandler.h"

#include "knmusicdetailtooltipbase.h"

using namespace MusicUtil;

class QTimeLine;
class KNScrollLabel;
class KNOpacityButton;
class KNProgressSlider;
class KNMusicBackend;
class KNMusicAlbumLabel;
/*!
 * \brief The KNMusicDetailTooltip class is a default realization of the
 * KNMusicDetailTooltip class. It provides the basic information of a song and
 * preview listening for the music.
 */
class KNMusicDetailTooltip : public KNMusicDetailTooltipBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicDetailTooltip with a given parent.
     * \param parent The parent widget.
     */
    explicit KNMusicDetailTooltip(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMusicDetailTooltipBase::showTooltip().
     */
    void showTooltip(const QPoint &position) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicDetailTooltipBase::setPreviewIndex().
     */
    void setPreviewIndex(KNMusicModel *musicModel,
                         const QModelIndex &index) Q_DECL_OVERRIDE;

protected:
    /*!
     * \brief Reimplemented from KNMusicDetailTooltipBase::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicDetailTooltipBase::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicDetailTooltipBase::focusOutEvent().
     */
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionHide();
    void onActionMouseInOut(int frame);
    void onActionThemeChanged();
    void onActionPlayNPauseClicked();

    void onActionPreviewPositionChanged(const qint64 &position);
    void onActionPreviewDurationChanged(const qint64 &position);
    void onActionPreviewStateChange(const int &state);

private:
    inline QTimeLine *generateTimeLine(const int &endFrame);
    inline void startAnime(QTimeLine *timeLine);
    inline void moveToPosition(const QPoint &position);
    inline void resetCounter();

    void loadPreview();

    enum ToolTipItems
    {
        ItemTitle,
        ItemTime,
        ItemArtist,
        ToolTipItemsCount
    };

    QTimer *m_fadeOutCounter;
    QTimeLine *m_mouseIn, *m_mouseOut;

    KNMusicAlbumLabel *m_albumArt;
    KNScrollLabel *m_labels[ToolTipItemsCount];
    KNOpacityButton *m_playNPause;
    KNProgressSlider *m_progress;
    QIcon m_playIcon, m_pauseIcon;

    bool m_isPlaying, m_progressPressed;
    KNMusicDetailInfo m_detailInfo;
    KNConnectionHandler m_backendHandler;
};

#endif // KNMUSICDETAILTOOLTIP_H
