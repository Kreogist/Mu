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
#ifndef KNMUSICMAINPLAYERCONTROL_H
#define KNMUSICMAINPLAYERCONTROL_H

#include <QWidget>

class QLabel;
class QGraphicsOpacityEffect;
class QTimeLine;
class KNEditableLabel;
class KNOpacityAnimeButton;
/*!
 * \brief The KNMusicMainPlayerControl class provides the widget for control the
 * current playing song and playing sequence.
 */
class KNMusicMainPlayerControl : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicMainPlayerControl widget.
     * \param parent The parent widget.
     */
    explicit KNMusicMainPlayerControl(QWidget *parent = nullptr);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QWidget::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private:
    inline KNOpacityAnimeButton *generateButton(
            const QString &imagePath=QString());
    inline void changeOpacity(int endFrame);
    QPixmap m_playIcon, m_pauseIcon;
    QTimeLine *m_changeOpacity;
    QGraphicsOpacityEffect *m_opacity;
    KNEditableLabel *m_position;
    QLabel *m_duration;
    KNOpacityAnimeButton *m_previous, *m_playNPause, *m_next;
    int m_currentOpacity;
};

#endif // KNMUSICMAINPLAYERCONTROL_H
