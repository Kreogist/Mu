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

#ifndef KNMUSICLYRICSDOWNLOADWIDGET_H
#define KNMUSICLYRICSDOWNLOADWIDGET_H

#include <QWidget>

class QBoxLayout;
class QPushButton;
class KNLabelLineEdit;
class KNMusicLyricsDownloadList;
class KNMusicLyricsDownloadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicLyricsDownloadWidget(QWidget *parent = 0);

signals:
    void requireExpand();

public slots:
    void setParameter(const QString &title=QString(),
                      const QString &artist=QString());

private slots:
    void retranslate();
    void onActionSearch();

private:
    inline KNLabelLineEdit *generateLineEdit(const QString &pixmapPath);
    KNLabelLineEdit *m_title, *m_artist;
    QPushButton *m_searchLyrics;
    QBoxLayout *m_searchTextLayout;
    KNMusicLyricsDownloadList *m_downloadLyrics;
};

#endif // KNMUSICLYRICSDOWNLOADWIDGET_H
