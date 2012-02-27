#ifndef WQWINKEYBOARD_H
#define WQWINKEYBOARD_H

#include <QtCore/QObject>
#include <QtCore/QString>

class WQWinKeyboard : public QObject
{
    Q_OBJECT
public:
    explicit WQWinKeyboard(QObject *parent = 0);

    static QString currentLayout();
    static QStringList availableLayouts();
    static void selectLayout(const QString &);
};

#endif // WQWINKEYBOARD_H
