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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNSHORTCUTEDITOR_H
#define KNSHORTCUTEDITOR_H

#include <QKeySequenceEdit>

/*!
 * \brief The KNShortcutEditor class provides a line edit widget to display the
 * key that pressed. Once the user first release the key, the key sequence will
 * be the action's key sequence.\n
 * There are several key bindings for this editor: ESC - cancel the setting,
 * Backspace or delete - clear the setting.
 */
class KNShortcutEditor : public QKeySequenceEdit
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNShortcutEditor widget.
     * \param parent The parent widget.
     */
    explicit KNShortcutEditor(QWidget *parent = 0);

    /*!
     * \brief Whether the editor is currently editing.
     * \return If the editor is enable for the editing, return ture.
     */
    bool isEditing() const;

signals:
    /*!
     * \brief When the Backspace is pressed, the clear the shortcut signal is
     * emitted.
     */
    void clearShortcut();
    void cancel();

public slots:
    /*!
     * \brief Start to edit the key sequence.
     */
    void startEditing();

protected:
    /*!
     * \brief Reimplemented from KNShortcutEditor::keyPressEvent().
     */
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    inline void cancelEdit();
    bool m_editing;
};

#endif // KNSHORTCUTEDITOR_H
