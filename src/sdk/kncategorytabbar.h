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
class QTimeLine;
/*!
 * \brief The KNCategoryTabBar class provides a container of the KNCategoryTab.
 * It will placed several tabs in horizontal way. It will use q box layout to
 * place the tab.
 */
class KNCategoryTabBar : public KNAbstractTabGroup
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNCategoryTabBar widget.
     * \param parent The parent widget.
     */
    explicit KNCategoryTabBar(QWidget *parent = 0);

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

    /*!
     * \brief Reimplemented from KNAbstractTabGroup::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNAbstractTabGroup::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onMouseInOut(int frame);

private:
    inline void startAnime(int endFrame);
    QTimeLine *m_mouseAnime;
    QBoxLayout *m_mainLayout;
};

#endif // KNCATEGORYTABBAR_H
