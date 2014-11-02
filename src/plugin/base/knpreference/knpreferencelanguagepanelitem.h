/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNPREFERENCELANGUAGEPANELITEM_H
#define KNPREFERENCELANGUAGEPANELITEM_H

#include <QVariant>
#include <QWidget>

class QLabel;
class QTimeLine;
class QBoxLayout;
class KNPreferenceLanguagePanelItem : public QWidget
{
    Q_OBJECT
public:
    explicit KNPreferenceLanguagePanelItem(QWidget *parent = 0);
    QString languageName() const;
    void setLanguageName(const QString &languageName);
    QPixmap languageIcon() const;
    void setLanguageIcon(const QPixmap &languageIcon);

signals:

public slots:

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);

private slots:
    void onActionChangeHighlight(const int &frame);

private:
    void configureMouseInOutTimeline(QTimeLine *timeLine);
    int m_itemHeight=40, m_iconX=10, m_iconSize=40,
        m_textX=m_iconX+m_iconSize+10;
    qreal m_highLightOpacity=0.0;
    QString m_languageName;
    QPixmap m_languageIcon;
    QTimeLine *m_mouseIn, *m_mouseOut;
    QLinearGradient m_highlight;
    QBoxLayout *m_mainLayout;
};

#endif // KNPREFERENCELANGUAGEPANELITEM_H
