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

#ifndef KNSAOSUBMENU_H
#define KNSAOSUBMENU_H

#include <QMenu>

class QLabel;
class QPropertyAnimation;
/*!
 * \brief The KNSaoSubMenu class provides a Sword Art Online style sub menu.
 */
class KNSaoSubMenu : public QMenu
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNSaoSubMenu menu.
     * \param parent The parent widget.
     */
    explicit KNSaoSubMenu(QWidget *parent = 0);
    ~KNSaoSubMenu();

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QMenu::showEvent().
     */
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QMenu::hideEvent().
     */
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;

private:
    inline void renderingIndicator(const QPixmap &indicator);
    QPixmap m_rawIndicator, m_rawInverseIndicator;
#ifndef Q_OS_MACX
    QPropertyAnimation *m_start;
#endif
    QWidget *m_indicator;
    QLabel *m_indicatorLabel;
};

#endif // KNSAOSUBMENU_H
