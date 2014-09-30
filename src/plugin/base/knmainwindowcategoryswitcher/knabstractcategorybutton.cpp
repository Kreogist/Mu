#include <QPainter>

#include "knabstractcategorybutton.h"

KNAbstractCategoryButton::KNAbstractCategoryButton(QWidget *parent) :
    KNAnimeCheckedButton(parent)
{
    //Set font.
    QFont captionFont=font();
    captionFont.setPixelSize(20);
    setFont(captionFont);
    //Set palette.
    QPalette pal=palette();
    pal.setColor(QPalette::WindowText, QColor(255,255,255));
    setPalette(pal);
}

QSize KNAbstractCategoryButton::sizeHint() const
{
    return QSize(m_iconSize + fontMetrics().width(m_text) + (m_margin<<1) + m_spacing,
                 m_iconSize + (m_margin<<1));
}

QPixmap KNAbstractCategoryButton::icon() const
{
    return m_icon;
}

void KNAbstractCategoryButton::setIcon(const QPixmap &icon)
{
    //Scale the icon.
    m_icon=icon.scaled(m_iconSize,
                       m_iconSize,
                       Qt::KeepAspectRatio,
                       Qt::SmoothTransformation);
}

QString KNAbstractCategoryButton::text() const
{
    return m_text;
}

void KNAbstractCategoryButton::setText(const QString &text)
{
    m_text=text;
}

void KNAbstractCategoryButton::paintEvent(QPaintEvent *event)
{
    KNAbstractButton::paintEvent(event);
    //Initial antialiasing painter.
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setPen(palette().color(QPalette::WindowText));
    //Paint the icon.
    painter.drawPixmap(m_margin,
                       m_margin,
                       m_icon);
    //Paint the text.
    painter.drawText(m_margin+m_iconSize+m_spacing,
                     m_margin,
                     width()-(m_margin<<1)-m_iconSize-m_spacing,
                     height()-(m_margin<<1),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     m_text);
}
