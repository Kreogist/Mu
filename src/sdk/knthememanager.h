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

#ifndef KNTHEMEMANAGER_H
#define KNTHEMEMANAGER_H

#include <QPalette>
#include <QLinkedList>

#include <QObject>

#define knTheme (KNThemeManager::instance())

/*!
 * \brief The KNThemeManager class is the global theme manager. It manages all
 * the palette of the widget according to the object name.
 */
class KNThemeManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Get the global instance of the theme manager.
     * \return The global theme manager instance.
     */
    static KNThemeManager *instance();

    /*!
     * \brief Initial the manager, generate the instance with the given parent
     * object.\n
     * Only the first time will create a instance.
     */
    static void initial(QObject *parent = 0);

    /*!
     * \brief Get the palette of a object according to the object name.
     * \param name The object name.
     * \return The palette of the specific object.
     */
    QPalette getPalette(const QString &name) const;

    /*!
     * \brief Register a widget in the theme widget list. The theme manager will
     * automatically update it's palette when a new theme list has been set.
     * \param widget The widget will be registered.
     */
    void registerWidget(QWidget *widget);

signals:
    /*!
     * \brief When the theme file changed, this signal will emit.
     */
    void themeChange();

public slots:
    /*!
     * \brief Load the palettes information from the specific theme dir.
     * \param themePath The theme directory path.
     */
    void loadThemeFiles(const QString &themeDirPath);

    /*!
     * \brief Set the current theme according to the index in the theme list.
     * \param index The theme index.
     */
    void setTheme(const int &index);

    /*!
     * \brief Set the theme according to the theme name.
     * \param themeName The theme name.
     */
    void setTheme(const QString &themeName);

private:
    static KNThemeManager *m_instance;
    explicit KNThemeManager(QObject *parent = 0);
    //Disable the copy of the instance.
    KNThemeManager(const KNThemeManager &);
    KNThemeManager(KNThemeManager &&);
    inline void addTheme(const QString &name,
                         const QString &path,
                         const QPixmap &preview);
    inline void loadTheme(const QString &themeFilePath);
    inline void parsePalette(const QString &name,
                             QJsonObject *data,
                             QHash<QString, QPalette> &map);

    struct ThemeItem
    {
        QString name;
        QString path;
        QPixmap preview;
        bool operator ==(const ThemeItem &value)
        {
            return value.name==name;
        }
    };

    QLinkedList<QWidget *> m_widgetList;
    QList<ThemeItem> m_themeList;
    int m_currentTheme;
    QHash<QString, QPalette> m_paletteList;
    QHash<QString, QPalette::ColorRole> m_colorRoleList;
};

#endif // KNTHEMEMANAGER_H
