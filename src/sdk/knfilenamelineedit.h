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

#ifndef KNFILENAMELINEEDIT_H
#define KNFILENAMELINEEDIT_H

#include <QDir>

#include "knlabellineedit.h"

/*!
 * \brief The KNFileNameLineEdit class provide a label line edit widget which
 * can detect a file name is valid or not. Set a file path to the line edit and
 * it could auto check whether the file path is valid or not.
 */
class KNFileNameLineEdit : public KNLabelLineEdit
{
    Q_OBJECT
public:
    enum FileNameStates
    {
        NotChange,
        Valid,
        Invalid,
        Exist,
        NoFileName,
        Empty,
        SuffixChange
    };

    /*!
     * \brief Construct a KNFileNameLineEdit widget.
     * \param parent The parent widget.
     */
    explicit KNFileNameLineEdit(QWidget *parent = 0);

    /*!
     * \brief Get the current state.
     * \return The current file name line edit.
     */
    int state() const;

signals:
    /*!
     * \brief When the line edit state changed, this signal will be emitted.
     * \param state The new state of the file name.
     */
    void stateChanged(int state);

public slots:
    /*!
     * \brief Set the file path for the line edit. It will automatically detect
     * the folder and check the new file name is contains or not.\n
     * When the file path is not exist, it will ignore the file.
     * \param filePath The file path.
     */
    void setFilePath(const QString &filePath);

private slots:
    void onActionTextChanged(const QString &fileName);

private:
    inline void setState(int state);

    QString m_fileName, m_suffix;
    QDir m_directory;
    int m_state;
};

#endif // KNFILENAMELINEEDIT_H
