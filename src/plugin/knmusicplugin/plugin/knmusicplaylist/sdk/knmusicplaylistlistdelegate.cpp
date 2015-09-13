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
#include <QPen>
#include <QPainter>

#include "kncancellineedit.h"

#include "knmusicplaylistlistdelegate.h"

#define ItemHeight 26
#define IconSize 24
#define Spacing 3

int KNMusicPlaylistListDelegate::m_hoverRow=-1;
KNMusicPlaylistListDelegate::IndicatorPosition
    KNMusicPlaylistListDelegate::m_indicator=
        KNMusicPlaylistListDelegate::OutOfItem;

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
                            QPainter::SmoothPixmapTransform, true);
    //Set the default text color.
    QColor textColor=option.palette.color(QPalette::Text);
    //Draw the background part of the widget.
    if(option.state & QStyle::State_Selected)
    {
        //Draw the high light background.
        painter->fillRect(option.rect,
                          option.palette.highlight());
        //Update the text color to highlighted color.
        textColor=option.palette.color(QPalette::HighlightedText);
    }
    //Draw the icon.
    index.data(Qt::DecorationRole).value<QIcon>().paint(
                painter,
                option.rect.x()+Spacing,
                option.rect.y()+((option.rect.height()-IconSize)>>1),
                IconSize,
                IconSize);
    //Draw the text.
    painter->setPen(textColor);
    painter->setFont(option.font);
    painter->drawText(QRect(option.rect.x()+IconSize+(Spacing<<1),
                            option.rect.y(),
                            option.rect.width()-IconSize-(Spacing<<1),
                            option.rect.height()),
                      Qt::AlignLeft | Qt::AlignVCenter,
                      index.data(Qt::DisplayRole).toString());

    //Check whether the select item is the current item.
    if(index.row()==m_hoverRow)
    {
        //Initial the pen.
        QPen indicatorPen(option.palette.color(QPalette::HighlightedText));
        //Configure the pen.
        indicatorPen.setWidth(2);
        //Set the pen.
        painter->setPen(indicatorPen);
        //Clear the brush.
        painter->setBrush(Qt::NoBrush);
        //Check the position.
        switch(m_indicator)
        {
        case OnItem:
            painter->setPen(indicatorPen);
            painter->drawRect(option.rect);
            break;
        case AboveItem:
            painter->drawLine(option.rect.x(),
                              option.rect.y(),
                              option.rect.width(),
                              option.rect.y());
            break;
        case BelowItem:
            painter->drawLine(option.rect.x(),
                              option.rect.bottom()+1,
                              option.rect.width(),
                              option.rect.bottom()+1);
            break;
        default:
            break;
        }
    }
}

QSize KNMusicPlaylistListDelegate::sizeHint(const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const
{
    //Set the item size as the fixed item height.
    return QSize(QStyledItemDelegate::sizeHint(option, index).width(),
                 ItemHeight);
}

QWidget *KNMusicPlaylistListDelegate::createEditor(
        QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    Q_UNUSED(index)
    //Generate the caption editor.
    KNCancelLineEdit *captionEditor=new KNCancelLineEdit(parent);

    //Hide the border of the editor.
    QPalette editorPalette=option.palette;
    editorPalette.setColor(QPalette::WindowText, QColor(0,0,0,0));
    editorPalette.setColor(QPalette::Button, QColor(0,0,0,0));
    captionEditor->setPalette(editorPalette);
    //Set the size of the editor.
    /*
     * This resize is meant to fixed this bug:
     * If your caption is too long in the tree view, the editor will be the same
     * width as your text is, and it's IMPOSSIBLE for user to move the cursor to
     * the end of the caption.
     * Set the size fixed to the left space of the treeview to avoid the
     * QListView resize the editor to the maximum size.
     */
    //To reduce 2 px more is only UI tweaks to make the border inside the view.
    captionEditor->setFixedSize(
                parent->size().width()-IconSize-(Spacing<<1)-2,
                option.rect.height());
    //Link the caption editor editing finished signal.
    //Sorry, but you cannot use lambda here.
    connect(captionEditor, &KNCancelLineEdit::editingEnsure,
            this, &KNMusicPlaylistListDelegate::commitAndCloseEditor);
    //Give back the caption line edit.
    return captionEditor;
}

void KNMusicPlaylistListDelegate::setEditorData(QWidget *editor,
                                                const QModelIndex &index) const
{
    //Cast the editor to be the line edit.
    (static_cast<KNCancelLineEdit *>(editor))->setText(
                //Set the text of the editor to be the edit role.
                index.data(Qt::EditRole).toString());
}

void KNMusicPlaylistListDelegate::setModelData(QWidget *editor,
                                               QAbstractItemModel *model,
                                               const QModelIndex &index) const
{
    //Set model data to editor's text.
    KNCancelLineEdit *captionEditor=static_cast<KNCancelLineEdit *>(editor);
    //Check the data, avoid the empty data.
    if(!captionEditor->text().isEmpty())
    {
        //Set the data.
        model->setData(index, captionEditor->text(), Qt::EditRole);
    }
}

void KNMusicPlaylistListDelegate::commitAndCloseEditor()
{
    //Cast the sender as line edit widget.
    KNCancelLineEdit *editor=static_cast<KNCancelLineEdit *>(sender());
    //Commit the data to the model first.
    emit commitData(editor);
    //Close the editor.
    emit closeEditor(editor, NoHint);
}

void KNMusicPlaylistListDelegate::setIndicator(const IndicatorPosition &indicator)
{
    m_indicator = indicator;
}

void KNMusicPlaylistListDelegate::setHoverRow(int hoverRow)
{
    m_hoverRow = hoverRow;
}
