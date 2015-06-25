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

KNMainWindowIconButton::KNMainWindowIconButton(QWidget *parent) :
    QAbstractButton(parent),
    m_mouseIn(generateTimeLine()),
    m_mouseOut(generateTimeLine()),
    m_mouseUp(generateTimeLine()),
    m_mouseDown(generateTimeLine()),
    m_mouseInOutParameter(0),
    m_textEffect(new QGraphicsOpacityEffect(this)),
    m_effectGradient(QLinearGradient(QPointF(0,0), QPointF(0,50)))
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setFixedHeight(70);
    setMinimumWidth(182);

    //Initial the labels of the main button.
    initialLabels();
    //Initial the timeline animations.
    initialTimeLines();

    //Link retranslate.
    knI18n->link(this, &KNMainWindowIconButton::retranslate);
    retranslate();

    //--Debug--
    setButtonIcon(QPixmap(":/plugin/music/public/icon.png"));
    setButtonText("Music");
}

void KNMainWindowIconButton::setButtonIcon(const QPixmap &icon)
{
    //Set the icon to label.
    m_icon[IndexApp]->setPixmap(icon);
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
    ;
}

void KNMainWindowIconButton::retranslate()
{
    //Update the preference label position.
    m_title[IndexPreference]->setText(tr("Preference"));

    //Update the label width.
    updateTitleLabel();
}

void KNMainWindowIconButton::onActionMouseInOut(const int &frame)
{
    //Move the app icon label.
    m_icon[IndexApp]->move(frame, m_icon[IndexApp]->y());
    //Move the configure icon label.
    m_icon[IndexPreference]->move(m_mouseInOutParameter-frame,
                                  m_icon[IndexPreference]->y());

    //Get the base number.
    int baseNumber=frame-IconX;
    //Move the app text label.
    m_title[IndexApp]->move(TextLeft,
                            TextY+(baseNumber>>3));
    QFont appFont=m_title[IndexApp]->font();
    appFont.setPointSizeF(TextSize+(baseNumber>>3));
    m_title[IndexApp]->setFont(appFont);
    //Move the preference lable.
    m_title[IndexPreference]->move(m_title[IndexPreference]->x(),
                                   height()+(baseNumber>>3)*3+9);
}

void KNMainWindowIconButton::initialLabels()
{
    //Construct two icon label: one for application icon, the other one for
    //preference icon.
    for(int i=0; i<2; i++)
    {
        //Generate the icon label.
        m_icon[i]=new QLabel(this);
        //Configure the icon label.
        m_icon[i]->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        m_icon[i]->setScaledContents(true);
    }

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
        m_title[i]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    }
    //Configure the mask gradient.
    m_effectGradient.setColorAt(0, QColor(0,0,0,255));
    m_effectGradient.setColorAt(1, QColor(0,0,0,0));
    m_textEffect->setOpacityMask(m_effectGradient);
    m_title[IndexApp]->setGraphicsEffect(m_textEffect);

    //Configure the icon label of application.
    m_icon[IndexApp]->move(IconX, IconY);
    m_icon[IndexApp]->setFixedSize(IconSize, IconSize);
    //Configure the icon label of preference.
    m_icon[IndexPreference]->setPixmap(QPixmap(":/plugin/preference/icon.png"));
    m_icon[IndexPreference]->setFixedSize(IconSize>>1,
                                          IconSize>>1);
    //Configure the text label of application.
    QFont textFont=font();
    textFont.setBold(true);
    textFont.setPointSizeF(TextSize);
    m_title[IndexApp]->setFont(textFont);
    //Configure the text label of the preference.
    textFont.setPointSizeF(TextSize>>1);
    m_title[IndexPreference]->setFont(textFont);
    m_title[IndexPreference]->move(m_title[IndexPreference]->x(),
                                   height());

    //Hide the preference labels.
    m_icon[IndexPreference]->move(-IconSize, (height()-(IconSize>>1))>>1);
}

void KNMainWindowIconButton::initialTimeLines()
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
                m_effectGradient.setColorAt(1, QColor(0,0,0,255.0*progress));
                m_textEffect->setOpacityMask(m_effectGradient);
                m_title[IndexApp]->setGraphicsEffect(m_textEffect);
            });
    connect(m_mouseOut, &QTimeLine::frameChanged,
            this, &KNMainWindowIconButton::onActionMouseInOut);
    connect(m_mouseOut, &QTimeLine::valueChanged,
            [=](const qreal &progress)
            {
                m_effectGradient.setColorAt(1, QColor(0,0,0,255.0*(1.0-progress)));
                m_textEffect->setOpacityMask(m_effectGradient);
                m_title[IndexApp]->setGraphicsEffect(m_textEffect);
            });
}

inline void KNMainWindowIconButton::startInOutAnime(QTimeLine *timeLine)
{
    //Stop in and out anime.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set the start range of the time line.
    timeLine->setStartFrame(m_icon[IndexApp]->x());
    //Set the animation.
    timeLine->start();
}

inline void KNMainWindowIconButton::updateTitleLabel()
{
    //Get the maximum label width.
    int titleLabelWidth=
            qMax(m_title[IndexApp]->fontMetrics().width(
                     m_title[IndexApp]->text()),
                 m_title[IndexPreference]->fontMetrics().width(
                     m_title[IndexPreference]->text()));
    //Resize the button.
    resize(titleLabelWidth+TextLeft, height());
    //Update the size of the title labels.
    m_title[IndexApp]->resize(titleLabelWidth,
                              m_title[IndexApp]->fontMetrics().height());
    m_title[IndexPreference]->resize(titleLabelWidth,
                                     m_title[IndexPreference]->height());
}

inline QTimeLine *KNMainWindowIconButton::generateTimeLine()
{
    QTimeLine *timeLine=new QTimeLine(200, this);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    timeLine->setUpdateInterval(16);
    return timeLine;
}
