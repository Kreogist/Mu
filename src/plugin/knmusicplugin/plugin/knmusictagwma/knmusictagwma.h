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

#ifndef KNMUSICTAGWMA_H
#define KNMUSICTAGWMA_H

#include "knmusictagparser.h"

/*!
 * \brief The KNMusicTagWma class provides the Windows Media Audio (WMA format)
 * audio tag reading parser.
 */
class KNMusicTagWma : public KNMusicTagParser
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicTagWma parser object.
     * \param parent The parent object.
     */
    explicit KNMusicTagWma(QObject *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicTagParser::parseTag.
     */
    bool parseTag(QFile &musicFile,
                  QDataStream &musicDataStream,
                  KNMusicAnalysisItem &analysisItem) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicTagParser::tagParserName().
     */
    QString tagParserName() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicTagParser::writeTag.
     */
    bool writeTag(const KNMusicAnalysisItem &analysisItem) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicTagParser::parseAlbumArt.
     */
    bool parseAlbumArt(KNMusicAnalysisItem &analysisItem) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicTagParser::writable().
     */
    bool writable() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicTagParser::writeCoverImage().
     */
    bool writeCoverImage() const Q_DECL_OVERRIDE;

signals:

public slots:

private:
    enum StandardFrameItems
    {
        ItemTitle,
        ItemAuthor,
        ItemCopyright,
        ItemDescription,
        ItemRating,
        StandardFrameItemsCount
    };
    struct KNMusicWMAFrame
    {
        QString name;
        QByteArray data;
    };
    inline quint64 dataToInt64(const char *dataArray);
    inline void parseStandardFrame(char *frameStart,
                                   quint64 frameSize,
                                   QList<KNMusicWMAFrame> &frameList);
    inline void parseExtendFrame(char *frameStart,
                                 quint64 frameSize,
                                 QList<KNMusicWMAFrame> &frameList);
    bool parseImageData(QByteArray imageData, QImage &albumArt);
    static QString m_standardFrameID[StandardFrameItemsCount];
    static QHash<QString, int> m_attributesIndex;
    static unsigned char m_headerMark[17];
    static unsigned char m_standardFrame[17];
    static unsigned char m_extendedFrame[17];
    static QTextCodec *m_localeCodec,
                      *m_isoCodec,
                      *m_utf16BECodec,
                      *m_utf16LECodec,
                      *m_utf16Codec,
                      *m_utf8Codec;
};

#endif // KNMUSICTAGWMA_H
