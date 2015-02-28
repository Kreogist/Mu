#ifndef KNCATEGORYPREFERENCE_H
#define KNCATEGORYPREFERENCE_H

#include "knpreferenceitem.h"

#include <QObject>

class KNConfigure;
class KNPreferenceItemPanel;
class KNCategoryPreference : public QObject
{
    Q_OBJECT
public:
    explicit KNCategoryPreference(QObject *parent = 0);
    ~KNCategoryPreference();
    virtual KNPreferenceItemPanel *panel()=0;
    virtual KNConfigure *configure()=0;
    virtual void saveConfigures()=0;
    virtual void loadConfigures()=0;

signals:

public slots:
};

#endif // KNCATEGORYPREFERENCE_H
