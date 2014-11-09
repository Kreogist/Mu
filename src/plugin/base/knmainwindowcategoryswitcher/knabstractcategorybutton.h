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
#ifndef KNABSTRACTCATEGORYBUTTON_H
#define KNABSTRACTCATEGORYBUTTON_H

#include "knanimecheckedbutton.h"

class KNAbstractCategoryButton : public KNAnimeCheckedButton
{
    Q_OBJECT
public:
    explicit KNAbstractCategoryButton(QWidget *parent = 0);
    QSize sizeHint() const;
    QPixmap icon() const;
    QString text() const;
    void setIcon(const QPixmap &icon);
    void setText(const QString &text);

signals:

public slots:
    virtual void startInAnime()=0;
    virtual void startOutAnime()=0;

protected:
    void paintEvent(QPaintEvent *event);

private:
    int m_iconSize=64,
        m_margin=10,
        m_spacing=10;
    QPixmap m_icon;
    QString m_text;
};

#endif // KNABSTRACTCATEGORYBUTTON_H
