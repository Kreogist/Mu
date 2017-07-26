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

#ifndef KNMUSICCATEGORYSEARCH_H
#define KNMUSICCATEGORYSEARCH_H

#include <QWidget>

class KNSearchBox;
class QPushButton;
/*!
 * \brief The KNMusicCategorySearch class provides a widget for the category
 * list to do the search. It will provides a search box and a cancel button.
 * When the text is changed, it will emit the text to do the instant search.
 */
class KNMusicCategorySearch : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicCategorySearch widget.
     * \param parent The parent widget.
     */
    explicit KNMusicCategorySearch(QWidget *parent = 0);

    /*!
     * \brief Set the place holder text of the search box.
     * \param text The place holder text.
     */
    void setPlaceHolderText(const QString &text);

signals:
    /*!
     * \brief When the search box content changed, this signal is emitted to
     * start the search.
     * \param keyword The text of the search box.
     */
    void requireSearch(const QString &keyword);

    /*!
     * \brief When the cancel button is clicked, this signal will be emitted.
     */
    void requireCancel();

public slots:
    /*!
     * \brief Clear the content of the search box.
     */
    void clear();

private slots:
    void retranslate();
    void onActionCancel();

private:
    KNSearchBox *m_searchBox;
    QPushButton *m_cancelButton;
};

#endif // KNMUSICCATEGORYSEARCH_H
