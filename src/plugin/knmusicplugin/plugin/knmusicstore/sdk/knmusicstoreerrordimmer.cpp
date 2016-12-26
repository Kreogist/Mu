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
#include <QLabel>
#include <QTimeLine>
#include <QNetworkReply>

#include "knglobal.h"
#include "knthememanager.h"
#include "knlocalemanager.h"

#include "knmusicstoreglobal.h"

#include "knmusicstoreerrordimmer.h"

#define MinBackgroundAlpha 0
#define ErrorBackgroundAlpha 220

KNMusicStoreErrorDimmer::KNMusicStoreErrorDimmer(QWidget *parent) :
    QWidget(parent),
    m_background(new QTimeLine(200, this)),
    m_title(new QLabel(this)),
    m_description(new QLabel(this)),
    m_errorType(-1),
    m_errorId(-1)
{
    setObjectName("MusicStoreErrorDimmer");
    //Set properties.
    setAutoFillBackground(true);
    //Configure the title font.
    m_title->setAlignment(Qt::AlignHCenter);
    QFont labelFont=m_title->font();
    labelFont.setBold(true);
    labelFont.setPixelSize(24);
    m_title->setFont(labelFont);
    //Configure the description label.
    m_description->setAlignment(Qt::AlignHCenter);
    labelFont=m_description->font();
    labelFont.setPixelSize(15);
    m_description->setFont(labelFont);
    //Configure the time line.
    m_background->setStartFrame(MinBackgroundAlpha);
    m_background->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_background, &QTimeLine::frameChanged,
            this, &KNMusicStoreErrorDimmer::onBackgroundAlphaChange);
    connect(m_background, &QTimeLine::finished,
            this, &KNMusicStoreErrorDimmer::onTimeLineFinish);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setSpacing(0);
    //Add widget to layout.
    mainLayout->addStretch();
    mainLayout->addWidget(m_title);
    mainLayout->addSpacing(15);
    mainLayout->addWidget(m_description);
    mainLayout->addStretch();
    setLayout(mainLayout);

    //Link the theme manager.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNMusicStoreErrorDimmer::onThemeChanged);
    onThemeChanged();

    //Link the retranslator.
    knI18n->link(this, &KNMusicStoreErrorDimmer::retranslate);
    retranslate();
}

void KNMusicStoreErrorDimmer::hideDimmer()
{
    //Hide the title and description.
    setLabelVisible(false);
    //Start to hide the dimmer.
    startAnime(MinBackgroundAlpha);
}

void KNMusicStoreErrorDimmer::showDimmer(int errorType, int errorId)
{
    //Save the type and id.
    m_errorType=errorType;
    m_errorId=errorId;
    //Update the label text.
    updateLabelText();
    //Set the initial background.
    onBackgroundAlphaChange(MinBackgroundAlpha);
    //Show the error dimmer.
    show();
    //Update the show dimmer.
    startAnime(ErrorBackgroundAlpha);
}

void KNMusicStoreErrorDimmer::reset()
{
    //Reset the error id and type.
    m_errorType=-1;
    m_errorId=-1;
    //Clear the label.
    m_title->clear();
    m_description->clear();
}

void KNMusicStoreErrorDimmer::retranslate()
{
    //Update the error text.
    m_connectionError=tr("Connection Error");
    m_contentError=tr("Reply Content Error");
    //Update the solution text.
    m_checkAndRetrySolution=tr("Please check your Internet connection, and then"
                               " click refresh.");
    m_contactSolution=tr("Please send an E-mail to kreogistdevteam@126.com to "
                         "report this bug.");
    //Update the label text.
    updateLabelText();
}

void KNMusicStoreErrorDimmer::onThemeChanged()
{
    //Update the palette.
    setPalette(knTheme->getPalette(objectName()));
    //Update the background by calling the frame change slot.
    onBackgroundAlphaChange(MinBackgroundAlpha);
}

void KNMusicStoreErrorDimmer::onTimeLineFinish()
{
    //Check show label flag for show the dimmer.
    if((m_background->endFrame()==ErrorBackgroundAlpha))
    {
        //Show the label.
        setLabelVisible(true);
    }
    else
    {
        //Hide the widget.
        hide();
    }
}

void KNMusicStoreErrorDimmer::onBackgroundAlphaChange(int frame)
{
    //Get the palette.
    QPalette pal=palette();
    //Get the background color.
    QColor backgroundColor=palette().color(QPalette::Window);
    //Update the alpha.
    backgroundColor.setAlpha(frame);
    //Update the background palette.
    pal.setColor(QPalette::Window, backgroundColor);
    //Set the palette.
    setPalette(pal);
}

inline void KNMusicStoreErrorDimmer::updateLabelText()
{
    //Check the invalid data.
    if(m_errorType==-1)
    {
        //Ignore the update label requirement.
        return;
    }
    //Check the type of the error.
    if(m_errorType)
    {
        //Customized error.
        m_title->setText(m_contentError);
        m_description->setText(knMusicStoreGlobal->contentErrorText(m_errorId)
                               + " #" + QString::number(m_errorId) + "\n" +
                               m_contactSolution);
        return;
    }
    //System error.
    m_title->setText(m_connectionError);
    m_description->setText(knGlobal->connectionErrorText(m_errorId)
                           + " #" + QString::number(m_errorId) + "\n" +
                           m_checkAndRetrySolution);
}

inline void KNMusicStoreErrorDimmer::startAnime(int endFrame)
{
    //Stop the background.
    m_background->stop();
    //Update stop the current time line.
    m_background->setEndFrame(endFrame);
    //Start the background.
    m_background->start();
}

inline void KNMusicStoreErrorDimmer::setLabelVisible(bool visible)
{
    //Configure the labels.
    m_title->setVisible(visible);
    m_description->setVisible(visible);
}
