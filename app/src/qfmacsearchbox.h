#include <QtGlobal>


#if defined(Q_WS_MAC) && !defined(QFMACSEARCHBOX_H)
#define QFMACSEARCHBOX_H

//#ifndef QFMACSEARCHBOX_H
//#define QFMACSEARCHBOX_H

#include <QWidget>

//#ifdef Q_WS_MAC
// Mac OS X - Carbon Headers
//#include <QMacCocoaViewContainer>
//#include <Cocoa/Cocoa.h>
//#include <Carbon/Carbon.h>
#include "searchlineedit.h"
/*
class QfMacSearchBox : public QWidget {
    Q_OBJECT

    public:
        QfMacSearchBox (QWidget *parent = 0);
        ~QfMacSearchBox();

        void setEnabled(bool );

        // Methods
        void raiseTextChanged (void);
        void raiseFindNext (void);

        QSize sizeHint (void) const;

        // GET Properties
        QString text (void) const;

    signals:
        void textChanged (const QString& text);
        void findNext (void);

    public slots:
        void setText (const QString& text);
        void clear (void);

    private:
        CFStringRef searchFieldText;
        HIViewRef searchField;
};
*/
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
    //QfMacSearchBox *searchBox() {return m_searchBox;}
    SearchLineEdit *searchBox() {return m_searchBox;}
private:
    SearchLineEdit *m_searchBox;
    //QfMacSearchBox *m_searchBox;
};

//#endif //Q_WS_MAC
#endif // QFMACSEARCHBOX_H
