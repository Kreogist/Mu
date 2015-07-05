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

#ifndef KNCATEGORYTABBAR_H
#define KNCATEGORYTABBAR_H

#include "knabstracttabgroup.h"

class QBoxLayout;
/*!
 * \brief The KNCategoryTabBar class provides a container of the KNCategoryTab.
 */
class KNCategoryTabBar : public KNAbstractTabGroup
{
    Q_OBJECT
public:
    explicit KNCategoryTabBar(QWidget *parent = 0);

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

#endif // KNCATEGORYTABBAR_H
