#include <QBoxLayout>

#include "knpreferencecategory.h"
#include "knpreferencecontents.h"

#include "knpreferencepanel.h"

KNPreferencePanel::KNPreferencePanel(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setAutoFillBackground(true);

    //Initial layout.
    m_layout=new QBoxLayout(QBoxLayout::LeftToRight,
                            this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);

    //Initial widgets.
    m_categoryList=new KNPreferenceCategory(this);
    m_layout->addWidget(m_categoryList, 1);
    m_contents=new KNPreferenceContents(this);
    m_layout->addWidget(m_contents, 1);
}
