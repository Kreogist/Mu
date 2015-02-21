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
#ifndef KNMUSICLYRICSLISTVIEW_H
#define KNMUSICLYRICSLISTVIEW_H

#include <QModelIndex>

#include <QWidget>

class QLabel;
class QListView;
class QAbstractItemModel;
class KNRoundButtonBar;
class KNHWidgetSwitcher;
class KNMusicLyricsListView : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicLyricsListView(QWidget *parent = 0);
    ~KNMusicLyricsListView();
    void setLyricsModel(QAbstractItemModel *model);
    QModelIndex currentLyricsIndex() const;

    void addPreviewer(const QPixmap &icon, QWidget *widget);

signals:
    void lyricsActivate(const QModelIndex &index);

public slots:

private slots:
    void retranslate();
    void currentLyricsChanged(const QModelIndex &current,
                              const QModelIndex &previous);

private:
    inline void initialLyricsList();
    inline void initialLyricsPreviewMode();
    QListView *m_lyricsList;
    KNRoundButtonBar *m_previewModeBar;
    KNHWidgetSwitcher *m_previewWidgets;
};

#endif // KNMUSICLYRICSLISTVIEW_H
