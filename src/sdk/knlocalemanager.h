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

#ifndef KNLOCALEMANAGER_H
#define KNLOCALEMANAGER_H

#include <QHash>
#include <QList>
#include <QPixmap>

#include <QObject>

#define knI18n (KNLocaleManager::instance())

class QTranslator;
/*!
 * \brief The KNLocaleManager class is the global locale manager. It can
 * provides real-time langauge retranslate signal and file management.\n
 * The KNLocaleManager will only load one locale file for translation.
 */
class KNLocaleManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Get the instance of the locale manager.
     * \return The instance of the locale manager.
     */
    static KNLocaleManager *instance();

    /*!
     * \brief Construct a KNLocaleManager class.
     * \param parent The parent object.
     */
    explicit KNLocaleManager(QObject *parent = 0);

    /*!
     * \brief Load the information about languages, load all the data about the
     * language file.
     * \param languageDir The language directory.
     */
    void loadLanguageFiles(const QString &languageDir);

signals:
    /*!
     * \brief When a new language file is loaded, this signal will emitted.
     */
    void languageChange();


public slots:
    /*!
     * \brief Set current langauge via the language key.
     * \param key The language key.
     */
    void setLanguage(const QString &key);

    /*!
     * \brief Set the langauge according to the index in the language list.
     * \param index The language index.
     */
    void setLanguage(const int &index);

    /*!
     * \brief Set the language according to the system locale.
     */
    void setDefaultLanguage();

private:
    static KNLocaleManager *m_instance;
    struct LanguageItem
    {
        QString key;
        QString name;
        QString filePath;
        QPixmap icon;
        bool operator ==(const LanguageItem &value)
        {
            return key==value.key;
        }
    };

    inline void loadLanguage(const QString &filePath);
    inline void loadLanguageInFolder(const QString &dirPath);
    inline void addLanguage(const QString &key,
                            const QString &name,
                            const QString &filePath,
                            const QPixmap &icon);
    QTranslator *m_translator;
    QPixmap m_noLanguageIcon;
    int m_currentLangauge;
    QHash<QString, QString> m_languageTranslation;
    QList<LanguageItem> m_languageList;
};

#endif // KNLOCALEMANAGER_H
