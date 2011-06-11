/***************************************************************************
                               main.cpp
                             -------------------

    begin                : Sat Apr 5 10:28:30 PST 2008
    copyright            : (C) 2008 by Peter Hedlund
    email                : peter.hedlund@kdemail.net

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QtCore/QUrl>
#include <QtCore/QFileInfo>
#include <QDebug>

#include "wqapplication.h"
#include "wordquiz.h"

int main(int argc, char ** argv)
{
  WQApplication app( argc, argv );

  if (app.isAlreadyRunning()){
    return 0;
  }

  WordQuizApp *win = new WordQuizApp();
  win->show();

  if (app.arguments().count() > 1) {
    QString fileToOpen = app.arguments().last();
    if (!fileToOpen.isEmpty()) {
      QFileInfo fi(fileToOpen);
      qDebug() << fileToOpen;
      if (fi.exists())
          win->openDocumentFile(QUrl::fromLocalFile(fileToOpen));
    }
  }

  return app.exec();
}
