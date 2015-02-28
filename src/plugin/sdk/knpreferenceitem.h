/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef KNPREFERENCEITEM_H
#define KNPREFERENCEITEM_H

#include <QVariant>
#include <QLinearGradient>

#include <QWidget>

#define PreferenceSingleItemHeight 34
#define PreferenceCaptionWidth 185

class QBoxLayout;
class QLabel;
class QTimeLine;
class KNOpacityButton;
class KNConfigure;
class KNPreferenceItem : public QWidget
{
    Q_OBJECT
public:
    explicit KNPreferenceItem(QWidget *parent = 0);
    ~KNPreferenceItem();

    QString caption() const;
    QString hint() const;
    QString valueName() const;
    virtual QVariant value() const;
    virtual QVariant defaultValue() const;
    KNConfigure *configure() const;
    void setConfigure(KNConfigure *configure);
    void setValueName(const QString &valueName);
    void loadValue();
    void saveValue();

signals:

public slots:
    void setCaption(const QString &caption);
    void setHint(const QString &hint);
    virtual void setDefaultValue(const QVariant &defaultValue);
    virtual void setValue(const QVariant &value);

protected:
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    QBoxLayout *mainLayout();
    void startMouseInAnime();
    void startMouseOutAnime();

private slots:
    void onActionChangeHighlight(const int &frame);
    void onActionUndo();

private:
    inline KNOpacityButton *generateIconButton(const QPixmap &icon);
    inline QTimeLine *generateTimeLine(const int &endFrame);
    inline void startAnimation(QTimeLine *timeLine);

    //Data.
    QVariant m_value, m_defaultValue;
    KNConfigure *m_configure=nullptr;
    QString m_valueName;

    //Widgets
    KNOpacityButton *m_undo, *m_itemHint;
    QBoxLayout *m_mainLayout;
    QLabel *m_caption;

    //Animes.
    QTimeLine *m_mouseIn, *m_mouseOut;

    //Parameter.
    qreal m_highLightOpacity=0.0;
    QLinearGradient m_highlight;
};

#endif // KNPREFERENCEITEM_H
