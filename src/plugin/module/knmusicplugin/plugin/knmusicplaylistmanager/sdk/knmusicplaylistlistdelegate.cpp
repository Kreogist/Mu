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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <QLineEdit>
#include <QPainter>

#include "knmusicplaylistlistdelegate.h"

#include <QDebug>

KNMusicPlaylistListDelegate::KNMusicPlaylistListDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
}

void KNMusicPlaylistListDelegate::paint(QPainter *painter,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
    //Set antialiasing.
    painter->setRenderHints(QPainter::Antialiasing |
                            QPainter::TextAntialiasing |
                            QPainter::SmoothPixmapTransform,
                            true);
    QColor textColor=option.palette.color(QPalette::Text);
    //Draw the background part of the widget.
    if (option.state & QStyle::State_Selected)
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(option.palette.highlight());
        painter->drawRect(option.rect);
        textColor=option.palette.color(QPalette::HighlightedText);
    }
    //Draw the icon.
    QIcon icon=index.data(Qt::DecorationRole).value<QIcon>();
    icon.paint(painter,
               option.rect.x()+m_spacing,
               option.rect.y(),
               m_iconSize,
               m_iconSize);
    //Draw the text.
    QRect textRect=QRect(option.rect.x()+m_iconSize+(m_spacing<<1),
                         option.rect.y(),
                         option.rect.width()-m_iconSize-(m_spacing<<1),
                         option.rect.height());
    painter->setPen(textColor);
    painter->setFont(option.font);
    painter->drawText(textRect,
                      Qt::AlignLeft | Qt::AlignVCenter,
                      index.data(Qt::DisplayRole).toString());
}

QSize KNMusicPlaylistListDelegate::sizeHint(const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const
{
    return QSize(QStyledItemDelegate::sizeHint(option, index).width(),
                 m_itemHeight);
}

QWidget *KNMusicPlaylistListDelegate::createEditor(QWidget *parent,
                                                   const QStyleOptionViewItem &option,
                                                   const QModelIndex &index) const
{
    Q_UNUSED(index)
    //Generate the caption editor.
    QLineEdit *captionEditor=new QLineEdit(parent);

    captionEditor->setPalette(option.palette);
    //Set the size of the editor.
    /*
     * This resize is meant to fixed this bug:
     * If your caption is too long in the tree view, the editor will be the same
     * width as your text is, and it's IMPOSSIBLE for user to move the cursor to
     * the end of the caption.
     * Set the size fixed to the left space of the treeview to avoid the
     * QListView resize the editor to the maximum size.
     */
    captionEditor->setFixedSize(parent->size().width()-m_iconSize-(m_spacing<<1),
                                option.rect.height());
    connect(captionEditor, &QLineEdit::editingFinished,
            this, &KNMusicPlaylistListDelegate::commitAndCloseEditor);
    return captionEditor;
}

void KNMusicPlaylistListDelegate::setEditorData(QWidget *editor,
                                                const QModelIndex &index) const
{
    QLineEdit *captionEditor=static_cast<QLineEdit *>(editor);
    captionEditor->setText(index.data(Qt::DisplayRole).toString());
}

void KNMusicPlaylistListDelegate::setModelData(QWidget *editor,
                                               QAbstractItemModel *model,
                                               const QModelIndex &index) const
{
    //Set model data to editor's text.
    QLineEdit *captionEditor=static_cast<QLineEdit *>(editor);
    if(!captionEditor->text().isEmpty())
    {
        model->setData(index, captionEditor->text(), Qt::DisplayRole);
    }
}

void KNMusicPlaylistListDelegate::commitAndCloseEditor()
{
    QWidget *editor=static_cast<QWidget *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
