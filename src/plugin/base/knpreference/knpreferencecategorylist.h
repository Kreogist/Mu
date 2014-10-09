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
#ifndef KNPREFERENCECATEGORYLIST_H
#define KNPREFERENCECATEGORYLIST_H

#include <QWidget>
#include <QList>

#include "knpreferencecategoryitem.h"

class QBoxLayout;
class QSignalMapper;
class KNPreferenceCategoryList : public QWidget
{
    Q_OBJECT
public:
    explicit KNPreferenceCategoryList(QWidget *parent = 0);
    QString categoryText(const int &index) const;
    int currentIndex() const;
    int itemCount() const;

signals:
    void currentIndexChanged(const int &index);

public slots:
    void addCategory(const QString &text, const QPixmap &pixmap);
    void setCategoryText(const int &index, const QString &caption);
    void setCurrentIndex(const int &index);

private slots:

private:
    QBoxLayout *m_mainLayout;
    QList<KNPreferenceCategoryItem *> m_itemList;
    QSignalMapper *m_buttonMapper;
    int m_currentIndex=-1;
};

#endif // KNPREFERENCECATEGORYLIST_H
