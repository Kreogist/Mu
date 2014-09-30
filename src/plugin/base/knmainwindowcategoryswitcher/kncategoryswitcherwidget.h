#ifndef KNCATEGORYSWITCHERWIDGET_H
#define KNCATEGORYSWITCHERWIDGET_H

#include <QWidget>

class QTimeLine;
class KNCategorySettingButton;
class KNCategorySwitcherWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KNCategorySwitcherWidget(QWidget *parent = 0);

signals:
    void requireResetHeaderButton();
    void requireShowPreference();

public slots:
    void showSwitcher();
    void hideSwitcher();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void onActionInOutBackground(const int &frame);
    void onActionHideComplete();

private:
    void initialTimeLine(QTimeLine *timeline);
    void initialButtons();
    void startButtonInAnime();
    void startButtonOutAnime();
    void stopAllAnime();
    QColor m_backgroundColor=QColor(0,0,0,0);
    QTimeLine *m_inBackground, *m_outBackground;
    QPalette m_palette;
    bool m_pressed=false;

    KNCategorySettingButton *m_settingButton;
};

#endif // KNCATEGORYSWITCHERWIDGET_H
