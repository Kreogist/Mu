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

#ifndef KNABSTRACTSLIDER_H
#define KNABSTRACTSLIDER_H

#include <QWidget>

/*!
 * \brief The KNAbstractSlider class provides the basic operation of a 64-bit
 * integer slider. All the data is in 64-bit integer type. This is an abstract
 * interface widget class. The slider widget should implement this widget and
 * realize the paintEvent() function.
 */
class KNAbstractSlider : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNAbstractSlider widget.
     * \param parent The parent widget.
     */
    explicit KNAbstractSlider(QWidget *parent = 0);

    /*!
     * \brief This property holds the slider's maximum value.\n
     * When setting this property, the minimum is adjusted if necessary to
     *  ensure that the range remains valid. Also the slider's current value is
     * adjusted to be within the new range.
     * \return The maximum value.
     */
    virtual qint64 maximum() const
    {
        return m_maximum;
    }

    /*!
     * \brief This property holds the sliders's minimum value.\n
     * When setting this property, the maximum is adjusted if necessary to
     * ensure that the range remains valid. Also the slider's current value is
     * adjusted to be within the new range.
     * \return The minimal value.
     */
    virtual qint64 minimal() const
    {
        return m_minimal;
    }

    /*!
     * \brief This property holds the slider's current value.\n
     * The slider forces the value to be within the legal range: minimum <=
     * value <= maximum.
     * \return The current slider value.
     */
    virtual qint64 value() const
    {
        return m_value;
    }

    /*!
     * \brief Get the range size of the slider.
     * \return The slider range gap.
     */
    virtual qint64 range() const
    {
        return m_range;
    }

    /*!
     * \brief Get the percentage of the current value to the range.
     * \return The percentage of the current value to the minimum value.
     */
    virtual qreal percentage() const
    {
        return m_percentage;
    }

    /*!
     * \brief Get the wheel step value.
     * \return The wheel step.
     */
    virtual qint64 wheelStep() const
    {
        return m_wheelStep;
    }

signals:
    /*!
     * \brief When either minimum or maximum changed, this signal will be
     * emitted.
     * \param min The minimum of the slider.
     * \param max The maximum of the slider.
     */
    void rangeChanged(float min, float max);

    /*!
     * \brief When the mouse moved the button of the slider, this signal will be
     * emitted.
     * \param value The value of the slider.
     */
    void sliderMoved(float value);

    /*!
     * \brief When the mouse pressed the button of the slider, this signal will
     * be emitted.
     */
    void sliderPressed();

    /*!
     * \brief When the mouse released the pressed of the button of the slider,
     * this signal will be emitted.
     */
    void sliderReleased();

    /*!
     * \brief When the value of the slider changed, this signal will be changed.
     * \param value The current value of the slider.
     */
    void valueChanged(qint64 value);

public slots:
    /*!
     * \brief Set the maximum value of the slider.
     * \param maximum The prefer maximum value.
     */
    virtual void setMaximum(qint64 maximum);

    /*!
     * \brief Set the minimal value of the slider.
     * \param maximum The prefer minimal value.
     */
    virtual void setMinimal(qint64 minimal);

    /*!
     * \brief Set both maximum and minimal value of the slider.
     * \param min The prefer minimal value.
     * \param max The prefer maximum value.
     */
    virtual void setRange(qint64 min, qint64 max);

    /*!
     * \brief Set the value of the slider.
     * \param value The prefer value.
     */
    virtual void setValue(qint64 value);

    /*!
     * \brief Set the wheel step of the scrolling of the mouse or trackpad.
     * \param wheelStep The wheel step of the slider.
     */
    virtual void setWheelStep(qint64 wheelStep)
    {
        m_wheelStep=wheelStep;
    }

protected:
    /*!
     * \brief Reimplemented from QWidget::wheelEvent().
     */
    void wheelEvent(QWheelEvent *event);

    /*!
     * \brief All the implement slider must provides the paintEvent() function.
     * \param event The event provided by the system.
     */
    virtual void paintEvent(QPaintEvent *event)=0;

private:
    inline void updateRange();
    qint64 m_maximum, m_minimal, m_value, m_range, m_wheelStep;
    qreal m_percentage;
};

#endif // KNABSTRACTSLIDER_H
