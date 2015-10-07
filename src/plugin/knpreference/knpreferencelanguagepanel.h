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
class KNPreferenceLanguageItem;
class KNPreferenceLanguagePanel : public QScrollArea
{
    Q_OBJECT
public:
    explicit KNPreferenceLanguagePanel(QWidget *parent = 0);

    KNPreferenceLanguageItem *languageListItem() const;

signals:

public slots:
    void generateLanguageList();

private slots:
    void setCurrentLanguage(const int &index);

private:
    inline void syncLanguageItem(const int &index);
    KNPreferenceLanguageItem *m_languageListItem;
    QBoxLayout *m_mainLayout;
    QSignalMapper *m_languageRequests;
};

#endif // KNPREFERENCELANGUAGEPANEL_H
