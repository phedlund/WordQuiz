#include "wqnotification.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QDir>
#include <Phonon/MediaObject>

QStringList WQNotification::NotificationResources = QStringList() << ":/src/sounds/correct.wav"
                                                                  << ":/src/sounds/error.wav"
                                                                  << ":/src/sounds/syntax_error.wav";

QStringList WQNotification::NotificationFileNames = QStringList() << QString("/correct.wav").prepend(QDir::tempPath())
                                                                  << QString("/error.wav").prepend(QDir::tempPath())
                                                                  << QString("/syntax_error.wav").prepend(QDir::tempPath());

WQNotification::WQNotification(QObject *parent) : QObject(parent)
{

}

void WQNotification::setup()
{
    int i = 0;
    foreach(const QString &res, WQNotification::NotificationResources) {
        QString fileName = WQNotification::NotificationFileNames[i];
        QFile file(fileName);
        if (file.open(QIODevice::ReadWrite))
        {
          QFile workFile(res);
          if(workFile.open(QIODevice::ReadOnly))
          {
            file.write(workFile.readAll());
            workFile.close();
          }
          file.close();
        }
        ++i;
    }
}

Phonon::MediaSource WQNotification::source(const QString &eventId, const QString& text)
{
    Q_UNUSED(text);
    QString fileName;
    QStringList sl = WQNotification::NotificationFileNames;

    if (eventId == "QuizCorrect")
        fileName = sl[0];
    if (eventId == "QuizError")
        fileName = sl[1];
    if (eventId == "SyntaxError")
        fileName = sl[2];

    return Phonon::MediaSource(fileName);
}
