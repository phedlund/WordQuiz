#include "kwqkeyboard.h"

#include <QtGui/QX11Info>

//Must come after Qt includes
#include <X11/XKBlib.h>

#define NON_SYMBOLS "group+inet+ctr+pc+ctrl+terminate"

KWQKeyboard::KWQKeyboard()
{
}


QString KWQKeyboard::currentLayout()
{
    return QString();
    /*
    XkbStateRec xkbState;
    XkbGetState(QX11Info::display(), XkbUseCoreKbd, &xkbState);
    return static_cast<int>(xkbState.group);
    */
}

QStringList KWQKeyboard::availableLayouts()
{
    QStringList result;

    XkbIgnoreExtension(false);

    XkbDescRec* kbdDescPtr = XkbAllocKeyboard();
    if (kbdDescPtr != NULL) {
        kbdDescPtr->dpy = QX11Info::display();
        XkbGetControls(QX11Info::display(), XkbAllControlsMask, kbdDescPtr);
        XkbGetNames(QX11Info::display(), XkbSymbolsNameMask, kbdDescPtr);

        Atom symbolNamesAtom = kbdDescPtr->names->symbols;
        if (symbolNamesAtom != None) {
            char* symbolNamesC = XGetAtomName(QX11Info::display(), symbolNamesAtom);
            QString symbolNames(symbolNamesC);
            XFree(symbolNamesC);

            if (!symbolNames.isEmpty()) {
                QStringList sl = symbolNames.split('+');
                foreach (const QString &symbol, sl) {
                    bool keep = true;
                    foreach (const QString &ns, QString(NON_SYMBOLS).split('+')) {
                        if (symbol.contains(ns)) {
                            keep = false;
                            continue;
                        }
                    }
                    if (keep) {
                        result.append(symbol.left(symbol.indexOf(':')));
                    }
                }

            }
        }

        XkbFreeKeyboard(kbdDescPtr, 0, true);
    }
    return result;
}

void KWQKeyboard::selectLayout(const QString &layout)
{
    int group = KWQKeyboard::availableLayouts().indexOf(layout);
    if (group == -1)
        group = 0;
    XkbLockGroup(QX11Info::display(), XkbUseCoreKbd, group);
}
