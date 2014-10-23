/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QBoxLayout>
#include <QPushButton>
#include <QFileDialog>

#include "knpathlineedit.h"

#include "knpreferenceitempathbrowser.h"

#include <QDebug>

KNPreferenceItemPathBrowser::KNPreferenceItemPathBrowser(QWidget *parent) :
    KNPreferenceItemBase(parent)
{
    //Initial the editor widget.
    QWidget *pathEditorWidget=new QWidget(this);
    pathEditorWidget->setContentsMargins(0,0,0,0);

    //Initial the editor widget layout.
    QBoxLayout *pathEditorLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                                pathEditorWidget);
    pathEditorLayout->setContentsMargins(0,0,0,0);
    pathEditorLayout->setSpacing(0);
    pathEditorWidget->setLayout(pathEditorLayout);

    //Initial the browse button.
    m_browse=new QPushButton(this);
    m_browse->setText("Browse");
    //Set palette.
    QPalette pal=m_browse->palette();
    pal.setColor(QPalette::Button, QColor(0x20, 0x20, 0x20));
    pal.setColor(QPalette::Window, QColor(0x20, 0x20, 0x20));
    pal.setColor(QPalette::ButtonText, QColor(0x60, 0x60, 0x60));
    m_browse->setPalette(pal);
    connect(m_browse, SIGNAL(clicked()),
            this, SLOT(onActionBrowseFolder()));
    pathEditorLayout->addWidget(m_browse);

    pathEditorLayout->addSpacing(5);

    //Initial the path editor.
    m_pathEditor=new KNPathLineEdit(this);
    //When start editing, set the palette to normal mode.
    connect(m_pathEditor, &KNPathLineEdit::startEditPath,
            this, &KNPreferenceItemPathBrowser::onActionPathExist);
    //Linked the exist and not exist process slot.
    connect(m_pathEditor, &KNPathLineEdit::pathExist,
            this, &KNPreferenceItemPathBrowser::onActionPathExist);
    connect(m_pathEditor, &KNPathLineEdit::pathNotExist,
            this, &KNPreferenceItemPathBrowser::onActionPathNotExist);
    //Set palette.
    m_existEditPalette=m_pathEditor->palette();
    m_existEditPalette.setColor(QPalette::Base, QColor(0x20, 0x20, 0x20));
    m_existEditPalette.setColor(QPalette::Window, QColor(0x4D, 0x4D, 0x4D));
    m_existEditPalette.setColor(QPalette::Text, QColor(0xA0, 0xA0, 0xA0));
    m_existEditPalette.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    m_existEditPalette.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    m_pathEditor->setPalette(m_existEditPalette);
    m_notExistEditPalette=m_existEditPalette;
    m_notExistEditPalette.setColor(QPalette::Base, QColor(255,0,0));
    pathEditorLayout->addWidget(m_pathEditor, 1);

    pathEditorLayout->addSpacing(5);

    insertControlWidget(pathEditorWidget);
}

QVariant KNPreferenceItemPathBrowser::defaultValue() const
{
    return m_defaultValue;
}

QVariant KNPreferenceItemPathBrowser::value() const
{
    return m_pathEditor->text();
}

void KNPreferenceItemPathBrowser::setDefaultValue(const QVariant &defaultValue)
{
    m_defaultValue=defaultValue.toString();
    //When default value changed, set the value to the default value.
    setValue(m_defaultValue);
}

void KNPreferenceItemPathBrowser::setValue(const QVariant &value)
{
    m_pathEditor->setText(value.toString());
}

void KNPreferenceItemPathBrowser::onActionBrowseFolder()
{
    //Initial the file dialog.
    QFileDialog importFile(this);
    //Set property.
    importFile.setFileMode(QFileDialog::Directory);
    importFile.setOption(QFileDialog::ShowDirsOnly);
    //If launch success.
    if(importFile.exec())
    {
        //Check is the user select a folder.
        if(!importFile.selectedFiles().isEmpty())
        {
            //Set the current value to that folder.
            setValue(importFile.selectedFiles().first());
        }
    }
}

void KNPreferenceItemPathBrowser::onActionPathExist()
{
    if(m_ensureExist)
    {
        m_pathEditor->setPalette(m_existEditPalette);
    }
}

void KNPreferenceItemPathBrowser::onActionPathNotExist()
{
    if(m_ensureExist)
    {
        m_pathEditor->setPalette(m_notExistEditPalette);
    }
}

bool KNPreferenceItemPathBrowser::ensureExist() const
{
    return m_ensureExist;
}

void KNPreferenceItemPathBrowser::setEnsureExist(bool ensureExist)
{
    m_ensureExist = ensureExist;
}

