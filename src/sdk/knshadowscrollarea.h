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

#ifndef KNSHADOWSCROLLAREA_H
#define KNSHADOWSCROLLAREA_H

#include <QScrollArea>

class KNSideShadowWidget;
/*!
 * \brief The KNShadowScrollArea class provides you a scroll area with side
 * shadow. When the vertical scroll bar is enabled, then the top and bottom
 * shadow will be shown.
 */
class KNShadowScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNShadowScrollArea widget.
     * \param parent The parent widget.
     */
    explicit KNShadowScrollArea(QWidget *parent = 0);

signals:

public slots:

protected:
    /*!
     * \brief Reimplement from QScrollArea::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event);

private slots:
    void onActionVerticalRangeChanged(int min, int max);
    void onActionVerticalValueChanged(int value);

private:
    KNSideShadowWidget *m_topShadow, *m_bottomShadow;
    static int m_shadowSize;
};

#endif // KNSHADOWSCROLLAREA_H
