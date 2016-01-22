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

#ifndef KNLOOPSCROLLLABEL_H
#define KNLOOPSCROLLLABEL_H

#include <QWidget>

/*!
 * \brief The KNLoopScrollLabel class is a scroll label which could provide the
 * scroll effect just like music label from the iPhone OS 1.0 to iOS 6.
 */
class KNLoopScrollLabel : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNLoopScrollLabel widget.
     * \param parent The parent widget pointer.
     */
    explicit KNLoopScrollLabel(QWidget *parent = 0);

    /*!
     * \brief The text of the label holds.
     * \return The text in QString.
     */
    QString text() const;

    /*!
     * \brief Reimplemented from QWidget::sizeHint().
     */
    QSize sizeHint() const Q_DECL_OVERRIDE;

    /*!
     * \brief The opacity value of the label.
     * \return The opacity of the label widget. The value will be ranged 0.0 to
     * 1.0.
     */
    qreal opacity() const;

    /*!
     * \brief Get the alignment of the label if the text is not full enough to
     * be scroll.
     * \return The text alignment.
     */
    int alignment() const;

signals:

public slots:
    /*!
     * \brief Set the new text alignment.
     * \param alignment The text alignement. It uses the same as QLabel.
     */
    void setAlignment(int alignment);

    /*!
     * \brief Set the label text opacity.
     * \param opacity The opacity. It should between 0.0 to 1.0.
     */
    void setOpacity(const qreal &opacity);

    /*!
     * \brief Set the label text.
     * \param text The label text.
     */
    void setText(const QString &text);

    /*!
     * \brief Start the scroll anime when animation is valid. If animation is
     * not valid, this function will do nothing.\n
     * When the widget visible change to true, this function will be
     * automatically called.
     */
    void startAnime();

    /*!
     * \brief Reset the scroll animation and stop the timer.\n
     * When the widget is hiding, this function will be automatically called.
     */
    void stopAnime();

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
    int m_alignment, m_textLeftMostX, m_textX;
    bool m_looped;
};

#endif // KNLOOPSCROLLLABEL_H
