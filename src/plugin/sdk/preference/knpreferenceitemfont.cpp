/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>

#include "knglobal.h"
#include "knfontdialog.h"
#include "knmousedetectlabel.h"

#include "knpreferenceitemfont.h"

#include <QDebug>

KNPreferenceItemFont::KNPreferenceItemFont(QWidget *parent) :
    KNPreferenceItem(parent)
{
    setFixedHeight((PreferenceSingleItemHeight<<1)+5);
    //Initial the overview label.
    m_fontOverview=new QLabel(this);
    //Set the font of overview.
    QPalette pal=m_fontOverview->palette();
    pal.setColor(QPalette::WindowText, QColor(0xA0, 0xA0, 0xA0));
    m_fontOverview->setPalette(pal);
    //Initial the preview label.
    m_previewIcon=new KNMouseDetectLabel(this);
    m_previewIcon->setScaledContents(true);
    m_previewIcon->setFixedSize(16, 16);
    m_previewIcon->setPixmap(QPixmap("://public/preview_font.png"));
    connect(m_previewIcon, &KNMouseDetectLabel::mouseEntered,
            this, &KNPreferenceItemFont::onActionShowTooltip);
    connect(m_previewIcon, &KNMouseDetectLabel::mouseLeaved,
            this, &KNPreferenceItemFont::onActionHideTooltip);
    //Initial the preview tooltip.
    m_previewTooltip=new QLabel;
    m_previewTooltip->setContentsMargins(10,10,10,10);
    m_previewTooltip->setWindowFlags(Qt::ToolTip);
    m_previewTooltip->hide();
    //Initial the change font button.
    m_changeFont=new QPushButton(this);
    pal=m_changeFont->palette();
    pal.setColor(QPalette::Button, QColor(0x20, 0x20, 0x20));
    pal.setColor(QPalette::Window, QColor(0x20, 0x20, 0x20));
    pal.setColor(QPalette::ButtonText, QColor(0x60, 0x60, 0x60));
    m_changeFont->setPalette(pal);
    m_changeFont->setText("...");
    m_changeFont->setFixedWidth(25);
    connect(m_changeFont, SIGNAL(clicked()), this, SLOT(onActionSelectFont()));

    //Insert these widgets.
    QBoxLayout *fontControlLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                                 mainLayout()->widget());
    fontControlLayout->setContentsMargins(0,0,0,0);
    fontControlLayout->setSpacing(5);
    mainLayout()->addLayout(fontControlLayout, 1);

    fontControlLayout->addWidget(m_fontOverview, 1, Qt::AlignLeft);

    QBoxLayout *overviewLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                              mainLayout()->widget());
    overviewLayout->setContentsMargins(0,0,0,0);
    overviewLayout->setSpacing(5);
    overviewLayout->addWidget(m_changeFont);
    overviewLayout->addWidget(m_previewIcon);
    overviewLayout->addStretch();
    fontControlLayout->addLayout(overviewLayout);

    //Linked the translation.
    connect(KNGlobal::instance(), &KNGlobal::requireRetranslate,
            this, &KNPreferenceItemFont::retranslate);
    //Do retranslate.
    retranslate();
}

KNPreferenceItemFont::~KNPreferenceItemFont()
{
    delete m_previewTooltip;
}

void KNPreferenceItemFont::setDefaultValue(const QVariant &defaultValue)
{
    //Set the font as default value.
    setValue(QVariant::fromValue(defaultValue.value<QFont>()));
}

void KNPreferenceItemFont::setValue(const QVariant &value)
{
    //Set the value.
    QFont fontValue=value.value<QFont>();
    //Set the preview font and preview text(The font name).
    m_previewTooltip->setFont(fontValue);
    m_previewTooltip->setText(fontValue.family());
    //Update the overview label info.
    updateOverview();
    //Set the value.
    KNPreferenceItem::setValue(QVariant::fromValue(fontValue));
}

void KNPreferenceItemFont::retranslate()
{
    //Update the font information.
    updateOverview();
}

void KNPreferenceItemFont::onActionSelectFont()
{
    setValue(KNFontDialog::getFont(m_previewTooltip->font()));
}

void KNPreferenceItemFont::onActionShowTooltip()
{
    //Move the tooltip.
    QPoint previewButtonPos=mapToGlobal(m_previewIcon->pos());
    m_previewTooltip->move(previewButtonPos.x()+m_previewIcon->width()+12,
                           previewButtonPos.y()+(m_previewIcon->height()>>1)-
                           (m_previewTooltip->sizeHint().height()>>1));
    //Show the tooltip.
    m_previewTooltip->show();
}

void KNPreferenceItemFont::onActionHideTooltip()
{
    m_previewTooltip->hide();
}

void KNPreferenceItemFont::updateOverview()
{
    QFont targetFont=m_previewTooltip->font();
    m_fontOverview->setText(targetFont.family() +
                            tr(", Size: ") +
                            QString::number(targetFont.pixelSize()) +
                            (targetFont.bold()?tr(", Bold"):"") +
                            (targetFont.italic()?tr(", Italic"):"") +
                            (targetFont.underline()?tr(", Underline"):"") +
                            (targetFont.strikeOut()?tr(", Strike Out"):"") +
                            (targetFont.kerning()?tr(", Kerning"):""));
}
