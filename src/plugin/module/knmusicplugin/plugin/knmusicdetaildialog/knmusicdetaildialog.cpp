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
#include "knmusicdetailpanel.h"

#include "knmessagebox.h"

#include "knmusicdetaildialog.h"

KNMusicDetailDialog::KNMusicDetailDialog(QObject *parent) :
    KNMusicDetailDialogBase(parent)
{
    //Initial the dialog.
    m_detailDialog=new KNMessageBox;
    m_detailDialog->setTitle("Information");

    m_detailPanel=new KNMusicDetailPanel(m_detailDialog);
    m_detailDialog->setContent(m_detailPanel);
}

KNMusicDetailDialog::~KNMusicDetailDialog()
{
    delete m_detailDialog;
}

void KNMusicDetailDialog::showDialog(const QString &filePath)
{
    //Set the file path to the dialog.
    m_detailPanel->setFilePath(filePath);
    //Launch the detail dialog.
    m_detailDialog->exec();
}
