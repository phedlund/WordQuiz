#ifndef WQMACKEYBOARD_H
#define WQMACKEYBOARD_H

#include <QtCore/QObject>
#include <QtCore/QString>

class WQMacKeyboard : public QObject
{
    Q_OBJECT
public:
    explicit WQMacKeyboard(QObject *parent = 0);

    static QString currentLayout();
    static QStringList availableLayouts();
    static void selectLayout(const QString &);
};

#endif // WQMACKEYBOARD_H
