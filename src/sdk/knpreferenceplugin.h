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

#ifndef KNPREFERENCEPLUGIN_H
#define KNPREFERENCEPLUGIN_H

#include <QWidget>

class KNPreferenceItem;
/*!
 * \brief The KNPreferencePlugin class is the main preference panel. It's only a
 * port widget. You should reimplement it and fill those pure virtual methods.\n
 */
class KNPreferencePlugin : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct KNPreferencePlugin class.
     * \param parent The parent widget.
     */
    KNPreferencePlugin(QWidget *parent = 0):QWidget(parent){}

    /*!
     * \brief Add a preference tab to prefernce plugin. It should provides two
     * widgets: a tab widget and a content widget.
     * \param tabWidget The tab widget.
     * \param content The content widget.
     */
    virtual void addTab(KNPreferenceItem *tabWidget, QWidget *content)=0;

signals:
    /*!
     * \brief When the header button clicked, this signal will be emitted to ask
     * to close the preference.
     */
    void requireClosePreference();

public slots:
};

#endif // KNPREFERENCEPLUGIN_H
