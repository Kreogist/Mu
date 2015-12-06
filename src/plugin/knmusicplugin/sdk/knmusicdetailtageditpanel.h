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

class QLabel;
class KNCircleIconButton;
class KNLabelLineEdit;
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
    QAbstractButton *tabButton();

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMusicDetailDialogPanel::setAnalysisItem().
     */
    void setAnalysisItem(const MusicUtil::KNMusicAnalysisItem &item);

private slots:
    void retranslate();

private:
    enum DetailRows
    {
        NameRow,
        DetailRowsCount
    };

    inline KNLabelLineEdit *generateLineEdit();
    QLabel *m_rowLabel[DetailRowsCount];
    KNLabelLineEdit *m_rowEditor[DetailRowsCount];
    KNCircleIconButton *m_button;

};

#endif // KNMUSICDETAILTAGEDITPANEL_H
