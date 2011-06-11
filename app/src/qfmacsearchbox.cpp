/**/
#include "qfmacsearchbox.h"

#include <QVarLengthArray>
#ifdef Q_WS_MAC
/*
static QString toQString(CFStringRef str)
{
    if(!str)
        return QString();

    CFIndex length = CFStringGetLength(str);
    const UniChar *chars = CFStringGetCharactersPtr(str);
    if (chars)
        return QString(reinterpret_cast<const QChar *>(chars), length);

    QVarLengthArray<UniChar> buffer(length);
    CFStringGetCharacters(str, CFRangeMake(0, length), buffer.data());
    return QString(reinterpret_cast<const QChar *>(buffer.constData()), length);
}

static OSStatus SearchFieldEventHandler(EventHandlerCallRef handlerCallRef, EventRef event, void *userData)
{
    Q_UNUSED(handlerCallRef);
    QfMacSearchBox *searchBox = (QfMacSearchBox *) userData;
    OSType eventClass = GetEventClass(event);
    UInt32 eventKind = GetEventKind(event);

    if (eventClass == kEventClassSearchField) {
        switch (eventKind) {
            case kEventSearchFieldCancelClicked:
                searchBox->clear();
                break;
            case kEventSearchFieldSearchClicked:
                searchBox->raiseFindNext();
                break;
            default:
                break;
        }
    } else if (eventClass == kEventClassTextField) {
        switch (eventKind) {
            case kEventTextDidChange:
                searchBox->raiseTextChanged();
                break;
            case kEventTextAccepted:
                searchBox->raiseFindNext();
                break;
            default:
                break;
        }
    }

    return(eventNotHandledErr);
}

// =======================================
//  MacSearchBox: PUBLIC Constructors/Destructors
// =======================================
QfMacSearchBox::QfMacSearchBox (QWidget *parent) : QWidget(parent)
{
    // Set Widget Properties
    //setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    // Create a native search field and pass its window id to QWidget::create.
    searchFieldText = CFStringCreateWithCString(0, (const char *) tr("Search").toAscii(), 0);
    HISearchFieldCreate(NULL, kHISearchFieldAttributesSearchIcon |
                              kHISearchFieldAttributesCancel,
                        NULL, searchFieldText, &searchField);
    create(reinterpret_cast<WId>(searchField));

    // Subscribe Events
    EventTypeSpec mySFieldEvents[] = {
                     { kEventClassSearchField, kEventSearchFieldCancelClicked },
                     { kEventClassTextField, kEventTextDidChange },
                     { kEventClassTextField, kEventTextAccepted }
                 };

    HIViewInstallEventHandler(searchField, SearchFieldEventHandler,
                              GetEventTypeCount(mySFieldEvents), mySFieldEvents,
                              (void *) this, NULL);


}

QfMacSearchBox::~QfMacSearchBox()
{
    CFRelease(searchField);
    CFRelease(searchFieldText);
}

void QfMacSearchBox::setEnabled(bool enabled)
{
    HIViewSetEnabled(searchField, enabled);
    QWidget::setEnabled(enabled);
}

void QfMacSearchBox::raiseTextChanged (void)
{
    emit textChanged(text());
}

void QfMacSearchBox::raiseFindNext (void)
{
    emit findNext();
}

QSize QfMacSearchBox::sizeHint (void) const
{
    HIRect optimalBounds;
    EventRef event;

    CreateEvent(0, kEventClassControl, kEventControlGetOptimalBounds, GetCurrentEventTime(), kEventAttributeUserEvent, &event);

    SendEventToEventTargetWithOptions(event, HIObjectGetEventTarget(HIObjectRef(winId())), kEventTargetDontPropagate);

    GetEventParameter(event, kEventParamControlOptimalBounds, typeHIRect, 0, sizeof(HIRect), 0, &optimalBounds);

    ReleaseEvent(event);
    return QSize(optimalBounds.size.width + 200, optimalBounds.size.height);
}


QString QfMacSearchBox::text (void) const
{
    CFStringRef cfString = HIViewCopyText(searchField);
    QString text = toQString(cfString);
    CFRelease(cfString);
    return(text);
}


void QfMacSearchBox::setText (const QString& text)
{
    CFRelease(searchFieldText);

    searchFieldText = CFStringCreateWithCString(0, (const char *) text.toAscii(), 0);
    HIViewSetText(searchField, searchFieldText);

    emit textChanged(text);
}

void QfMacSearchBox::clear (void)
{
    setText(QString());
}
*/
Spacer::Spacer(QWidget *parent) : QWidget(parent)
{
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setSizePolicy(sizePolicy);
}

QSize Spacer::sizeHint() const
{
    return QSize(1, 1);
}


SearchWrapper::SearchWrapper(QWidget *parent) : QWidget(parent)
{
    //m_searchBox = new QfMacSearchBox(this);
    m_searchBox = new SearchLineEdit(this);
    m_searchBox->move(2,2);
    setFocusProxy(m_searchBox);
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
}

QSize SearchWrapper::sizeHint() const
{
    return m_searchBox->sizeHint() + QSize(12, 2);
}

#endif
