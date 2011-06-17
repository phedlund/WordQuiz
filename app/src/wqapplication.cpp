/***************************************************************************
                               wqapplication.cpp
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

#include "wqapplication.h"
 #include <stdio.h>
 #include <stdlib.h>
 
#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtCore/QFileInfo>
#include <QtCore/QUrl>
#include <QtGui/QFileOpenEvent>
#include <QtGui/QMessageBox>
#include <QtGui/QMenuBar>
#include <QtGui/QFileDialog>
#include <QtGui/QWhatsThis>
#include <QtGui/QDesktopServices>

#include "wordquiz.h"
#include "wordquizprefs.h"
#include "prefs.h"

#ifdef Q_WS_WIN
  #include <windows.h>
#endif
#ifdef Q_WS_MAC
  #include <Carbon/Carbon.h>
#endif

WQApplication::WQApplication(int & argc, char ** argv) : QApplication(argc, argv)
{
  QCoreApplication::setOrganizationName("PHSoftware");
  QCoreApplication::setOrganizationDomain("peterandlinda.com");
  QCoreApplication::setApplicationName("WordQuiz");

#ifdef Q_WS_MAC
  setQuitOnLastWindowClosed(false);

  QMenuBar *appMenu = new QMenuBar(0);
  QMenu *fileMenu = appMenu->addMenu(tr("&File"));
  QMenu *editMenu = appMenu->addMenu(tr("&Edit"));
  QMenu *vocabularyMenu = appMenu->addMenu(tr("&Vocabulary"));
  QMenu *modeMenu = appMenu->addMenu(tr("&Mode"));
  QMenu *quizMenu = appMenu->addMenu(tr("&Quiz"));
  QMenu *settingsMenu = appMenu->addMenu(tr("&Settings"));
  QMenu *helpMenu = appMenu->addMenu(tr("&Help"));

  QAction *a = fileMenu->addAction("", this, SLOT(slotAbout()));
  a->setMenuRole(QAction::AboutRole);

  a = fileMenu->addAction("", this, SLOT(aboutQt()));
  a->setMenuRole(QAction::AboutQtRole);

  a = fileMenu->addAction("", this, SLOT(slotPreferences()));
  a->setMenuRole(QAction::PreferencesRole);

  fileMenu->addAction(QIcon(), tr("&New"), this, SLOT(slotNew()), QKeySequence(QKeySequence::New));
  fileMenu->addAction(QIcon(), tr("&Open..."), this, SLOT(slotOpen()), QKeySequence(QKeySequence::Open));
  a = fileMenu->addAction(QIcon(), tr("&Open Recent"), this, 0, 0);

  m_recentFilesActionGroup = new QActionGroup(this);
  connect(m_recentFilesActionGroup, SIGNAL(triggered(QAction *)), this, SLOT(slotFileOpenRecent(QAction *)));
  for (int i = 0; i <=9; i++) {
    QAction *ar = new QAction(this);
    ar->setVisible(false);
    m_recentFilesActionGroup->addAction(ar);
  }
  QMenu *m = new QMenu;
  m->addActions(m_recentFilesActionGroup->actions());
  a->setMenu(m);

  a = fileMenu->addAction(QIcon(), tr("&Download New Vocabularies..."), this, SLOT(slotUpgradeToKWordQuiz()), QKeySequence("CTRL+G"));
  a->setVisible(false);
  fileMenu->addSeparator();
  a = fileMenu->addAction(QIcon(), tr("&Save"), this, 0, QKeySequence(QKeySequence::Save));
  a->setEnabled(false);
  a = fileMenu->addAction(QIcon(), tr("Save &As..."), this, 0, 0);
  a->setEnabled(false);
  fileMenu->addSeparator();
  a = fileMenu->addAction(QIcon(), tr("&Print"), this, 0, QKeySequence(QKeySequence::Print));
  a->setEnabled(false);
  fileMenu->addSeparator();
  a = fileMenu->addAction(QIcon(), tr("&Close"), this, 0, QKeySequence(QKeySequence::Close));
  a->setEnabled(false);

  a = editMenu->addAction(QIcon(), tr("&Undo"), this, 0, QKeySequence(QKeySequence::Undo));
  a->setEnabled(false);
  a = editMenu->addAction(QIcon(), tr("&Redo"), this, 0, QKeySequence(QKeySequence::Redo));
  a->setEnabled(false);
  editMenu->addSeparator();
  a = editMenu->addAction(QIcon(), tr("&Cut"), this, 0, QKeySequence(QKeySequence::Cut));
  a->setEnabled(false);
  a = editMenu->addAction(QIcon(), tr("&Copy"), this, 0, QKeySequence(QKeySequence::Copy));
  a->setEnabled(false);
  a = editMenu->addAction(QIcon(), tr("&Paste"), this, 0, QKeySequence(QKeySequence::Paste));
  a->setEnabled(false);
  a = editMenu->addAction(QIcon(), tr("C&lear"), this, 0, QKeySequence(QKeySequence::Delete));
  a->setEnabled(false);
  editMenu->addSeparator();
  a = editMenu->addAction(QIcon(), tr("&Insert Row"), this, 0, QKeySequence("CTRL+I"));
  a->setEnabled(false);
  a = editMenu->addAction(QIcon(), tr("&Delete Row"), this, 0, QKeySequence("CTRL+K"));
  a->setEnabled(false);
  editMenu->addSeparator();
  a = editMenu->addAction(QIcon(), tr("&Mark Blank"), this, 0, QKeySequence("CTRL+M"));
  a->setEnabled(false);
  a = editMenu->addAction(QIcon(), tr("&Unmark Blank"), this, 0, 0);
  a->setEnabled(false);

  a = vocabularyMenu->addAction(QIcon(), tr("&Column Titles..."), this, 0, QKeySequence("CTRL+L"));
  a->setEnabled(false);
  a = vocabularyMenu->addAction(QIcon(), tr("&Font..."), this, 0, 0);
  a->setEnabled(false);
  vocabularyMenu->addSeparator();
  a = vocabularyMenu->addAction(QIcon(), tr("Link &Image..."), this, 0, 0);
  a->setEnabled(false);
  a = vocabularyMenu->addAction(QIcon(), tr("Link &Sound..."), this, 0, 0);
  a->setEnabled(false);
  vocabularyMenu->addSeparator();
  a = vocabularyMenu->addAction(QIcon(), tr("&Rows/Columns..."), this, 0, 0);
  a->setEnabled(false);
  a = vocabularyMenu->addAction(QIcon(), tr("Adjust Row Heights"), this, 0, 0);
  a->setEnabled(false);
  vocabularyMenu->addSeparator();
  a = vocabularyMenu->addAction(QIcon(), tr("&Shuffle"), this, 0, 0);
  a->setEnabled(false);

  a = modeMenu->addAction(QIcon(), tr("&1 Column 1 -> Column 2 In Order"), this, 0, 0);
  a->setEnabled(false);
  a = modeMenu->addAction(QIcon(), tr("&2 Column 2 -> Column 1 In Order"), this, 0, 0);
  a->setEnabled(false);
  a = modeMenu->addAction(QIcon(), tr("&3 Column 1 -> Column 2 Randomly"), this, 0, 0);
  a->setEnabled(false);
  a = modeMenu->addAction(QIcon(), tr("&4 Column 2 -> Column 1 Randomly"), this, 0, 0);
  a->setEnabled(false);
  a = modeMenu->addAction(QIcon(), tr("&5 Column 1 <-> Column 2 Randomly"), this, 0, 0);
  a->setEnabled(false);

  a = quizMenu->addAction(QIcon(), tr("&Check"), this, 0, QKeySequence(Qt::Key_Return));
  a->setEnabled(false);
  quizMenu->addSeparator();
  a = quizMenu->addAction(QIcon(), tr("I &Know"), this, 0, QKeySequence(Qt::Key_K));
  a->setEnabled(false);
  a = quizMenu->addAction(QIcon(), tr("I &Do Not Know"), this, 0,QKeySequence(Qt::Key_D));
  a->setEnabled(false);
  quizMenu->addSeparator();
  a = quizMenu->addAction(QIcon(), tr("&Hint"), this, 0, QKeySequence("CTRL+H"));
  a->setEnabled(false);
  a = quizMenu->addAction(QIcon(), tr("&Play Audio"), this, 0, QKeySequence("CTRL+B"));
  a->setEnabled(false);
  quizMenu->addSeparator();
  a = quizMenu->addAction(QIcon(), tr("&Restart"), this, 0, QKeySequence("CTRL+R"));
  a->setEnabled(false);
  a = quizMenu->addAction(QIcon(), tr("Repeat &Errors"), this, 0, QKeySequence("CTRL+E"));
  a->setEnabled(false);
  quizMenu->addSeparator();
  a = quizMenu->addAction(QIcon(), tr("&Export Errors As..."), this, 0, 0);
  a->setEnabled(false);

  a = settingsMenu->addAction(QIcon(), tr("Show &Search"), this, 0, 0);
  a->setEnabled(false);
  a->setVisible(false);
  settingsMenu->addSeparator();
  a = settingsMenu->addAction(QIcon(), tr("&Toolbars"), this, 0, 0);
  a->setEnabled(false);
  a = settingsMenu->addAction(QIcon(), tr("Show St&atusbar"), this, 0, 0);
  a->setEnabled(false);
  settingsMenu->addSeparator();
  a = settingsMenu->addAction(QIcon(), tr("Configure S&hortcuts..."), this, SLOT(slotUpgradeToKWordQuiz()), 0);
  a->setVisible(false);
  a = settingsMenu->addAction(QIcon(), tr("Configure Tool&bars..."), this, SLOT(slotUpgradeToKWordQuiz()), 0);
  a->setVisible(false);
  a = settingsMenu->addAction(QIcon(), tr("Configure &Notifications..."), this, SLOT(slotUpgradeToKWordQuiz()), 0);
  a->setVisible(false);

  a = helpMenu->addAction(QIcon(), tr("WordQuiz &Handbook"), this, SLOT(slotHelpHandbook()), QKeySequence(QKeySequence::HelpContents));
  a = helpMenu->addAction(QIcon(), tr("What's &This?"), this, SLOT(slotHelpWhatsThis()), QKeySequence(QKeySequence::WhatsThis));
  //helpMenu->addSeparator();
  a = helpMenu->addAction(QIcon(), tr("&Report Bug..."), this, SLOT(slotUpgradeToKWordQuiz()), 0);
  a->setVisible(false);
  //helpMenu->addSeparator();
  a = helpMenu->addAction(QIcon(), tr("Switch Application &Language..."), this, SLOT(slotUpgradeToKWordQuiz()), 0);
  a->setVisible(false);

  updateRecentFileActions();
  setAttribute(Qt::AA_DontShowIconsInMenus);  // Icons are *no longer shown* in menus
#endif
  connect(this, SIGNAL(lastWindowClosed()) , this, SLOT(slotLastWindowClosed()));
}

#ifdef Q_WS_WIN
bool WQApplication::winEventFilter(MSG * msg, long * result)
{
  if(msg->message == WM_COPYDATA) {
    foreach(QWidget *w, topLevelWidgets()) {
      WordQuizApp *mainWin = qobject_cast<WordQuizApp *>(w);
      if (mainWin) {
        if (msg->hwnd == mainWin->winId()) {
          PCOPYDATASTRUCT cpd = (PCOPYDATASTRUCT)msg->lParam;
          if (cpd) {
            QByteArray data((char *)cpd->lpData, cpd->cbData);
            QString arg = QString::fromUtf8(data).trimmed();
            mainWin->openUrl(QUrl(arg));
            *result = (long) 1;
            return true;
          }
        }
      }
    }
  }
  return QCoreApplication::winEventFilter(msg, result);
}
#endif

#ifdef Q_WS_MAC
bool WQApplication::macEventFilter(EventHandlerCallRef caller, EventRef event)
{
// based on http://svn.netlabs.org/qtapps/browser/psi/vendor/affinix/current/src/psiapplication.h?rev=2
  Q_UNUSED(caller);
  UInt32 eclass = GetEventClass(event);
  int etype = GetEventKind(event);
  if (eclass == 'eppc' && etype == kEventAppleEvent) {
    int i = 0;
    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
      WordQuizApp *mainWin = qobject_cast<WordQuizApp *>(widget);
      if (mainWin)
          i++;
    }
    if (i == 0) {
      WordQuizApp *win = new WordQuizApp();
      win->show();
    }
    dockActivated();
  }
  return false;
/* sample
    if(otetsudaiqt != NULL && GetEventClass(event) == kEventClassCommand && GetEventKind(event) == kEventCommandProcess)
    {
        HICommand commandStruct;
        GetEventParameter (event, kEventParamDirectObject,
                           typeHICommand, NULL, sizeof(HICommand),
                           NULL, &commandStruct);

        switch (commandStruct.commandID)
        {
        case kHICommandAbout:
            otetsudaiqt->slotHelpAbout();
            return true;
        case kHICommandPreferences:
            otetsudaiqt->slotMainPref();
            return true;
        }
    }
    return false;
*/
}
#endif

bool WQApplication::event(QEvent *ev)
{
  bool handled;
  switch (ev->type()) {
    case QEvent::FileOpen: {
      //////filecheck.fromLocalFile(QString());
      QString fileToOpen = static_cast<QFileOpenEvent *>(ev)->file();
      if (!fileToOpen.isEmpty()) {
        QFileInfo fi(fileToOpen);
        if (fi.exists()) {
          int i = 0;
          foreach (QWidget *widget, QApplication::topLevelWidgets()) {
            WordQuizApp *mainWin = qobject_cast<WordQuizApp *>(widget);
            if (mainWin) {
              mainWin->openUrl(QUrl(fileToOpen));
              i++;
              break;
            }
          }
          if (i == 0) {
            WordQuizApp *win = new WordQuizApp();
            win->openUrl(QUrl(fileToOpen));
          }
          dockActivated();
        }
      }
      handled = true;
      break;
    }

  /*case QEvent::Close: {
      QCloseEvent *closeEvent = static_cast<QCloseEvent *>(ev);
      handled = true;
      break;
  }*/
  default:
      handled = QApplication::event(ev);
      break;
  }
  return QApplication::event(ev);
}


#ifdef Q_WS_WIN
HWND m_hWnd;

BOOL CALLBACK SearchProc(HWND hWnd, LPARAM lParam)
{
  TCHAR title[MAX_PATH];
  ZeroMemory(title, sizeof(title));
  int length = GetWindowTextLength(hWnd);
  length = GetWindowText(hWnd, title, length + 1);
  //fprintf(stderr, "Debug: %s\n", title);
  QString s = QString::fromUtf16((ushort *)title, length);
  if (s.contains("- WordQuiz")){
    m_hWnd = hWnd;
    return false;
  }
  return true;
}
#endif

bool WQApplication::isAlreadyRunning(){
#ifdef Q_WS_WIN
  m_hWnd = 0;
  bool mustHalt = false;
  //This mutex prevents the InnoSetup based installer for WQ to run if WQ is running
  HANDLE mutex = CreateMutexA(0, false, (CHAR*)"phWORDQUIZ_APPLICATION_MUTEX");

  if (GetLastError() == ERROR_ALREADY_EXISTS){
    mustHalt = true;
    QString arg;
    if (arguments().count() > 1) {
      arg = arguments().last();
    } else {
      arg = "@EMPTY@";
    }
    if (WaitForSingleObject(mutex, 5000) == WAIT_OBJECT_0) {
      EnumWindows(SearchProc, 0);
      if (m_hWnd !=0) {
        COPYDATASTRUCT CopyData;
        CopyData.dwData = 0;
        QByteArray data = arg.toUtf8();
        CopyData.lpData = data.data();
        CopyData.cbData = data.length();
        LRESULT result = SendMessage(m_hWnd, WM_COPYDATA, 0, (LPARAM)&CopyData);
        if ((bool)result == true){
          mustHalt = true;
        }
      }
    }
    ReleaseMutex(mutex);
  }
  return mustHalt;
#else
  return false;
#endif
}

void WQApplication::slotLastWindowClosed() {
#ifdef Q_WS_MAC
  updateRecentFileActions();
#endif
  if (quitOnLastWindowClosed()) {
    quit();
  }
}

void WQApplication::slotPreferences() {
  WordQuizPrefs *dlg = new WordQuizPrefs(0);
  //connect(dlg, SIGNAL(settingsChanged()), this, SLOT(slotApplyPreferences()));
  dlg->exec();
  delete dlg;
}

void WQApplication::slotAbout() {
  QMessageBox about;
  about.setWindowTitle(applicationName());
  about.setText(tr("<html><p><b>WordQuiz %1</b></p>" \
                     "<p>A powerful flashcard and vocabulary learning program</p>" \
                     "<p><a href='http://peterandlinda.com/wordquiz'>Home Page</a></p>" \
                     "<p>%2</p>" \
                     "<p>Published under the GNU General Public License</p></html>").arg(WQ_VERSION).arg(WQ_COPYRIGHT_STR));
  about.addButton(QMessageBox::Ok);
  about.setEscapeButton(QMessageBox::Ok);
  about.setIconPixmap(QPixmap(":/kwordquiz/src/icons/hi32-app-kwordquiz.png"));
  about.exec();
}


void WQApplication::slotUpgradeToKWordQuiz()
{
  QMessageBox about;
  about.setWindowTitle(applicationName());
  about.setText(tr("<html><p><b>Upgrade to KWordQuiz and a KDE desktop</b></p>" \
                     "<p>The functionality normally provided by this command is " \
                     "only available in <a href='http://edu.kde.org'>KWordQuiz</a>.</p>" \
                     "<p>Consider upgrading to a <a href='http://www.kde.org'>KDE Desktop</a> " \
                     "for a more integrated experience.</p></html>"));
  about.addButton(QMessageBox::Ok);
  about.setEscapeButton(QMessageBox::Ok);
  about.setIconPixmap(QPixmap(":/kwordquiz/src/icons/hi32-app-kwordquiz.png"));
  about.exec();
}

void WQApplication::slotNew() {
  WordQuizApp *win = new WordQuizApp();
  win->show();
}

void WQApplication::slotOpen() {
  QStringList filters = QString(KEduVocDocument::pattern(KEduVocDocument::Reading)).split('\n');
  QString qtFilters;
  foreach (const QString filter, filters) {
    QStringList temp = filter.split('|');
    qtFilters.append(QString(temp[1] + " (" + temp[0] + ");;"));
  }
  QStringList files = QFileDialog::getOpenFileNames (0, tr("Open Vocabulary Document"), QString(), qtFilters); 
  if (!files.isEmpty())  {
    QStringList l = files;
    foreach(const QString & fileToOpen, files) {
      WordQuizApp *win = new WordQuizApp();
      win->openDocumentFile(QUrl(fileToOpen));
      dockActivated();
    }
  }
}

void WQApplication::slotFileOpenRecent(QAction* action) {
  if (action) {
    WordQuizApp *win = new WordQuizApp();
    win->openDocumentFile(QUrl::fromLocalFile(action->data().toString()));
    dockActivated();
  }
}

void WQApplication::slotHelpHandbook()
{
  QDesktopServices::openUrl(QUrl("http://docs.kde.org/development/en/kdeedu/kwordquiz/"));
}

void WQApplication::slotHelpWhatsThis()
{
  QWhatsThis::enterWhatsThisMode();
}

void WQApplication::updateRecentFileActions()
{
  QStringList files = Prefs::recentFiles();

  int numRecentFiles = qMin(files.size(), 10);

  for (int i = 0; i < numRecentFiles; ++i) {
      QString text = tr("&%1 %2").arg(i + 1).arg(files[i]);
      m_recentFilesActionGroup->actions().at(i)->setText(text);
      m_recentFilesActionGroup->actions().at(i)->setData(files[i]);
      m_recentFilesActionGroup->actions().at(i)->setVisible(true);
  }
  for (int j = numRecentFiles; j < 10; ++j)
    m_recentFilesActionGroup->actions().at(j)->setVisible(false);
}
