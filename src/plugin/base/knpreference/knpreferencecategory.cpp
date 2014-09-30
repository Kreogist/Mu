#include <QPainter>
#include <QBoxLayout>
#include <QBrush>

#include "knpreferencetitle.h"

#include "knpreferencecategory.h"

KNPreferenceCategory::KNPreferenceCategory(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setFixedWidth(m_listWidth);

    //Set layout.
    m_layout=new QBoxLayout(QBoxLayout::TopToBottom,
                            this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);

    //Initial the header button.
    m_title=new KNPreferenceTitle(this);
    m_layout->addWidget(m_title);

    m_layout->addStretch();

    //Set translation.
    retranslateAndSet();
}

void KNPreferenceCategory::retranslate()
{
    m_configureText=tr("Configure");
}

void KNPreferenceCategory::retranslateAndSet()
{
    retranslate();
}

void KNPreferenceCategory::paintEvent(QPaintEvent *event)
{
    //Initial antialiasing painter.
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    //Paint the background.
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_backgroundColor);
    painter.drawRect(rect());
    QBrush backgroundLine;
    backgroundLine.setStyle(Qt::FDiagPattern);
    backgroundLine.setColor(m_lineColor);
    painter.setBrush(backgroundLine);
    painter.drawRect(rect());
    //Paint the border.
    painter.setBrush(m_borderColor);
    painter.drawRect(width()-m_borderWidth,
                     0,
                     m_borderWidth,
                     height());
    //Draw other things.
    QWidget::paintEvent(event);
}
