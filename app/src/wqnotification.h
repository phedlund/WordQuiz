#ifndef WQNOTIFICATION_H
#define WQNOTIFICATION_H

#include <QObject>

class WQNotification : public QObject
{
    Q_OBJECT
public:
    explicit WQNotification(QObject *parent = 0);

    static void setup();
    static void event(const QString &eventId, const QString& text=QString());

    static QStringList NotificationFileNames;
    static QStringList NotificationResources;
};

#endif // WQNOTIFICATION_H
