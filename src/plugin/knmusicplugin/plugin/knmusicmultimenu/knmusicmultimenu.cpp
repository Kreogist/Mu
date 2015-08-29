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
#include "knthememanager.h"
#include "knlocalemanager.h"

#include "knutil.h"

#include "knmusicproxymodel.h"
#include "knmusicmodel.h"

#include "knmusicmultimenu.h"

KNMusicMultiMenu::KNMusicMultiMenu(QWidget *parent) :
    KNMusicMultiMenuBase(parent)
{
    setObjectName("MusicMultiMenu");
    //Set the separator color.
    setSeparatorColor(QColor(255, 255, 255, 100));
    //Link to the theme manager.
    knTheme->registerWidget(this);
    //Generate the actions.
    for(int i=0; i<MultiMenuActionCount; ++i)
    {
        //Generate the action.
        m_actions[i]=new QAction(this);
    }
    //Link actions.
    connect(m_actions[CopyFilePaths], &QAction::triggered,
            this, &KNMusicMultiMenu::onActionCopyFilePaths);
    connect(m_actions[CopyItems], &QAction::triggered,
            this, &KNMusicMultiMenu::onActionCopyItems);
    connect(m_actions[Delete], &QAction::triggered,
            this, &KNMusicMultiMenu::onActionDelete);

    //Add actions to menu.
    addAction(m_actions[CopyFilePaths]);
    addAction(m_actions[CopyItems]);
    addSeparator();
    addAction(m_actions[Delete]);

    //Link the locale manager.
    knI18n->link(this, &KNMusicMultiMenu::retranslate);
    retranslate();
}

void KNMusicMultiMenu::setMusicRows(KNMusicProxyModel *model,
                                    const QList<int> &rows,
                                    const QModelIndex &clickedIndex)
{
    //Check the model and the item index.
    if((!model) || rows.isEmpty() || (!clickedIndex.isValid()))
    {
        return;
    }
    //Save model, rows and clicked item index.
    m_model=model->musicModel();
    m_rows=rows;
    m_itemIndex=model->mapToSource(clickedIndex);
    //Translate the row list.
    model->mapRowListToSource(m_rows);
    //Get row count number text.
    QString selectedRows=QString::number(rows.size());
    //Update item texts.
    m_actions[CopyItems]->setText(
                m_actionTitles[CopyItems].arg(selectedRows));
    m_actions[Delete]->setText(
                m_actionTitles[Delete].arg(selectedRows));
}

void KNMusicMultiMenu::retranslate()
{
    //Translate all the action caption. Save them in title caption string list.
    m_actionTitles[CopyFilePaths]=tr("Copy locations");
    m_actionTitles[CopyItems]=tr("Copy %1 items this column text");
    m_actionTitles[Delete]=tr("Delete %1 items");

    //Apply the translation.
    //Because you can't change the language when you operate the menu. so we
    //don't need to care with the data of %1.
    for(int i=0; i<MultiMenuActionCount; i++)
    {
        m_actions[i]->setText(m_actionTitles[i]);
    }
}

void KNMusicMultiMenu::onActionCopyFilePaths()
{
    //Generate the file paths content.
    QString filePaths;
    //Get all the paths from the model.
    for(int i : m_rows)
    {
        //Append the path and a return to the file paths
        filePaths.append(m_model->rowProperty(i, FilePathRole).toString() +
                         "\n");
    }
    //Set the data to clipboard.
    KNUtil::setClipboardText(filePaths);
}

void KNMusicMultiMenu::onActionCopyItems()
{
    //Generate the item text content.
    QString itemTextContent;
    const int column=m_itemIndex.column();
    //Get all the item text from the model.
    for(int i : m_rows)
    {
        //Append the item text to the text content.
        itemTextContent.append(m_model->textData(i, column));
    }
    //Set the data to clipboard.
    KNUtil::setClipboardText(itemTextContent);
}

void KNMusicMultiMenu::onActionDelete()
{
    //Check the music model first.
    if(!m_model)
    {
        return;
    }
    //Remove all the rows from the music model.
    m_model->removeRowList(m_rows);
}
