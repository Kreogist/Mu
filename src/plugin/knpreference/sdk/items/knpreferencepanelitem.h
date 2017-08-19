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

#ifndef KNPREFERENCEPANELITEM_H
#define KNPREFERENCEPANELITEM_H

#include "../knpreferenceutil.h"

#include <QWidget>

using namespace PreferenceUtil;

class QLabel;
class QTimeLine;
class KNImageLabel;
class KNOpacityButton;
/*!
 * \brief The KNPreferencePanelItem class provides a port for the item. It won't
 * construct any configure widget. It will only provides the common widget for
 * the items. For different types of the items, they should apply their own
 * widgets.
 */
class KNPreferencePanelItem : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNPreferencePanelItem widget.
     * \param parent The parent widget.
     */
    explicit KNPreferencePanelItem(QWidget *parent = 0);

    /*!
     * \brief Get the value of the panel item.
     * \return The result value data.
     */
    virtual QVariant value() const=0;

    /*!
     * \brief Set the config of the item.
     * \param config The config data of the preference item panel.
     */
    virtual void setConfig(const QVariant &config);

    /*!
     * \brief Set the value of the widget.
     * \param value The value data.
     */
    void setValue(const QVariant &value);

signals:
    /*!
     * \brief When the item value changed, this signal will be emitted.
     */
    void valueChanged();

public slots:
    /*!
     * \brief Set the preference option which would set the data.
     * \param option The preference option data.
     */
    void setPreferenceOption(const PreferencePanelOption &option);

protected:
    /*!
     * \brief Get the magnification of the item height. The height of the item
     * would be the value of this function multiply the minimum height.
     * \return The item height magnification.
     */
    virtual int itemHeightMultiple() const;

    /*!
     * \brief Reimplemented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Build the layout of the preference item. Which would contains the
     * layout of the controller widget. It is impossible to use the virtual
     * function to do the staff, thus we will use the widget to build this.
     * \param layout The content layout.
     */
    void buildWidgetLayout(QLayout *layout);

    /*!
     * \brief Set the widget to display specific value.
     * \param value The display value.
     */
    virtual void setWidgetValue(const QVariant &value)=0;

    /*!
     * \brief Get whether the current value is equal to the original value.
     * \param currentValue The current value state.
     * \param originalValue The original value state.
     * \return If the value is the same, return true. Or else, return false.
     */
    virtual bool isEqual(const QVariant &currentValue,
                         const QVariant &originalValue)=0;

    /*!
     * \brief Get the default value. This default value means the original value
     * set in the configure.
     * \param defaultValue The applciation default set value.
     * \return The default value.
     */
    virtual QVariant getValueFromConfig(const QVariant &defaultValue);

    /*!
     * \brief Initialize the preference item with the specific value.
     * \param defaultValue The default value.
     */
    virtual void initialValue(const QVariant &defaultValue);

    /*!
     * \brief Get the title text of the label.
     * \return The title label text.
     */
    QString titleText() const;

    /*!
     * \brief Get the name of the value.
     * \return The value name string.
     */
    QString valueName() const;

private slots:
    void onActionChangeHighlight(int frame);
    void onValueChanged();

private:
    inline void startAnime(int endFrame);
    QStringList m_path;
    QVariant m_defaultValue;
    QLinearGradient m_highlight;
    qreal m_highLightOpacity;
    QLabel *m_titleLabel;
    KNImageLabel *m_hintLabel;
    KNOpacityButton *m_undoButton;
    QTimeLine *m_backgroundAnime;
};

#endif // KNPREFERENCEPANELITEM_H
