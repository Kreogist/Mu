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
#include <QPushButton>
#include <QLabel>

#include "kndpimanager.h"
#include "knmousedetectlabel.h"
#include "knfontdialog.h"

#include "knpreferencepanelfontitem.h"

#define LabelWidth 218

KNPreferencePanelFontItem::KNPreferencePanelFontItem(QWidget *parent) :
    KNPreferencePanelItem(parent),
    m_previewLabel(new KNMouseDetectLabel(this)),
    m_selectFont(new QPushButton(this)),
    m_informationLabel(new QLabel(this)),
    m_previewTooltip(new QLabel)
{
    //Configure the select button.
    m_selectFont->setFixedSize(knDpi->size(25, 20));
    m_selectFont->setText("...");
    m_selectFont->setPalette(palette());
    connect(m_selectFont, &QPushButton::clicked,
            [=]
            {
                //Prepare the original font.
                QFont selectedFont;
                //Get the font from the dialog.
                if(KNFontDialog::getFont(
                            selectedFont,
                            tr("Select font for %1").arg(titleText()),
                            m_font))
                {
                    //Generate a variable for the font.
                    QFont fontVariable=selectedFont;
                    //Update the font data.
                    setWidgetValue(fontVariable);
                    //Update the signal.
                    emit valueChanged();
                }
            });
    //Configure preview label.
    m_previewLabel->setFixedSize(knDpi->size(16, 16));
    m_previewLabel->setScaledContents(true);
    m_previewLabel->setPixmap(QPixmap("://preference/preview_font.png"));
    connect(m_previewLabel, &KNMouseDetectLabel::mouseEntered,
            this, &KNPreferencePanelFontItem::onActionShowTooltip);
    connect(m_previewLabel, &KNMouseDetectLabel::mouseLeaved,
            m_previewTooltip, &QLabel::hide);
    //Configure the preview tooltip widget.
    m_previewTooltip->setContentsMargins(knDpi->margins(10,10,10,10));
    m_previewTooltip->setWindowFlags(Qt::ToolTip);
    m_previewTooltip->hide();
    //Configure the information data label.
    m_informationLabel->setPalette(palette());
    //Construct the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    mainLayout->setSpacing(knDpi->width(5));
    mainLayout->addWidget(m_selectFont);
    mainLayout->addWidget(m_previewLabel);
    mainLayout->addWidget(m_informationLabel, 2);
    //Build the widget.
    buildWidgetLayout(mainLayout);
}

QVariant KNPreferencePanelFontItem::value() const
{
    return m_font;
}

void KNPreferencePanelFontItem::setWidgetValue(const QVariant &value)
{
    //Save the font.
    m_font=value.value<QFont>();
    //Set the font to the preview data.
    QFont dpiScaledFont=m_font;
    dpiScaledFont.setPixelSize(knDpi->height(m_font.pixelSize()));
    m_previewTooltip->setFont(dpiScaledFont);
    m_previewTooltip->setText(titleText());
    //Separate the data.
    QString fontText=tr("%1, %2px%3%4%5%6%7").arg(
                m_font.family(),
                QString::number(m_font.pixelSize()),
                m_font.bold()?tr(", Bold"):"",
                m_font.italic()?tr(", Italic"):"",
                m_font.underline()?tr(", Underline"):"",
                m_font.strikeOut()?tr(", Strike Out"):"",
                m_font.kerning()?tr(", Kerning"):""),
            elidedFontText=m_informationLabel->fontMetrics().elidedText(
                fontText, Qt::ElideRight, LabelWidth);
    //Check the font text is the same as elided.
    if(elidedFontText==fontText)
    {
        //Clear the tooltip.
        m_informationLabel->setText(fontText);
        m_informationLabel->setToolTip("");
    }
    else
    {
        //Set the elided text.
        m_informationLabel->setText(elidedFontText);
        m_informationLabel->setToolTip(fontText);
    }
}

bool KNPreferencePanelFontItem::isEqual(const QVariant &currentValue,
                                        const QVariant &originalValue)
{
    //Cast it as font.
    QFont currentFont=currentValue.value<QFont>(),
            originalFont=originalValue.value<QFont>();
    //Check the font data.
    return currentFont.family()==originalFont.family() &&
            currentFont.pixelSize()==originalFont.pixelSize() &&
            currentFont.bold()==originalFont.bold() &&
            currentFont.italic()==originalFont.italic() &&
            currentFont.underline()==originalFont.underline() &&
            currentFont.strikeOut()==originalFont.strikeOut() &&
            currentFont.kerning()==originalFont.kerning();
}

void KNPreferencePanelFontItem::onActionShowTooltip()
{
    //Move the tooltip.
    QPoint previewButtonPos=mapToGlobal(m_previewLabel->pos());
    m_previewTooltip->move(previewButtonPos.x()+m_previewLabel->width()+12,
                           previewButtonPos.y()+(m_previewLabel->height()>>1)-
                           (m_previewTooltip->sizeHint().height()>>1));
    //Show the tooltip.
    m_previewTooltip->show();
}
