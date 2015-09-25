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

#ifndef KNWIDGETSWITCHER_H
#define KNWIDGETSWITCHER_H

#include <QWidget>

class QPropertyAnimation;
class QParallelAnimationGroup;
/*!
 * \brief The KNWidgetSwitcher class provides you a stacked group which can
 * display multiple widget in a single widget. This is a tabbed-like widget. But
 * KNWidgetSwitcher can switch the widget in animated way.\n
 * This class is only an abstract class. You have to implement it and override
 * the updateAnimationPosition() to give the specific parameter for the
 * animation inAnimation() and outAnimation().
 */
class KNWidgetSwitcher : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNWidgetSwitcher widget.
     * \param parent The given parent.
     */
    explicit KNWidgetSwitcher(QWidget *parent = 0);

    /*!
     * \brief Add a widget to switcher stack.
     * \param widget The widget to be added to stack.
     */
    void addWidget(QWidget *widget);

    /*!
     * \brief Get the index of current widget. If there's no widget, this value
     * will be -1.
     * \return The current widget index.
     */
    int currentIndex() const;

    /*!
     * \brief Remove a widget from the widget switcher.
     * \param widget Check the widget.
     * \return If we can find the widget and remove it from the switcher, return
     * true.
     */
    bool removeWidget(QWidget *widget);

    /*!
     * \brief Remove all the widgets from the widget switcher.
     */
    void clear();

    /*!
     * \brief Get the number of the widgets which is in the switcher stack.
     * \return The number of the stacked widgets.
     */
    int count() const;

    /*!
     * \brief Get the widget at specific index.
     * \param index The index of the widget.
     * \return The widget pointer. If the index is out of range, it will be
     * nullptr.
     */
    QWidget *widgetAt(const int &index);

    /*!
     * \brief Get the current widget.
     * \return The current widget pointer. If the current index is -1, this will
     * return nullptr.
     */
    QWidget *currentWidget();

    /*!
     * \brief Reimplemented from QWidget::sizeHint().
     */
    QSize sizeHint() const;

signals:

public slots:
    /*!
     * \brief Set the current display widget with the index.
     * \param currentIndex The current widget index.
     */
    void setCurrentIndex(const int &currentIndex);

protected:
    /*!
     * \brief In this function, the widget should update the start and final
     * position of the animation. We provides you several functions to get the
     * in/out animation(protected), current index(public) and out widget index
     * (protected).
     */
    virtual void updateAnimationPosition()=0;

    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event);

    /*!
     * \brief Get the widget in animation object.
     * \return The animation object.
     */
    QPropertyAnimation *inAnimation() const;

    /*!
     * \brief Get the widget out animation object.
     * \return The animation object.
     */
    QPropertyAnimation *outAnimation() const;

    /*!
     * \brief Get the index of the moving out widget.
     * \return The moving out widget index.
     */
    int outWidgetIndex() const;

private:
    inline QPropertyAnimation *generateAnimation();
    inline void setWidgetVisible(const int &index, bool visible);

    QList<QWidget *> m_widgets;
    QParallelAnimationGroup *m_movingAnimationGroup;
    QPropertyAnimation *m_inAnimation, *m_outAnimation;
    int m_currentIndex, m_outWidgetIndex, m_maxWidth, m_maxHeight;
};

#endif // KNWIDGETSWITCHER_H
