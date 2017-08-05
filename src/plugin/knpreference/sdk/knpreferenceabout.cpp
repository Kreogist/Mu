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
#include <QApplication>
#include <QBoxLayout>
#include <QLabel>
#include <QScrollBar>
#include <QTextEdit>

#include "knimagelabel.h"
#include "knlocalemanager.h"
#include "knthememanager.h"
#include "knlabelbutton.h"
#include "kndpimanager.h"
#include "sao/knsaostyle.h"

#include "knpreferenceabout.h"

#include <QDebug>

KNPreferenceAbout::KNPreferenceAbout(QWidget *parent) :
    QWidget(parent),
    m_iconContent(new KNImageLabel(this)),
    m_checkUpdate(new KNLabelButton(this)),
    m_otherText(new QTextEdit(this))
{
    //Configure the label fonts.
    QFont labelFonts=font();
    labelFonts.setPixelSize(knDpi->height(15));
    //Initial the labels.
    for(int i=0; i<ContentIndexCount; ++i)
    {
        //Initial the label.
        m_textContent[i]=new QLabel(this);
        //Configure the label.
        m_textContent[i]->setObjectName("PreferenceAboutLabel");
        m_textContent[i]->setAlignment(Qt::AlignHCenter);
        m_textContent[i]->setFont(labelFonts);
    }
    //Register the version and copyright label.
    knTheme->registerWidget(m_textContent[Copyright]);
    //Configure the other text edit.
    m_otherText->setObjectName("PreferenceAboutCopyright");
    m_otherText->setFrameStyle(QFrame::NoFrame);
    m_otherText->setReadOnly(true);
    m_otherText->viewport()->setCursor(Qt::ArrowCursor);
    knTheme->registerWidget(m_otherText);
    KNSaoStyle::styleVerticalScrollBar(m_otherText->verticalScrollBar());
    //Configure check update button.
    m_checkUpdate->setText(QApplication::applicationVersion());
    m_checkUpdate->setObjectName("PreferenceAboutLabel");
    m_checkUpdate->setAlignment(Qt::AlignCenter);
    m_checkUpdate->setCursor(Qt::PointingHandCursor);
    knTheme->registerWidget(m_checkUpdate);

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(knDpi->height(15));
    setLayout(mainLayout);
    //Add the stretch.
    mainLayout->addStretch();
    //Add the icon to main layout.
    mainLayout->addWidget(m_iconContent, 0, Qt::AlignHCenter);
    //Initial the content layout.
    //Initial the main layout.
    QBoxLayout *contentLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                             mainLayout->widget());
    contentLayout->setContentsMargins(0,0,0,0);
    contentLayout->setSpacing(knDpi->height(8));
    mainLayout->addLayout(contentLayout);
    //Add the stretch to bottom.
    mainLayout->addStretch();
    //Add version and copyright labels to content layout.
    contentLayout->addWidget(m_textContent[Title], 0, Qt::AlignHCenter);
    contentLayout->addSpacing(knDpi->height(8));
    contentLayout->addWidget(m_checkUpdate, 0, Qt::AlignHCenter);
    contentLayout->addWidget(m_textContent[Copyright], 0, Qt::AlignHCenter);
    contentLayout->addWidget(m_otherText, 0, Qt::AlignHCenter);
    contentLayout->addStretch();

    //Set the default label.
    m_iconContent->setPixmap(QPixmap("://icon/mu.png"));
    //Configure the label.
    m_iconContent->setFixedSize(knDpi->size(16, 16));
    m_textContent[Title]->setText(QApplication::applicationDisplayName());

    //Link the retranslate.
    knI18n->link(this, &KNPreferenceAbout::retranslate);
    retranslate();
}

void KNPreferenceAbout::resizeEvent(QResizeEvent *event)
{
    //Do resize.
    QWidget::resizeEvent(event);
    //Calculate Icon size.
    int iconSize=(qreal)qMin(width(), height())*0.236;
    //Resize the icon.
    m_iconContent->setFixedSize(iconSize, iconSize);
    //Resize the maximum width of the text content.
    m_otherText->setMinimumWidth(iconSize+(iconSize<<1));
    //Change the caption size.
    QFont captionFont=m_textContent[Title]->font();
    //Calculate the title size.
    iconSize=(qreal)iconSize*0.236;
    captionFont.setPixelSize(iconSize);
    //Set to caption font.
    m_textContent[Title]->setFont(captionFont);
    //Set the maximum height of the title.
    m_otherText->setMaximumHeight(iconSize<<2);
    //Calculate the caption size.
    int minimumSize=knDpi->height(13);
    iconSize=iconSize>>2;
    captionFont.setPixelSize(iconSize<minimumSize?minimumSize:iconSize);
    //Set font to those captions.
    m_checkUpdate->setFont(captionFont);
    m_textContent[Copyright]->setFont(captionFont);
}

void KNPreferenceAbout::retranslate()
{
    //Update the content data.
    m_textContent[Copyright]->setText(QString(QChar(169)) +
                                      " 2013-2017 Kreogist Dev Team\n");
    //Update the thank list.
    m_otherText->setPlainText(
                tr("This program is free software; you can redistribute it "
                   "and/or modify it under the terms of the GNU General Public "
                   "License as published by the Free Software Foundation; "
                   "either version 2 of the License, or (at your option) any "
                   "later version.\n\n"
                   "This program is distributed in the hope that it will be "
                   "useful, but WITHOUT ANY WARRANTY; without even the implied "
                   "warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR "
                   "PURPOSE.  See the GNU General Public License for more "
                   "details.\n\n"
                   "You should have received a copy of the GNU General Public "
                   "License along with this program; if not, see <http://www.gn"
                   "u.org/licenses/>.")+"\n\n"+
                tr("Author")+"\n"
                "Tojo Saki <tomguts@126.com>\n"
                "Luming Wang <wlm199558@126.com>\n"
                "Dayou Zhang <1079286704@qq.com>\n"
                "Jiayi Zhang <bf109g2@126.com>\n"
                "Youraku <1497667718@qq.com>\n"
                "Joe Johnson <liquanquan123@vip.qq.com>\n"
                "Freddie <freddie.wanah@gmail.com>\n"
                "Nick Tang <1953547787@qq.com>\n"
                "Frantic1048 <archer@frantic1048.com>\n"
                "Yayuan Yan <yy1119@nyu.edu>\n"
                "\n"+
                tr("Special Thanks") +
                "\n"
                "WangBin\n"
                "Sou Bunnbu\n"
                "darkblackswords\n"
                "Yuri Ogawara\n"
                "1dot75cm\n"
                "Lin Yuan\n"
                "miaolapd\n"
                "Haoting Xu\n"
                "hosiet\n"
                "\n"+
                tr("This program uses Qt Version ")+
                QT_VERSION_STR+
                tr("\n"
                   "Qt is a C++ toolkit for cross-platform application developm"
                   "ent.\n"
                   "Copyright (C) 2015 The Qt Company Ltd and other contributor"
                   "s.\n"
                   "Qt and the Qt logo are trademarks of The Qt Company Ltd.\n"
                   "Qt is The Qt Company Ltd product developed as an open sourc"
                   "e project. See qt.io for more information."));
}

void KNPreferenceAbout::onActionShowUpdate()
{
    ;
}
