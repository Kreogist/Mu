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

#ifndef KNMUSICTREEVIEWHEADERMENU_H
#define KNMUSICTREEVIEWHEADERMENU_H

#include "knmusicglobal.h"

#include "knanimationmenu.h"

class QSignalMapper;
/*!
 * \brief The KNMusicTreeViewHeaderMenu class provides a fixed content menu for
 * music tree view header to manage the visibility of all the columns and tweak
 * the width of the column.
 */
class KNMusicTreeViewHeaderMenu : public KNAnimationMenu
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicTreeViewHeaderMenu widget.
     * \param parent The parent widget.
     */
    explicit KNMusicTreeViewHeaderMenu(QWidget *parent = 0);

    /*!
     * \brief Set the visible state of a visible control action.
     * \param logicalIndex The index of the action.
     * \param visible The visible state.
     */
    void setVisibleState(const int &logicalIndex,
                         const bool &visible);

signals:
    /*!
     * \brief When one column is being asked to resize its size to the hint,
     * this signal will be emitted.
     * \param index The index of the action.
     */
    void requireResize(const int &index);

    /*!
     * \brief When tweak all width action is triggered, this signal will be
     * emitted.
     */
    void requireResizeAll();

    /*!
     * \brief When the visible property of one column is being asked to be
     * changed, this signal will be emitted.
     * \param column The column index.
     * \param ok To make the column visible, make this signal to be true.
     */
    void requireSetColumnVisible(const int &column,
                                 const bool &ok);

public slots:
    /*!
     * \brief Set the logical index where the mouse pressed down.
     * \param index The logical index.
     */
    void setMouseDownLogicalIndex(const int &index);

    /*!
     * \brief Set the visibility of the column control action.
     * \param index The index of column.
     * \param actionVisible The visible of the action.
     */
    void setActionVisible(const int &index, const bool &actionVisible);

private slots:
    void retranslate();
    void onActionVisibleChange(const int &index);
    void onActionResizeCurrent();

private:
    void createWidthTweakMenu();
    void createVisibleMenu();
    QAction *m_tweakAllWidth, *m_tweakWidth, *m_itemVisible[MusicDataCount];
    QSignalMapper *m_visibleMapper;
    int m_mouseDownLogicalIndex;
};

#endif // KNMUSICTREEVIEWHEADERMENU_H
