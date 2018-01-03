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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNFILENAMEMESSAGEBOX_H
#define KNFILENAMEMESSAGEBOX_H

#include "knmessagebox.h"

class QLabel;
class KNFileNameLineEdit;
/*!
 * \brief The KNFileNameMessageBox class provides a message box for inputing the
 * file name. This message box will check the file name validation.
 */
class KNFileNameMessageBox : public KNMessageBox
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNFileNameMessageBox widget.
     * \param parent The parent widget.
     */
    explicit KNFileNameMessageBox(QWidget *parent = 0);

    /*!
     * \brief Get a new file name.
     * \param text The display text of the message dialog.
     * \param title The title of the message dialog.
     * \param filePath The path of the file.
     * \return The file name content returns from the message box.
     */
    static QString getFileName(const QString &text,
                               const QString &title,
                               const QString &filePath);

    /*!
     * \brief Get the user input file name.
     * \return The file name text.
     */
    QString fileName() const;

public slots:
    /*!
     * \brief Set the whole file path, and display the file name at the line
     * edit widget.
     * \param filePath The file path.
     */
    void setFilePath(const QString &filePath);

    /*!
     * \brief Set the hint display text.
     * \param hintText The hint text string.
     */
    void setHintText(const QString &hintText);

protected:
    /*!
     * \brief Reimplemented from KNMessageBox::okayPressed().
     */
    bool okayPressed() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMessageBox::cancelPressed().
     */
    void cancelPressed() Q_DECL_OVERRIDE;

private slots:
    void onActionStateChanged(int state);

private:
    QLabel *m_hintText;
    KNFileNameLineEdit *m_lineEdit;
    QString m_newFileName;
};

#endif // KNFILENAMEMESSAGEBOX_H
