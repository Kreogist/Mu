#ifndef KNMAINWINDOWPLUGIN_H
#define KNMAINWINDOWPLUGIN_H

#include <QObject>

class KNMainWindowPlugin : public QObject
{
    Q_OBJECT
public:
    virtual void setHeaderWidget(QWidget *widget)=0;
    virtual void setCategoryStack(QWidget *widget)=0;
    virtual void setCategorySwitcher(QWidget *widget)=0;
    virtual void setPreferencePanel(QWidget *widget)=0;

signals:

public slots:
    virtual void showPreference()=0;
    virtual void hidePreference()=0;

private:
};

#endif // KNMAINWINDOWPLUGIN_H
