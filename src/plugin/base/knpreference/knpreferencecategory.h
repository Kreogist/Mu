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
#ifndef KNPREFERENCECATEGORY_H
#define KNPREFERENCECATEGORY_H

#include <QWidget>

class QBoxLayout;
class QTimeLine;
class KNPreferenceTitle;
class KNPreferenceCategory : public QWidget
{
    Q_OBJECT
public:
    explicit KNPreferenceCategory(QWidget *parent = 0);

signals:
    void requireHidePreference();

public slots:
    void retranslate();

protected:
    void paintEvent(QPaintEvent *event);

private:
    QColor m_backgroundColor=QColor(32,32,32),
           m_borderColor=QColor(255,255,255,67);
    int m_listWidth=250,
        m_borderWidth=4;
    QBoxLayout *m_layout;
    QString m_configureText;
    KNPreferenceTitle *m_title;
};

#endif // KNPREFERENCECATEGORY_H
