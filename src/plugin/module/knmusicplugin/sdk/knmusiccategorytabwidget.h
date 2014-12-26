#ifndef KNMUSICCATEGORYTABWIDGET_H
#define KNMUSICCATEGORYTABWIDGET_H

#include "kncategorytabwidget.h"

class KNMusicCategoryTabWidget : public KNCategoryTabWidget
{
public:
    explicit KNMusicCategoryTabWidget(QWidget *parent = 0);
    ~KNMusicCategoryTabWidget();
    QWidget *mainPlayer() const;
    void setMainPlayer(QWidget *mainPlayer);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QWidget *m_mainPlayer=nullptr;
};

#endif // KNMUSICCATEGORYTABWIDGET_H
