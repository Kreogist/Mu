/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICSTANDARDBACKEND_H
#define KNMUSICSTANDARDBACKEND_H

#include "knmusicbackend.h"

class KNMusicBackendThread;
class KNMusicStandardBackend : public KNMusicBackend
{
    Q_OBJECT
public:
    explicit KNMusicStandardBackend(QObject *parent = 0);
    bool loadMusic(const QString &filePath);
    qint64 duration() const;
    qint64 position() const;
    bool playSection(const QString &fileName,
                     const qint64 &start=-1,
                     const qint64 &duration=-1);
    void play();
    void pause();
    void stop();
    void resetMainPlayer();

    void loadPreview(const QString &filePath);
    qint64 previewDuration() const;
    qint64 previewPosition() const;
    void playPreviewFile(const QString &fileName);
    void setPreviewSection(const qint64 &start=-1,
                           const qint64 &duration=-1);
    void playPreviewSection(const QString &fileName,
                            const qint64 &start=-1,
                            const qint64 &duration=-1);
    void playPreview();
    void stopPreview();
    void pausePreview();
    void resetPreviewPlayer();

signals:

public slots:
    void changeMuteState();
    void setVolume(int volumeSize);
    void setMute(const bool &mute);
    void setPosition(const qint64 &position);
    void volumeUp();
    void volumeDown();

    void setPreviewPosition(const qint64 &position);

protected:
    void setMainThread(KNMusicBackendThread *thread);
    void setPreviewThread(KNMusicBackendThread *thread);
    virtual void changeVolume(const int &volumeSize)=0;
    virtual int volumeLevel() const=0;
    virtual qreal smartVolumeScale() const=0;

private:
    inline void changeAndSyncVolume(const int &volumeSize);
    inline void smartVolumeOn();
    inline void smartVolumeOff();

    int m_originalVolume=-1,
        m_volumeBeforeMute=0;
    qint64 m_backupPosition=-1;
    bool m_mute=false;
    KNMusicBackendThread *m_main=nullptr, *m_preview=nullptr;
};

#endif // KNMUSICSTANDARDBACKEND_H
