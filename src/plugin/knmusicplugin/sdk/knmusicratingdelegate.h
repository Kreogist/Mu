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

#ifndef KNMUSICRATINGDELEGATE_H
#define KNMUSICRATINGDELEGATE_H

#include <QStyledItemDelegate>

/*!
 * \brief The KNMusicRatingDelegate class
 */
class KNMusicRatingDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit KNMusicRatingDelegate(QWidget *parent = 0);

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
     * \brief Reimplemented from QStyledItemDelegate::createEditor().
     */
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QStyledItemDelegate::setEditorData().
     */
    void setEditorData(QWidget *editor,
                       const QModelIndex &index) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QStyledItemDelegate::setModelData().
     */
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const Q_DECL_OVERRIDE;

    /*!
     * \brief This is going to get the star size. It will be constant.
     */
    static int starSizeHint();

signals:

public slots:

private slots:
    void commitAndCloseEditor();

private:
    QPixmap m_star;
    static int m_starSizeHint;
};

#endif // KNMUSICRATINGDELEGATE_H
