#ifndef KNABSTRACTCATEGORYBUTTON_H
#define KNABSTRACTCATEGORYBUTTON_H

#include "knanimecheckedbutton.h"

class KNAbstractCategoryButton : public KNAnimeCheckedButton
{
    Q_OBJECT
public:
    explicit KNAbstractCategoryButton(QWidget *parent = 0);
    QSize sizeHint() const;
    QPixmap icon() const;
    QString text() const;
    void setIcon(const QPixmap &icon);
    void setText(const QString &text);

signals:

public slots:
    virtual void startInAnime()=0;
    virtual void startOutAnime()=0;

protected:
    void paintEvent(QPaintEvent *event);

private:
    int m_iconSize=64,
        m_margin=10,
        m_spacing=10;
    QPixmap m_icon;
    QString m_text;
};

#endif // KNABSTRACTCATEGORYBUTTON_H
