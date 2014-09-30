#include "knpreferencetitle.h"

KNPreferenceTitle::KNPreferenceTitle(QWidget *parent) :
    QLabel(parent)
{
    //Set content margin.
    setContentsMargins(30,0,0,0);
    //Set font.
    QFont titleFont=font();
    titleFont.setPixelSize(35);
    setFont(titleFont);
    //Set text.
    retranslate();
}

void KNPreferenceTitle::retranslate()
{
    setText(tr("Preference"));
}
