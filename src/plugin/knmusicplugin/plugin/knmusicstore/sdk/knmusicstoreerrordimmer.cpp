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
#include "knopacityanimetextbutton.h"

#include "knmusicstoreglobal.h"

#include "knmusicstoreerrordimmer.h"

#define MinBackgroundAlpha      0
#define ErrorBackgroundAlpha    220
#define MinimumButtonWidth      84

KNMusicStoreErrorDimmer::KNMusicStoreErrorDimmer(QWidget *parent) :
    QWidget(parent),
    m_background(new QTimeLine(500, this)),
    m_title(new QLabel(this)),
    m_description(new QLabel(this)),
    m_okay(new KNOpacityAnimeTextButton(this)),
    m_retry(new KNOpacityAnimeTextButton(this)),
    m_contact(new KNOpacityAnimeTextButton(this)),
    m_errorType(-1),
    m_errorId(-1),
    m_enabledOkay(false)
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
    //Configure buttons.
    m_okay->setMinimumWidth(MinimumButtonWidth);
    connect(m_okay, &KNOpacityAnimeTextButton::clicked,
            this, &KNMusicStoreErrorDimmer::hideDimmer);
    m_retry->setMinimumWidth(MinimumButtonWidth);
    connect(m_retry, &KNOpacityAnimeTextButton::clicked,
            [=]
            {
                //Send refresh signal.
                emit requireRefresh();
                //Hide the dimmer.
                hideDimmer();
            });
    m_contact->setMinimumWidth(MinimumButtonWidth);
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
    setLayout(mainLayout);
    //Add widget to layout.
    mainLayout->addStretch();
    mainLayout->addWidget(m_title);
    mainLayout->addSpacing(15);
    mainLayout->addWidget(m_description);
    //Add button layout.
    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    buttonLayout->setSpacing(9);
    //Add button layout to main layout.
    mainLayout->addSpacing(24);
    mainLayout->addLayout(buttonLayout);
    //Add buttons to layout.
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_retry);
    buttonLayout->addWidget(m_okay);
    buttonLayout->addWidget(m_contact);
    buttonLayout->addStretch();
    mainLayout->addStretch();

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
    setContentVisible(false);
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
    //Hide the label and buttons.
    setContentVisible(false);
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

void KNMusicStoreErrorDimmer::setOkayEnabled(bool isEnabled)
{
    //Save the value.
    m_enabledOkay=isEnabled;
}

void KNMusicStoreErrorDimmer::retranslate()
{
    //Update the error text.
    m_connectionError=tr("Connection Error");
    m_contentError=tr("Reply Content Error");
    //Update the solution text.
    m_checkAndRetrySolution=tr("Please check your Internet connection, and then"
                               " click 'Refresh'.");
    m_contactSolution=tr("Please send an E-mail to kreogistdevteam@126.com to "
                         "report this bug.");
    //Update the button text.
    m_okay->setText(tr("Okay"));
    m_retry->setText(tr("Refresh"));
    m_contact->setText(tr("Contact"));
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
        setContentVisible(true);
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
    m_background->setFrameRange(palette().color(QPalette::Window).alpha(),
                                endFrame);
    //Start the background.
    m_background->start();
}

inline void KNMusicStoreErrorDimmer::setContentVisible(bool visible)
{
    //Configure the labels.
    m_title->setVisible(visible);
    m_description->setVisible(visible);
    //Check visible.
    if(visible)
    {
        //Check the control button.
        m_okay->setVisible(m_enabledOkay); //Controlled by value.
        m_retry->setVisible(!m_errorType); //Retry show for Internet err.
        //Ignore the contact for the future expanding.
        //m_contact->setVisible(m_errorType); //Contact show for customized err.
    }
    else
    {
        //Hide all buttons.
        m_okay->hide();
        m_retry->hide();
        m_contact->hide();
    }
}
