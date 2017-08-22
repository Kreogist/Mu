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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNPREFERENCEPANEL_H
#define KNPREFERENCEPANEL_H

#include "knpreferenceutil.h"

#include <QWidget>

using namespace PreferenceUtil;

class QBoxLayout;
class QScrollBar;
class QTimeLine;
class KNPreferencePanelItem;
/*!
 * \brief The KNPreferencePanel class provides a panel which could load a
 * setting panel item list and generate the widget for the top level widgets.
 */
class KNPreferencePanel : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNPreferencePanel widget.
     * \param parent The parent widget.
     */
    explicit KNPreferencePanel(QWidget *parent = 0);


signals:

public slots:
    /*!
     * \brief Set the panel blocks to the panel list. The panel list will
     * genereate the widget to the panel.
     * \param blocks The block list.
     */
    void setPanelBlocks(const QList<PreferencePanelBlock> &blocks);

    /*!
     * \brief Set whether the advanced items should be shown or not.
     * \param isShow To show the items, set the value to true.
     */
    void setAdvancedItemShown(bool isShow);

    /*!
     * \brief Set the scroll of the preference panel.
     * \param scrollBar The scroll bar widget pointer.
     */
    void setScrollBar(QScrollBar *scrollBar);

protected:
    /*!
     * \brief Reimplemented from QWidget::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;


private slots:
    void onMouseInOut(int frame);

private:
    inline void startAnime(int endFrame);
    inline KNPreferencePanelItem *generateItem(
            const PreferencePanelOption &option);
    QFont m_titleFont;
    QList<QWidget *> m_advancedItems;
    QBoxLayout *m_mainLayout;
    QTimeLine *m_mouseAnime;
    QScrollBar *m_scrollBar;
    int m_currentFrame;
};

#endif // KNPREFERENCEPANEL_H
