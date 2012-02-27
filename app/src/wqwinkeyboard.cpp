#include <QtCore/QStringList>
#include <QtCore/QSettings>

#include "wqwinkeyboard.h"

#ifdef Q_WS_WIN
#include <windows.h>
#define PRELOAD_PATH "HKEY_CURRENT_USER\\Keyboard Layout\\Preload"
#define LAYOUTS_PATH "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Keyboard Layouts"
#endif

WQWinKeyboard::WQWinKeyboard(QObject *parent) : QObject(parent)
{
}

QString WQWinKeyboard::currentLayout()
{
    return QString();
}

QStringList WQWinKeyboard::availableLayouts()
{
    QStringList result;
#ifdef Q_WS_WIN
    QSettings preload(PRELOAD_PATH, QSettings::NativeFormat);
    QSettings layouts(LAYOUTS_PATH, QSettings::NativeFormat);
    QStringList keys = preload.allKeys();
    foreach(const QString &key, keys) {
        QString nameKey = QString("%1/Layout Text").arg(preload.value(key).toString());
        result.append(layouts.value(nameKey).toString());
    }
#endif
    return result;
}

void WQWinKeyboard::selectLayout(const QString & layout)
{
#ifdef Q_WS_WIN
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
#else
    Q_UNUSED(layout);
#endif
}
