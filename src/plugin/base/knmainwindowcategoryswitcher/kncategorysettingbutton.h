#ifndef KNCATEGORYSETTINGBUTTON_H
#define KNCATEGORYSETTINGBUTTON_H

#include "knabstractcategorybutton.h"

class QPropertyAnimation;
class KNCategorySettingButton : public KNAbstractCategoryButton
{
    Q_OBJECT
public:
    explicit KNCategorySettingButton(QWidget *parent = 0);

signals:

public slots:
    void startInAnime();
    void startOutAnime();

protected:
    void startMouseInAnime();
    void startMouseOutAnime();
    void startMouseDownAnime();
    void startMouseUpAnime();

private:
    void initialGeometryAnime(QPropertyAnimation *anime);
    void stopSlideAnime();
    QRect generateOut();
    QRect generateIn();
    QPropertyAnimation *m_slideIn, *m_slideOut;
};

#endif // KNCATEGORYSETTINGBUTTON_H
