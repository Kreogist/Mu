#include <QPropertyAnimation>

#include "knheaderswitcher.h"

KNHeaderSwitcher::KNHeaderSwitcher(QWidget *parent) :
    KNWidgetSwitcher(parent)
{
    //Set properties.
    setFixedHeight(70);
}

void KNHeaderSwitcher::updateAnimationPosition()
{
    //Get the three position: left, center, right.
    QRect centerPosition=QRect(0,
                               0,
                               width(),
                               height());
    m_inAnimation->setEndValue(centerPosition);
    m_outAnimation->setStartValue(centerPosition);
    m_outAnimation->setEndValue(QRect(-width(),
                                      0,
                                      width(),
                                      height()));
    m_inAnimation->setStartValue(QRect(0,
                                       0,
                                       width(),
                                       height()));
}
