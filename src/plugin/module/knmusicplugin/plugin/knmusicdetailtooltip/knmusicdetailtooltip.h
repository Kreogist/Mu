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

#include "knmusicdetailtooltipbase.h"

class QBoxLayout;
class QTimer;
class QLabel;
class QTimeLine;
class KNFilePathLabel;
class KNOpacityButton;
class KNConnectionHandler;
class KNProgressSlider;
class KNMusicDetailTooltipArtwork;
class KNMusicDetailTooltip : public KNMusicDetailTooltipBase
{
    Q_OBJECT
public:
    explicit KNMusicDetailTooltip(QWidget *parent = 0);
    void setBackend(KNMusicBackend *backend);

signals:

public slots:
    void showTooltip();
    void setPreviewIndex(KNMusicModel *musicModel,
                         const QPersistentModelIndex &index,
                         const QPoint &position);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void focusOutEvent(QFocusEvent *event);

private slots:
    void onActionHide();
    void onActionMouseInOut(const int &frame);
    void onActionPlayNPauseClick();
    void onActionPreviewStatusChange(const int &state);
    void onActionProgressPressed();
    void onActionProgressReleased();
    void onActionPreviewPositionChanged(const qint64 &position);
    void onActionPreviewDurationChanged(const qint64 &duration);
    void startDisappearCountWithAnime();

private:
    enum ToolTipItems
    {
        ItemTitle,
        ItemTime,
        ItemArtist,
        ToolTipItemsCount
    };

    inline void linkBackend();
    inline void cutLinkWithBackend();

    inline void loadCurrentToPreview();
    inline void resetDisappearCounter();
    inline void resetPreviewPlayer();
    inline void initialTimeLine(QTimeLine *timeline);
    inline void setEliedText(QLabel *label, const QString &text);
    inline void moveToPosition(const QPoint &position);
    int m_tooltipWidth=448, m_tooltipHeight=176,
        m_labelWidth=m_tooltipWidth-m_tooltipHeight-11;
    bool m_isButtonPlay=true, m_progressPressed=false;
    QBoxLayout *m_previewPlayer;
    QColor m_backgroundColor=QColor(255,255,255),
           m_textColor=QColor(255,255,255);
    KNMusicDetailTooltipArtwork *m_albumArt;
    QLabel *m_labels[ToolTipItemsCount];
    KNFilePathLabel *m_fileName;
    KNConnectionHandler *m_backendHandler;
    QPalette m_palette;
    QTimer *m_disappearCounter;
    QPersistentModelIndex m_currentIndex;
    KNMusicModel *m_currentMusicModel;
    QTimeLine *m_mouseIn, *m_mouseOut;
    KNMusicBackend *m_backend;
    KNOpacityButton *m_playNPause;
    KNProgressSlider *m_progress;
    QPixmap m_playIcon, m_pauseIcon;
    QPolygonF m_imageShape;
};

#endif // KNMUSICDETAILTOOLTIP_H
