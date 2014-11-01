/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNLOCALEMANAGER_H
#define KNLOCALEMANAGER_H

#include <QPixmap>
#include <QStringList>
#include <QObject>

class QTranslator;
class KNLocaleManager : public QObject
{
    Q_OBJECT
public:
    static KNLocaleManager *instance();
    QString currentLanguageName() const;
    QPixmap currentLanguageIcon() const;

signals:
    void requireRetranslate();

public slots:
    void setLanguage(const int &index);

private:
    void loadLanguageFiles();
    static KNLocaleManager *m_instance;
    explicit KNLocaleManager(QObject *parent = 0);
    int m_currentIndex=-1;
    QStringList m_languageFiles, m_languageNames;
    QList<QPixmap> m_languageIcons;
    QPixmap m_noImageIcon;
    QTranslator *m_translator;
};

#endif // KNLOCALEMANAGER_H
