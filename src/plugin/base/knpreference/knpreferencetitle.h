#ifndef KNPREFERENCETITLE_H
#define KNPREFERENCETITLE_H

#include <QLabel>

class KNPreferenceTitle : public QLabel
{
    Q_OBJECT
public:
    explicit KNPreferenceTitle(QWidget *parent = 0);

signals:

public slots:
    void retranslate();

private:
    QLabel *m_title;
};

#endif // KNPREFERENCETITLE_H
