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

#ifndef KNPREFERENCELANGUAGEPANEL_H
#define KNPREFERENCELANGUAGEPANEL_H

#include <QScrollArea>

class QBoxLayout;
class QSignalMapper;
class KNConfigure;
class KNPreferenceLanguageItem;
/*!
 * \brief The KNPreferenceLanguagePanel class provides the preference item and
 * the language item panel.\n
 * When the language changed, it will emit the signal for updating the title of
 * preference header, and update the language in the language panel.
 */
class KNPreferenceLanguagePanel : public QScrollArea
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNPreferenceLanguagePanel widget.
     * \param parent The parent widget.
     */
    explicit KNPreferenceLanguagePanel(QWidget *parent = 0);

    /*!
     * \brief Get the language list item widegt of the panel.
     * \return The panel widget pointer.
     */
    KNPreferenceLanguageItem *languageListItem() const;

signals:
    /*!
     * \brief Require to update the header title bar text.
     * \param headerText The new title bar text.
     */
    void requireUpdateTitle(const QString &headerText);

public slots:
    /*!
     * \brief Generate the language list from language manager.
     */
    void generateLanguageList();

private slots:
    void setCurrentLanguage(int index);

private:
    inline void syncLanguageItem(int index);
    KNPreferenceLanguageItem *m_languageListItem;
    QBoxLayout *m_mainLayout;
    QSignalMapper *m_languageRequests;
    KNConfigure *m_globalConfigure;
};

#endif // KNPREFERENCELANGUAGEPANEL_H
