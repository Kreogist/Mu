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

#ifndef KNSCROLLLABEL_H
#define KNSCROLLLABEL_H

#include <QWidget>

/*!
 * \brief The KNScrollLabel class provides a label widget which can scroll the
 * text when the text is too long. When the text is too long it will set the
 * whole text as the tooltip of the widget.\n
 * You can set the opacity of the scroll label content by calling the
 * setOpacity() function.
 */
class KNScrollLabel : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNScrollLabel widget.
     * \param parent The parent widget.
     */
    explicit KNScrollLabel(QWidget *parent = 0);

    /*!
     * \brief Get the label text.
     * \return The text of the label.
     */
    QString text() const;

    /*!
     * \brief Reimplemented from QWidget::sizeHint().
     */
    QSize sizeHint() const Q_DECL_OVERRIDE;

    /*!
     * \brief The opacity of the scroll label.
     * \return
     */
    qreal opacity() const;

signals:

public slots:
    /*!
     * \brief Set the opacity of the scroll label.
     * \param opacity The opacity. It should be a real between 0.0 to 1.0.
     */
    void setOpacity(const qreal &opacity);

    /*!
     * \brief Set the scroll label text.
     * \param text The label content.
     */
    void setText(const QString &text);

protected:
    /*!
     * \brief Reimplemented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::showEvent().
     */
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::hideEvent().
     */
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;

private slots:
    void moveText();

private:
    inline void stopTimers();
    inline void updateAnimeParameters();
    QString m_text;
    QTimer *m_move, *m_wait;
    qreal m_opacity;
    int m_textLeftMostX, m_textX;
    bool m_movingLeft;
};

#endif // KNSCROLLLABEL_H
