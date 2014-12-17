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
    inline QString processKeywordsToGBK(const QString &keywords);
    inline QString generateRequestString(const QString &id);
    QTextCodec *m_gbkCodec;
};

#endif // KNMUSICQQLYRICS_H
