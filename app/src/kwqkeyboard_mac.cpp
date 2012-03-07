#include "kwqkeyboard.h"

#import <Foundation/Foundation.h>
#import <Carbon/Carbon.h>

KWQKeyboard::KWQKeyboard()
{
}


QString KWQKeyboard::currentLayout()
{
    TISInputSourceRef current = TISCopyCurrentKeyboardLayoutInputSource();
    NSString *currentName = (NSString *)TISGetInputSourceProperty(current, kTISPropertyLocalizedName);
    CFRelease(current);
    return [currentName UTF8String];
}

QStringList KWQKeyboard::availableLayouts()
{
    QStringList result;
    NSArray  *inputArray = (NSArray *)TISCreateInputSourceList ( NULL, false);
    NSUInteger i;
    NSString *currentName;
    for (i = 0; i < [inputArray count]; ++i) {
        TISInputSourceRef current = (TISInputSourceRef)[inputArray objectAtIndex:i];
        currentName = (NSString *)TISGetInputSourceProperty(current, kTISPropertyLocalizedName);
        result.append([currentName UTF8String]);
    }
    [inputArray release];
    return result;
}

void KWQKeyboard::selectLayout(const QString &layout)
{
    NSArray  *inputArray = (NSArray *)TISCreateInputSourceList ( NULL, false);
    NSUInteger i;
    NSString *currentName;
    for (i = 0; i < [inputArray count]; ++i) {
        TISInputSourceRef current = (TISInputSourceRef)[inputArray objectAtIndex:i];
        currentName = (NSString *)TISGetInputSourceProperty(current, kTISPropertyLocalizedName);
        if (layout == [currentName UTF8String])
            TISSelectInputSource (current);
    }
    [inputArray release];
}
