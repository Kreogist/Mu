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

class KNPreferenceItem;
class KNPreferenceTitleBar;
class KNPreferenceItemList;
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

    /*!
     * \brief Add an item widget to sidebar.
     * \param item The preference item widget pointer.
     */
    void addItemWidget(KNPreferenceItem *item);

signals:
    /*!
     * \brief When the header button clicked, this signal will be emitted to ask
     * to close the preference.
     */
    void requireClosePreference();

    /*!
     * \brief When the current tab index is changed, this signal will be emitted
     * to ask to change the content to specific index.
     * \param tabIndex The tab index.
     */
    void requireChangeContent(int tabIndex);

public slots:

protected:
    /*!
     * \brief Reimplement from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event);

private slots:
    void retranslate();
    void onActionIndexChanged(const int &index);

private:
    inline void initialBottomBar();
    KNPreferenceTitleBar *m_titleBar;
    KNPreferenceItemList *m_itemList;
    KNLinearSenseWidget *m_bottomBar;
    KNSideShadowWidget *m_rightShadow;
    const int m_shadowWidth;
};

#endif // KNPREFERENCESIDEBAR_H
