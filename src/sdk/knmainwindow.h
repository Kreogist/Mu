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

#ifndef KNMAINWINDOW_H
#define KNMAINWINDOW_H

#include <QMainWindow>

class KNConfigure;
/*!
 * \brief The KNMainWindow class provides a main application window for mu.\n
 * It contains several parts: header, main widget and preference.
 *  * Header\n
 * The header parts of the application. Managed header player and search box.
 *  * Main widget\n
 * The main part of the widget. It contains the categories and music shops.
 *  * Preference\n
 * The preference panel. It contains all the preference settings.
 */
class KNMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /*!
     * \brief Construct KNMainWindow class.
     * \param parent The parent widget object.
     */
    explicit KNMainWindow(QWidget *parent = 0);

signals:

public slots:

protected:
    void closeEvent(QCloseEvent *event);

private slots:

private:
    inline void recoverGeometry();
    inline void backupGeometry();
    inline int getCacheValue(const QString &valueName);
    inline void setCacheValue(const QString &valueName, const int &value);
    inline void zoomParameter(int &parameter, const qreal &ratio);
    KNConfigure *m_cacheConfigure;
};

#endif // KNMAINWINDOW_H
