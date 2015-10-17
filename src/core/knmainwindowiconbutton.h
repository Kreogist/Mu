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

#ifndef KNMAINWINDOWICONBUTTON_H
#define KNMAINWINDOWICONBUTTON_H

#include <QAbstractButton>

class QLabel;
class QTimeLine;
class QGraphicsOpacityEffect;
/*!
 * \brief The KNMainWindowIconButton class is a huge button on the top left of
 * the header. It will display the icon of the application, and the application
 * title. When the mouse entered, it will change to configure button.\n
 * The only use of this button is to enter preference panel.
 */
class KNMainWindowIconButton : public QAbstractButton
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMainWindowIconButton widget.
     * \param parent The parent widget.
     */
    explicit KNMainWindowIconButton(QWidget *parent = 0);

    /*!
     * \brief Set the icon of the main button.
     * \param icon The icon pixmap.
     */
    void setButtonIcon(const QPixmap &icon);

    /*!
     * \brief Set the text of the button
     * \param text The button text.
     */
    void setButtonText(const QString &text);

signals:
    void buttonSizeChange();

public slots:

protected:
    /*!
     * \brief Reimplemented from QAbstractButton::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief resizeEvent
     * \param event
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void retranslate();
    void onActionMouseInOut(const int &frame);
    void onActionPaletteChanged();

private:
    inline void initialLabels();
    inline void configureTimeLines();
    inline void startInOutAnime(QTimeLine *timeLine);
    inline void updateTitleLabel();
    inline QTimeLine *generateTimeLine();

    QPixmap m_iconImage[2];
    QString m_titleText[2];
    QFont m_titleTextFont[2];
    QPalette m_titleTextPalette;
    QLabel *m_title[2];
    QTimeLine *m_mouseIn, *m_mouseOut;
    int m_mouseInOutParameter, m_iconX, m_preferenceIconX, m_preferenceIconY,
        m_preferenceTextY, m_normalWidth, m_widthExpand;
    QGraphicsOpacityEffect *m_textEffect;
    QLinearGradient m_effectGradient;
};

#endif // KNMAINWINDOWICONBUTTON_H
