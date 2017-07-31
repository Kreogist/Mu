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
#include <QPushButton>

#include "kndpimanager.h"
#include "knshortcuteditor.h"
#include "knshortcutmanager.h"
#include "knthememanager.h"

#include "knpreferencepanelshortcutitem.h"

#include <QDebug>

KNPreferencePanelShortcutItem::KNPreferencePanelShortcutItem(QWidget *parent) :
    KNPreferencePanelItem(parent),
    m_sequence(QKeySequence()),
    m_shortcutText(new QLabel(this)),
    m_startEdit(new QPushButton(this)),
    m_shortcutEditor(new KNShortcutEditor(this))
{
    //Configure the display label.
    m_shortcutText->setPalette(palette());
    m_shortcutEditor->hide();
    //Link the shortcut text signals.
    QString editText=tr("Edit");
    m_startEdit->setFixedHeight(knDpi->height(20));
    m_startEdit->setText(editText);
    m_startEdit->setFixedWidth(fontMetrics().width(editText)+knDpi->width(16));
    m_startEdit->setPalette(palette());
    connect(m_startEdit, &QPushButton::clicked,
            this, &KNPreferencePanelShortcutItem::onStartEdit);
    //Link the shortcut editor signals.
    connect(m_shortcutEditor, &KNShortcutEditor::clearShortcut,
            this, &KNPreferencePanelShortcutItem::onShortcutClear);
    connect(m_shortcutEditor, &KNShortcutEditor::cancel,
            this, &KNPreferencePanelShortcutItem::onShortcutCancel);
    connect(m_shortcutEditor, &KNShortcutEditor::editingFinished,
            this, &KNPreferencePanelShortcutItem::onShortcutAccept);
    //Link the manager to the item.
    connect(knShortcut, &KNShortcutManager::shortcutUpdate,
            this, &KNPreferencePanelShortcutItem::onShortcutUpdate);

    //Set the click label as the trigger.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    mainLayout->setSpacing(knDpi->width(8));
    mainLayout->addWidget(m_startEdit);
    mainLayout->addWidget(m_shortcutText, 1);
    mainLayout->addWidget(m_shortcutEditor);
    //Build the widget.
    buildWidgetLayout(mainLayout);
}

KNPreferencePanelShortcutItem::~KNPreferencePanelShortcutItem()
{
    //Disconnect the link between the item and the manager.
    disconnect(knShortcut, &KNShortcutManager::shortcutUpdate,
               this, &KNPreferencePanelShortcutItem::onShortcutUpdate);
}

QVariant KNPreferencePanelShortcutItem::value() const
{
    return m_sequence;
}

void KNPreferencePanelShortcutItem::setWidgetValue(const QVariant &value)
{
    //The value should be a key sequence instance.
    if(!value.canConvert<QKeySequence>())
    {
        //Invalid variable.
        return;
    }
    //Convert to key sequence.
    m_sequence=value.value<QKeySequence>();
    //Set the shortcut text to the sequence, update the text from the manager.
    onShortcutUpdate();
}

bool KNPreferencePanelShortcutItem::isEqual(const QVariant &currentValue,
                                       const QVariant &originalValue)
{
    //Check the instance.
    if(!currentValue.canConvert<QKeySequence>() ||
            !originalValue.canConvert<QKeySequence>())
    {
        //The format is not correct.
        return false;
    }
    //Check the translated string.
    return currentValue.value<QKeySequence>().toString()==
            originalValue.value<QKeySequence>().toString();
}

void KNPreferencePanelShortcutItem::onShortcutUpdate()
{
    //Get the shortcut text.
    QString actionSequenceText=m_sequence.toString(QKeySequence::NativeText);
    //Check the text.
    if(actionSequenceText.isEmpty())
    {
        //Set the None text.
        m_shortcutText->setText(tr("<No shortcut>"));
        return;
    }
    //Check the conflict key information from the shortcut manager.
    QList<int> conflictKeys=knShortcut->conflictShortcut(valueName());
    //Get the palette.
    QColor errorColor=knTheme->getPalette(
                "PreferenceShortcutLabel").color(QPalette::Text);
    QString redPart=QString::number(errorColor.red(), 16),
            greenPart=QString::number(errorColor.green(), 16),
            bluePart=QString::number(errorColor.blue(), 16),
            colorText=(errorColor.red()<16?"0":"")+redPart+
                      (errorColor.green()<16?"0":"")+greenPart+
                      (errorColor.blue()<16?"0":"")+bluePart;
    //Get the key text.
    for(auto keySequence : conflictKeys)
    {
        //Get the conflict shortcut text.
        QString conflictText=
                QKeySequence(keySequence).toString(QKeySequence::NativeText),
                //Replace the conflict text.
                colorConflictText="<span style=\"color:#"+colorText+"\">"+
                conflictText+"</span>";
        //Replace the raw sequence text to color text.
        actionSequenceText.replace(conflictText, colorConflictText);
    }
    //Update the shortcut text.
    m_shortcutText->setText(actionSequenceText);
}

void KNPreferencePanelShortcutItem::onStartEdit()
{
    //Clear the shortcut editor.
    m_shortcutEditor->startEditing();
    //Show the editor and hide the label.
    m_shortcutText->hide();
    m_startEdit->hide();
    m_shortcutEditor->show();
    //Set the focus on the shortcut editor.
    m_shortcutEditor->setFocus(Qt::MouseFocusReason);
}

void KNPreferencePanelShortcutItem::onShortcutCancel()
{
    //Simply hide the editor and show the original text, nothing changed.
    showShortcutText();
}

void KNPreferencePanelShortcutItem::onShortcutAccept()
{
    if(!m_shortcutEditor->isEditing())
    {
        //Ignore for the shortcut editor is not editing.
        return;
    }
    //Save the shortcut from the editor.
    m_sequence=m_shortcutEditor->keySequence();
    //Set the new text to the view.
    m_shortcutText->setText(m_sequence.toString(QKeySequence::NativeText));
    //Show the shortcut text.
    showShortcutText();
    //Emit the value changed signal.
    emit valueChanged();
}

void KNPreferencePanelShortcutItem::onShortcutClear()
{
    //Reset the sequence.
    m_sequence=QKeySequence();
    //Clear the sequence text.
    m_shortcutText->setText(tr("<No shortcut>"));
    //Show the shortcut text.
    showShortcutText();
    //Emit the value changed signal.
    emit valueChanged();
}

inline void KNPreferencePanelShortcutItem::showShortcutText()
{
    //Hide the editor and show the text.
    m_shortcutEditor->hide();
    m_shortcutText->show();
    m_startEdit->show();
}
