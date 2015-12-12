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

#ifndef KNMUSICDETAILDIALOG_H
#define KNMUSICDETAILDIALOG_H

#include <QLinkedList>
#include <QModelIndex>

#include "knmusicglobal.h"

#include "knmessagebox.h"

class QLabel;
class KNHTabGroup;
class KNHWidgetSwitcher;
class KNMusicProxyModel;
class KNMusicDetailDialogPanel;
class KNMusicDetailTagEditPanel;
/*!
 * \brief The KNMusicDetailDialog class provides a dialog to display the basic
 * information of a selected music.
 */
class KNMusicDetailDialog : public KNMessageBox
{
    Q_OBJECT
public:
    /*!
     * \brief Construct the KNMusicDetailDialog class.
     * \param parent The parent widget.
     */
    explicit KNMusicDetailDialog(QWidget *parent = 0);

    /*!
     * \brief Add detail dialog widget to dialog panel.
     * \param panel The panel widget.
     */
    void addPanel(KNMusicDetailDialogPanel *panel);

    /*!
     * \brief Get the tag edit panel from the detail dialog.
     * \return The tag edit panel pointer.
     */
    KNMusicDetailTagEditPanel *tagEditPanel() const;

    /*!
     * \brief Add the tag edit panel to the detail dialog.
     * \param tagEditPanel The panel.
     */
    void addTagEditPanel(KNMusicDetailTagEditPanel *tagEditPanel);

signals:

public slots:
    /*!
     * \brief Show the dialog and display the detail info of the specific file.
     * \param analysisItem The analysis item of the file.
     * \param proxyModel The proxy model of the item.
     * \param proxyIndex The index of the item in the proxy model.
     */
    void showDialog(KNMusicAnalysisItem analysisItem,
                    KNMusicProxyModel *proxyModel,
                    const QModelIndex &proxyIndex);

private slots:
    void onActionUpdateFileInfo();

private:
    //Basic Information.
    enum BasicInformation
    {
        LabelAlbumArt,
        LabelName,
        LabelDuration,
        LabelArtist,
        LabelAlbum,
        BasicInformationCount
    };
    QLabel *m_basicInfoLabel[BasicInformationCount];

    inline void updateAnalysisItem(const KNMusicAnalysisItem &analysisItem);
    //Panel list.
    QLinkedList<KNMusicDetailDialogPanel *> m_panelList;
    //Proxy model and current index.
    QModelIndex m_proxyIndex;
    KNMusicProxyModel *m_proxyModel;
    //Panel switcher and container.
    KNHTabGroup *m_panelSwitcher;
    KNHWidgetSwitcher *m_panelContainer;
    //Special panels.
    KNMusicDetailTagEditPanel *m_tagEditPanel;
};

#endif // KNMUSICDETAILDIALOG_H
