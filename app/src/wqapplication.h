/***************************************************************************
                               wqapplication.h
                             -------------------

    begin                : Fri Apr 25 10:09:00 PDT 2008
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

#ifndef WQAPPLICATION_H
#define WQAPPLICATION_H

#include <QtGui/QApplication>
#include <QtGui/QActionGroup>

#define wqApp (static_cast<WQApplication*>(QCoreApplication::instance()))

class WQApplication : public QApplication
{
Q_OBJECT
public:
  WQApplication( int & argc, char ** argv );

#ifdef Q_WS_WIN
  virtual bool winEventFilter(MSG * msg, long * result);
#endif

#ifdef Q_WS_MAC
  virtual bool macEventFilter(EventHandlerCallRef caller, EventRef event);
#endif

  bool isAlreadyRunning();
  void updateRecentFileActions();

public slots:
  void slotLastWindowClosed();
  void slotPreferences();
  void slotAbout();
  void slotNew();
  void slotOpen();
  void slotFileOpenRecent(QAction *);

  void slotHelpHandbook();
  void slotHelpWhatsThis();

  void slotUpgradeToKWordQuiz();

signals:
  void dockActivated();

protected:
  bool event(QEvent *ev);

private:
  QActionGroup *m_recentFilesActionGroup;
};
#endif
