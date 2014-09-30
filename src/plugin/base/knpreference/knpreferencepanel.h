#ifndef KNPREFERENCEPANEL_H
#define KNPREFERENCEPANEL_H

#include <QWidget>

class QBoxLayout;
class KNPreferenceContents;
class KNPreferenceCategory;
class KNPreferencePanel : public QWidget
{
    Q_OBJECT
public:
    explicit KNPreferencePanel(QWidget *parent = 0);

signals:

public slots:

private:
    QBoxLayout *m_layout;
    KNPreferenceCategory *m_categoryList;
    KNPreferenceContents *m_contents;
};

#endif // KNPREFERENCEPANEL_H
