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
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDomDocument>

#include "knmusicparser.h"
#include "knmusicmodelassist.h"
#include "../../sdk/knmusicplaylistlistitem.h"
#include "../../sdk/knmusicplaylistmodel.h"

#include "knmusicwplparser.h"

#include <QDebug>

KNMusicWPLParser::KNMusicWPLParser(QObject *parent) :
    KNMusicPlaylistParser(parent)
{
}

QString KNMusicWPLParser::playlistType() const
{
    return tr("Windows Media Playlist");
}

QString KNMusicWPLParser::playlistSuffix() const
{
    return "*.wpl";
}

bool KNMusicWPLParser::parse(const QString &playlistFilePath,
                             KNMusicPlaylistListItem *playlistItem)
{
    //Open the playlist file first.
    QFile wplFile(playlistFilePath);
    QFileInfo wplFileInfo(wplFile);
    if(!wplFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    //Use QDomDocument to parse the file.
    QDomDocument wplDocument;
    if(!wplDocument.setContent(&wplFile, true))
    {
        //If there's any error, return false.
        return false;
    }
    //Close the playlist file.
    wplFile.close();
    //Get the smil element of the document.
    QDomNodeList testList=wplDocument.elementsByTagName("smil");
    //Check if there's any smil in the document.
    if(testList.size()!=1)
    {
        return false;
    }
    //Get the 'smil' dom node.
    //Get the head and the body node of the smil node.
    QDomNode smilNode=testList.item(0),
            headNode=smilNode.firstChildElement("head"),
            bodyNode=smilNode.firstChildElement("body"),
            seqNode=bodyNode.firstChildElement("seq");
    if(headNode.isNull() || bodyNode.isNull() || seqNode.isNull())
    {
        return false;
    }
    //Read the title and body information
    playlistItem->setText(headNode.firstChildElement("title").text());
    //Get the model and the parser.
    KNMusicPlaylistModel *playlistModel=playlistItem->playlistModel();
    KNMusicParser *parser=KNMusicGlobal::parser();
    //Read the seq node information.
    QString nativeSeparatorPlaylistPath=
            QDir::toNativeSeparators(wplFileInfo.absolutePath());
    for(QDomNode i=seqNode.firstChild();
        i!=seqNode.lastChild();
        i=i.nextSibling())
    {
        if(i.nodeName()=="media" && i.hasAttributes())
        {
            QDomElement currentElement=i.toElement();
            QString srcInformation=currentElement.attribute("src");
            //Check is the file path absolute path, treat the other file as
            //relative path.
            QFileInfo currentFileInfo((srcInformation.size()>3 &&
                                       srcInformation.at(0).isLetter() &&
                                       srcInformation.at(1)==':' &&
                                       srcInformation.at(2)=='\\')?
                                          srcInformation:
                                          nativeSeparatorPlaylistPath+"\\"+currentElement.attribute("src"));
            //Parse as a file.
            KNMusicDetailInfo currentDetail;
            parser->parseFile(currentFileInfo.absoluteFilePath(),
                              currentDetail);
            //Add to playlist.
            playlistModel->appendMusicRow(KNMusicModelAssist::generateRow(currentDetail));
        }
    }
    return true;
}

bool KNMusicWPLParser::write(const QString &playlistFilePath,
                             KNMusicPlaylistListItem *playlistItem)
{
    QDomDocument wplDocument;
    //Initial the root element.
    QDomElement root=wplDocument.createElement("smil");
    wplDocument.appendChild(root);

    //Initial the playlist model.
    KNMusicPlaylistModel *playlistModel=playlistItem->playlistModel();

    //Initial the head element.
    QDomElement head=wplDocument.createElement("head");
    root.appendChild(head);
    //Add meta data to the head element.
    QDomElement metaGenerator=wplDocument.createElement("meta"),
                metaIsNetworkFeed=wplDocument.createElement("meta"),
                metaItemCount=wplDocument.createElement("meta"),
                metaIsFavorite=wplDocument.createElement("meta"),
                metaContentPartnerListID=wplDocument.createElement("meta"),
                metaContentPartnerNameType=wplDocument.createElement("meta"),
                metaContentPartnerName=wplDocument.createElement("meta"),
                metaSubtitle=wplDocument.createElement("meta"),
                author=wplDocument.createElement("author"),
                title=wplDocument.createElement("title");
    //Set name to the elements.
    metaGenerator.setAttribute("name","Generator");
    metaIsNetworkFeed.setAttribute("name","IsNetworkFeed");
    metaItemCount.setAttribute("name","ItemCount");
    metaIsFavorite.setAttribute("name","IsFavorite");
    metaContentPartnerListID.setAttribute("name","ContentPartnerListID");
    metaContentPartnerNameType.setAttribute("name","ContentPartnerNameType");
    metaContentPartnerName.setAttribute("name","ContentPartnerName");
    metaSubtitle.setAttribute("name","Subtitle");
    //Set content.
    metaGenerator.setAttribute("content", "Kreogist Mu");
    metaIsNetworkFeed.setAttribute("content", "0");
    metaItemCount.setAttribute("content", QString::number(playlistModel->rowCount()));
    //Set title.
    QDomText titleText=wplDocument.createTextNode(playlistItem->text());
    title.appendChild(titleText);
    //Add these data to head.
    head.appendChild(metaGenerator);
    head.appendChild(metaIsNetworkFeed);
    head.appendChild(metaItemCount);
    head.appendChild(metaIsFavorite);
    head.appendChild(metaContentPartnerListID);
    head.appendChild(metaContentPartnerNameType);
    head.appendChild(metaContentPartnerName);
    head.appendChild(metaSubtitle);
    head.appendChild(author);
    head.appendChild(title);

    //Generate the list.
    QDomElement body=wplDocument.createElement("body"),
                seq=wplDocument.createElement("seq");
    root.appendChild(body);
    body.appendChild(seq);
    for(int i=0; i<playlistModel->rowCount(); i++)
    {
        QDomElement media=wplDocument.createElement("media");
        //Set the media info.
        media.setAttribute("src", QDir::toNativeSeparators(playlistModel->filePathFromRow(i)));
        //Add to seq.
        seq.appendChild(media);
    }

    //Write the data to playlist file.
    return writePlaylistContentToFile(playlistFilePath,
                                      "<?wpl version=\"1.0\"?>"+
                                      wplDocument.toString(4));
}
