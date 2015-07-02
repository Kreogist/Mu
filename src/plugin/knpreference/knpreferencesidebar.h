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

#ifndef KNPREFERENCESIDEBAR_H
#define KNPREFERENCESIDEBAR_H

#include <QWidget>

class KNPreferenceTitleBar;
class KNLinearSenseWidget;
class KNSideShadowWidget;
/*!
 * \brief The KNPreferenceSidebar class is the official preference sidebar. It
 * contains a header button, the category item list, and a bottom bar. The
 * header will display the current tab's icon, title and back button. The item
 * list will display the category item list title.
 */
class KNPreferenceSidebar : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct the KNPreferenceSidebar widget with a given parent.
     * \param parent The parent widget.
     */
    explicit KNPreferenceSidebar(QWidget *parent = 0);

signals:
    /*!
     * \brief When the header button clicked, this signal will be emitted to ask
     * to close the preference.
     */
    void requireClosePreference();

public slots:

protected:
    /*!
     * \brief Reimplement from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event);

private:
    inline void initialBottomBar();
    KNPreferenceTitleBar *m_titleBar;
    KNLinearSenseWidget *m_bottomBar;
    KNSideShadowWidget *m_rightShadow;
    const int m_shadowWidth;
};

#endif // KNPREFERENCESIDEBAR_H
