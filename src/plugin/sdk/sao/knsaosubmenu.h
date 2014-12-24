#ifndef KNSAOSUBMENU_H
#define KNSAOSUBMENU_H

#include <QMenu>

class QLabel;
class QPropertyAnimation;
class KNSAOSubMenu : public QMenu
{
    Q_OBJECT
public:
    explicit KNSAOSubMenu(QWidget *parent = 0);
    ~KNSAOSubMenu();

signals:

public slots:

protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private:
    QPropertyAnimation *m_start;
    QWidget *m_indicator;
};

#endif // KNSAOSUBMENU_H
