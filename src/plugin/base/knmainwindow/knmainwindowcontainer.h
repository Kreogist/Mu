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
#ifndef KNMAINWINDOWCONTAINER_H
#define KNMAINWINDOWCONTAINER_H

#include <QWidget>

class QGraphicsOpacityEffect;
class QPropertyAnimation;
class QParallelAnimationGroup;
class KNMainWindowContainer : public QWidget
{
    Q_OBJECT
public:
    explicit KNMainWindowContainer(QWidget *parent = 0);
    void setHeaderWidget(QWidget *widget);
    void setCategoryStack(QWidget *widget);
    void setCategorySwitcher(QWidget *widget);
    void setPreferencePanel(QWidget *widget);

signals:

public slots:
    void showPreference();
    void hidePreference();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    void refreshGeometry();
    void refreshAnimeStartValue();
    void refreshShowEndValue();
    void refreshHideEndValue();
    void configureOpacityAnime(QPropertyAnimation *anime);
    void configureGeometryAnime(QPropertyAnimation *anime);
    QWidget *m_header=nullptr,
            *m_categoryStack=nullptr,
            *m_categorySwitcher=nullptr,
            *m_preferencePanel=nullptr;
    QGraphicsOpacityEffect *m_settingsEffect;
    QPropertyAnimation *m_headerPositionAnime,
                       *m_categoryPositionAnime,
                       *m_settingPositionAnime,
                       *m_settingOpacityAnime;
    QParallelAnimationGroup *m_preferenceAnimeGroup;
};

#endif // KNMAINWINDOWCONTAINER_H
