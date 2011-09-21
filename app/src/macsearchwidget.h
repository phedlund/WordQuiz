#ifndef MACSEARCHWIDGET_H
#define MACSEARCHWIDGET_H

#include <QtGui/QMacCocoaViewContainer>

#ifdef Q_WS_MAC
#import <qmaccocoaviewcontainer_mac.h>
#ifdef __OBJC__
    #define ADD_COCOA_NATIVE_REF(CocoaClass) \
        @class CocoaClass; \
        typedef CocoaClass* Native##CocoaClass##Ref
#else
    #define ADD_COCOA_NATIVE_REF(CocoaClass) typedef void* Native##CocoaClass##Ref
#endif
ADD_COCOA_NATIVE_REF(NSSearchField);

class MacSearchWidget : public QMacCocoaViewContainer
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)
public:
    MacSearchWidget(QWidget *parent = 0);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    QString text() const;

public slots:
    void setText(const QString &text);
    void clear();

signals:
    void textChanged(const QString &text);
    void textAccepted();

private:
    NativeNSSearchFieldRef searchField;

    friend void emit_text_changed(MacSearchWidget *);
    friend void emit_text_accepted(MacSearchWidget *);
};

#endif
#endif // MACSEARCHWIDGET_H
