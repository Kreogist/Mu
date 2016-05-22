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

#ifndef KNMUSICCATEGORYLISTVIEWBASE_H
#define KNMUSICCATEGORYLISTVIEWBASE_H

#include <QListView>

class QTimeLine;
class KNMusicCategorySearch;
/*!
 * \brief The KNMusicCategoryListViewBase class provides the mouse in and out
 * background detection and drag n drop enabled switcher. Many music category
 * list view widget will be implemented from this class, like the playlist list
 * view.
 */
class KNMusicCategoryListViewBase : public QListView
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicCategoryListViewBase class.
     * \param parent The parent widget.
     */
    explicit KNMusicCategoryListViewBase(QWidget *parent = 0);

    /*!
     * \brief Set the search place holder text when the text box is empty.
     * \param text The place holder text.
     */
    void setSearchPlaceHolderText(const QString &text);

    /*!
     * \brief Enabled a search action to the list view.\n
     * Notice: You can only call this function once.
     */
    void enabledSearch();

signals:
    /*!
     * \brief Ask for searching in the category model.
     * \param text The keyword to search.
     */
    void requireSearchCategory(const QString &text);

public slots:
    /*!
     * \brief Hide the search staff.
     */
    void hideSearchBar();

protected:
    /*!
     * \brief Enabled drag and drop features of the list view. Once you enabled
     * this feature, you can't disabled it.
     */
    void enabledDragDrop();

    /*!
     * \brief Reimpelemnt from QListView::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimpelemnt from QListView::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimpelemnt from QListView::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimpelemnt from QListView::keyPressEvent().
     */
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionPaletteChange();
    void onActionSearch();
    void onActionMouseInOut(int frame);
    void onActionSearchInOut(int frame);

private:
    inline void startMouseAnime(int endFrame);
    inline void startSearchAnime(int endFrame);
    QPalette m_palette;
    QColor m_backgroundColor, m_textColor, m_buttonColor;
    QTimeLine *m_mouseAnime, *m_searchAnime;
    QScrollBar *m_scrollBar;
    KNMusicCategorySearch *m_searchBox;
};

#endif // KNMUSICCATEGORYLISTVIEWBASE_H
