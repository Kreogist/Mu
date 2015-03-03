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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QApplication>
#include <QFile>
#include <QFont>
#include <QJsonDocument>
#include <QJsonValue>

#include "knconfigure.h"

#include <QDebug>

QHash<QString, int> KNConfigure::m_objectType;

KNConfigure::KNConfigure(QObject *parent) :
    QObject(parent)
{
    if(m_objectType.isEmpty())
    {
        buildObjectTypeList();
    }
}

KNConfigure::~KNConfigure()
{
    ;
}

QJsonObject KNConfigure::configureObject()
{
    //Update the data object first.
    if(!m_subConfigures.isEmpty())
    {
        //Set the data to the sub objects.
        for(QLinkedList<KNConfigure *>::iterator i=m_subConfigures.begin();
            i!=m_subConfigures.end();
            ++i)
        {
            //Set the data object.
            m_dataObject.insert((*i)->caption(), (*i)->configureObject());
        }
    }
    return m_dataObject;
}

void KNConfigure::setConfigureObject(const QJsonObject &configureObject)
{
    //Update the data object information.
    m_dataObject = configureObject;
    //Update the sub objects data.
    updateSubConfigure();
}

void KNConfigure::addSubConfigure(KNConfigure *configure)
{
    //Add the configure to configure list.
    if(configure!=nullptr)
    {
        //Add to configure list.
        m_subConfigures.append(configure);
        //Check if we have already have the data which the configure should
        //contains.
        if(m_dataObject.contains(configure->caption()))
        {
            configure->setConfigureObject(m_dataObject.value(configure->caption()).toObject());
        }
    }
}

QString KNConfigure::filePath() const
{
    return m_filePath;
}

void KNConfigure::loadConfigure()
{
    //Check if the file path is empty.
    if(m_filePath.isEmpty())
    {
        return;
    }
    //Open the file at read only mode.
    QFile configureFile(m_filePath);
    if(!configureFile.exists() ||
            !configureFile.open(QIODevice::ReadOnly))
    {
        return;
    }
    //Read the file.
    m_dataObject=QJsonDocument::fromJson(configureFile.readAll()).object();
    //Close the file.
    configureFile.close();

    //Update the sub objects data.
    updateSubConfigure();
}

void KNConfigure::saveConfigure()
{
    //Check if the file path is empty.
    if(m_filePath.isEmpty())
    {
        return;
    }
    //Open the file at write only mode.
    QFile configureFile(m_filePath);
    if(!configureFile.open(QIODevice::WriteOnly))
    {
        qDebug("Can't save configure to file.");
        return;
    }
    //Genereate the json document.
    QJsonDocument configureDocument;
    configureDocument.setObject(configureObject());
    //Save the configure.
    configureFile.write(configureDocument.toJson());
    //Close the file.
    configureFile.close();
}

QVariant KNConfigure::getData(const QString &key,
                              const QVariant &defaultValue)
{
    QJsonValue value=m_dataObject.value(key);
    //Check if value is available.
    if(value.type()==QJsonValue::Undefined)
    {
        //Save the default value to data object.
        setData(key, defaultValue);
        return defaultValue;
    }
    //Check if the value is a advanced type.
    if(value.type()==QJsonValue::Object)
    {
        //Translate to object.
        QJsonObject valueObject=value.toObject();
        //Check whether this object contains the advacned type record.
        if(valueObject.contains("Type"))
        {
            int objectType=m_objectType.value(valueObject.value("Type").toString(),
                                              -1);
            if(objectType==-1)
            {
                //We cannot translate this value.
                return defaultValue;
            }
            switch(objectType)
            {
            case FontType:
            {
                QFont valueFont=QApplication::font();
                valueFont.setFamily(valueObject.value("Family").toString());
                valueFont.setPixelSize(valueObject.value("PixelSize").toInt());
                valueFont.setBold(valueObject.value("Bold").toBool());
                valueFont.setItalic(valueObject.value("Italic").toBool());
                valueFont.setUnderline(valueObject.value("Underline").toBool());
                valueFont.setStrikeOut(valueObject.value("Strikeout").toBool());
                valueFont.setKerning(valueObject.value("Kerning").toBool());
                return QVariant::fromValue(valueFont);
            }
            default:
                return defaultValue;
            }
        }
        return defaultValue;
    }
    return QVariant(value);
}

void KNConfigure::setData(const QString &key, const QVariant &value)
{
    //Set the value via the type of QVariant.
    switch(value.type())
    {
    case QVariant::Double:
        m_dataObject.insert(key, value.toDouble());
        break;
    case QVariant::String:
        m_dataObject.insert(key, value.toString());
        break;
    case QVariant::Int:
        m_dataObject.insert(key, value.toInt());
        break;
    case QVariant::Bool:
        m_dataObject.insert(key, value.toBool());
        break;
    case QVariant::Font:
    {
        //Generate the font object.
        QFont font=value.value<QFont>();
        QJsonObject fontObject;
        fontObject.insert("Type", "Font");
        fontObject.insert("Family", font.family());
        fontObject.insert("PixelSize", font.pixelSize());
        fontObject.insert("Bold", font.bold());
        fontObject.insert("Italic", font.italic());
        fontObject.insert("Underline", font.underline());
        fontObject.insert("Strikeout", font.strikeOut());
        fontObject.insert("Kerning", font.kerning());
        //Insert the font object.
        m_dataObject.insert(key, fontObject);
        break;
    }
    default:
        break;
    }
}

void KNConfigure::setFilePath(const QString &configureFilePath)
{
    m_filePath = configureFilePath;
}

QString KNConfigure::caption() const
{
    return m_caption;
}

void KNConfigure::setCaption(const QString &configureTitle)
{
    m_caption = configureTitle;
}

void KNConfigure::updateSubConfigure()
{
    //Set the data to the sub objects.
    for(QLinkedList<KNConfigure *>::iterator i=m_subConfigures.begin();
        i!=m_subConfigures.end();
        ++i)
    {
        //Set the data object.
        (*i)->setConfigureObject(m_dataObject.value((*i)->caption()).toObject());
    }
}

void KNConfigure::buildObjectTypeList()
{
    m_objectType.insert("Font", FontType);
}
