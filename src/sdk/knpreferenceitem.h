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

#ifndef KNPREFERENCEITEM_H
#define KNPREFERENCEITEM_H

#include <QAbstractButton>

class QTimeLine;
/*!
 * \brief The KNPreferenceItem class is a specific button for preference plugin.
 * It will display a title and a icon of a preference tab.
 */
class KNPreferenceItem : public QAbstractButton
{
    Q_OBJECT
public:
    /*!
     * \brief Construct the KNPreferenceItem widget.
     * \param parent The parent widget.
     */
    explicit KNPreferenceItem(QWidget *parent = 0);

    /*!
     * \brief Get the header icon pixmap.
     * \return The header icon. If you never set it before, it will return a
     * null QPixmap.
     */
    QPixmap headerIcon() const;

    /*!
     * \brief Get whether the item is an advanced item. Advanced item would be
     * shown only when the advanced switch is turned on.
     * \return If the tab is advanced tab, return true. The default value is
     * false.
     */
    bool isAdvanced() const;

signals:

public slots:
    /*!
     * \brief Set the header icon which will be displayed on the top of the
     * header.
     * \param headerIcon The header icon pixmap.
     */
    void setHeaderIcon(const QPixmap &headerIcon);

    /*!
     * \brief Set whether the tab item is advanced item.
     * \param isAdvanced To set the tab as an advanced item, set it to true.
     */
    void setIsAdvanced(bool isAdvanced);

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
     * \brief Reimplemented from QAbstractButton::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Paint the preference item content.
     * \param painter The antialias painter generate in the paint event.
     */
    virtual void paintContent(QPainter *painter);

private slots:
    void onActionMouseInOut(int frame);
    void onToggled(bool checked);

private:
    inline void startAnime(int endFrame);
    static QLinearGradient m_upShadowGradient, m_downShadowGradient;
    static bool m_initial;
    QPixmap m_headerIcon;
    qreal m_backgroundOpacity;
    QTimeLine *m_mouseAnime;
    int m_progress, m_textX;
    bool m_isAdvanced;
};

#endif // KNPREFERENCEITEM_H
