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

#ifndef KNMUSICMAINPLAYERPANEL_H
#define KNMUSICMAINPLAYERPANEL_H

#include "knmusicutil.h"

#include <QWidget>

using namespace MusicUtil;

class QLabel;
class KNLoopScrollLabel;
class KNHighLightLabel;
/*!
 * \brief The KNMusicMainPlayerPanel class
 */
class KNMusicMainPlayerPanel : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicMainPlayerPanel(QWidget *parent = 0);

signals:

public slots:
    void setAnalysisItem(const KNMusicAnalysisItem &item);

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    enum DetailLabels
    {
        SongLabel,
        ArtistLabel,
        AlbumLabel,
        GenreLabel,
        DetailLabelCount
    };
    QLabel *m_detailIcons[DetailLabelCount];
    KNLoopScrollLabel *m_detailText[DetailLabelCount];
    QWidget *m_detailPanel;
    KNHighLightLabel *m_albumArt;
};

#endif // KNMUSICMAINPLAYERPANEL_H
