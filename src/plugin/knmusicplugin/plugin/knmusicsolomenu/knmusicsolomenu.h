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

#ifndef KNMUSICSOLOMENU_H
#define KNMUSICSOLOMENU_H

#include <QModelIndex>

#include "knmusicsolomenubase.h"

/*!
 * \brief The KNMusicSoloMenu class is a default realized of the
 * KNMusicSoloMenuBase. This will use all the default sdk. This is a example of
 * the solo menu.
 */
class KNMusicSoloMenu : public KNMusicSoloMenuBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicSoloMenu object.
     * \param parent The parent widget.
     */
    explicit KNMusicSoloMenu(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicSoloMenuBase::setMusicRow().
     */
    void setMusicRow(KNMusicProxyModel *model,
                     const QModelIndex &itemIndex) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicSoloMenuBase::triggeredModel().
     */
    KNMusicProxyModel *triggeredModel() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicSoloMenuBase::triggeredIndex().
     */
    QModelIndex triggeredIndex() const Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMusicSoloMenuBase::appendMusicActions().
     */
    void appendMusicActions(const QList<QAction *> &actions) Q_DECL_OVERRIDE;

private slots:
    void retranslate();
    void onActionOpen();
    void onActionGetInfo();
    void onActionShowInGraphicShell();
    void onActionSearchItemText();
    void onActionDownloadLyrics();
    void onActionRenameToPrefer();
    void onActionRename();
    void onActionCopyFilePath();
    void onActionCopyItemText();
    void onActionRemove();

private:
    enum SoloMenuActions
    {
        PlayCurrent,
        Open,
        GetInfo,
        ShowInGraphicShell,
        SearchItemText,
        DownloadLyrics,
        CopyFilePath,
        CopyItemText,
        Rename,
        RenameToArtistHyphonName,
        Delete,
        SoloMenuActionCount
    };
    inline void renameFile(const QString &newName);
    QAction *m_actions[SoloMenuActionCount];
    QString m_actionTitles[SoloMenuActionCount];

    KNMusicProxyModel *m_model;
    QModelIndex m_itemIndex;
    QString m_itemText, m_filePath, m_preferFileName;
};

#endif // KNMUSICSOLOMENU_H
