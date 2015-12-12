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
#ifndef KNMUSICDETAILDIALOGPANEL_H
#define KNMUSICDETAILDIALOGPANEL_H

#include <QModelIndex>

#include "knmusicutil.h"

#include <QWidget>

using namespace MusicUtil;

class QAbstractButton;
class KNMusicProxyModel;
/*!
 * \brief The KNMusicDetailDialogPanel class provides the basic functions of a
 * detail dialog panel should have. It's a pure virtual port class.
 */
class KNMusicDetailDialogPanel : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicDetailDialogPanel class.
     * \param parent The parent widget.
     */
    KNMusicDetailDialogPanel(QWidget *parent = 0):QWidget(parent){}

    /*!
     * \brief The tab button. When this button get pushed, it will switch to
     * this panel.
     * \return The button widget pointer.
     */
    virtual QAbstractButton *tabButton()=0;

signals:
    /*!
     * \brief When the file is updated, this signal will be emitted for detail
     * info to updated the file.
     */
    void requireUpdateFileInfo();

public slots:
    /*!
     * \brief Set the analysis item. It provides the file path and some other
     * basic parameters.
     * \param item The analysis item. It will be provided by the detail dialog.
     * \param proxyModel The proxy model of the item.
     * \param proxyIndex The index of the item in the proxy model.
     */
    virtual void setAnalysisItem(const KNMusicAnalysisItem &item,
                                 KNMusicProxyModel *proxyModel,
                                 const QModelIndex &proxyIndex)=0;
};

#endif // KNMUSICDETAILDIALOGPANEL_H
