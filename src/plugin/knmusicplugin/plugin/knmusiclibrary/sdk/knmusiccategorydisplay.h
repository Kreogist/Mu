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

#ifndef KNMUSICCATEGORYDISPLAY_H
#define KNMUSICCATEGORYDISPLAY_H

#include <QWidget>

class QGraphicsOpacityEffect;
class QLabel;
class KNMusicTab;
class KNSideShadowWidget;
class KNMusicLibraryTreeView;
class KNMusicLibraryModel;
class KNMusicCategoryDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicCategoryDisplay(QWidget *parent = 0, KNMusicTab *tab = 0);

signals:

public slots:
    void setLibraryModel(KNMusicLibraryModel *model);
    void scrollToSourceRow(const int &row);
    void setCategoryText(const QString &text);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void retranslate();
    void onActionSearch();

private:
    inline void updateBackgroundIcon();
    QRadialGradient m_iconGradient;
    QString m_songCount[3], m_minuateCount[3], m_searchResultIn,
            m_searchCount[3];
    QLabel *m_largeIcon;
    QGraphicsOpacityEffect *m_iconEffect;
    QLabel *m_categoryTitle, *m_categoryInfo;
    KNMusicLibraryTreeView *m_categoryTreeView;
    KNSideShadowWidget *m_leftShadow;
};

#endif // KNMUSICCATEGORYDISPLAY_H
