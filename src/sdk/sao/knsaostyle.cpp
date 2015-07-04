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
#include <QWidget>

#include "knsaostyle.h"

KNSaoStyle::KNSaoStyle() :
    QCommonStyle()
{

}

void KNSaoStyle::styleVerticalScrollBar(QWidget *widget)
{
    widget->setStyleSheet("QScrollBar:vertical {"
                          "   border: 0px solid grey;"
                          "   background: rgba(0, 0, 0, 0);"
                          "   width: 8px;"
                          "}"
                          "QScrollBar::handle:vertical {"
                          "   background: rgba(100, 100, 100);"
                          "   min-height: 10px;"
                          "   border-radius: 4px;"
                          "}"
                          "QScrollBar::add-line:vertical {"
                          "   border: 0px solid grey;"
                          "   background: rgba(0, 0, 0, 100);"
                          "   height: 0px;"
                          "   subcontrol-position: down;"
                          "   subcontrol-origin: margin;"
                          "}"
                          "QScrollBar::sub-line:vertical {"
                          "   border: 0px solid grey;"
                          "   background: rgba(0, 0, 0, 100);"
                          "   height: 0px;"
                          "   subcontrol-position: up;"
                          "   subcontrol-origin: margin;"
                          "}");
}

