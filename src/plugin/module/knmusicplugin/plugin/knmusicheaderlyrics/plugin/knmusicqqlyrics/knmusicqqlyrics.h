#ifndef KNMUSICQQLYRICS_H
#define KNMUSICQQLYRICS_H

#include "../../sdk/knmusiclyricsdownloader.h"

class KNMusicQQLyrics : public KNMusicLyricsDownloader
{
    Q_OBJECT
public:
    explicit KNMusicQQLyrics(QObject *parent = 0);
    QString downloadLyrics(const KNMusicDetailInfo &detailInfo);

signals:

public slots:

private:
    QString processKeywordsToGBK(QString s);
    inline QString generateQueryUrl(const KNMusicDetailInfo &detailInfo);
    inline QString generateRequestString(const QString &id);
    QTextCodec *m_gbkCodec;
    QString QQFilter;
};

#endif // KNMUSICQQLYRICS_H
