/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QAction>
#include <QBoxLayout>
#include <QFocusEvent>
#include <QKeySequence>
#include <QKeyEvent>
#include <QTimeLine>
#include <QFocusEvent>
#include <QCursor>

#include <QDebug>

#include "knsearchbox.h"

KNSearchLineEdit::KNSearchLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    //Set properties.
    setTextMargins(0,0,0,0);
    setContentsMargins(0,0,0,0);
    setFrame(false);
}

void KNSearchLineEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    //When getting a focus, emit a signal.
    emit getFocus();
}

void KNSearchLineEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    //When losing a focus, emit a signal.
    emit lostFocus();
}

KNSearchButton::KNSearchButton(QWidget *parent) :
    QLabel(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
}

KNSearchBox::KNSearchBox(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //Initial the search button.
    m_button=new KNSearchButton(this);
    m_button->setPixmap(QPixmap("://public/SearchIcon.png"));
    mainLayout->addWidget(m_button);

    //Initial the text content box.
    m_textContent=new KNSearchLineEdit(this);
    setFocusProxy(m_textContent);
    m_textContent->setPalette(m_palette);
    connect(m_textContent, &KNSearchLineEdit::getFocus,
            this, &KNSearchBox::onFocusGet);
    connect(m_textContent, &KNSearchLineEdit::lostFocus,
            this, &KNSearchBox::onFocusLost);
    connect(m_textContent, &KNSearchLineEdit::editingFinished,
            this, &KNSearchBox::editingFinished);
    connect(m_textContent, &KNSearchLineEdit::returnPressed,
            this, &KNSearchBox::returnPressed);
    connect(m_textContent, &KNSearchLineEdit::selectionChanged,
            this, &KNSearchBox::selectionChanged);
    connect(m_textContent, &KNSearchLineEdit::textChanged,
            this, &KNSearchBox::textChanged);
    connect(m_textContent, &KNSearchLineEdit::textEdited,
            this, &KNSearchBox::textEdited);
    mainLayout->addWidget(m_textContent, 1);

    //Initial the default palette.
    m_palette=palette();
    m_palette.setColor(QPalette::Text, m_textColor);
    m_palette.setColor(QPalette::Highlight, QColor(0xf7, 0xcf, 0x3d));
    setPalette(m_palette);
    onActionBackgroundChanged(m_minLightness);

    //When pressed Esc key, do escape focus.
    QAction *escapeAction=new QAction(m_textContent);
    escapeAction->setShortcut(QKeySequence(Qt::Key_Escape));
    escapeAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(escapeAction, SIGNAL(triggered()),
            this, SLOT(onActionLostFocus()));
    addAction(escapeAction);

    //This is a hack to process the tab request.
    QAction *tabAction=new QAction(m_textContent);
    tabAction->setShortcut(QKeySequence(Qt::Key_Tab));
    tabAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(tabAction, SIGNAL(triggered()),
            this, SLOT(onActionLostFocus()));
    addAction(tabAction);

    m_mouseEnterAnime=new QTimeLine(100, this);
    m_mouseEnterAnime->setUpdateInterval(5);
    m_mouseEnterAnime->setEndFrame(0x60);
    connect(m_mouseEnterAnime, &QTimeLine::frameChanged,
            this, &KNSearchBox::onActionBackgroundChanged);

    m_mouseLeaveAnime=new QTimeLine(100, this);
    m_mouseLeaveAnime->setUpdateInterval(5);
    m_mouseLeaveAnime->setEndFrame(m_minLightness);
    connect(m_mouseLeaveAnime, &QTimeLine::frameChanged,
            this, &KNSearchBox::onActionBackgroundChanged);

    m_focusGet=new QTimeLine(100, this);
    m_focusGet->setUpdateInterval(5);
    m_focusGet->setEndFrame(0xff);
    connect(m_focusGet, &QTimeLine::frameChanged,
            this, &KNSearchBox::onActionTextBackgroundChange);

    m_focusLost=new QTimeLine(200, this);
    m_focusLost->setUpdateInterval(5);
    m_focusLost->setEndFrame(m_minLightness);
    connect(m_focusLost, &QTimeLine::frameChanged,
            this, &KNSearchBox::onActionTextBackgroundChange);

    setFixedHeight(m_textContent->height()-8);
}

void KNSearchBox::setPlaceHolderText(const QString &text)
{
    m_textContent->setPlaceholderText(text);
}

void KNSearchBox::clear()
{
    m_textContent->clear();
}

QString KNSearchBox::text() const
{
    return m_textContent->text();
}

void KNSearchBox::setText(const QString &text)
{
    m_textContent->setText(text);
}

void KNSearchBox::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    if(!m_textContent->hasFocus())
    {
        //Stop all timeline.
        m_mouseLeaveAnime->stop();
        m_mouseEnterAnime->stop();
        //Set parameter and start time line.
        m_mouseEnterAnime->setStartFrame(m_baseColor.value());
        m_mouseEnterAnime->start();
    }
}

void KNSearchBox::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    if(!m_textContent->hasFocus())
    {
        //Stop all timeline.
        m_mouseEnterAnime->stop();
        m_mouseLeaveAnime->stop();
        //Set parameter and start time line.
        m_mouseLeaveAnime->setStartFrame(m_baseColor.value());
        m_mouseLeaveAnime->start();
    }
}

void KNSearchBox::onActionLostFocus()
{
    //Set the focus to the widget we set at first.
    if(m_escFocusTo!=nullptr)
    {
        m_escFocusTo->setFocus();
    }
    else if(m_defaultEscFocusTo!=nullptr)
    {
        m_defaultEscFocusTo->setFocus();
    }
    //Do lost focus animation.
    onFocusLost();
}

void KNSearchBox::onActionBackgroundChanged(const int &frame)
{
    //Change color and palette.
    m_baseColor.setHsv(m_baseColor.hue(),
                       m_baseColor.saturation(),
                       frame);
    m_palette.setColor(QPalette::Base, m_baseColor);
    m_palette.setColor(QPalette::Window, m_baseColor);
    //Set palette.
    setPalette(m_palette);
    m_textContent->setPalette(m_palette);
}

void KNSearchBox::onActionTextBackgroundChange(const int &frame)
{
    //Change color and palette.
    m_baseColor.setHsv(m_baseColor.hue(),
                       m_baseColor.saturation(),
                       frame);
    m_palette.setColor(QPalette::Base, m_baseColor);
    m_palette.setColor(QPalette::Window, m_baseColor);
    m_textColor.setHsv(m_textColor.hue(),
                       m_textColor.saturation(),
                       0xff-frame);
    m_palette.setColor(QPalette::Text, m_textColor);
    //Set palette.
    setPalette(m_palette);
    m_textContent->setPalette(m_palette);
}

void KNSearchBox::onFocusGet()
{
    //Stop focus timeline.
    m_focusLost->stop();
    m_focusGet->stop();
    //Set parameters and start timeline.
    m_focusGet->setStartFrame(m_baseColor.value());
    m_focusGet->start();
}

void KNSearchBox::onFocusLost()
{
    //Clear focus source widget.
    m_escFocusTo=nullptr;
    //Stop focus timeline.
    m_focusGet->stop();
    m_focusLost->stop();
    //Set parameters and start timeline.
    m_focusLost->setStartFrame(m_baseColor.value());
    m_focusLost->start();
    //Emit
    emit requireLostFocus();
}
QWidget *KNSearchBox::defaultEscFocusTo() const
{
    return m_defaultEscFocusTo;
}

void KNSearchBox::setDefaultEscFocusTo(QWidget *defaultEscFocusTo)
{
    m_defaultEscFocusTo = defaultEscFocusTo;
}

QWidget *KNSearchBox::escFocusTo() const
{
    return m_escFocusTo;
}

void KNSearchBox::setEscFocusTo(QWidget *escFocusTo)
{
    m_escFocusTo=escFocusTo;
}
