#ifndef KWQKEYBOARD_H
#define KWQKEYBOARD_H

#include <QtCore/QString>
#include <QtCore/QStringList>

class KWQKeyboard
{
public:
    explicit KWQKeyboard();

    static QString currentLayout();
    static QStringList availableLayouts();
    static void selectLayout(const QString &);
};

#endif // KWQKEYBOARD_H
