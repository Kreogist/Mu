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

#ifndef KNMUSICDETAILPANELARTWORK_H
#define KNMUSICDETAILPANELARTWORK_H

#include "knmusicdetaildialogpanel.h"

class KNCircleIconButton;

class QLabel;
class KNMusicDetailPanelArtwork : public KNMusicDetailDialogPanel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicDetailPanelArtwork widget.
     * \param parent The parent widget.
     */
    explicit KNMusicDetailPanelArtwork(QWidget *parent = 0);

    /*!
     * \brief Reimplement from KNMusicDetailDialogPanel::tabButton.
     */
    QAbstractButton *tabButton() Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief Reimplement from KNMusicDetailDialogPanel::setAnalysisItem.
     */
    void setAnalysisItem(const MusicUtil::KNMusicAnalysisItem &item)
    Q_DECL_OVERRIDE;

private:
    KNCircleIconButton *m_button;
    QLabel *m_albumArt;
};

#endif // KNMUSICDETAILPANELARTWORK_H
