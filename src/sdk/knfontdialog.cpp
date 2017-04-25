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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "knfontdialog.h"

#include <QScopedPointer>
#include <QFontComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QListWidget>
#include <QBoxLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QSlider>
#include <QCheckBox>

#include "knfontdialog.h"

#include <QDebug>

KNFontDialog::KNFontDialog(QWidget *parent) :
    QDialog(parent)
{
    //Initial the layout.
    QGridLayout *mainLayout=new QGridLayout(this);
    setLayout(mainLayout);
    //Generate the caption.
    m_caption[0]=new QLabel(tr("Font"), this);
    mainLayout->addWidget(m_caption[0], 0, 0, 1, 1);
    m_caption[1]=new QLabel(tr("Size"), this);
    mainLayout->addWidget(m_caption[1], 0, 1, 1, 1);
    m_caption[2]=new QLabel(tr("Styles"), this);
    mainLayout->addWidget(m_caption[2], 4, 1, 1, 1);

    //Initial the font family combo box and list view.
    m_fontFamily=new QFontComboBox(this);
    m_fontFamily->setMaximumWidth(204);
    m_fontModel=m_fontFamily->model();
    connect(m_fontFamily,
            static_cast<void (QFontComboBox::*)(int)>(
                &QFontComboBox::currentIndexChanged),
            this, &KNFontDialog::onFontChanged);
    mainLayout->addWidget(m_fontFamily, 1, 0, 1, 1);
    m_fontList=new QListView(this);
    m_fontList->setMaximumWidth(204);
    m_fontList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_fontList->setModel(m_fontModel);
    m_fontList->setItemDelegate(m_fontFamily->itemDelegate());
    connect(m_fontList->selectionModel(), &QItemSelectionModel::currentRowChanged,
            [=](const QModelIndex &current, const QModelIndex &previous)
            {
                Q_UNUSED(previous)
                onFontChanged(current.row());
            });
    mainLayout->addWidget(m_fontList, 2, 0, 4, 1);

    //Initial the size spin.
    m_sizeSpin=new QSpinBox(this);
    m_sizeSpin->setRange(1, 1296);
    connect(m_sizeSpin,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &KNFontDialog::onSizeChanged);
    mainLayout->addWidget(m_sizeSpin, 1, 1, 1, 1);
    //Initial the slider box and standard font size list.
    QBoxLayout *sizeBox=new QBoxLayout(QBoxLayout::LeftToRight,
                                       mainLayout->widget());
    sizeBox->setContentsMargins(0,0,0,0);
    sizeBox->setSpacing(0);

    m_fontSizeList=new QListWidget(this);
    m_fontSizeList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_fontSizeList->setMaximumWidth(70);
    m_standardSize<<6<<8<<9<<10<<11<<12<<13<<14<<18<<24<<36<<48<<64<<72<<96
                  <<144<<288;
    QStringList fontSizes;
    for(auto i=m_standardSize.begin(); i!=m_standardSize.end(); ++i)
    {
        fontSizes << QString::number(*i);
    }
    m_fontSizeList->addItems(fontSizes);
    connect(m_fontSizeList, &QListWidget::currentRowChanged,
            [=](int currentRow)
               {
                   onSizeChanged(m_standardSize.at(currentRow));
               });
    sizeBox->addWidget(m_fontSizeList, 1);

    m_sizeSlider=new QSlider(Qt::Vertical, this);
    m_sizeSlider->setRange(1, 288);
    connect(m_sizeSlider, &QSlider::valueChanged, this, &KNFontDialog::onSizeChanged);
    sizeBox->addWidget(m_sizeSlider);
    mainLayout->addLayout(sizeBox, 3, 1, 1, 1);

    //Initial the font styles box.
    QBoxLayout *stylesBox=new QBoxLayout(QBoxLayout::TopToBottom,
                                         mainLayout->widget());
    stylesBox->setContentsMargins(0,0,0,0);
    stylesBox->setSpacing(0);
    for(int i=0; i<FontStylesCount; i++)
    {
        m_fontStyles[i]=new QCheckBox(this);
        stylesBox->addWidget(m_fontStyles[i]);
        connect(m_fontStyles[i], SIGNAL(stateChanged(int)),
                this, SLOT(synchronizeFont()));
    }
    m_fontStyles[StyleBold]->setText(tr("Bold"));
    m_fontStyles[StyleItalic]->setText(tr("Italic"));
    m_fontStyles[StyleUnderline]->setText(tr("Underline"));
    m_fontStyles[StyleStrikeout]->setText(tr("Strike Out"));
    m_fontStyles[StyleKerning]->setText(tr("Kerning"));
    stylesBox->addStretch();
    mainLayout->addLayout(stylesBox, 5, 1, 1, 1);

    QBoxLayout *finalLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                           mainLayout->widget());
    //Initial the okay, cancel and preview.
    m_okayButton=new QPushButton(tr("OK"), this);
    connect(m_okayButton, SIGNAL(clicked()), this, SLOT(accept()));
    finalLayout->addWidget(m_okayButton);
    m_cancelButton=new QPushButton(tr("Cancel"), this);
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    finalLayout->addWidget(m_cancelButton);

    finalLayout->addStretch();

    m_caption[3]=new QLabel(tr("Sample"), this);
    finalLayout->addWidget(m_caption[3]);

    //Initial the preview edit.
    m_previewEdit=new QLineEdit(this);
    m_previewEdit->setAlignment(Qt::AlignCenter);
    m_previewEdit->setText(tr("AaBbYyZz"));
    m_previewEdit->setMinimumSize(205, 85);
    finalLayout->addWidget(m_previewEdit);
    mainLayout->addLayout(finalLayout, 0, 3, 6, 1);
}

KNFontDialog::~KNFontDialog()
{

}

bool KNFontDialog::getFont(QFont &selectedFont,
                           const QString &title,
                           const QFont &defaultFont)
{
    //Initial the font dialog.
    QScopedPointer<KNFontDialog> fontDialog(new KNFontDialog);
    //Configure the dialog.
    if(!title.isEmpty())
    {
        fontDialog->setWindowTitle(title);
    }
    //Set the font.
    fontDialog->setCurrentFont(defaultFont);
    //Launch the dialog.
    if(fontDialog->exec()==QDialog::Accepted)
    {
        //Save the font to selected font.
        selectedFont=fontDialog->currentFont();
        //Return the true for success.
        return true;
    }
    //Set the font as default font.
    selectedFont=defaultFont;
    //Return false for cancel or error happend.
    return false;
}

void KNFontDialog::setCurrentFont(const QFont &font)
{
    m_currentFont=font;
    //Set the widget to display the font information.
    applyFontData();
}

QFont KNFontDialog::currentFont() const
{
    return m_currentFont;
}

void KNFontDialog::onFontChanged(int currentIndex)
{
    //Block the font signals.
    blockFontListWidget();
    //Set the data.
    m_fontFamily->setCurrentIndex(currentIndex);
    QModelIndex fontIndex=m_fontModel->index(currentIndex, 0);
    m_fontList->setCurrentIndex(fontIndex);
    m_fontList->scrollTo(fontIndex);
    m_fontList->viewport()->update();
    //Synchronize the font.
    synchronizeFont();
    //Release the font tunner signals.
    releaseFontListWidget();
}

void KNFontDialog::onSizeChanged(int currentSize)
{
    //Block the size tunner signals.
    blockFontSizeWidget();
    //Apply size changed.
    applySizeChanged(currentSize);
    //Synchronize the font.
    synchronizeFont();
    //Release the size tunner signals.
    releaseFontSizeWidget();
}

void KNFontDialog::synchronizeFont()
{
    //Get the font data.
    m_currentFont=m_fontFamily->currentFont();
    m_currentFont.setFamily(m_fontFamily->currentText());
    m_currentFont.setPixelSize(m_sizeSpin->value());
    m_currentFont.setBold(m_fontStyles[StyleBold]->isChecked());
    m_currentFont.setItalic(m_fontStyles[StyleItalic]->isChecked());
    m_currentFont.setUnderline(m_fontStyles[StyleUnderline]->isChecked());
    m_currentFont.setStrikeOut(m_fontStyles[StyleStrikeout]->isChecked());
    m_currentFont.setKerning(m_fontStyles[StyleKerning]->isChecked());
    //Give the font to text edit.
    m_previewEdit->setFont(m_currentFont);
}

void KNFontDialog::applyFontData()
{
    //Block widgets signal.
    blockFontListWidget();
    blockFontSizeWidget();
    blockFontStyleWidget();
    //Apply font settings.
    m_fontFamily->setCurrentFont(m_currentFont);
    QModelIndex currentFontIndex=m_fontModel->index(m_fontFamily->currentIndex(),
                                                    0);
    m_fontList->setCurrentIndex(currentFontIndex);
    m_fontList->scrollTo(currentFontIndex);
    //Apply size changed.
    applySizeChanged(m_currentFont.pixelSize());
    //Set the style.
    m_fontStyles[StyleBold]->setChecked(m_currentFont.bold());
    m_fontStyles[StyleItalic]->setChecked(m_currentFont.italic());
    m_fontStyles[StyleUnderline]->setChecked(m_currentFont.underline());
    m_fontStyles[StyleStrikeout]->setChecked(m_currentFont.strikeOut());
    m_fontStyles[StyleKerning]->setChecked(m_currentFont.kerning());
    //Set the preview font to this.
    m_previewEdit->setFont(m_currentFont);
    //Release widgets signal.
    releaseFontListWidget();
    releaseFontSizeWidget();
    releaseFontStyleWidget();
}

void KNFontDialog::applySizeChanged(int currentSize)
{
    //Set the data.
    m_sizeSpin->setValue(currentSize);
    m_sizeSlider->setValue(currentSize);
    int sizeIndex=m_standardSize.indexOf(currentSize);
    if(sizeIndex!=-1)
    {
        m_fontSizeList->setCurrentRow(sizeIndex);
    }
}

void KNFontDialog::blockFontListWidget()
{
    m_fontFamily->blockSignals(true);
    m_fontList->blockSignals(true);
    m_fontList->selectionModel()->blockSignals(true);
}

void KNFontDialog::releaseFontListWidget()
{
    m_fontFamily->blockSignals(false);
    m_fontList->blockSignals(false);
    m_fontList->selectionModel()->blockSignals(false);
}

void KNFontDialog::blockFontSizeWidget()
{
    m_sizeSpin->blockSignals(true);
    m_sizeSlider->blockSignals(true);
    m_fontSizeList->blockSignals(true);
}

void KNFontDialog::releaseFontSizeWidget()
{
    m_sizeSpin->blockSignals(false);
    m_sizeSlider->blockSignals(false);
    m_fontSizeList->blockSignals(false);
}

void KNFontDialog::blockFontStyleWidget()
{
    for(int i=0; i<FontStylesCount; i++)
    {
        m_fontStyles[i]->blockSignals(true);
    }
}

void KNFontDialog::releaseFontStyleWidget()
{
    for(int i=0; i<FontStylesCount; i++)
    {
        m_fontStyles[i]->blockSignals(false);
    }
}
