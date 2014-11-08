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

    //Initial the default palette.
    m_originalGrey=0x3A;
    m_grey=m_originalGrey;
    m_palette=palette();
    m_palette.setColor(QPalette::Base, QColor(m_originalGrey, m_originalGrey, m_originalGrey));
    m_palette.setColor(QPalette::Window, QColor(m_originalGrey, m_originalGrey, m_originalGrey));
    m_palette.setColor(QPalette::Text, QColor(0xff, 0xff, 0xff));
    m_palette.setColor(QPalette::Highlight, QColor(0xf7, 0xcf, 0x3d));
    setPalette(m_palette);

    m_layout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);

    m_button=new KNSearchButton(this);
    m_button->setPixmap(QPixmap("://public/SearchIcon.png"));
    m_layout->addWidget(m_button);

    m_keyText=new KNSearchLineEdit(this);
    m_keyText->setPalette(m_palette);
    connect(m_keyText, &KNSearchLineEdit::getFocus,
            this, &KNSearchBox::onFocusGet);
    connect(m_keyText, &KNSearchLineEdit::lostFocus,
            this, &KNSearchBox::onFocusLost);
    connect(m_keyText, &KNSearchLineEdit::editingFinished,
            this, &KNSearchBox::editingFinished);
    connect(m_keyText, &KNSearchLineEdit::returnPressed,
            this, &KNSearchBox::returnPressed);
    connect(m_keyText, &KNSearchLineEdit::selectionChanged,
            this, &KNSearchBox::selectionChanged);
    connect(m_keyText, &KNSearchLineEdit::textChanged,
            this, &KNSearchBox::textChanged);
    connect(m_keyText, &KNSearchLineEdit::textEdited,
            this, &KNSearchBox::textEdited);
    m_layout->addWidget(m_keyText, 1);

    QAction *escapeAction=new QAction(m_keyText);
    escapeAction->setShortcut(QKeySequence(Qt::Key_Escape));
    escapeAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(escapeAction, SIGNAL(triggered()),
            this, SLOT(onFocusLost()));
    m_keyText->addAction(escapeAction);

    m_mouseEnterAnime=new QTimeLine(100, this);
    m_mouseEnterAnime->setEndFrame(0x60);
    connect(m_mouseEnterAnime, &QTimeLine::frameChanged,
            this, &KNSearchBox::onActionBackgroundChanged);

    m_mouseLeaveAnime=new QTimeLine(100, this);
    m_mouseLeaveAnime->setEndFrame(m_originalGrey);
    connect(m_mouseLeaveAnime, &QTimeLine::frameChanged,
            this, &KNSearchBox::onActionBackgroundChanged);

    m_focusGet=new QTimeLine(100, this);
    m_focusGet->setEndFrame(0xff);
    connect(m_focusGet, &QTimeLine::frameChanged,
            this, &KNSearchBox::onActionTextBackgroundChange);

    m_focusLost=new QTimeLine(200, this);
    connect(m_focusLost, &QTimeLine::frameChanged,
            this, &KNSearchBox::onActionTextBackgroundChange);

    setFixedHeight(m_keyText->height()-8);
}

void KNSearchBox::setPlaceHolderText(const QString &text)
{
    m_keyText->setPlaceholderText(text);
}

void KNSearchBox::setSearchFocus()
{
    m_keyText->setFocus();
}

void KNSearchBox::clear()
{
    m_keyText->clear();
}

QString KNSearchBox::text() const
{
    return m_keyText->text();
}

void KNSearchBox::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    if(!m_focus)
    {
        m_mouseLeaveAnime->stop();
        m_mouseEnterAnime->setStartFrame(m_grey);
        m_mouseEnterAnime->start();
    }
}

void KNSearchBox::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    if(!m_focus)
    {
        m_mouseEnterAnime->stop();
        m_mouseLeaveAnime->setStartFrame(m_grey);
        m_mouseLeaveAnime->start();
    }
}

void KNSearchBox::onActionBackgroundChanged(const int &frame)
{
    m_grey=frame;
    m_palette.setColor(QPalette::Base, QColor(frame, frame, frame));
    m_palette.setColor(QPalette::Window, QColor(frame, frame, frame));
    setPalette(m_palette);
    m_keyText->setPalette(m_palette);
}

void KNSearchBox::onActionTextBackgroundChange(const int &frame)
{
    m_grey=frame;
    m_palette.setColor(QPalette::Base, QColor(frame, frame, frame));
    m_palette.setColor(QPalette::Window, QColor(frame, frame, frame));
    m_palette.setColor(QPalette::Text, QColor(0xff-frame, 0xff-frame, 0xff-frame));
    setPalette(m_palette);
    m_keyText->setPalette(m_palette);
}

void KNSearchBox::onFocusGet()
{
    if(!m_focus)
    {
        m_focus=true;
        m_focusLost->stop();
        m_focusGet->setStartFrame(m_grey);
        m_focusGet->start();
    }
}

void KNSearchBox::onFocusLost()
{
    if(m_focus)
    {
        m_focus=false;
        m_focusGet->stop();
        m_focusLost->setEndFrame(m_originalGrey);
        m_focusLost->start();
        emit requireLostFocus();
    }
}
