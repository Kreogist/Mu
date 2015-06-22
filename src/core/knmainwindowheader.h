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

#ifndef KNMAINWINDOWHEADER_H
#define KNMAINWINDOWHEADER_H

#include "knmainwindowheaderbase.h"

class QTimeLine;
class KNMainWindowIconButton;
/*!
 * \brief The KNMainWindowHeader class is a default realize of the main window
 * header.
 */
class KNMainWindowHeader : public KNMainWindowHeaderBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMainWindowHeader widget.
     * \param parent
     */
    explicit KNMainWindowHeader(QWidget *parent = 0);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private slots:
    void changeBackgroundColor(const int &frame);
    void onActionPaletteChanged();

private:
    inline QTimeLine *generateTimeline();
    QTimeLine *m_mouseInOut;
    KNMainWindowIconButton *m_iconButton;
    int m_rangeStart, m_rangeEnd;
};

#endif // KNMAINWINDOWHEADER_H
