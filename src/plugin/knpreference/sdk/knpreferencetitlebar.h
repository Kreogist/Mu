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

#ifndef KNPREFERENCETITLEBAR_H
#define KNPREFERENCETITLEBAR_H

#include <QWidget>

class QLabel;
class KNPreferenceHeaderButton;
/*!
 * \brief The KNPreferenceTitlebar widget is the title bar of the preference
 * sidebar. It will display the caption of the current panel. And contains a
 * button to be back to main window.
 */
class KNPreferenceTitleBar : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Constrcut a KNPreferenceTitlebar widget.
     * \param parent The parent widget.
     */
    explicit KNPreferenceTitleBar(QWidget *parent = 0);

    /*!
     * \brief Change the header icon.
     * \param icon The header icon.
     */
    void setIcon(const QPixmap &icon);

    /*!
     * \brief setText
     * \param text
     */
    void setText(const QString &text);

signals:
    /*!
     * \brief When the header button clicked, this signal will be emitted to ask
     * to close the preference.
     */
    void requireClosePreference();

public slots:

protected:
    /*!
     * \brief Reimplemented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    KNPreferenceHeaderButton *m_button;
    QLabel *m_title;
    QBrush m_dullPolishBrush;
    QLinearGradient m_highlightGradient;
};

#endif // KNPREFERENCETITLEBAR_H
