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
#include <QBoxLayout>
#include <QTimeLine>
#include <QPainter>
#include <QLabel>

#include "knopacitybutton.h"
#include "knimagelabel.h"
#include "knthememanager.h"
#include "knconfiguremanager.h"

#include "knpreferencepanelitem.h"

#include <QDebug>

#define PreferenceSingleItemHeight 34
#define PreferenceCaptionWidth 185

KNPreferencePanelItem::KNPreferencePanelItem(QWidget *parent) :
    QWidget(parent),
    m_path(QStringList()),
    m_defaultValue(QVariant()),
    m_highlight(QLinearGradient(0, 0, 0, PreferenceSingleItemHeight)),
    m_highLightOpacity(0.0),
    m_titleLabel(new QLabel(this)),
    m_hintLabel(new KNImageLabel(this)),
    m_undoButton(new KNOpacityButton(this)),
    m_backgroundAnime(new QTimeLine(100, this))
{
    //Set properties.
    setContentsMargins(0, 0, 0, 0);
    //Set the palette.
    QPalette pal=knTheme->getPalette("PreferencePanelItem");
    setPalette(pal);
    //Configure the label.
    m_titleLabel->setFixedWidth(PreferenceCaptionWidth);
    m_titleLabel->setContentsMargins(10, 0, 10, 0);
    //Configure the hint label.
    m_hintLabel->setFixedSize(PreferenceSingleItemHeight,
                              PreferenceSingleItemHeight);
    m_hintLabel->setPixmap(QPixmap("://preference/PreferenceHint.png"));
    //Configure the hint label.
    m_undoButton->setFixedSize(PreferenceSingleItemHeight,
                               PreferenceSingleItemHeight);
    m_undoButton->setIcon(QIcon("://preference/PreferenceUndo.png"));
    connect(m_undoButton, &KNOpacityButton::clicked,
            [=]
            {
                //Hide the undo button.
                m_undoButton->setEnabled(false);
                //Get the configure value.
                setWidgetValue(m_defaultValue);
                //Update the signal.
                emit valueChanged();
            });
    //Initial the highlight.
    m_highlight.setColorAt(0, pal.color(QPalette::AlternateBase));
    m_highlight.setColorAt(1, pal.color(QPalette::Window));

    //Link the value changed.
    connect(this, &KNPreferencePanelItem::valueChanged,
            this, &KNPreferencePanelItem::onValueChanged);

    //Update the panel.
    m_backgroundAnime->setUpdateInterval(16);
    connect(m_backgroundAnime, &QTimeLine::frameChanged,
            this, &KNPreferencePanelItem::onActionChangeHighlight);
    onActionChangeHighlight(0);
}

void KNPreferencePanelItem::setConfig(const QVariant &config)
{
    //Default, the preference item doesn't need any configure.
    Q_UNUSED(config)
}

void KNPreferencePanelItem::setValue(const QVariant &value)
{
    //Check the value data.
    setWidgetValue(value.isNull() ? m_defaultValue : value);
}

void KNPreferencePanelItem::setPreferenceOption(
        const PreferencePanelOption &option)
{
    //Update the height.
    int itemHeight=itemHeightMultiple() * PreferenceSingleItemHeight;
    setFixedHeight(itemHeight);
    //Update the title.
    m_titleLabel->setText(option.title);
    //Set the hint.
    m_hintLabel->setDisplayContent(!option.explain.isEmpty());
    m_hintLabel->setToolTip(option.explain);
    //Save the path.
    m_path=option.path;
    //Save the default value.
    m_defaultValue=knConf->configureValue(m_path, option.defaultValue);
    //Get the configure value.
    setWidgetValue(m_defaultValue);
    //Set the configure data.
    setConfig(option.configure);
    //Disable the value.
    m_undoButton->setEnabled(false);
}

int KNPreferencePanelItem::itemHeightMultiple() const
{
    return 1;
}

void KNPreferencePanelItem::paintEvent(QPaintEvent *event)
{
    //Update the widget.
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

void KNPreferencePanelItem::enterEvent(QEvent *event)
{
    //Do original the event.
    QWidget::enterEvent(event);
    //Start the mouse in anime.
    startAnime(100);
}

void KNPreferencePanelItem::leaveEvent(QEvent *event)
{
    //Do original the event.
    QWidget::leaveEvent(event);
    //Start the mouse out anime.
    startAnime(0);
}

void KNPreferencePanelItem::buildWidgetLayout(QLayout *layout)
{
    //Construct the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
    //Add the basic widgets.
    mainLayout->addWidget(m_titleLabel, 0, Qt::AlignVCenter);
    //Add the undo button.
    mainLayout->addWidget(m_undoButton, 0, Qt::AlignVCenter);
    //Add the content layout to widget.
    mainLayout->addLayout(layout, 1);
    //Add the helper widgets.
    mainLayout->addWidget(m_hintLabel, 0, Qt::AlignVCenter);
}

QString KNPreferencePanelItem::titleText() const
{
    return m_titleLabel->text();
}

void KNPreferencePanelItem::onActionChangeHighlight(int frame)
{
    //Change the highlight opacity.
    m_highLightOpacity=(qreal)frame/100.0;
    //Update the background.
    update();
}

void KNPreferencePanelItem::onValueChanged()
{
    //Get the current value.
    QVariant currentValue=value();
    //Check the value state.
    m_undoButton->setEnabled(!isEqual(currentValue, m_defaultValue));
    //Save the value.
    knConf->setConfigureValue(m_path, currentValue);
}

inline void KNPreferencePanelItem::startAnime(int endFrame)
{
    //Stop the time line.
    m_backgroundAnime->stop();
    //Set the end frame.
    m_backgroundAnime->setFrameRange(m_highLightOpacity * 100,
                                     endFrame);
    //Start the anime.
    m_backgroundAnime->start();
}
