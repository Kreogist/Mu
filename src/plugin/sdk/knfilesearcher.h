/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNFILESEARCHER_H
#define KNFILESEARCHER_H

#include <QStringList>

#include <QObject>

class KNFileSearcher : public QObject
{
    Q_OBJECT
public:
    explicit KNFileSearcher(QObject *parent = 0);
    bool isFilePathAccept(const QString &filePath);

signals:
    void fileFound(QString filePath);
    void requireAnalysisNext();
    void requireAnlaysisFolder(const QString &folderPath);

public slots:
    void analysisUrls(QStringList urls);

protected:
    virtual bool isSuffixAccept(const QString &suffix)=0;

private slots:
    void analysisFile(const QString &suffix,
                      const QString &filePath);
    void analysisFolder(const QString &folderPath);
    void analysisNext();

private:
    QStringList m_fileSearchQueue;
};

#endif // KNFILESEARCHER_H
