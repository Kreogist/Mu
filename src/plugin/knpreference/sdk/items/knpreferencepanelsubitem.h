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

#ifndef KNPREFERENCEPANELSUBITEM_H
#define KNPREFERENCEPANELSUBITEM_H

#include <QAbstractButton>

/*!
 * \brief The KNPreferencePanelSublItem class provides the panel widget for the
 * second and all the other lines for the panel preference items.
 */
class KNPreferencePanelSubItem : public QAbstractButton
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNPreferencePanelSublItem widget.
     * \param parent The parent widget.
     */
    explicit KNPreferencePanelSubItem(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from QAbstractButton::setText().
     */
    void setText(const QString &text);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QAbstractButton::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    inline void updateButtonText();
    inline QRect textRect() const;
    QString m_rawText, m_displayText;
};

#endif // KNPREFERENCEPANELSUBITEM_H
