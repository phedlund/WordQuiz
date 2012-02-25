#include <QDebug>
#include <QtCore/QStringList>

#include "wqmackeyboard.h"

#ifdef Q_WS_MAC
#import <Foundation/Foundation.h>
#import <Carbon/Carbon.h>
#endif

WQMacKeyboard::WQMacKeyboard(QObject *parent) : QObject(parent)
{
}

QString WQMacKeyboard::currentLayout()
{
#ifdef Q_WS_MAC
    TISInputSourceRef current = TISCopyCurrentKeyboardLayoutInputSource();
    NSString *currentName = (NSString *)TISGetInputSourceProperty(current, kTISPropertyLocalizedName);
    CFRelease(current);
    return [currentName UTF8String];
#else
    return QString();
#endif
}

QStringList WQMacKeyboard::availableLayouts()
{
    QStringList result;
#ifdef Q_WS_MAC
    NSArray  *inputArray = (NSArray *)TISCreateInputSourceList ( NULL, false);
    NSUInteger i;
    NSString *currentName;
    for (i = 0; i < [inputArray count]; ++i) {
        TISInputSourceRef current = (TISInputSourceRef)[inputArray objectAtIndex:i];
        currentName = (NSString *)TISGetInputSourceProperty(current, kTISPropertyLocalizedName);
        qDebug() << [currentName UTF8String];
        result.append([currentName UTF8String]);
    }
    [inputArray release];
#endif
    return result;
}

void WQMacKeyboard::selectLayout(const QString & layout)
{
#ifdef Q_WS_MAC
    NSArray  *inputArray = (NSArray *)TISCreateInputSourceList ( NULL, false);
    NSUInteger i;
    NSString *currentName;
    for (i = 0; i < [inputArray count]; ++i) {
        TISInputSourceRef current = (TISInputSourceRef)[inputArray objectAtIndex:i];
        currentName = (NSString *)TISGetInputSourceProperty(current, kTISPropertyLocalizedName);
        qDebug() << [currentName UTF8String];
        if (layout == [currentName UTF8String])
            TISSelectInputSource (current);
        qDebug() << currentLayout();
    }
    [inputArray release];
#else
    Q_UNUSED(layout);
#endif
}
