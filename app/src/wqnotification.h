#ifndef WQNOTIFICATION_H
#define WQNOTIFICATION_H

#include <QObject>
#include <Phonon/MediaSource>

class WQNotification : public QObject
{
    Q_OBJECT
public:
    explicit WQNotification(QObject *parent = 0);

    static void setup();
    static Phonon::MediaSource source(const QString &eventId, const QString& text=QString());

    static QStringList NotificationFileNames;
    static QStringList NotificationResources;
};

#endif // WQNOTIFICATION_H
