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
#ifndef KNMUSICDETAILPANEL_H
#define KNMUSICDETAILPANEL_H

#include <QWidget>

class KNHWidgetSwitcher;
class KNMusicDetailOverview;
class KNMusicDetailPanel : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicDetailPanel(QWidget *parent = 0);
    QSize sizeHint() const;

signals:

public slots:
    void setFilePath(const QString &filePath);

private:
    void initialOverview();
    KNHWidgetSwitcher *m_contents;
    KNMusicDetailOverview *m_overview;
};

#endif // KNMUSICDETAILPANEL_H
