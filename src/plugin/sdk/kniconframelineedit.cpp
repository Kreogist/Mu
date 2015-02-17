/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QAction>
#include <QBoxLayout>
#include <QTimeLine>

#include "knlabelbutton.h"
#include "knfocuslineedit.h"

#include "kniconframelineedit.h"

KNIconFrameLineEdit::KNIconFrameLineEdit(QWidget *parent) :
    QFrame(parent)
{
    //Set properties.
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);
    setFrameStyle(QFrame::NoFrame);
    setLineWidth(1);
    setMidLineWidth(0);

    //Intial the palette.
    m_palette=palette();
    m_palette.setColor(QPalette::Text, m_textColor);
    m_palette.setColor(QPalette::Highlight, QColor(0xf7, 0xcf, 0x3d));
    m_palette.setColor(QPalette::HighlightedText, QColor(0, 0, 0));
    setPalette(m_palette);

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //Initial the search button.
    m_iconButton=new KNLabelButton(this);
    m_iconButton->setScaledContents(true);
    mainLayout->addWidget(m_iconButton);

    //Initial the line edit.
    m_textContent=new KNFocusLineEdit(this);
    m_textContent->setContentsMargins(0,0,0,0);
    m_textContent->setTextMargins(0,0,0,0);
    m_textContent->setFrame(false);
    setFocusProxy(m_textContent);
    mainLayout->addWidget(m_textContent, 1);
    //Height hack.
    int lineEditHeight=m_textContent->height()-8;
    setFixedHeight(lineEditHeight);
    m_iconButton->setFixedSize(lineEditHeight, lineEditHeight);
    //Link the text content signals.
    connect(m_textContent, &KNFocusLineEdit::editingFinished,
            this, &KNIconFrameLineEdit::editingFinished);
    connect(m_textContent, &KNFocusLineEdit::returnPressed,
            this, &KNIconFrameLineEdit::returnPressed);
    connect(m_textContent, &KNFocusLineEdit::selectionChanged,
            this, &KNIconFrameLineEdit::selectionChanged);
    connect(m_textContent, &KNFocusLineEdit::textChanged,
            this, &KNIconFrameLineEdit::textChanged);
    connect(m_textContent, &KNFocusLineEdit::textEdited,
            this, &KNIconFrameLineEdit::textEdited);
    //Link the focus signals.
    connect(m_textContent, &KNFocusLineEdit::focusIn,
            this, &KNIconFrameLineEdit::onActionFocusIn);
    connect(m_textContent, &KNFocusLineEdit::focusOut,
            this, &KNIconFrameLineEdit::onActionFocusOut);

    //Initial actions.
    initialActions();
    //Initial timelines.
    initialTimelines();

    //Set the palette to the default state.
    changeTextBackground(m_minimumLightness);
}

void KNIconFrameLineEdit::setIcon(const QPixmap &icon)
{
    m_iconButton->setPixmap(icon);
}

void KNIconFrameLineEdit::clear()
{
    m_textContent->clear();
}

QString KNIconFrameLineEdit::text() const
{
    return m_textContent->text();
}

void KNIconFrameLineEdit::setPlaceHolderText(const QString &text)
{
    m_textContent->setPlaceholderText(text);
}

void KNIconFrameLineEdit::disableEscapeKey()
{
    m_escPressed->setEnabled(false);
}

void KNIconFrameLineEdit::changeBackground(const int &frame)
{
    //frame = the value(brightness) of the m_baseColor in HSV module.
    m_baseColor.setHsv(m_baseColor.hue(),
                       m_baseColor.saturation(),
                       frame);
    //Update the palette.
    m_palette.setColor(QPalette::Base, m_baseColor);
    m_palette.setColor(QPalette::Window, m_baseColor);
    setPalette(m_palette);
    //Update the text palette.
    m_textContent->setPalette(m_palette);
}

void KNIconFrameLineEdit::changeTextBackground(const int &frame)
{
    //frame = the value(brightness) of the m_baseColor in HSV module.
    m_baseColor.setHsv(m_baseColor.hue(),
                       m_baseColor.saturation(),
                       frame);
    //Update the palette.
    m_palette.setColor(QPalette::Base, m_baseColor);
    m_palette.setColor(QPalette::Window, m_baseColor);
    setPalette(m_palette);
    //Update the text content palette.
    m_textColor.setHsv(m_textColor.hue(),
                       m_textColor.saturation(),
                       0xFF-frame);
    m_palette.setColor(QPalette::Text, m_textColor);
    m_textContent->setPalette(m_palette);
}

inline void KNIconFrameLineEdit::initialActions()
{
    //When pressed ESC, hand over the text focus to escaped widget.
    m_escPressed=new QAction(m_textContent);
    m_escPressed->setShortcut(QKeySequence(Qt::Key_Escape));
    m_escPressed->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(m_escPressed, SIGNAL(triggered()), this, SLOT(handOverFocus()));
    m_textContent->addAction(m_escPressed);

    //When user pressed tab, hand over the focus to the escaped widget.
    //This is hack for custumize the tab order.
    QAction *tabPressed=new QAction(m_textContent);
    tabPressed->setShortcut(QKeySequence(Qt::Key_Tab));
    tabPressed->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(tabPressed, SIGNAL(triggered()), this, SLOT(handOverFocus()));
    m_textContent->addAction(tabPressed);
}

void KNIconFrameLineEdit::initialTimelines()
{
    //Generate time lines.
    m_mouseEnterAnime=generateTimeLines(100);
    m_mouseLeaveAnime=generateTimeLines(100);
    m_focusInAnime=generateTimeLines(100);
    m_focusOutAnime=generateTimeLines(200);

    //Set the final stops.
    m_mouseEnterAnime->setEndFrame(0x60);
    m_mouseLeaveAnime->setEndFrame(m_minimumLightness);
    m_focusInAnime->setEndFrame(0xFF);
    m_focusOutAnime->setEndFrame(m_minimumLightness);

    //Link the timeline animations.
    connect(m_mouseEnterAnime, &QTimeLine::frameChanged,
            this, &KNIconFrameLineEdit::changeBackground);
    connect(m_mouseLeaveAnime, &QTimeLine::frameChanged,
            this, &KNIconFrameLineEdit::changeBackground);
    connect(m_focusInAnime, &QTimeLine::frameChanged,
            this, &KNIconFrameLineEdit::changeTextBackground);
    connect(m_focusOutAnime, &QTimeLine::frameChanged,
            this, &KNIconFrameLineEdit::changeTextBackground);
}

inline void KNIconFrameLineEdit::stopAllAnimtaion()
{
    //Stop all the animations.
    m_mouseEnterAnime->stop();
    m_mouseLeaveAnime->stop();
    m_focusInAnime->stop();
    m_focusOutAnime->stop();
}

inline void KNIconFrameLineEdit::configureAndStartAnimation(QTimeLine *animation)
{
    animation->setStartFrame(m_baseColor.value());
    animation->start();
}

inline QTimeLine *KNIconFrameLineEdit::generateTimeLines(int duration)
{
    QTimeLine *timeLine=new QTimeLine(duration, this);
    timeLine->setUpdateInterval(5);
    return timeLine;
}

void KNIconFrameLineEdit::setMinimumLightness(int minimumLightness)
{
    m_minimumLightness=minimumLightness;
    //Update the leave animation.
    m_mouseLeaveAnime->setEndFrame(m_minimumLightness);
    m_focusOutAnime->setEndFrame(m_minimumLightness);
    //Update the palette.
    changeTextBackground(m_minimumLightness);
}

QWidget *KNIconFrameLineEdit::defaultEscFocusTo()
{
    return m_defaultEscFocusTo;
}

void KNIconFrameLineEdit::setDefaultEscFocusTo(QWidget *defaultEscFocusTo)
{
    m_defaultEscFocusTo = defaultEscFocusTo;
}

void KNIconFrameLineEdit::setMouseEnterLightness(int mouseEnterLightness)
{
    m_mouseEnterAnime->setEndFrame(mouseEnterLightness);
}

void KNIconFrameLineEdit::setText(const QString &text)
{
    m_textContent->setText(text);
}

void KNIconFrameLineEdit::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    //If the text content gets the focus, ignore the mouse in.
    if(m_textContent->hasFocus())
    {
        return;
    }
    //Stop animations, configure mouse enter animation and start.
    stopAllAnimtaion();
    configureAndStartAnimation(m_mouseEnterAnime);
}

void KNIconFrameLineEdit::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    //If the text content gets the focus, ignore the mouse out.
    if(m_textContent->hasFocus())
    {
        return;
    }
    //Stop animations, configure mouse leave animations and start.
    stopAllAnimtaion();
    configureAndStartAnimation(m_mouseLeaveAnime);
}

void KNIconFrameLineEdit::handOverFocus()
{
    //Hand over the focus to the escaped widget.
    if(m_escFocusTo!=nullptr)
    {
        m_escFocusTo->setFocus();
    }
    else
    {
        //If the widget is null, check the default widget, hand over the focus.
        if(m_defaultEscFocusTo!=nullptr)
        {
            m_defaultEscFocusTo->setFocus();
        }
    }
    //Launch the lost focus animation.
    onActionFocusOut();
}

void KNIconFrameLineEdit::onActionFocusIn()
{
    //Stop the animation timelines.
    stopAllAnimtaion();
    //Configure focus get timeline and start.
    configureAndStartAnimation(m_focusInAnime);
}

void KNIconFrameLineEdit::onActionFocusOut()
{
    //Clear the focus hand over widget.
    m_escFocusTo=nullptr;
    //Stop the animation timelines.
    stopAllAnimtaion();
    //Configure focus lost timeline and start.
    configureAndStartAnimation(m_focusOutAnime);
}

QWidget *KNIconFrameLineEdit::escFocusTo()
{
    return m_escFocusTo;
}

void KNIconFrameLineEdit::setEscFocusTo(QWidget *escFocusTo)
{
    m_escFocusTo = escFocusTo;
}
