#ifndef KNPREFERENCECATEGORY_H
#define KNPREFERENCECATEGORY_H

#include <QWidget>

class QBoxLayout;
class QTimeLine;
class KNPreferenceTitle;
class KNPreferenceCategory : public QWidget
{
    Q_OBJECT
public:
    explicit KNPreferenceCategory(QWidget *parent = 0);

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

protected:
    void paintEvent(QPaintEvent *event);

private:
    QColor m_lineColor=QColor(39,39,39),
           m_backgroundColor=QColor(51,51,51),
           m_borderColor=QColor(67,67,67);
    int m_listWidth=241,
        m_borderWidth=4;
    QBoxLayout *m_layout;
    QString m_configureText;
    KNPreferenceTitle *m_title;
};

#endif // KNPREFERENCECATEGORY_H
