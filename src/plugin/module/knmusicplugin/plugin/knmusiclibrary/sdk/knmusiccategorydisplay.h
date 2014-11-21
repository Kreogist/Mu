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
#ifndef KNMUSICCATEGORYDISPLAY_H
#define KNMUSICCATEGORYDISPLAY_H

#include <QPixmap>

#include <QWidget>

class QLabel;
class QGraphicsOpacityEffect;
class KNSideShadowWidget;
class KNMusicLibraryModel;
class KNMusicLibraryTreeView;
class KNMusicCategoryDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicCategoryDisplay(QWidget *parent = 0);

signals:

public slots:
    void retranslate();
    void updateDetailInfo();
    void setLibraryModel(KNMusicLibraryModel *model);
    void showNoCategoryItem(const QString &title);
    void setCategoryText(const QString &text);
    void setCategoryIcon(const QPixmap &pixmap);
    void setCategoryColumn(const int &column);
    void scrollToRow(const int &row);
    void scrollToSourceRow(const int &row);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    void updateBackgroundIcon();
    QLabel *m_categoryTitle, *m_categoryInfo, *m_largeIcon;
    QGraphicsOpacityEffect *m_iconEffect;
    QRadialGradient m_iconGradient;
    KNSideShadowWidget *m_leftShadow;
    KNMusicLibraryTreeView *m_categoryTreeView;
    QString m_songCount[3], m_minuateCount[3], m_searchResultIn,
            m_searchCount[3];
};

#endif // KNMUSICCATEGORYDISPLAY_H
