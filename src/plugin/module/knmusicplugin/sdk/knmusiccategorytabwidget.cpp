#include "knmusiccategorytabwidget.h"

KNMusicCategoryTabWidget::KNMusicCategoryTabWidget(QWidget *parent) :
    KNCategoryTabWidget(parent)
{
    ;
}

KNMusicCategoryTabWidget::~KNMusicCategoryTabWidget()
{
    ;
}

void KNMusicCategoryTabWidget::resizeEvent(QResizeEvent *event)
{
    //Do the resize first.
    KNCategoryTabWidget::resizeEvent(event);
    //Check the main player is null or not, resize the player.
    if(m_mainPlayer!=nullptr && m_mainPlayer->isVisible())
    {
        m_mainPlayer->resize(size());
    }
}

QWidget *KNMusicCategoryTabWidget::mainPlayer() const
{
    return m_mainPlayer;
}

void KNMusicCategoryTabWidget::setMainPlayer(QWidget *mainPlayer)
{
    //Save the player.
    m_mainPlayer = mainPlayer;
    //Change the parent relationship.
    m_mainPlayer->setParent(this);
}


