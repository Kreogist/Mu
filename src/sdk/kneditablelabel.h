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

#ifndef KNEDITABLELABEL_H
#define KNEDITABLELABEL_H

#include <QLabel>

class KNCancelLineEdit;
/*!
 * \brief The KNEditableLabel class provides you a editable label. You can click
 * the label and edit the content of the label.
 */
class KNEditableLabel : public QLabel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNEditableLabel widget with given parent.
     * \param parent Parent widget.
     */
    explicit KNEditableLabel(QWidget *parent = 0);

    /*!
     * \brief Check whether the label is being edited.
     * \return If the editor is visible, return true.
     */
    bool isEditing() const;

signals:
    /*!
     * \brief When the content of the label is changed because of the editor,
     * this signal will be emitted.
     */
    void contentChanged();

public slots:

protected:
    /*!
     * \brief Reimplemented from QLabel::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event);

    /*!
     * \brief Reimplemented from QLabel::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void showEditor();
    void hideEdior();

private:
    KNCancelLineEdit *m_editor;
    bool m_pressed;
};

#endif // KNEDITABLELABEL_H
