/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNPREFERENCEITEMBASE_H
#define KNPREFERENCEITEMBASE_H

#include <QVariant>
#include <QWidget>

class QLabel;
class QTimeLine;
class QBoxLayout;
class KNPreferenceItemBase : public QWidget
{
    Q_OBJECT
public:
    explicit KNPreferenceItemBase(QWidget *parent = 0);
    virtual QVariant defaultValue() const=0;
    virtual QVariant value() const=0;
    QString caption() const;
    QString valueName() const;

signals:

public slots:
    void setValueName(const QString &valueName);
    void setCaption(const QString &caption);
    virtual void setDefaultValue(const QVariant &defaultValue)=0;
    virtual void setValue(const QVariant &value)=0;

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);
    void insertWidget(QWidget *widget, int stretch = 0);
    void insertSpacing(int size);
    void insertStretch();

    void startLeaveAnime();

private slots:
    void onActionChangeHighlight(const int &frame);

private:
    void configureMouseInOutTimeline(QTimeLine *timeLine);
    int m_itemHeight=34;
    QString m_valueName;
    QLabel *m_caption;
    qreal m_highLightOpacity=0.0;
    QTimeLine *m_mouseIn, *m_mouseOut;
    QLinearGradient m_highlight;
    QBoxLayout *m_mainLayout;
};

#endif // KNPREFERENCEITEMBASE_H
