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

#ifndef KNMUSICTAGID3V2_H
#define KNMUSICTAGID3V2_H

#include <QLinkedList>

#include "knmusictagparser.h"

class KNMusicTagId3v2 : public KNMusicTagParser
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicTagId3v2 class.
     * \param parent The parent object.
     */
    explicit KNMusicTagId3v2(QObject *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicTagParser::parseTag.
     */
    bool parseTag(QFile &musicFile,
                  QDataStream &musicDataStream,
                  KNMusicAnalysisItem &analysisItem) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicTagParser::writeTag.
     */
    bool writeTag(const KNMusicAnalysisItem &analysisItem) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicTagParser::parseAlbumArt.
     */
    bool parseAlbumArt(KNMusicAnalysisItem &analysisItem) Q_DECL_OVERRIDE;

    /*!
     * \brief Translate a content data byte array to string. It starts with a
     * encode byte, it defines the codec of the string. Follow the binary string
     * content.
     * \param content The content data.
     * \return The parsed string.
     */
    QString contentToString(QByteArray content);

    /*!
     * \brief Translate a string to content data byte array. It the inverse
     * function of contentToString().
     * \param string The string wait to be translated.
     * \param encoding The codec will be used.
     * \return The content array.
     */
    QByteArray stringToContent(const QString &string,
                               const quint8 &encoding);

signals:

public slots:

protected:
    enum ID3v2HeaderFlag
    {
        Unsynchronisation=0x80,
        ExtendedHeader=0x40,
        ExperimentalIndicator=0x20
    };
    enum ID3v2FrameFlag
    {
        FrameDataLengthIndicator=0x01,
        FrameUnsynchronisation=0x02
    };
    enum ID3v2TextEncoding
    {
        EncodeISO,
        EncodeUTF16BELE,
        EncodeUTF16,
        EncodeUTF8
    };
    /*!
     * \brief The ID3v2Header struct stores the header data of a ID3v2 tag. It
     * contains the major and minor version, size and global flag of the tag.
     */
    struct ID3v2Header
    {
        quint32 size;
        quint8 major;
        quint8 minor;
        quint8 flag;
        ID3v2Header() :
            size(0),
            flag(0)
        {
            //We have to initial the major and minor in this ugly way because of
            //the fucking GCC macros/functions define under Linux.
            major=0;
            minor=0;
        }
    };
    /*!
     * \brief The ID3v2Frame struct describes a raw ID3v2 frame. It can be
     * simply translated to the raw ID3v2 data.
     */
    struct ID3v2Frame
    {
        quint32 size;
        char frameID[5];
        char *start;
        char flags[2];
        ID3v2Frame() :
            size(0)
        {
        }
    };
    /*!
     * \brief The ID3v2DataFrame struct will store a raw ID3v2 frame data. The
     * different between a ID3v2DataFrame and a ID3v2Frame is that a ID3v2Frame
     * need another char array to store all the raw data, but a ID3v2DataFrame
     * doesn't. This structure also doesn't contains a frame ID. It should be
     * used with a hash class.
     */
    struct ID3v2DataFrame
    {
        QByteArray data;
        char flags[2];
        ID3v2DataFrame()
        {
            //Clear the flag bytes data.
            flags[0]=0;
            flags[1]=0;
        }
    };

    typedef quint32 (*FrameSizeCalculator)(char *);
    typedef void (*FlagSaver)(char *, ID3v2Frame &);
    typedef void (*FrameSizeWriter)(char *, const quint32 &);
    /*!
     * \brief The ID3v2FunctionSet struct is a set of function, including the
     * size calculator and the flag saver function according to the major and
     * minor version of the tag.
     */
    struct ID3v2FunctionSet
    {
        FrameSizeCalculator toSize;
        FrameSizeWriter writeSize;
        FlagSaver saveFlag;
        int frameIDSize;
        int frameHeaderSize;
        ID3v2FunctionSet() :
            toSize(nullptr),
            writeSize(nullptr),
            saveFlag(nullptr),
            frameIDSize(0),
            frameHeaderSize(0)
        {
        }
    };

    /*!
     * \brief The ID3v2PictureFrame struct describes a APIC or PIC frame data.
     * It has the mime type of the image frame, the description of the image and
     * the QImage of the album art.
     */
    struct ID3v2PictureFrame
    {
        QString mimeType;
        QString description;
        QImage image;
    };

    /*!
     * \brief Write an ID3v2 header structure data to a char array.
     * \param rawHeader The char data array.
     * \param header The header structure.
     */
    inline void generateID3v2Header(char *rawHeader,
                                    ID3v2Header &header)
    {
        //Set very beginning to 'ID3'.
        rawHeader[0]='I';
        rawHeader[1]='D';
        rawHeader[2]='3';
        //Set the major and minor version.
        rawHeader[3]=header.major;
        rawHeader[4]=header.minor;
        //Save the flag information.
        rawHeader[5]=header.flag;
        //Set the size data.
        rawHeader[6]=(header.size>>21) & 0x0000007F;
        rawHeader[7]=(header.size>>14) & 0x0000007F;
        rawHeader[8]=(header.size>>7 ) & 0x0000007F;
        rawHeader[9]= header.size      & 0x0000007F;
    }

    /*!
     * \brief Try to parse the ID3v2 header from the raw header data.
     * \param rawHeader The char array stores the raw header data. It should be
     * a fixed size array whose length is 10.
     * \param header The header structure.
     * \return If parse the header successfully, return true.
     */
    inline bool parseID3v2Header(char *rawHeader,
                                 ID3v2Header &header)
    {
        //Check 'ID3' from the very beginning.
        if(rawHeader[0]!='I' || rawHeader[1]!='D' || rawHeader[2]!='3')
        {
            return false;
        }
        //Get the version of the tag.
        header.major=(quint8)rawHeader[3];
        header.minor=(quint8)rawHeader[4];
        //Get the flag.
        header.flag=(quint8)rawHeader[5];
        //Calculate tag size.
        header.size=(((quint32)rawHeader[6]<<21) & 0x0FE00000)+
                    (((quint32)rawHeader[7]<<14) & 0x001FC000)+
                    (((quint32)rawHeader[8]<<7)  & 0x00003F80)+
                    ( (quint32)rawHeader[9]      & 0x0000007F);
        return true;
    }

    /*!
     * \brief This function can parse the raw data of a ID3v2 tag to a raw frame
     * list.
     * \param rawTagData The char array which stores the raw tag data.
     * \param header The header of the tag.
     * \param property The function set which generate according to the header
     * minor version.
     * \param frameList The frame list.
     * \return If the function parse the data successfully, return true.
     */
    bool parseID3v2RawData(char *rawTagData,
                           const ID3v2Header &header,
                           const ID3v2FunctionSet &property,
                           QLinkedList<ID3v2Frame> &frameList);

    /*!
     * \brief Write the raw frame to details info.
     * \param frames The raw frame list.
     * \param property The ID3v2 function tool set.
     * \param analysisItem The target analysis item.
     */
    void writeFrameToDetails(const QLinkedList<ID3v2Frame> &frames,
                             const ID3v2FunctionSet &property,
                             KNMusicAnalysisItem &analysisItem);

private:
    //Translate rating to 5-level star.
    static inline int ratingStars(const quint8 &hex)
    {
        //1-31  = 1 star.
        //32-95 = 2 stars.
        //96-159 = 3 stars.
        //160-223 = 4 stars.
        //224-255 = 5 stars.
        if(hex>0 && hex<32)
        {
            return 1;
        }
        if(hex>31 && hex<96)
        {
            return 2;
        }
        if(hex>95 && hex<160)
        {
            return 3;
        }
        if(hex>159 && hex<224)
        {
            return 4;
        }
        if(hex>223)
        {
            return 5;
        }
        return 0;
    }
    //Translate 5-level star to ratings.
    static inline int starsRating(const quint8 &stars)
    {
        //0 star  = 0
        //1 star  = 31
        //2 stars = 95
        //3 stars = 159
        //4 stars = 223
        //5 stars = 255 (Default for else)
        switch(stars)
        {
        case 0:
            return 0;
        case 1:
            return 31;
        case 2:
            return 95;
        case 3:
            return 159;
        case 4:
            return 223;
        default:
            return 255;
        }
    }

    //Because the ID3v2 has so many version and the standard has been changed
    //for many times, we have to use different calculate function to process
    //these frames in different ways.
    inline void getId3v2FunctionSet(const quint8 &major,
                                    ID3v2FunctionSet &toolSet)
    {
        switch(major)
        {
        case 0:
        case 1:
        case 2:
            toolSet.frameIDSize=3;
            toolSet.frameHeaderSize=6;
            toolSet.toSize=KNMusicTagId3v2::major2Size;
            toolSet.writeSize=KNMusicTagId3v2::writeMajor2Size;
            toolSet.saveFlag=nullptr;
            break;
        case 3:
            toolSet.frameIDSize=4;
            toolSet.frameHeaderSize=10;
            toolSet.toSize=KNMusicTagId3v2::major3Size;
            toolSet.writeSize=KNMusicTagId3v2::writeMajor3Size;
            toolSet.saveFlag=KNMusicTagId3v2::saveFlag;
            break;
        case 4:
            toolSet.frameIDSize=4;
            toolSet.frameHeaderSize=10;
            toolSet.toSize=KNMusicTagId3v2::major4Size;
            toolSet.writeSize=KNMusicTagId3v2::writeMajor4Size;
            toolSet.saveFlag=KNMusicTagId3v2::saveFlag;
            break;
        default:
            break;
        }
    }
    //ID3v2.0, ID3v2.1 and ID3v2.2 version size calculator.
    static inline quint32 major2Size(char *rawTagData)
    {
        return (((quint32)rawTagData[0]<<16) & 0x00FF0000)+
               (((quint32)rawTagData[1]<<8)  & 0x0000FF00)+
               ( (quint32)rawTagData[2]      & 0x000000FF);
    }
    //ID3v2.3 version(most popular) size calculator.
    static inline quint32 major3Size(char *rawTagData)
    {
        return KNMusicUtil::charToInt32(rawTagData);
    }
    //ID3v2.4 version size calculator.
    static inline quint32 major4Size(char *rawTagData)
    {
        return (((quint32)rawTagData[0]<<21) & 0x0FE00000)+
               (((quint32)rawTagData[1]<<14) & 0x001FC000)+
               (((quint32)rawTagData[2]<<7 ) & 0x00003F80)+
               ( (quint32)rawTagData[3]      & 0x0000007F);
    }
    //Write ID3v2.0, ID3v2.1 and ID3v2.2 version size.
    static inline void writeMajor2Size(char *rawTagData, const quint32 &size)
    {
        rawTagData[0]=(size >> 16) & 0x000000FF;
        rawTagData[1]=(size >> 8 ) & 0x000000FF;
        rawTagData[2]=(size      ) & 0x000000FF;
    }
    //ID3v2.3 version(most popular) size calculator.
    static inline void writeMajor3Size(char *rawTagData, const quint32 &size)
    {
        KNMusicUtil::int32ToChar(rawTagData, size);
    }
    //ID3v2.4 version size calculator.
    static inline void writeMajor4Size(char *rawTagData, const quint32 &size)
    {
        rawTagData[0]=(size & 0x0FE00000) >> 21;
        rawTagData[1]=(size & 0x001FC000) >> 14;
        rawTagData[2]=(size & 0x00003F80) >> 7 ;
        rawTagData[3]=(size & 0x0000007F);
    }
    //Copy the flag from raw data to frame data.
    static inline void saveFlag(char *rawTagData,
                                ID3v2Frame &frameData)
    {
        frameData.flags[0]=rawTagData[8];
        frameData.flags[1]=rawTagData[9];
    }
    //APIC frame and PIC frame generator.
    inline QByteArray generateImageData(const QImage &image,
                                        const int &frameIDSize);
    //APIC frame and PIC frame parser.
    inline void parseAPICImageData(QByteArray imageData,
                                   QHash<int, ID3v2PictureFrame> &imageMap);
    inline void parsePICImageData(QByteArray imageData,
                                  QHash<int, ID3v2PictureFrame> &imageMap);

    //Translate a ID3v2DataFrame to raw bytes.
    inline QByteArray frameToRawData(const QString frameID,
                                     const ID3v2FunctionSet &toolset,
                                     const ID3v2DataFrame &frame);

    //Some codec, locale codec data for iso encoding, UTF-16BE/LE and UTF-8.
    static QTextCodec *m_localeCodec,
                      *m_isoCodec,
                      *m_utf16BECodec,
                      *m_utf16LECodec,
                      *m_utf16Codec,
                      *m_utf8Codec;
    //The unsynchronisation data and replaced data.
    static QByteArray m_unsynchronisationRaw, m_unsynchronisationTo;
    //The frame hash list.
    static QHash<QString, int> m_frameIDIndex;
    static QHash<int, QString> m_IndexFrameID3, m_IndexFrameID4;
    //Use the system default codec, for default it will be true.
    //Because most of codec is compatible with ISO-8859-1.
    bool m_useDefaultCodec;
};

#endif // KNMUSICTAGID3V2_H
