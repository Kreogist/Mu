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

#include <QWidget>

class QBoxLayout;
class QSignalMapper;
class KNPreferenceItem;
/*!
 * \brief The KNPreferenceItemList class provides a widget which will contains
 * all the prefercen items. When an item was clicked, then it will emit a signal
 * to notice to change the index of the content.
 */
class KNPreferenceItemList : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNPreferenceItemList widget.
     * \param parent The parent widget.
     */
    explicit KNPreferenceItemList(QWidget *parent = 0);

    /*!
     * \brief Add preference item widget to the list. It will take over the
     * control of the click signal of the item.
     * \param item The preference item widget.
     */
    void addItemWidget(KNPreferenceItem *item);

    /*!
     * \brief Get the specific text of the item at index.
     * \param index The item index.
     * \return The item caption text.
     */
    QString itemText(const int &index);

    /*!
     * \brief Get the current item index. If there's no item it will be -1.
     * \return The current item index.
     */
    int currentIndex() const;

signals:
    /*!
     * \brief When there's a new item clicked, this signal will be emitted.
     * \param index The new index.
     */
    void currentIndexChange(int index);

public slots:

    void setCurrentIndex(const int &currentIndex);

private:
    inline KNPreferenceItem *itemAt(const int &index);
    QSignalMapper *m_itemMapper;
    QBoxLayout *m_mainLayout;
    int m_currentIndex;
};

#endif // KNPREFERENCEITEMLIST_H
