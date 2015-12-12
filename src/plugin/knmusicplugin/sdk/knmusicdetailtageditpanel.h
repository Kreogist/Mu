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

#ifndef KNMUSICDETAILTAGEDITPANEL_H
#define KNMUSICDETAILTAGEDITPANEL_H

#include "knmusicdetaildialogpanel.h"

class QComboBox;
class QLabel;
class QPushButton;
class KNCircleIconButton;
class KNLabelLineEdit;
class KNMusicRatingEditor;
/*!
 * \brief The KNMusicDetailTagEditPanel class provide the tag editor panel
 * widget.\n
 * It will have two modes: Basic and Advanced. In the basic mode, you can simply
 * edit all kinds of tags without worry about which kind of tag you are editing.
 * In the advanced mode, you can edit definitly one type of the tag.
 */
class KNMusicDetailTagEditPanel : public KNMusicDetailDialogPanel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct KNMusicDetailTagEditPanel widget.
     * \param parent The parent widget.
     */
    explicit KNMusicDetailTagEditPanel(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicDetailDialogPanel::tabButton().
     */
    QAbstractButton *tabButton() Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMusicDetailDialogPanel::setAnalysisItem().
     */
    void setAnalysisItem(const KNMusicAnalysisItem &item,
                         KNMusicProxyModel *proxyModel,
                         const QModelIndex &proxyIndex) Q_DECL_OVERRIDE;

private slots:
    void retranslate();
    void onActionItemChanged();
    void onActionWriteTag();

private:
    enum DetailRows
    {
        NameRow,
        ArtistRow,
        AlbumRow,
        AlbumArtistRow,
        ComposerRow,
        CommentRow,
        DetailRowCount
    };
    enum LeftColumnRows
    {
        GenreRow,
        RatingRow,
        BpmRow,
        LeftColumnRowCount
    };
    enum RightColumnRows
    {
        YearRow,
        TrackRow,
        DiscRow,
        RightColumnRowCount
    };

    inline KNLabelLineEdit *generateLineEdit();
    KNMusicAnalysisItem m_analysisItem;
    QModelIndex m_proxyIndex;
    KNMusicProxyModel *m_proxyModel;
    QLabel *m_rowLabel[DetailRowCount], *m_leftRowLabel[LeftColumnRowCount],
           *m_rightRowLabel[RightColumnRowCount];
    KNLabelLineEdit *m_rowEditor[DetailRowCount], *m_trackEditor[2],
                    *m_discEditor[2], *m_yearEditor, *m_bpmEditor;
    QLabel *m_trackEditorOf, *m_discEditorOf;
    QComboBox *m_genreEditor;
    KNMusicRatingEditor *m_ratingEditor;
    KNCircleIconButton *m_button;
    QPushButton *m_writeTag;
};

#endif // KNMUSICDETAILTAGEDITPANEL_H
