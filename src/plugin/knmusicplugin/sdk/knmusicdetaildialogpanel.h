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
#ifndef KNMUSICDETAILDIALOGPANEL_H
#define KNMUSICDETAILDIALOGPANEL_H

#include <QWidget>

class QAbstractButton;
class KNMusicDetailDialogPanel : public QWidget
{
    Q_OBJECT
public:
    KNMusicDetailDialogPanel(QWidget *parent = 0):QWidget(parent){}

    virtual QAbstractButton *tabButton()=0;

signals:

public slots:
    virtual void setFilePath(const QString &filePath,
                             const QString &indexFilePath=QString(),
                             const int &index=-1)=0;
};

#endif // KNMUSICDETAILDIALOGPANEL_H
