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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#ifndef KNEDITABLELABEL_H
#define KNEDITABLELABEL_H

#include <QLabel>

class KNCancelLineEdit;
class KNEditableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit KNEditableLabel(QWidget *parent = 0);
    bool editing() const;
    void setEditing(bool editing);

signals:
    void editingFinished();

public slots:

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void onActionEditFinished();
    void onActionEditCancel();

private:
    void showEdit();
    void hideEdit();
    bool m_pressed=false, m_editing=false;
    KNCancelLineEdit *m_edit;
};

#endif // KNEDITABLELABEL_H
