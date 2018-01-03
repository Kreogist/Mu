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
#include <QCompleter>
#include <QDirModel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

#include "knutil.h"
#include "kndpimanager.h"
#include "knthememanager.h"
#include "knlabelbutton.h"
#include "knselectionmessagebox.h"

#include "knpreferencepaneldiritem.h"

#include <QDebug>

KNPreferencePanelDirItem::KNPreferencePanelDirItem(QWidget *parent) :
    KNPreferencePanelItem(parent),
    m_gotoFolder(new KNLabelButton(this)),
    m_pathEditor(new QLineEdit(this))
{
    //Set the icon to the label.
    m_gotoFolder->setPixmap(QPixmap("://preference/goto_folder.png"));
#ifdef Q_OS_WIN
    m_gotoFolder->setToolTip(tr("Show in Explorer"));
#endif
#ifdef Q_OS_MAC
    m_gotoFolder->setToolTip(tr("Show in Finder"));
#endif
#ifdef Q_OS_LINUX
    m_gotoFolder->setToolTip(tr("Show the contains folder"));
#endif
    connect(m_gotoFolder, &KNLabelButton::clicked,
            [=]
            {
                //Get the dir object.
                QDir targetDir(m_pathEditor->text());
                //Check the directory exists.
                if(targetDir.exists())
                {
                    //Show the folder.
                    KNUtil::showInGraphicalShell(targetDir.absolutePath());
                }
            });
    //Construct the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    mainLayout->setSpacing(knDpi->width(5));
    //Construct the select button.
    QPushButton *selectFont=new QPushButton(this);
    selectFont->setFixedSize(knDpi->size(25, 20));
    selectFont->setText("...");
    selectFont->setPalette(palette());
    connect(selectFont, &QPushButton::clicked,
            [=]
            {
                //Open the folder select dialog.
                QString currentDirPath=dirPath(m_pathEditor->text()),
                        targetPath=QFileDialog::getExistingDirectory(
                            this,
                            tr("Please select a directory for %1").arg(
                                titleText().toLower()),
                            m_pathEditor->text());
                //Check the dir path result.
                if(targetPath.isEmpty())
                {
                    //Ignore the path.
                    return;
                }
                //Check the dir path.
                QDir targetDir(targetPath);
                if(currentDirPath==targetDir.absolutePath())
                {
                    //The path is not changed.
                    return;
                }
                //Use the message box to get the selection result.
                QStringList dirOptions;
                dirOptions << tr("Set the directory to \"%1\"").arg(
                                  targetDir.dirName())
                           << tr("Copy the files to \"%1\"").arg(
                                  targetDir.dirName())
                           << tr("Move the files to \"%1\"").arg(
                                  targetDir.dirName());
                int option=KNSelectionMessageBox::getSelection(
                            "Operation",
                            tr("Do you want to set the path to the directory\n "
                               "copy all files to the directory or move all \n"
                               "files to the directory?"),
                            dirOptions);
                //Check the option.
                if(option==-1)
                {
                    //Cancel the setting.
                    return;
                }
                //Save the new directory path.
                m_pathEditor->setText(targetPath);
                //When the option is 1.
                switch(option)
                {
                case 1:
                    //Copy the content to the new directory.
                    KNUtil::copyDirectory(currentDirPath,
                                          targetPath,
                                          true);
                    break;
                case 2:
                    //We need to move the files to the new directory.
                    KNUtil::moveDirectory(currentDirPath, targetPath);
                    break;
                }
            });
    mainLayout->addWidget(selectFont);
    //Add the goto folder button.
    mainLayout->addWidget(m_gotoFolder);
    //Construct the text edit layout.
    m_pathEditor->setPalette(knTheme->getPalette("PreferenceItemLineEdit"));
    m_pathEditor->setReadOnly(true);
    mainLayout->addWidget(m_pathEditor);
    connect(m_pathEditor, &QLineEdit::textChanged,
            this, &KNPreferencePanelDirItem::valueChanged);
    //Build the widget.
    buildWidgetLayout(mainLayout);
}

QVariant KNPreferencePanelDirItem::value() const
{
    //Use QDir to get the absolute path of the directory.
    return QDir(m_pathEditor->text()).absolutePath();
}

void KNPreferencePanelDirItem::setWidgetValue(const QVariant &value)
{
    //Set the value to the widget.
    m_pathEditor->setText(value.toString());
    //Move to the beginning.
    m_pathEditor->setCursorPosition(0);
}

bool KNPreferencePanelDirItem::isEqual(const QVariant &currentValue,
                                       const QVariant &originalValue)
{
    //Use QDir to check the value.
    return dirPath(currentValue.toString())==dirPath(originalValue.toString());
}

inline QString KNPreferencePanelDirItem::dirPath(const QString &rawPath)
{
    //Use QDir to get the path.
    return QDir(rawPath).absolutePath();
}
