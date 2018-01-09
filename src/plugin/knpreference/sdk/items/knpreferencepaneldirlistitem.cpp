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
#include <QBoxLayout>
#include <QFileDialog>
#include <QAbstractButton>
#include <QPainter>

#include "knlabelbutton.h"
#include "kndpimanager.h"

#include "knpreferencepanelsubitem.h"

#include "knpreferencepaneldirlistitem.h"

QLinearGradient KNPreferencePanelDirListItem::m_shadow=
        QLinearGradient(0, 0, 0, 0);
bool KNPreferencePanelDirListItem::m_notInitialed=true;

KNPreferencePanelDirListItem::KNPreferencePanelDirListItem(QWidget *parent) :
    KNPreferencePanelItem(parent),
    m_mainLayout(new QBoxLayout(QBoxLayout::TopToBottom, this)),
    m_remove(generateButton("://preference/folder_delete.png",
                            tr("Remove the selected paths"))),
    m_checkedCounter(0)
{
    //Check static initialized.
    if(m_notInitialed)
    {
        //Initialized.
        m_notInitialed=false;
        //Update the shadow gradient.
        m_shadow=QLinearGradient(
                    0, knDpi->heightF(PreferenceSingleItemHeight),
                    0, knDpi->heightF(PreferenceSingleItemHeight)*1.382);
        //Set shadow color.
        m_shadow.setColorAt(0, QColor(0, 0, 0, 70));
        m_shadow.setColorAt(1, QColor(0, 0, 0, 0));
    }
    //Configure the add button.
    KNLabelButton *add=generateButton("://preference/folder_add.png",
                                      tr("Add a new path"));
    connect(add, &KNLabelButton::clicked,
            this, &KNPreferencePanelDirListItem::onAddPath);
    //Configure the remove button.
    m_remove->hide();
    connect(m_remove, &KNLabelButton::clicked,
            this, &KNPreferencePanelDirListItem::onRemovePath);

    //Construct the main layout.
    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    buttonLayout->setSpacing(knDpi->width(5));
    //Add the button to the layout.
    buttonLayout->addWidget(add);
    buttonLayout->addWidget(m_remove);
    buttonLayout->addStretch();
    //Construct the main layout.
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);
    //Add the button layout to the default layout.
    m_mainLayout->addLayout(createWidgetLayout(buttonLayout));
    setLayout(m_mainLayout);
}

QVariant KNPreferencePanelDirListItem::value() const
{
    //Loop and create the string list from the button list.
    return m_pathList;
}

int KNPreferencePanelDirListItem::itemHeightMultiple() const
{
    //Default header + path list count.
    return 1 + m_pathList.size();
}

void KNPreferencePanelDirListItem::setWidgetValue(const QVariant &value)
{
    //Check the button size is the same or not.
    QStringList &&valueList=value.toStringList();
    //Check if we need create more buttons.
    if(valueList.size() > m_pathList.size())
    {
        //Generate more button.
        int createCount=valueList.size()-m_pathList.size();
        while(createCount--)
        {
            //Add the item to the layout.
            addSubItem(generateSubItem());
        }
    }
    else if(valueList.size() < m_pathList.size())
    {
        //Remove the last several items.
        int removeIndex=valueList.size(),
            removeCount=m_pathList.size()-valueList.size();
        while(removeCount--)
        {
            //Remove the button.
            removeSubItem(removeIndex);
        }
    }
    //Now the button number is the same as the path list size.
    //Update the list path.
    m_pathList=valueList;
    //Update all the buttons.
    for(int i=0; i<m_pathList.size(); ++i)
    {
        //Get the i from the list.
        m_pathButtonList.at(i)->setText(m_pathList.at(i));
    }
}

bool KNPreferencePanelDirListItem::isEqual(const QVariant &currentValue,
                                           const QVariant &originalValue)
{
    //Check the two string list.
    return currentValue.toStringList()==originalValue.toStringList();
}

void KNPreferencePanelDirListItem::paintEvent(QPaintEvent *event)
{
    //Draw the sub item background.
    QPainter painter(this);
    painter.setOpacity(getHighLightOpacity());
    //Fill the entire background first.
    painter.fillRect(rect(), palette().color(QPalette::ToolTipBase));
    //Draw the seperate line.
    int lineHeight=knDpi->height(PreferenceSingleItemHeight),
            lineY=lineHeight<<1;
    painter.setOpacity(1.0);
    //The base color is used as seperate line color.
    painter.setPen(palette().color(QPalette::Base));
    for(int i=1; i<m_pathList.size(); ++i)
    {
        //Draw the line.
        painter.drawLine(knDpi->width(15), lineY,
                         width()-knDpi->width(5), lineY);
        //Increase line y.
        lineY+=lineHeight;
    }
    //Fill the shadow.
    painter.setOpacity(getHighLightOpacity());
    painter.fillRect(rect(), m_shadow);
    //Call original paint event.
    KNPreferencePanelItem::paintEvent(event);
}

void KNPreferencePanelDirListItem::onAddPath()
{
    //Get the new directory path.
    QString addedPath=QFileDialog::getExistingDirectory(
                this,
                tr("Add directory"));
    QList<int> coveredPaths;
    //User cancel the selecion.
    if(addedPath.isEmpty() || coverPath(addedPath, coveredPaths))
    {
        //Ignore the cancel operation.
        return;
    }
    //Removed all the covered paths.
    removeSubItems(coveredPaths);
    //Save the result.
    m_pathList.append(addedPath);
    //Construct the button.
    KNPreferencePanelSubItem *item=generateSubItem();
    item->setText(addedPath);
    //Add the item to the layout.
    addSubItem(item);
    //Update item height.
    updateItemHeight();
}

void KNPreferencePanelDirListItem::onRemovePath()
{
    QList<int> checkedIndex;
    //Get the selected item index.
    for(int i=0; i<m_pathButtonList.size(); ++i)
    {
        //Check whether it is checked.
        if(m_pathButtonList.at(i)->isChecked())
        {
            //Append to checked index.
            checkedIndex.append(i);
        }
    }
    //Removed all the checked indexs.
    removeSubItems(checkedIndex);
    //Reset the checked index.
    m_checkedCounter=0;
    //Hide the remove button.
    m_remove->hide();
    //Update the item height.
    updateItemHeight();
}

void KNPreferencePanelDirListItem::onItemClicked(bool checked)
{
    //Check the clicked counter.
    m_checkedCounter=(checked)?(m_checkedCounter+1):(m_checkedCounter-1);
    //Check the counter value, show or hide the button.
    m_remove->setVisible(m_checkedCounter);
}

inline KNLabelButton *KNPreferencePanelDirListItem::generateButton(
        const QString &iconPath, const QString &tooltip)
{
    //Generate the button.
    KNLabelButton *button=new KNLabelButton(this);
    //Set the properties of the button.
    button->setPixmap(QPixmap(iconPath));
    button->setToolTip(tooltip);
    //Give back the button.
    return button;
}

inline KNPreferencePanelSubItem *KNPreferencePanelDirListItem::generateSubItem()
{
    //Create the item.
    KNPreferencePanelSubItem *item=new KNPreferencePanelSubItem(this);
    //Configure the item.
    item->setContentsMargins(knDpi->margins(15, 0, 5, 0));
    item->setFixedHeight(knDpi->height(PreferenceSingleItemHeight));
    item->setCheckable(true);
    return item;
}

inline void KNPreferencePanelDirListItem::addSubItem(
        KNPreferencePanelSubItem *item)
{
    //Add item to the layout.
    m_mainLayout->addWidget(item);
    //Add item to the path button list.
    m_pathButtonList.append(item);
    //Link the item to the list item.
    connect(item, SIGNAL(clicked(bool)), this, SLOT(onItemClicked(bool)));
}

inline void KNPreferencePanelDirListItem::removeSubItems(
        const QList<int> &checkedIndex)
{
    //Loop and remove all the item from the list.
    for(int i=checkedIndex.size()-1; i>-1; --i)
    {
        //Remove the button.
        removeSubItem(checkedIndex.at(i));
        //Remove the path from the list.
        m_pathList.removeAt(checkedIndex.at(i));
    }
}

inline void KNPreferencePanelDirListItem::removeSubItem(int index)
{
    //Remove the item from the layout.
    m_mainLayout->takeAt(index+1);
    //Remove the button item.
    KNPreferencePanelSubItem *item=m_pathButtonList.takeAt(index);
    //Disconnect the item.
    disconnect(item, 0, 0, 0);
    //Recover the memory.
    item->deleteLater();
}

inline bool KNPreferencePanelDirListItem::coverPath(const QString &targetPath,
                                                    QList<int> &prefixList)
{
    //Check whether there is a path in the list is the prefix of the target
    //path.
    for(int i=0; i<m_pathList.size(); ++i)
    {
        //Check the path list.
        if(targetPath.startsWith(m_pathList.at(i)))
        {
            //Cover the path.
            return true;
        }
        //Check whether the new added path is the prefix of the item in the path
        //list.
        if(m_pathList.at(i).startsWith(targetPath))
        {
            //This widget should be removed.
            prefixList.append(i);
        }
    }
    //No one could cover the path.
    return false;
}

inline void KNPreferencePanelDirListItem::updateItemHeight()
{
    //Update the height.
    setFixedHeight(knDpi->height(itemHeightMultiple() *
                                 PreferenceSingleItemHeight));
    //Emit the height changed signal.
    emit heightChanged();
}
