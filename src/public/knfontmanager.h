/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNFONTMANAGER_H
#define KNFONTMANAGER_H

#include <QFileInfo>

#include <QObject>

class KNFontManager : public QObject
{
    Q_OBJECT
public:
    static KNFontManager *instance();
    void loadCustomFont(const QString &fontPath);
    void loadCustomFont(const QFileInfo &fontInfo);
    void loadCustomFontFolder(const QString &folderPath);
    void setGlobalFont(const QString &fontName, const int &pixelSize=12);
    void initialDefaultFont();

signals:

public slots:

private:
    static KNFontManager *m_instance;
    explicit KNFontManager(QObject *parent = 0);
};

#endif // KNFONTMANAGER_H
