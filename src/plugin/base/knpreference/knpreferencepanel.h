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

#include <QWidget>

class QBoxLayout;
class KNLocaleManager;
class KNAnimeCheckedButton;
class KNPreferenceLanguageItem;
class KNPreferenceLanguagePanel;
class KNPreferenceContents;
class KNPreferenceCategory;
class KNPreferenceWidgetsPanel;
class KNPreferencePanel : public QWidget
{
    Q_OBJECT
public:
    explicit KNPreferencePanel(QWidget *parent = 0);

signals:
    void requireSavePreference();
    void requireHidePreference();

public slots:
    void addLanguageButton(KNAnimeCheckedButton *languageButton,
                           const QPixmap &headerIcon,
                           QWidget *panel);
    int addCategory(const QString &title,
                    const QPixmap &icon,
                    const QPixmap &headerIcon,
                    KNPreferenceWidgetsPanel *contentWidget);
    void setCategoryText(const int &index,
                         const QString &title);
    void setCurrentIndex(const int &index);
    void retranslate();

private slots:
    void onActionCategoryIndexChange(const int &index);

private:
    void initialLanguagePanel();
    KNLocaleManager *m_localeManager;
    QBoxLayout *m_layout;
    KNPreferenceLanguageItem *m_languageItem;
    KNPreferenceLanguagePanel *m_languagePanel;
    KNPreferenceCategory *m_categoryList;
    KNPreferenceContents *m_contents;
    KNPreferenceWidgetsPanel *m_generalPanel;
};

#endif // KNPREFERENCEPANEL_H
