/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QLabel>
#include <QPushButton>

#include "../knfontdialog.h"
#include "../knmousedetectlabel.h"

#include "knlocalemanager.h"

#include "knpreferenceitemfont.h"

#include <QDebug>

KNPreferenceItemFont::KNPreferenceItemFont(QWidget *parent) :
    KNPreferenceItemBase(parent)
{
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
    insertStretch();
    insertWidget(m_fontOverview);
    insertSpacing(5);
    insertWidget(m_changeFont);
    insertSpacing(5);
    insertWidget(m_previewIcon);

    //Linked the translation.
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
            this, &KNPreferenceItemFont::retranslate);
    //Do retranslate.
    retranslate();
}

KNPreferenceItemFont::~KNPreferenceItemFont()
{
    delete m_previewTooltip;
}

QVariant KNPreferenceItemFont::defaultValue() const
{
    return QVariant::fromValue(m_defaultFont);
}

QVariant KNPreferenceItemFont::value() const
{
    return QVariant::fromValue(m_value);
}

void KNPreferenceItemFont::setDefaultValue(const QVariant &defaultValue)
{
    //Save the font.
    m_defaultFont=defaultValue.value<QFont>();
    //Set the font as default value.
    setValue(QVariant::fromValue(m_defaultFont));
}

void KNPreferenceItemFont::setValue(const QVariant &value)
{
    //Set the value.
    m_value=value.value<QFont>();
    //Set the preview font and preview text(The font name).
    m_previewTooltip->setFont(m_value);
    m_previewTooltip->setText(m_value.family());
    //Update the overview label info.
    updateOverview();
}

void KNPreferenceItemFont::retranslate()
{
    //Update the font information.
    updateOverview();
}

void KNPreferenceItemFont::onActionSelectFont()
{
    setValue(KNFontDialog::getFont(m_value));
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
    m_fontOverview->setText(m_value.family() +
                            tr(", Size: ") +
                            QString::number(m_value.pixelSize()) +
                            (m_value.bold()?tr(", Bold"):"") +
                            (m_value.italic()?tr(", Italic"):"") +
                            (m_value.underline()?tr(", Underline"):"") +
                            (m_value.strikeOut()?tr(", Strike Out"):"") +
                            (m_value.kerning()?tr(", Kerning"):""));
}
