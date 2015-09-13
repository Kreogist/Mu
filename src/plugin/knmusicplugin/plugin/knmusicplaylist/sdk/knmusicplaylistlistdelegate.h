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

#ifndef KNMUSICPLAYLISTLISTDELEGATE_H
#define KNMUSICPLAYLISTLISTDELEGATE_H

#include <QStyledItemDelegate>

/*!
 * \brief The KNMusicPlaylistListDelegate class is designed to tweak the UI of
 * the playlist list view. It fixed the bug of the line edit of the original
 * list view.
 */
class KNMusicPlaylistListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    enum IndicatorPosition
    {
        AboveItem,
        OnItem,
        BelowItem,
        OutOfItem
    };

    /*!
     * \brief Construct a KNMusicPlaylistListDelegate widget.
     * \param parent The parent widget.
     */
    explicit KNMusicPlaylistListDelegate(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from QStyledItemDelegate::paint().
     */
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QStyledItemDelegate::sizeHint().
     */
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const Q_DECL_OVERRIDE;

    /*!
     * \brief  Reimplemented from QStyledItemDelegate::createEditor().
     */
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    /*!
     * \brief  Reimplemented from QStyledItemDelegate::setEditorData().
     */
    void setEditorData(QWidget *editor,
                       const QModelIndex &index) const Q_DECL_OVERRIDE;

    /*!
     * \brief  Reimplemented from QStyledItemDelegate::setModelData().
     */
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const Q_DECL_OVERRIDE;

    static void setHoverRow(int hoverRow);

    static void setIndicator(const IndicatorPosition &indicator);

signals:

public slots:

private slots:
    void commitAndCloseEditor();

private:
    static int m_hoverRow;
    static IndicatorPosition m_indicator;
};

#endif // KNMUSICPLAYLISTLISTDELEGATE_H
