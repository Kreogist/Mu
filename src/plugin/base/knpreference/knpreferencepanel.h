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
#ifndef KNPREFERENCEPANEL_H
#define KNPREFERENCEPANEL_H

#include <QLinkedList>

#include <QWidget>

class QBoxLayout;
class KNAnimeCheckedButton;
class KNLocaleManager;
class KNConfigure;
class KNCategoryPlugin;
class KNCategoryPreference;
class KNPreferenceLanguageItem;
class KNPreferenceLanguagePanel;
class KNPreferenceContents;
class KNPreferenceCategory;
class KNPreferenceItemPanel;
class KNPreferenceGeneral;
class KNPreferencePanel : public QWidget
{
    Q_OBJECT
public:
    explicit KNPreferencePanel(QWidget *parent = 0);

signals:
    void requireUpdateInfrastructure();
    void requireHidePreference();

public slots:
    void addLanguageButton(KNAnimeCheckedButton *languageButton,
                           const QPixmap &headerIcon,
                           QWidget *panel);
    int addCategory(KNCategoryPlugin *plugin);
    void loadConfigures();
    void saveConfigures();
    void setCategoryText(const int &index,
                         const QString &title);
    void setCurrentIndex(const int &index);
    void retranslate();

private slots:
    void onActionCategoryIndexChange(const int &index);

private:
    inline void initialLanguagePanel();
    inline void addCategoryPreference(KNCategoryPreference *preference);
    KNLocaleManager *m_localeManager;
    QBoxLayout *m_layout;
    KNPreferenceLanguageItem *m_languageItem;
    KNPreferenceLanguagePanel *m_languagePanel;
    KNPreferenceCategory *m_categoryList;
    KNPreferenceContents *m_contents;
    KNPreferenceGeneral *m_generalPreference;

    QLinkedList<KNPreferenceItemPanel *> m_panelList;
};

#endif // KNPREFERENCEPANEL_H
