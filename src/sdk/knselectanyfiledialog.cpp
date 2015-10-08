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
#include <QListView>
#include <QTreeView>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPushButton>

#include "knselectanyfiledialog.h"

#include <QDebug>

KNSelectAnyFileDialog::KNSelectAnyFileDialog(QWidget *parent,
                                             const QString &caption,
                                             const QString &directory,
                                             const QString &filter) :
    QFileDialog(parent, caption, directory, filter),
    m_buttonBox(nullptr)
{
    //Configure the dialog.
    setFileMode(QFileDialog::Directory);
#ifdef Q_OS_MACX
    setWindowFlags(Qt::Sheet);
#endif
    setOptions(QFileDialog::DontUseNativeDialog);

    //Configure the listview.
    //Get the list view.
    QListView *listView=findChild<QListView *>("listview");
    //Check out the pointer.
    if(nullptr!=listView)
    {
        //Update the selection mode.
        listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
    //Configure the tree view.
    QTreeView *treeView=findChild<QTreeView *>();
    //Get the tree view.
    if(nullptr!=treeView)
    {
        //Update the selection mode.
        treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        connect(treeView->selectionModel(),
                &QItemSelectionModel::selectionChanged,
                this,
                &KNSelectAnyFileDialog::treeViewSelectionChanged);
    }
    //Get the button box.
    m_buttonBox=findChild<QDialogButtonBox *>("buttonBox");
    //Check out the button box.
    if(!m_buttonBox)
    {
        return;
    }
    //Get the okay button from the button group.
     QPushButton *button=m_buttonBox->button(QDialogButtonBox::Open);
     //Check out the button.
     if(!button)
     {
         return;
     }
     //Update the enabled state.
     button->setEnabled(false);
}

void KNSelectAnyFileDialog::treeViewSelectionChanged(
        const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected)
    //Check out the button box.
    if(!m_buttonBox)
    {
        return;
    }
    //Get the okay button from the button group.
     QPushButton *button=m_buttonBox->button(QDialogButtonBox::Open);
     //Check out the button.
     if(!button)
     {
         return;
     }
     //Update the enabled state.
     button->setEnabled(!selected.indexes().isEmpty());
}
