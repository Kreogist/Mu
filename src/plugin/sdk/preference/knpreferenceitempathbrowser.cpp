/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QCompleter>
#include <QDirModel>
#include <QBoxLayout>
#include <QPushButton>
#include <QFileDialog>

#include "knpathlineedit.h"
#include "knglobal.h"

#include "knlocalemanager.h"

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
    //Set palette.
    QPalette pal=m_browse->palette();
    pal.setColor(QPalette::Button, QColor(0x20, 0x20, 0x20));
    pal.setColor(QPalette::Window, QColor(0x20, 0x20, 0x20));
    pal.setColor(QPalette::ButtonText, QColor(0x60, 0x60, 0x60));
    m_browse->setPalette(pal);
    connect(m_browse, SIGNAL(clicked()),
            this, SLOT(onActionBrowseFolder()));

    //Initial the move to button.
    m_moveTo=new QPushButton(this);
    m_moveTo->setPalette(pal);
    connect(m_moveTo, SIGNAL(clicked()),
            this, SLOT(onActionMoveFolder()));

    //Initial the path editor.
    m_pathEditor=new KNPathLineEdit(this);
    //Generate auto path completer for path editor.
    QCompleter *pathCompleter=new QCompleter(m_pathEditor);
    QDirModel *pathCompleterModel=new QDirModel(m_pathEditor);
    //Configure the dir model.
    pathCompleterModel->setFilter(QDir::Dirs |
                                  QDir::NoDotAndDotDot |
                                  QDir::NoSymLinks);
    pathCompleter->setModel(pathCompleterModel);
    m_pathEditor->setCompleter(pathCompleter);
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

    //Add widgets to item.
    insertSpacing(5);
    insertWidget(m_pathEditor, 1);
    insertSpacing(5);
    insertWidget(m_moveTo);
    insertSpacing(5);
    insertWidget(m_browse);

    //Connect retranslate signal.
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
            this, &KNPreferenceItemPathBrowser::retranslate);
    //Do translation.
    retranslate();
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
    QFileInfo defaultValueInfo(defaultValue.toString());
    m_defaultValue=defaultValueInfo.absoluteFilePath();
    //When default value changed, set the value to the default value.
    setValue(m_defaultValue);
}

void KNPreferenceItemPathBrowser::setValue(const QVariant &value)
{
    //Block the signal first.
    m_pathEditor->blockSignals(true);
    //Set the value.
    QFileInfo defaultValueInfo(value.toString());
    m_pathEditor->setText(defaultValueInfo.absoluteFilePath());
    //Release the block signal.
    m_pathEditor->blockSignals(false);
}

void KNPreferenceItemPathBrowser::onActionBrowseFolder()
{
    //Initial the file dialog.
    QFileDialog selectFolder(this);
    //Set property.
    selectFolder.setFileMode(QFileDialog::Directory);
    selectFolder.setOption(QFileDialog::ShowDirsOnly);
    //If launch success.
    if(selectFolder.exec())
    {
        //Check is the user select a folder.
        if(!selectFolder.selectedFiles().isEmpty())
        {
            //Set the current value to that folder.
            setValue(selectFolder.selectedFiles().first());
        }
    }
}

void KNPreferenceItemPathBrowser::onActionMoveFolder()
{
    //Initial the file dialog.
    QFileDialog moveToDir(this);
    //Set property.
    moveToDir.setFileMode(QFileDialog::Directory);
    moveToDir.setOption(QFileDialog::ShowDirsOnly);
    //If launch success.
    if(moveToDir.exec())
    {
        //Check is the user select a folder.
        if(!moveToDir.selectedFiles().isEmpty())
        {
            QString destinationPath=moveToDir.selectedFiles().first();
            //Get the current folder is exist.
            QFileInfo currentDirInfo(value().toString());
            if(currentDirInfo.isDir() && currentDirInfo.exists())
            {
                QDir currentDir;
                //Ensure that the destination dir exist.
                QFileInfo destinationDirInfo(destinationPath);
                if(!destinationDirInfo.exists())
                {
                    currentDir.mkpath(destinationDirInfo.absoluteFilePath());
                }
                //Move the folder.
                KNGlobal::moveFolder(currentDirInfo.absoluteFilePath(),
                                     destinationDirInfo.absoluteFilePath());
                setValue(destinationPath);
                //Emit dir moved signal.
                emit dirMoved();
            }
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

void KNPreferenceItemPathBrowser::retranslate()
{
    m_browse->setText(tr("Browse"));
    m_moveTo->setText(tr("Move To"));
}

