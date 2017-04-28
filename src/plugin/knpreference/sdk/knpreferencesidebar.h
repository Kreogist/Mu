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

class QBoxLayout;
class QLabel;
class QSignalMapper;
class KNPreferenceItem;
class KNRoundSwitchButton;
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

    /*!
     * \brief Add an item widget to sidebar at the fixed layout area. The item
     * at fixed area won't be shown at scroll bar area.
     * \param item The preference item widget poitner.
     */
    void addFixedItemWidget(KNPreferenceItem *item);

signals:
    /*!
     * \brief When the header button clicked, this signal will be emitted to ask
     * to close the preference.
     */
    void requireClosePreference();

    /*!
     * \brief When the current tab index is changed, this signal will be emitted
     * to ask to change the content to specific index.
     * \param tabIndex The content tab index.
     */
    void requireChangeContent(int tabIndex);

    /*!
     * \brief When the current panel index is changed, this signal will be
     * emitted to ask to change the panel data to specific index.\n
     * The different of content and panel is that the panel is a specific
     * setting panel which would be dynamicly loaded. The content is a fixed
     * widget panel.
     * \param panelIndex The panel index.
     */
    void requireChangePanel(int panelIndex);

    /*!
     * \brief When advanced setting switch is toggled, this signal will be
     * emitted.
     * \param toggle The toggle state of the advanced switch button.
     */
    void advancedToggle(bool toggle);

public slots:
    /*!
     * \brief Set the header text of the title bar.
     * \param text The header text.
     */
    void setHeaderText(const QString &text);

    /*!
     * \brief Set the item button title of a specific item.
     * \param index The item index.
     * \param title The title of the item button.
     */
    void setItemTitle(int index, const QString &title);

    /*!
     * \brief Update the sidebar title text. When the current language changed,
     * this function should be called.
     */
    void updateTitleBarText();

    /*!
     * \brief Set the advanced option is enabled or not.
     * \param shown To enabled the advanced option, set it to true.
     */
    void setAdvancedShown(bool shown);

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event);

private slots:
    void retranslate();
    void updateRoundedButtonPalette();
    void onActionIndexChanged(int index);
    void onActionFixedIndexChanged(int index);

private:
    inline void initialBottomBar();
    inline void setTitleBarContent(KNPreferenceItemList *list,
                                   int index);
    KNPreferenceTitleBar *m_titleBar;
    KNPreferenceItemList *m_fixedItemList, *m_itemList;
    KNLinearSenseWidget *m_bottomBar;
    KNRoundSwitchButton *m_advancedButton;
    KNSideShadowWidget *m_rightShadow;
    QLabel *m_advancedLabel;
    const int m_shadowWidth;
};

#endif // KNPREFERENCESIDEBAR_H
