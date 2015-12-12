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
class KNFileNameMessageBox : public KNMessageBox
{
    Q_OBJECT
public:
    explicit KNFileNameMessageBox(QWidget *parent = 0);

    static QString getFileName(const QString &text,
                               const QString &title,
                               const QString &filePath);

    QString fileName() const;

public slots:
    void setFilePath(const QString &filePath);

    void setHintText(const QString &hintText);

protected:
    /*!
     * \brief okayPressed
     * \return
     */
    bool okayPressed() Q_DECL_OVERRIDE;

    /*!
     * \brief cancelPressed
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
