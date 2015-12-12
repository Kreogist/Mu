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
#include <QFileInfo>

#include "knfilenamelineedit.h"

#include <QDebug>

KNFileNameLineEdit::KNFileNameLineEdit(QWidget *parent) :
    KNLabelLineEdit(parent),
    m_fileName(QString()),
    m_suffix(QString()),
    m_directory(QDir()),
    m_state(NotChange)
{
    //Link the file name valid check data.
    connect(this, &KNFileNameLineEdit::textChanged,
            this, &KNFileNameLineEdit::onActionTextChanged);
}

void KNFileNameLineEdit::setFilePath(const QString &filePath)
{
    //Generate the file info.
    QFileInfo targetFile(filePath);
    //Check the existance of the file path.
    if(!targetFile.exists())
    {
        //Ignore the value which doesn't exist.
        return;
    }
    //Save the file path.
    m_fileName=targetFile.fileName();
    //Save the suffix.
    m_suffix=targetFile.suffix();
    //Get the directory of the file path.
    m_directory=targetFile.absoluteDir();
    //Set the file name to be the line edit text.
    setText(m_fileName);
    //Select all the parts except the suffix.
    setSelection(0, m_fileName.length()-targetFile.suffix().length()-1);
}

void KNFileNameLineEdit::onActionTextChanged(const QString &fileName)
{
    //Check whether the file name is the same as the original.
    if(fileName==m_fileName)
    {
        //Set the state to be not change state.
        setState(NotChange);
        //Mission complete.
        return;
    }
    //Check whether the file name is empty.
    if(fileName.isEmpty())
    {
        //Set the state to be empty.
        setState(Empty);
        //Mission complete.
        return;
    }
    //Check whether the file name is empty. (Suffix only)
    if(fileName.startsWith('.'))
    {
        //Set the state to be no file name.
        setState(NoFileName);
        //Misson complete.
        return;
    }
    //Get the file info of the new file name.
    QFileInfo changedFile(m_directory.filePath(fileName));
    //Check out the file name is already exist in the folder.
    if(changedFile.exists())
    {
        //Set the state to be exist.
        setState(Exist);
        //Mission complete.
        return;
    }
    //Check out the validation of the file name.
    if(fileName.contains(QRegExp("[@#\\$%\\^&\\*]")) ||
            fileName=="." || fileName=="..")
    {
        //Invalid file name.
        setState(Invalid);
        //Mission Complete.
        return;
    }
    //Check the suffix of the file name.
    if(changedFile.suffix()!=m_suffix)
    {
        //Set the state to be suffix changed.
        setState(SuffixChange);
    }
    //Ok, then all the other is valid.
    setState(Valid);
}

inline void KNFileNameLineEdit::setState(int state)
{
    //Check the state.
    if(state==m_state)
    {
        //Ignore the same state.
        return;
    }
    //Save the state.
    m_state=state;
    //Emit the state changed signal.
    emit stateChanged(m_state);
}

int KNFileNameLineEdit::state() const
{
    return m_state;
}
