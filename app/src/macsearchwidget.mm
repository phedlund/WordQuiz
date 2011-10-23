#include "macsearchwidget.h"

#ifdef Q_WS_MAC
#import <Cocoa/Cocoa.h>

void emit_text_changed(MacSearchWidget *field)
{
    emit field->textChanged(field->text());
}

void emit_text_accepted(MacSearchWidget *field)
{
    emit field->textAccepted();
}

@interface MacSearchField : NSSearchField {
    MacSearchWidget *searchField;
}

- (id)initWithSearchField:(MacSearchWidget *)searchField;
- (void)textDidChange:(NSNotification *)aNotification;
- (void)textDidEndEditing:(NSNotification *)aNotification;

@end

@implementation MacSearchField

- (id)initWithSearchField:(MacSearchWidget *)sf {
    self = [super init];
    if (self) {
        searchField = sf;
    }
    return self;
}

- (void)textDidChange:(NSNotification *)aNotification {
    emit_text_changed(searchField);
    [super textDidChange:aNotification];
}

- (void)textDidEndEditing:(NSNotification *)aNotification {
    emit_text_accepted(searchField);
    [super textDidEndEditing:aNotification];
}

@end

MacSearchWidget::MacSearchWidget(QWidget *parent) : QMacCocoaViewContainer(0, parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    //NSAutoreleasePool* autoreleasePool = [[NSAutoreleasePool alloc] init];

    searchField = [[MacSearchField alloc] initWithSearchField:this];
    setCocoaView(searchField);

    [[searchField cell] setSendsSearchStringImmediately:true];

    [searchField release];
    //[autoreleasePool release];
}

QSize MacSearchWidget::sizeHint() const
{
    NSRect rect = [searchField bounds];
    return QSize(200, rect.size.height);
}

QSize MacSearchWidget::minimumSizeHint() const
{
    return QSize(200, sizeHint().height());
}

QString MacSearchWidget::text() const
{
    return [[searchField stringValue] UTF8String];
}

void MacSearchWidget::setText(const QString &newText)
{
    if (text() == newText)
        return;

    [searchField setStringValue:[NSString stringWithUTF8String:newText.toUtf8()]];
    emit textChanged(newText);
}

void MacSearchWidget::clear()
{
    setText(QString());
}

#endif
