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
    QString QQStringFilter(QString s);
    QString QQQueryString(const KNMusicDetailInfo &detailInfo);
    QString QQRequestString(const QString &id);
    QTextCodec *m_gbkCodec;
    QString QQFilter;
};

#endif // KNMUSICQQLYRICS_H
