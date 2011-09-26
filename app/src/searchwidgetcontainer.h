#ifndef SEARCHWIDGETCONTAINER_H
#define SEARCHWIDGETCONTAINER_H

#include <QtGui/QWidget>

class Spacer : public QWidget
{
    Q_OBJECT
public:
    Spacer(QWidget *parent = 0);
    QSize sizeHint() const;
};

#ifdef Q_WS_MAC
#include "macsearchwidget.h"
#else
#include "searchlineedit.h"
#endif

class SearchWrapper : public QWidget
{
    Q_OBJECT
public:
    SearchWrapper(QWidget *parent = 0);
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

#ifdef Q_WS_MAC
    MacSearchWidget *searchBox() {return m_searchBox;}
private:
    MacSearchWidget *m_searchBox;
#else
    SearchLineEdit *searchBox() {return m_searchBox;}
private:
    SearchLineEdit *m_searchBox;
#endif //Q_WS_MAC
};

#endif //SEARCHWIDGETCONTAINER_H
