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

#ifndef KNMOUSESENSEWIDGET_H
#define KNMOUSESENSEWIDGET_H

#include <QWidget>

class QTimeLine;
/*!
 * \brief The KNMouseSenseWidget class\n
 * IMPORTANT: You have to set a object name for every mouse sense widget's
 * implement widget using updateObjectName(QString) function. DO NOT use
 * setObjectName() function.
 */
class KNMouseSenseWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMouseSenseWidget widget.
     * \param parent The parent widget.
     */
    explicit KNMouseSenseWidget(QWidget *parent = 0);

    /*!
     * \brief Update the object name of the widget. And update the palette as
     * well.
     * \param name The object name of the widget.
     */
    void updateObjectName(const QString &name);

    /*!
     * \brief Change the backgroud color's opacity instead of changing the
     * brightness of the color.
     * \return If it change the opacity of the background, return true. The
     * default value is false.
     */
    bool changeOpacity() const;

    /*!
     * \brief Set the widget animation to change the opacity instead of the
     * brightness.
     * \param changeAlpha To change the opacity, set to true.
     */
    void setChangeOpacity(bool changeOpacity);

    /*!
     * \brief Set the changing range of the color variable. The mouse sense
     * widget will have a default chaning range, to change the default value to
     * a specific range, use the set range function. The range won't changed
     * after manually set.
     * \param start The start value.
     * \param end The end value.
     */
    void setSenseRange(int start, int end);

    /*!
     * \brief Get the start of the range.
     * \return The start of the background range start.
     */
    int senseRangeStart() const;

    /*!
     * \brief Get the end of the range.
     * \return The end of the background range end.
     */
    int senseRangeEnd() const;

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QWidget::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

protected slots:
    /*!
     * \brief When the palette changed, this slot will be called. It needs to
     * handle all the request.
     */
    virtual void onPaletteChanged();

private slots:
    void changeBackgroundColor(int frame);

private:
    inline QTimeLine *generateTimeline();
    inline void startAnime(int endFrame);
    QTimeLine *m_mouseInOut;
    int m_rangeStart, m_rangeEnd, m_rangeAlpha;
    bool m_changeAlpha, m_manualRange;
};

#endif // KNMOUSESENSEWIDGET_H
