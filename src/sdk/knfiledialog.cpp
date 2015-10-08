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
#include <QTreeView>
#include <QListView>
#include <QScrollBar>
#include <QComboBox>
#include <QLayout>
#include <QSplitter>

#include "knthememanager.h"
#include "knmousesenseheader.h"
#include "sao/knsaostyle.h"

#include "knfiledialog.h"

KNFileDialog::KNFileDialog(QWidget *parent,
                           const QString &caption,
                           const QString &directory,
                           const QString &filter) :
    QFileDialog(parent, caption, directory, filter)
{
    //Set properties.
    setOptions(QFileDialog::DontUseNativeDialog);
#ifdef Q_OS_MACX
    setWindowFlags(Qt::Sheet);
#endif
    QPalette pal=knTheme->getPalette("FileDialog");
    setPalette(pal);

    //Configure the QSplitter.
    QSplitter *splitter=findChild<QSplitter *>("splitter");
    //Check out the pointer.
    if(nullptr!=splitter)
    {
        //Set properties.
        splitter->setHandleWidth(0);
    }
    //Configure the list view.
    //Get the list view.
    QListView *listView=findChild<QListView *>("listview");
    //Check out the pointer.
    if(nullptr!=listView)
    {
        //Set properties.
        listView->setFrameShape(QFrame::NoFrame);
        listView->setContentsMargins(0,0,0,0);
        listView->setLineWidth(0);
        listView->setUniformItemSizes(true);
        listView->viewport()->setContentsMargins(0,0,0,0);
    }
    //Configure the tree view.
    QTreeView *treeView=findChild<QTreeView *>();
    //Get the tree view.
    if(nullptr!=treeView)
    {
        //Set properties.
        treeView->setFrameStyle(QFrame::NoFrame);
        treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
        treeView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        treeView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        KNSaoStyle::styleVerticalScrollBar(treeView->verticalScrollBar());
        //Update the header widget of the treeview.
        treeView->setHeader(new KNMouseSenseHeader(treeView));
    }

    //Configure the combo box.
    QComboBox *fileTypeCombo=findChild<QComboBox *>();
    //Update the palette.
    fileTypeCombo->setPalette(pal);
}
