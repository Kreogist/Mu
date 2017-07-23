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

#include "../../sdk/knmusicplaylistmodel.h"

#include "knmusicplaylistwplparser.h"

KNMusicPlaylistWplParser::KNMusicPlaylistWplParser(QObject *parent) :
    KNMusicPlaylistParser(parent)
{
}

QString KNMusicPlaylistWplParser::description()
{
    return tr("Windows Media Playlist");
}

QString KNMusicPlaylistWplParser::suffix()
{
    return "*.wpl";
}

KNMusicPlaylistModel *KNMusicPlaylistWplParser::read(const QString &filePath)
{
    //Get the wpl file first.
    QFile wplFile(filePath);
    //Open the playlist file in read only mode.
    if(!wplFile.open(QIODevice::ReadOnly))
    {
        return nullptr;
    }
    //Generate the document parse object.
    QDomDocument wplDocument;
    //Use QDomDocument to parse the file.
    if(!wplDocument.setContent(&wplFile, true))
    {
        //If there's any error, return nullptr.
        return nullptr;
    }
    //Close the playlist file.
    wplFile.close();
    //Find 'smil' element in the document.
    QDomNodeList testList=wplDocument.elementsByTagName("smil");
    //Check if there's any 'smil' element in the document.
    if(testList.size()!=1)
    {
        //Cannot find the 'smil' element.
        return nullptr;
    }
    //Get the 'smil' dom node.
    QDomNode smilNode=testList.item(0),
    //Get the 'head' and the 'body' node from the 'smil' node.
             headNode=smilNode.firstChildElement("head"),
             bodyNode=smilNode.firstChildElement("body");
    //If any of them is null, then we cannot load the playlist.
    if(headNode.isNull() || bodyNode.isNull())
    {
        return nullptr;
    }
    //Get the 'seq' dom node from the 'body' node, all the song information is
    //in the 'seq' node.
    QDomNode seqNode=bodyNode.firstChildElement("seq");
    //Check the null of the 'seq' node.
    if(seqNode.isNull())
    {
        return nullptr;
    }
    //Generate the playlist model.
    KNMusicPlaylistModel *playlistModel=new KNMusicPlaylistModel(thread());
    //Set the title of the playlist.
    playlistModel->setTitle(headNode.firstChildElement("title").text());
    //Get the playlist path. Because the music file path inside the wpl playlist
    //maybe RELATIVE path. FUCK Microsoft!!!
    QString nativeSeparatorPlaylistPath=
            QDir::toNativeSeparators(QFileInfo(wplFile).absolutePath());
    //Generate a file path list.
    QStringList playlistFileList;
    //Check all the node in 'seq' dom node.
    for(QDomNode i=seqNode.firstChild();
            i!=seqNode.lastChild();
            i=i.nextSibling())
    {
        //Only when the name of the node is 'media' and it has attributes,
        //we will parse the node.
        if(i.nodeName()=="media" && i.hasAttributes())
        {
            //Translate the node to element.
            QDomElement currentElement=i.toElement();
            //Get the music file path.
            QString srcInformation=currentElement.attribute("src");
            //Check whether the file exists, if it doesn't exist, it may be
            //relative path.
            if(!QFileInfo::exists(srcInformation))
            {
                //Add the playlist file folder in front of the file path.
                srcInformation=nativeSeparatorPlaylistPath+"\\"+srcInformation;
                //Check the existence once more.
                if(!QFileInfo::exists(srcInformation))
                {
                    continue;
                }
            }
            //Add the music file path to the file list.
            playlistFileList.append(
                        QFileInfo(srcInformation).absoluteFilePath());
        }
    }
    //Add the playlsit file list to the model.
    playlistModel->appendFiles(playlistFileList);
    //Give back the playlist model.
    return playlistModel;
}

bool KNMusicPlaylistWplParser::write(KNMusicPlaylistModel *playlist,
                                     const QString &filePath)
{
    //Generate the dom document.
    QDomDocument wplDocument;
    //Initial the root 'smil' element.
    QDomElement root=wplDocument.createElement("smil");
    //Add 'smil' element to the document.
    wplDocument.appendChild(root);

    //Generate 'head' element.
    QDomElement head=wplDocument.createElement("head");
    //Add 'head' to 'smil' element.
    root.appendChild(head);
    //Generate meta data.
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
    metaItemCount.setAttribute("content",
                               QString::number(playlist->rowCount()));
    //Set playlist title.
    QDomText titleText=wplDocument.createTextNode(playlist->title());
    title.appendChild(titleText);
    //Add meta data to head.
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

    //Generate the 'body' and 'seq' element.
    QDomElement body=wplDocument.createElement("body"),
                seq=wplDocument.createElement("seq");
    //Add 'body' to 'smil', add 'seq' to 'body'.
    root.appendChild(body);
    body.appendChild(seq);
    //Add the music file path to the 'seq' element.
    for(int i=0; i<playlist->rowCount(); i++)
    {
        //Generate the media element.
        QDomElement media=wplDocument.createElement("media");
        //Set the media info.
        media.setAttribute("src",
                           QDir::toNativeSeparators(
                               playlist->rowProperty(i,
                                                     FilePathRole).toString()));
        //Add the media element to seq.
        seq.appendChild(media);
    }

    //Write the data to playlist file.
    return writeContent(filePath,
                        "<?wpl version=\"1.0\"?>\n"+
                        wplDocument.toString(4));
}
