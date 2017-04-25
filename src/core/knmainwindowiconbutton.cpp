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
#include <QTimeLine>
#include <QPainter>
#include <QGraphicsOpacityEffect>

#include "knthememanager.h"
#include "knlocalemanager.h"

#include "knmainwindowiconbutton.h"

#include <QDebug>

#define IndexApp 0
#define IndexPreference 1

#define IconSize 190
#define IconX -80
#define IconY -85
#define TextSize 40
#define TextLeft 63
#define TextY 20
#define PreferenceStartY 90

KNMainWindowIconButton::KNMainWindowIconButton(QWidget *parent) :
    QAbstractButton(parent),
    m_mouseIn(generateTimeLine()),
    m_mouseOut(generateTimeLine()),
    m_mouseInOutParameter(0),
    m_iconX(IconX),
    m_preferenceIconX(-IconSize),
    m_preferenceIconY(PreferenceStartY),
    m_preferenceTextY(PreferenceStartY),
    m_normalWidth(0),
    m_widthExpand(0),
    m_textEffect(new QGraphicsOpacityEffect(this)),
    m_effectGradient(QLinearGradient(QPointF(0,0), QPointF(0,50)))
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setFixedHeight(70);
    setMinimumWidth(182);
    //Update the preference icon Y.
    m_preferenceIconY=(height()-(IconSize>>1))>>1;

    //Initial the labels of the main button.
    initialLabels();
    //Initial the timeline animations.
    configureTimeLines();

    //Link theme manager.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNMainWindowIconButton::onActionPaletteChanged);
    onActionPaletteChanged();
    //Link retranslate.
    knI18n->link(this, &KNMainWindowIconButton::retranslate);
    retranslate();
}

void KNMainWindowIconButton::setButtonIcon(const QPixmap &icon)
{
    //Set the icon to label.
    m_iconImage[IndexApp]=icon.scaled(IconSize,
                                      IconSize,
                                      Qt::KeepAspectRatio,
                                      Qt::SmoothTransformation);
    //Update the button.
    update();
}

void KNMainWindowIconButton::setButtonText(const QString &text)
{
    //Set the text to label.
    m_title[IndexApp]->setText(text);
    //Update the title label.
    updateTitleLabel();
}

void KNMainWindowIconButton::enterEvent(QEvent *event)
{
    //Start the in out time line to the left position.
    startInOutAnime(m_mouseIn);
    //Do the original enter event.
    QAbstractButton::enterEvent(event);
}

void KNMainWindowIconButton::leaveEvent(QEvent *event)
{
    //Start the in out time line to the normal position.
    startInOutAnime(m_mouseOut);
    //Do the leave event.
    QAbstractButton::leaveEvent(event);
}

void KNMainWindowIconButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the paint event.
    QPainter painter(this);
    //Configure the painter.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Check icon image pixmap.
    if(!m_iconImage[IndexApp].isNull())
    {
        //Draw the icon of the music disk.
        painter.drawPixmap(m_iconX, IconY, m_iconImage[IndexApp]);
    }
    //Paint the preference icon.
    painter.drawPixmap(m_preferenceIconX,
                       m_preferenceIconY,
                       m_iconImage[IndexPreference]);
    //Draw the text above the images.
    //Draw the preference text.
    painter.setFont(m_titleTextFont[IndexPreference]);
    painter.setPen(m_titleTextPalette.color(QPalette::WindowText));
    painter.drawText(TextLeft,
                     m_preferenceTextY,
                     m_titleText[IndexPreference]);
}

void KNMainWindowIconButton::resizeEvent(QResizeEvent *event)
{
    //Resize the button.
    QAbstractButton::resizeEvent(event);
    //Emit the size changed signal.
    emit buttonSizeChange();
}

void KNMainWindowIconButton::retranslate()
{
    //Update the preference label position.
    m_titleText[IndexPreference]=tr("Preference");

    //Update the label width.
    updateTitleLabel();
}

void KNMainWindowIconButton::onActionMouseInOut(const int &frame)
{
    //Move the app icon label.
    m_iconX=frame;
    //Move the configure icon label.
    m_preferenceIconX=m_mouseInOutParameter-frame;

    //Get the base number.
    int baseNumber=frame-IconX;
    //Move the app text label.
    m_title[IndexApp]->move(TextLeft,
                            TextY+(baseNumber>>3));
    QFont appFont=m_title[IndexApp]->font();
    appFont.setPixelSize(TextSize+(baseNumber>>3));
    m_title[IndexApp]->setFont(appFont);
    //Move the preference lable.
    m_preferenceTextY=PreferenceStartY+(baseNumber>>2);
    //Update the widget.
    update();
}

void KNMainWindowIconButton::onActionPaletteChanged()
{
    //Update the button text.
    m_titleTextPalette=knTheme->getPalette("MainWindowButtonText");
    //Update the widget.
    update();
}

void KNMainWindowIconButton::initialLabels()
{
    //Generate the label font.
    //Construct two text label.
    for(int i=0; i<2; i++)
    {
        //Generate the text label.
        m_title[i]=new QLabel(this);
        //Configure the text label.
        m_title[i]->setObjectName("MainWindowButtonText");
        knTheme->registerWidget(m_title[i]);
        m_title[i]->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        m_title[i]->move(TextLeft, TextY);
        m_title[i]->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }
    //Configure the mask gradient.
    m_effectGradient.setColorAt(0, QColor(0,0,0,255));
    m_effectGradient.setColorAt(1, QColor(0,0,0,0));
    m_textEffect->setOpacityMask(m_effectGradient);
    m_title[IndexApp]->setGraphicsEffect(m_textEffect);

    //Configure the icon label of preference.
    m_iconImage[IndexPreference]=
            QPixmap(":/plugin/preference/icon.png").scaled(
                IconSize>>1,
                IconSize>>1,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation);
    //Configure the text label of application.
    QFont textFont=font();
    textFont.setBold(true);
    textFont.setPixelSize(TextSize);
    m_title[IndexApp]->setFont(textFont);
    //Configure the text label of the preference.
    textFont.setPixelSize(TextSize>>1);
    m_titleTextFont[IndexPreference]=textFont;
}

inline void KNMainWindowIconButton::configureTimeLines()
{
    //Update the mouse in/out parameter.
    m_mouseInOutParameter=(IconX<<1)-34;
    //Set the end frame of the mouse in/out time line.
    m_mouseIn->setEndFrame(-IconSize);
    m_mouseOut->setEndFrame(IconX);
    //Link the label moving slot.
    connect(m_mouseIn, &QTimeLine::frameChanged,
            this, &KNMainWindowIconButton::onActionMouseInOut);
    connect(m_mouseIn, &QTimeLine::valueChanged,
            [=](const qreal &progress)
            {
                //Update the gradient.
                m_effectGradient.setColorAt(1, QColor(0, 0, 0, 255.0*progress));
                m_textEffect->setOpacityMask(m_effectGradient);
                m_title[IndexApp]->setGraphicsEffect(m_textEffect);
                //Resize the button size.
                resize(m_normalWidth+((qreal)m_widthExpand*progress),
                       height());
            });
    connect(m_mouseOut, &QTimeLine::frameChanged,
            this, &KNMainWindowIconButton::onActionMouseInOut);
    connect(m_mouseOut, &QTimeLine::valueChanged,
            [=](const qreal &progress)
            {
                //Calculate the progress level.
                qreal progressLevel=1.0-progress;
                //Set the effect size.
                m_effectGradient.setColorAt(1, QColor(0, 0, 0,
                                                      255.0*progressLevel));
                m_textEffect->setOpacityMask(m_effectGradient);
                m_title[IndexApp]->setGraphicsEffect(m_textEffect);
                //Resize the button size.
                resize(m_normalWidth+((qreal)m_widthExpand*progressLevel),
                       height());
            });
}

inline void KNMainWindowIconButton::startInOutAnime(QTimeLine *timeLine)
{
    //Stop in and out anime.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set the start range of the time line.
    timeLine->setStartFrame(m_iconX);
    //Set the animation.
    timeLine->start();
}

inline void KNMainWindowIconButton::updateTitleLabel()
{
    //Get the maximum label width.
    int musicLabelWidth=m_title[IndexApp]->fontMetrics().width(
                            m_title[IndexApp]->text());
    //Resize the button.
    m_normalWidth=musicLabelWidth+TextLeft;
    m_widthExpand=qMax(musicLabelWidth,
                       QFontMetrics(m_titleTextFont[IndexPreference]).width(
                           m_titleText[IndexPreference]))-musicLabelWidth;
    //Resize the button the initial size.
    resize(m_normalWidth, height());
    //Update the size of the title labels.
    m_title[IndexApp]->resize(musicLabelWidth+m_widthExpand,
                              m_title[IndexApp]->fontMetrics().height());
}

inline QTimeLine *KNMainWindowIconButton::generateTimeLine()
{
    //Generate the time line.
    QTimeLine *timeLine=new QTimeLine(200, this);
    //Configure the time line.
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    timeLine->setUpdateInterval(16);
    //Return the time line.
    return timeLine;
}
