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

#ifndef KNHTABGROUP_H
#define KNHTABGROUP_H

#include "knabstracttabgroup.h"

class QBoxLayout;
/*!
 * \brief The KNHTabGroup class will placed several button in horizontal. This
 * is a little the same as KNCategoryTabBar. But all the widget will be placed
 * at alignment right. This class is mostly used at SAO styled elements.
 */
class KNHTabGroup : public KNAbstractTabGroup
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNHTabGroup widget.
     * \param parent The parent widget.
     */
    explicit KNHTabGroup(QWidget *parent = 0);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from KNAbstractTabGroup::isEmpty().
     */
    bool isEmpty() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemnt from KNAbstractTabGroup::addTabToWidget().
     */
    int addTabToWidget(QAbstractButton *tab) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNAbstractTabGroup::tabAt().
     */
    QAbstractButton *tabAt(int index) Q_DECL_OVERRIDE;

private:
    QBoxLayout *m_mainLayout;
};

#endif // KNHTABGROUP_H
