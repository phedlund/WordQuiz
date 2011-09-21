#ifndef MACSEARCHWIDGETCONTAINER_H
#define MACSEARCHWIDGETCONTAINER_H

#include <QtGui/QWidget>

#ifdef Q_WS_MAC

#include "macsearchwidget.h"

class Spacer : public QWidget
{
    Q_OBJECT
public:
    Spacer(QWidget *parent = 0);
    QSize sizeHint() const;
};

class SearchWrapper : public QWidget
{
    Q_OBJECT
public:
    SearchWrapper(QWidget *parent = 0);
    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    MacSearchWidget *searchBox() {return m_searchBox;}

private:
    MacSearchWidget *m_searchBox;
};

#endif //Q_WS_MAC
#endif //MACSEARCHWIDGETCONTAINER_H
