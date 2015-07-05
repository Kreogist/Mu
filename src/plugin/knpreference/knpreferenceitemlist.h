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

#ifndef KNPREFERENCEITEMLIST_H
#define KNPREFERENCEITEMLIST_H

#include "knabstracttabgroup.h"

class QBoxLayout;
/*!
 * \brief The KNPreferenceItemList class provides a widget which will contains
 * all the prefercen items.
 */
class KNPreferenceItemList : public KNAbstractTabGroup
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNPreferenceItemList widget.
     * \param parent The parent widget.
     */
    explicit KNPreferenceItemList(QWidget *parent = 0);

    /*!
     * \brief Get the specific text of the item at index.
     * \param index The item index.
     * \return The item caption text.
     */
    QString itemText(const int &index);

protected:
    /*!
     * \brief Reimplement from KNAbstractTabGroup::isEmpty().
     */
    bool isEmpty();

    /*!
     * \brief Reimplemnt from KNAbstractTabGroup::addTabToWidget().
     */
    int addTabToWidget(QAbstractButton *tab);

    /*!
     * \brief Reimplement from KNAbstractTabGroup::tabAt().
     */
    QAbstractButton *tabAt(const int &index);

private:
    QBoxLayout *m_mainLayout;
};

#endif // KNPREFERENCEITEMLIST_H
