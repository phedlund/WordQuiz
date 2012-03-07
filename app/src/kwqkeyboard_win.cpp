#include "kwqkeyboard.h"

#include <QtCore/QSettings>

#include <windows.h>
#define PRELOAD_PATH "HKEY_CURRENT_USER\\Keyboard Layout\\Preload"
#define LAYOUTS_PATH "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Keyboard Layouts"

KWQKeyboard::KWQKeyboard()
{
}


QString KWQKeyboard::currentLayout()
{
    return QString();
}

QStringList KWQKeyboard::availableLayouts()
{
    QStringList result;
    QSettings preload(PRELOAD_PATH, QSettings::NativeFormat);
    QSettings layouts(LAYOUTS_PATH, QSettings::NativeFormat);
    QStringList keys = preload.allKeys();
    foreach(const QString &key, keys) {
        QString nameKey = QString("%1/Layout Text").arg(preload.value(key).toString());
        result.append(layouts.value(nameKey).toString());
    }
    return result;
}

void KWQKeyboard::selectLayout(const QString &layout)
{
    QSettings preload(PRELOAD_PATH, QSettings::NativeFormat);
    QSettings layouts(LAYOUTS_PATH, QSettings::NativeFormat);
    QStringList keys = preload.allKeys();
    foreach(const QString &key, keys) {
        QString nameKey = QString("%1/Layout Text").arg(preload.value(key).toString());
        if (layouts.value(nameKey).toString() == layout) {
            QString str = preload.value(key).toString();
            wchar_t *we = new wchar_t[str.size()+1];
            str.toWCharArray(we);
            we[str.size()] = 0;
            LoadKeyboardLayout(we, KLF_ACTIVATE | KLF_SUBSTITUTE_OK);
            delete[] we;
            continue;
        }
    }
}
