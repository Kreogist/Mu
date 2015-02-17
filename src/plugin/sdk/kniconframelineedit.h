/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNICONFRAMELINEEDIT_H
#define KNICONFRAMELINEEDIT_H

#include <QFrame>

class QTimeLine;
class KNLabelButton;
class KNFocusLineEdit;
class KNIconFrameLineEdit : public QFrame
{
    Q_OBJECT
public:
    explicit KNIconFrameLineEdit(QWidget *parent = 0);
    //UI
    void setIcon(const QPixmap &icon);

    //Content controls.
    void clear();
    QString text() const;
    void setPlaceHolderText(const QString &text);

    //Focus control widgets.
    QWidget *escFocusTo();
    void setEscFocusTo(QWidget *escFocusTo);
    QWidget *defaultEscFocusTo();
    void setDefaultEscFocusTo(QWidget *defaultEscFocusTo);

signals:
    void editingFinished();
    void returnPressed();
    void selectionChanged();
    void textChanged(const QString & text);
    void textEdited(const QString & text);

public slots:
    void setText(const QString &text);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private slots:
    void handOverFocus();
    void onActionFocusIn();
    void onActionFocusOut();
    void changeBackground(const int &frame);
    void changeTextBackground(const int &frame);

private:
    inline void initialActions();
    inline void initialTimelines();
    inline void stopAllAnimtaion();
    inline void configureAndStartAnimation(QTimeLine *animation);

    inline QTimeLine *generateTimeLines(int duration);
    //Widgets.
    KNLabelButton *m_iconButton;
    KNFocusLineEdit *m_textContent;

    //Focus escape pointer.
    QWidget *m_escFocusTo=nullptr,
            *m_defaultEscFocusTo=nullptr;

    //TimeLines.
    QTimeLine *m_mouseEnterAnime,
              *m_mouseLeaveAnime,
              *m_focusInAnime,
              *m_focusOutAnime;

    //UI elements.
    QPalette m_palette;
    QColor m_baseColor=QColor(0xff, 0xff, 0xff),
           m_textColor=QColor(0xff, 0xff, 0xff);
};

#endif // KNICONFRAMELINEEDIT_H
