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

#ifndef KNABSTRACTTABGROUP_H
#define KNABSTRACTTABGROUP_H

#include <QWidget>

class QAbstractButton;
class QSignalMapper;
/*!
 * \brief The KNAbstractTabGroup class provides you a way to manage such a
 * container for all checkable buttons. Like those KNPreferenceItem and
 * KNCategoryTab. When any different checked tab was clicked, then it will emit
 * a signal to notice to change the index of the tab content. All the tab button
 * add to this tab bar will be taken over the pressed signal.
 */
class KNAbstractTabGroup : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNAbstractTabGroup widget.
     * \param parent The parent widget.
     */
    explicit KNAbstractTabGroup(QWidget *parent = 0);

    /*!
     * \brief Add a tab to the tab group.
     * \param tab The tab button.
     */
    virtual void addTab(QAbstractButton *tab);

    /*!
     * \brief Get the current item index. If there's no item it will be -1.
     * \return The current item index.
     */
    int currentIndex() const;

    /*!
     * \brief Get whether any item of the tab group is selected.
     * \return If one of the item is selected, return true. If not or there is
     * no button, return false.
     */
    bool isButtonSelected() const;

    /*!
     * \brief Whether the group should automatically set the first adding button
     * to checked state.
     * \return True if the group automatically set it to checked. Default value
     * is true.
     */
    bool autoSelect() const;

signals:
    /*!
     * \brief When there's a new item clicked, this signal will be emitted.
     * \param index The new index.
     */
    void currentIndexChange(int index);

public slots:
    /*!
     * \brief Set the current item via index.
     * \param currentIndex The index of the item.
     */
    void setCurrentIndex(int currentIndex);

    /*!
     * \brief Set whether the group should auto select the first item when
     * adding the button.
     * \param autoSelect Set it to true for enabling this feature.
     */
    void setAutoSelect(bool autoSelect);

    /*!
     * \brief Reset the current selected item. Make none of the button is
     * selected.
     */
    void reset();

protected:
    /*!
     * \brief Get this button group is empty or not. The implement class should
     * provide this function for empty checking.
     * \return If the group is empty, return true. Or else false.
     */
    virtual bool isEmpty()=0;

    /*!
     * \brief Add a tab widget to the group. Tweak the UI of the group when add
     * the tab. Return the index of the button.
     * \param tab The tab button.
     * \return The tab button index.
     */
    virtual int addTabToWidget(QAbstractButton *tab)=0;

    /*!
     * \brief Get a tab widget via index.
     * \param index The tab index.
     * \return The tab widget pointer.
     */
    virtual QAbstractButton *tabAt(int index)=0;

private slots:
    void onMapped(int index);

private:
    QSignalMapper *m_itemMapper;
    int m_currentIndex;
    bool m_autoSelect;
};

#endif // KNABSTRACTTABGROUP_H
