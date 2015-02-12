#ifndef KNMUSICLYRICSGLOBAL_H
#define KNMUSICLYRICSGLOBAL_H

#include <QObject>

class KNMusicLyricsGlobal : public QObject
{
    Q_OBJECT
public:
    static QString lyricsFolderPath();
    static void setLyricsFolderPath(const QString &lyricsFolderPath);

signals:

public slots:

private:
    explicit KNMusicLyricsGlobal(QObject *parent = 0);
    static QString m_lyricsFolderPath;
};

#endif // KNMUSICLYRICSGLOBAL_H
