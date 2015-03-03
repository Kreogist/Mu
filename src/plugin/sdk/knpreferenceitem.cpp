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
#include <QLabel>
#include <QBoxLayout>
#include <QTimeLine>
#include <QPainter>

#include "knopacitybutton.h"
#include "knconfigure.h"

#include "knpreferenceitem.h"

#include <QDebug>

KNPreferenceItem::KNPreferenceItem(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);
    setFixedHeight(PreferenceSingleItemHeight);

    //Initial the highlight.
    m_highlight=QLinearGradient(0,0,0,PreferenceSingleItemHeight);
    m_highlight.setColorAt(0, QColor(0x79, 0x79, 0x79));
    m_highlight.setColorAt(1, QColor(0x4a, 0x4a, 0x4a));

    //Initial layout.
    m_mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);

    //Initial caption label.
    m_caption=new QLabel(this);
    m_caption->setContentsMargins(10,9,0,0);
    m_caption->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_caption->setFixedWidth(PreferenceCaptionWidth);
    QFont captionFont=m_caption->font();
    captionFont.setPixelSize(15);
    m_caption->setFont(captionFont);
    QPalette pal=m_caption->palette();
    pal.setColor(QPalette::WindowText, QColor(220,220,220));
    m_caption->setPalette(pal);
    m_mainLayout->addWidget(m_caption);

    //Initial the undo and hint button.
    m_undo=generateIconButton(QPixmap("://public/items/PreferenceUndo.png"));
    m_itemHint=generateIconButton(QPixmap("://public/items/PreferenceHint.png"));
    m_itemHint->setOpacityGap(0); //Fake button.
    m_mainLayout->addWidget(m_undo, 0, Qt::AlignTop);
    m_mainLayout->addWidget(m_itemHint, 0, Qt::AlignTop);
    //Link undo button.
    connect(m_undo, &KNOpacityButton::clicked,
            this, &KNPreferenceItem::onActionUndo);

    //Initial the timelines.
    m_mouseIn=generateTimeLine(100);
    m_mouseOut=generateTimeLine(0);
}

KNPreferenceItem::~KNPreferenceItem()
{

}

QString KNPreferenceItem::caption() const
{
    return m_caption->toolTip().isEmpty()?m_caption->text():m_caption->toolTip();
}

QVariant KNPreferenceItem::value() const
{
    return m_value;
}

void KNPreferenceItem::setValue(const QVariant &value)
{
    m_value=value;
    //Check is the current value is the same as the default value, enable or
    //disabled the undo button.
    m_undo->setEnabled(m_value!=m_defaultValue);
}

void KNPreferenceItem::setHint(const QString &hint)
{
    if(hint.isEmpty())
    {
        m_itemHint->setToolTip("");
        m_itemHint->setEnabled(false);
        return;
    }
    //Set the tooltip.
    m_itemHint->setToolTip(hint);
    m_itemHint->setEnabled(true);
}

QVariant KNPreferenceItem::defaultValue() const
{
    return m_defaultValue;
}

void KNPreferenceItem::setDefaultValue(const QVariant &defaultValue)
{
    m_defaultValue=defaultValue;
    //When the default value changed, change the value.
    setValue(m_defaultValue);
}

void KNPreferenceItem::setCaption(const QString &caption)
{
    if(m_caption->fontMetrics().width(caption)>PreferenceCaptionWidth)
    {
        m_caption->setText( m_caption->fontMetrics().elidedText(caption,
                                                                   Qt::ElideRight,
                                                                   PreferenceCaptionWidth));
        m_caption->setToolTip(caption);
        return;
    }
    m_caption->setToolTip(QString());
    m_caption->setText(caption);
}

void KNPreferenceItem::paintEvent(QPaintEvent *event)
{
    //Paint the highlight at the opacity.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform,
                           true);
    //Draw the rect.
    painter.setOpacity(m_highLightOpacity);
    //Paint the background.
    painter.fillRect(rect(), m_highlight);
    //Paint other things.
    QWidget::paintEvent(event);
}

void KNPreferenceItem::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    startMouseInAnime();
}

void KNPreferenceItem::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    startMouseOutAnime();
}

QBoxLayout *KNPreferenceItem::mainLayout()
{
    return m_mainLayout;
}

void KNPreferenceItem::startMouseInAnime()
{
    startAnimation(m_mouseIn);
}

void KNPreferenceItem::startMouseOutAnime()
{
    startAnimation(m_mouseOut);
}

void KNPreferenceItem::onActionChangeHighlight(const int &frame)
{
    //Change the highlight opacity.
    m_highLightOpacity=(qreal)frame/100.0;
    update();
}

void KNPreferenceItem::onActionUndo()
{
    setValue(m_defaultValue);
}

KNOpacityButton *KNPreferenceItem::generateIconButton(const QPixmap &icon)
{
    KNOpacityButton *button=new KNOpacityButton(this);
    button->setFixedSize(PreferenceSingleItemHeight,
                         PreferenceSingleItemHeight);
    button->setEnabled(false);
    button->setIcon(icon);
    return button;
}

QTimeLine *KNPreferenceItem::generateTimeLine(const int &endFrame)
{
    QTimeLine *timeLine=new QTimeLine(100, this);
    timeLine->setEndFrame(endFrame);
    timeLine->setUpdateInterval(5);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNPreferenceItem::onActionChangeHighlight);
    return timeLine;
}

void KNPreferenceItem::startAnimation(QTimeLine *timeLine)
{
    //Stop all animations.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Start the animation.
    timeLine->setStartFrame(m_highLightOpacity*100);
    timeLine->start();
}

QString KNPreferenceItem::valueName() const
{
    return m_valueName;
}

void KNPreferenceItem::setValueName(const QString &valueName)
{
    m_valueName = valueName;
}

void KNPreferenceItem::loadValue()
{
    if(m_configure==nullptr)
    {
        return;
    }
    setDefaultValue(m_configure->getData(m_valueName, m_defaultValue));
}

void KNPreferenceItem::saveValue()
{
    if(m_configure==nullptr)
    {
        return;
    }
    m_configure->setData(m_valueName, value());
}

KNConfigure *KNPreferenceItem::configure() const
{
    return m_configure;
}

void KNPreferenceItem::setConfigure(KNConfigure *configure)
{
    m_configure = configure;
}
