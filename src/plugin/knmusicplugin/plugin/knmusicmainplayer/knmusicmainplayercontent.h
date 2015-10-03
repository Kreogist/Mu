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

#ifndef KNMUSICMAINPLAYERCONTENT_H
#define KNMUSICMAINPLAYERCONTENT_H

#include <QWidget>

class QParallelAnimationGroup;class QPropertyAnimation;
class KNMusicMainPlayerContentSwitcher;
class KNMusicMainPlayerContent : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicMainPlayerContent(QWidget *parent = 0);

signals:
    void columnCountChanged(int columnCount);

public slots:
    void setColumnCount(int columnCount);
    void setColumnWidget(const int &index, QWidget *widget);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void onActionSingleToPrevious();
    void onActionSingleToNext();
    void onActionSingleFinished();

private:
    inline void setWidgetVisible(int index, bool visible);
    inline void regeometryWidgets();
    inline void updateSinglePrevParameter();
    inline void updateSingleNextParameter();
    inline QPropertyAnimation *generateAnime();
    QPropertyAnimation *m_singleIn, *m_singleOut, *m_doublePlaylist;
    QParallelAnimationGroup *m_singleAnime;
    KNMusicMainPlayerContentSwitcher *m_singleSwitcher;
    QWidget *m_columnWidgets[3];
    int m_columnCount, m_currentIndex;
    bool m_movingPrevious;
};

#endif // KNMUSICMAINPLAYERCONTENT_H
