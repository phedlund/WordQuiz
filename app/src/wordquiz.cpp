/***************************************************************************
                               wordquiz.cpp
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

#include "wordquiz.h"
#include "wqapplication.h"

#include <QDebug>
#include <QtGui/QFileDialog>
#include <QtGui/QFontDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QStackedWidget>
#include <QtGui/QUndoStack>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QPainter>
#include <QtGui/QBitmap>
#include <QtGui/QCompleter>

#include <QtCore/QSignalMapper>

#include "keduvoclesson.h"
#include "keduvocexpression.h"

#include "ui_dlglanguage.h"
#include "flashview.h"
#include "multipleview.h"
#include "qaview.h"
#include "kwqquizmodel.h"
#include "prefs.h"
#include "kwqcommands.h"
#include "wordquizprefs.h"
#include "searchlineedit.h"
#include "wqnotification.h"
#include "wqmackeyboard.h"
#include "wqwinkeyboard.h"

#ifdef Q_WS_WIN
  #include <windows.h>
  #include "winsparkle.h"
#endif

WordQuizApp::WordQuizApp( QWidget * parent, Qt::WFlags f) : QMainWindow(parent, f)
{
  setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose, true);
  m_quiz = 0;
  m_messageBox = 0;

  initStatusBar();
  initActions();
  initDocument();
  initModel();

  readOptions();

  initView();

  //qtport m_dirWatch = KDirWatch::self();
  WQNotification::setup();

  QAction *a = m_modeActionGroup->actions()[Prefs::mode() - 1];
  slotModeActionGroupTriggered(a);

  editMarkBlank->setEnabled(Prefs::enableBlanks());
  editUnmarkBlank->setEnabled(Prefs::enableBlanks());

  if (Prefs::firstRun())
  {
    //qtport fileOpenRecent->addUrl(KUrl::fromPath(KStandardDirs::locate("data", "kwordquiz/examples/example.kvtml")));
    //fileOpenRecent->addUrl(KUrl::fromPath(KStandardDirs::locate("data", "kwordquiz/examples/french_verbs.kvtml")));
    //fileOpenRecent->addUrl(KUrl::fromPath(KStandardDirs::locate("data", "kwordquiz/examples/fill_in_the_blank.kvtml")));
    //fileOpenRecent->addUrl(KUrl::fromPath(KStandardDirs::locate("data", "kwordquiz/examples/us_states_and_capitals.kvtml")));
    toolBarCharacters->setVisible(false);
    Prefs::setFirstRun(false);
  }

  resize(Prefs::mainWindowSize());
  move(Prefs::mainWindowPosition());

  restoreState(Prefs::mainWindowState());
  setUnifiedTitleAndToolBarOnMac(true);

  connect(wqApp, SIGNAL(dockActivated()), this, SLOT(slotDockActivated()));
  m_tableView->setFocus();
}


void WordQuizApp::initStatusBar()
{
  m_modeLabel = new QLabel();
  m_modeLabel->setFixedWidth(250);
  m_modeLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  statusBar()->addPermanentWidget(m_modeLabel, 0);
  statusBar()->setVisible(Prefs::showStatusbar());
  configShowStatusbar->setChecked(Prefs::showStatusbar());
}


void WordQuizApp::initActions()
{
  connect(fileNew, SIGNAL(triggered()), this, SLOT(slotFileNew()));
  fileNew->setWhatsThis(tr("Creates a new blank vocabulary document"));
  fileNew->setToolTip(fileNew->whatsThis());
  fileNew->setStatusTip(fileNew->whatsThis());

  connect(fileOpen, SIGNAL(triggered()), this, SLOT(slotFileOpen()));
  fileOpen->setWhatsThis(tr("Opens an existing vocabulary document"));
  fileOpen->setToolTip(fileOpen->whatsThis());
  fileOpen->setStatusTip(fileOpen->whatsThis());

  m_recentFilesActionGroup = new QActionGroup(this);
  connect(m_recentFilesActionGroup, SIGNAL(triggered(QAction *)), this, SLOT(slotFileOpenRecent(QAction *)));
  for (int i = 0; i <=9; i++) {
    QAction *a = new QAction(this);
    a->setVisible(false);
    m_recentFilesActionGroup->addAction(a);
  }
  QMenu *m = new QMenu;
  m->addActions(m_recentFilesActionGroup->actions());
  fileOpenRecent->setMenu(m);

  connect(fileGHNS, SIGNAL(triggered()), wqApp, SLOT(slotUpgradeToKWordQuiz/*slotFileGHNS*/()));
  fileGHNS->setWhatsThis(tr("Downloads new vocabularies"));
  fileGHNS->setToolTip(fileGHNS->whatsThis());
  fileGHNS->setStatusTip(fileGHNS->whatsThis());
  fileGHNS->setVisible(false);

  connect(fileSave, SIGNAL(triggered()), this, SLOT(slotFileSave()));
  fileSave->setWhatsThis(tr("Saves the active vocabulary document"));
  fileSave->setToolTip(fileSave->whatsThis());
  fileSave->setStatusTip(fileSave->whatsThis());

  connect(fileSaveAs, SIGNAL(triggered()), this, SLOT(slotFileSaveAs()));
  fileSaveAs->setWhatsThis(tr("Saves the active vocabulary document with a different name"));
  fileSaveAs->setToolTip(fileSaveAs->whatsThis());
  fileSaveAs->setStatusTip(fileSaveAs->whatsThis());

  connect(fileClose, SIGNAL(triggered()), this, SLOT(slotFileClose()));
  fileClose->setWhatsThis(tr("Closes the active vocabulary document"));
  fileClose->setStatusTip(fileClose->whatsThis());

  connect(filePrint, SIGNAL(triggered()), this, SLOT(slotFilePrint()));
  filePrint->setWhatsThis(tr("Prints the active vocabulary document"));
  filePrint->setToolTip(filePrint->whatsThis());
  filePrint->setStatusTip(filePrint->whatsThis());

  connect(filePrintPreview, SIGNAL(triggered()), this, SLOT(slotFilePrintPreview()));
  filePrintPreview->setWhatsThis(tr("Shows a preview of the active vocabulary document"));
  filePrintPreview->setToolTip(filePrint->whatsThis());
  filePrintPreview->setStatusTip(filePrint->whatsThis());

  connect(fileQuit, SIGNAL(triggered()), this, SLOT(slotFileQuit/*closeAllWindows*/()));
  fileQuit->setWhatsThis(tr("Quits WordQuiz"));
  fileQuit->setToolTip(fileQuit->whatsThis());
  fileQuit->setStatusTip(fileQuit->whatsThis());

  m_undoStack = new QUndoStack(this);
  QAction *editUndo = m_undoStack->createUndoAction(this);
  editUndo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
  editUndo->setIcon(QIcon(":/src/pics/edit-undo.png"));
  menuEdit->insertAction(menuEdit->actions().at(0), editUndo);
  toolBarMain->insertAction(toolBarMain->actions().at(5), editUndo);

  QAction *editRedo = m_undoStack->createRedoAction(this);
  editRedo->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Z));
  editRedo->setIcon(QIcon(":/src/pics/edit-redo.png"));
  menuEdit->insertAction(menuEdit->actions().at(1), editRedo);
  toolBarMain->insertAction(toolBarMain->actions().at(6), editRedo);
  
  connect(m_undoStack, SIGNAL(cleanChanged(bool)), this, SLOT(slotCleanChanged(bool)));
  connect(m_undoStack, SIGNAL(undoTextChanged(const QString &)), this, SLOT(slotUndoTextChanged(const QString &)));
  connect(m_undoStack, SIGNAL(redoTextChanged(const QString &)), this, SLOT(slotRedoTextChanged(const QString &)));

  connect(editCut, SIGNAL(triggered()), this, SLOT(slotEditCut()));
  editCut->setWhatsThis(tr("Cuts the text from the selected cells and places it on the clipboard"));
  editCut->setToolTip(editCut->whatsThis());
  editCut->setStatusTip(editCut->whatsThis());

  connect(editCopy, SIGNAL(triggered()), this, SLOT(slotEditCopy()));
  editCopy->setWhatsThis(tr("Copies the text from the selected cells and places it on the clipboard"));
  editCopy->setToolTip(editCopy->whatsThis());
  editCopy->setStatusTip(editCopy->whatsThis());

  connect(editPaste, SIGNAL(triggered()), this, SLOT(slotEditPaste()));
  editPaste->setWhatsThis(tr("Pastes previously cut or copied text from the clipboard into the selected cells"));
  editPaste->setToolTip(editPaste->whatsThis());
  editPaste->setStatusTip(editPaste->whatsThis());

  connect(editClear, SIGNAL(triggered()), this, SLOT(slotEditClear()));
  editClear->setWhatsThis(tr("Clears the content of the selected cells"));
  editClear->setToolTip(editClear->whatsThis());
  editClear->setStatusTip(editClear->whatsThis());

  editInsert->setWhatsThis(tr("Inserts a new row above the current row"));
  editInsert->setToolTip(editInsert->whatsThis());
  editInsert->setStatusTip(editInsert->whatsThis());
  connect(editInsert, SIGNAL(triggered(bool)), this, SLOT(slotEditInsert()));

  editDelete->setWhatsThis(tr("Deletes the selected row(s)"));
  editDelete->setToolTip(editDelete->whatsThis());
  editDelete->setStatusTip(editDelete->whatsThis());
  connect(editDelete, SIGNAL(triggered(bool)), this, SLOT(slotEditDelete()));

  editMarkBlank->setWhatsThis(tr("Marks the current or selected word as a blank for Fill-in-the-blank"));
  editMarkBlank->setToolTip(editMarkBlank->whatsThis());
  editMarkBlank->setStatusTip(editMarkBlank->whatsThis());
  connect(editMarkBlank, SIGNAL(triggered(bool)), this, SLOT(slotEditMarkBlank()));

  editUnmarkBlank->setWhatsThis(tr("Removes blanks from the current or selected word"));
  editUnmarkBlank->setToolTip(editUnmarkBlank->whatsThis());
  editUnmarkBlank->setStatusTip(editUnmarkBlank->whatsThis());
  connect(editUnmarkBlank, SIGNAL(triggered(bool)), this, SLOT(slotEditUnmarkBlank()));

  vocabLanguages->setWhatsThis(tr("Defines the column titles for the active vocabulary"));
  vocabLanguages->setToolTip(vocabLanguages->whatsThis());
  vocabLanguages->setStatusTip(vocabLanguages->whatsThis());
  connect(vocabLanguages, SIGNAL(triggered(bool)), this, SLOT(slotVocabLanguages()));

  vocabFont->setWhatsThis(tr("Defines the font used by the editor"));
  vocabFont->setToolTip(vocabFont->whatsThis());
  vocabFont->setStatusTip(vocabFont->whatsThis());
  connect(vocabFont, SIGNAL(triggered(bool)), this, SLOT(slotVocabFont()));

  vocabImage->setWhatsThis(tr("Links an image with the current entry"));
  vocabImage->setToolTip(vocabImage->whatsThis());
  vocabImage->setStatusTip(vocabImage->whatsThis());
  connect(vocabImage, SIGNAL(triggered(bool)), this, SLOT(slotVocabImage()));

  vocabSound->setWhatsThis(tr("Links a sound with the current entry"));
  vocabSound->setToolTip(vocabSound->whatsThis());
  vocabSound->setStatusTip(vocabSound->whatsThis());
  connect(vocabSound, SIGNAL(triggered(bool)), this, SLOT(slotVocabSound()));

  vocabAdjustRows->setWhatsThis(tr("Automatically adjusts the height of selected rows"));
  vocabAdjustRows->setToolTip(vocabAdjustRows->whatsThis());
  vocabAdjustRows->setStatusTip(vocabAdjustRows->whatsThis());
  connect(vocabAdjustRows, SIGNAL(triggered(bool)), this, SLOT(slotVocabAdjustRows()));

  vocabShuffle->setWhatsThis(tr("Shuffles the entries of the active vocabulary"));
  vocabShuffle->setToolTip(vocabShuffle->whatsThis());
  vocabShuffle->setStatusTip(vocabShuffle->whatsThis());
  connect(vocabShuffle, SIGNAL(triggered(bool)), this, SLOT(slotVocabShuffle()));

  mode0->setIconText(tr("Mode"));
  mode0->setWhatsThis(tr("Changes the mode used in quiz sessions"));
  mode0->setToolTip(mode0->whatsThis());
  mode0->setStatusTip(mode0->whatsThis());
  connect(mode0, SIGNAL(triggered(bool)), this, SLOT(slotModeChange()));

  m_modeActionGroup = new QActionGroup(this);
  connect(m_modeActionGroup, SIGNAL(triggered(QAction *)), this, SLOT(slotModeActionGroupTriggered(QAction *)));
  m_modeActionGroup->addAction(mode1);
  m_modeActionGroup->addAction(mode2);
  m_modeActionGroup->addAction(mode3);
  m_modeActionGroup->addAction(mode4);
  m_modeActionGroup->addAction(mode5);
  
  m = new QMenu;
  m->addActions(m_modeActionGroup->actions());
  mode0->setMenu(m);

  for (int i = 1; i <= 5; i++){
    QAction * a = m_modeActionGroup->actions()[i - 1];
    a->setData(i);
    a->setCheckable(true);
    a->setWhatsThis(tr("Selects this mode"));
    a->setToolTip(a->whatsThis());
    a->setStatusTip(a->whatsThis());
  }

  quizCheck->setWhatsThis(tr("Checks your answer to this question"));
  quizCheck->setToolTip(quizCheck->whatsThis());
  quizCheck->setStatusTip(quizCheck->whatsThis());

  flashKnow->setWhatsThis(tr("Counts this card as correct and shows the next card"));
  flashKnow->setToolTip(flashKnow->whatsThis());
  flashKnow->setStatusTip(flashKnow->whatsThis());

  flashDontKnow->setWhatsThis(tr("Counts this card as incorrect and shows the next card"));
  flashDontKnow->setToolTip(flashDontKnow->whatsThis());
  flashDontKnow->setStatusTip(flashDontKnow->whatsThis());

  qaHint->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));
  qaHint->setWhatsThis(tr("Gets the next correct letter of the answer"));
  qaHint->setToolTip(qaHint->whatsThis());
  qaHint->setStatusTip(qaHint->whatsThis());

  quizRestart->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
  quizRestart->setWhatsThis(tr("Restarts the quiz session from the beginning"));
  quizRestart->setToolTip(quizRestart->whatsThis());
  quizRestart->setStatusTip(quizRestart->whatsThis());

  quizRepeatErrors->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
  quizRepeatErrors->setWhatsThis(tr("Repeats all incorrectly answered questions"));
  quizRepeatErrors->setToolTip(quizRepeatErrors->whatsThis());
  quizRepeatErrors->setStatusTip(quizRepeatErrors->whatsThis());

  quizExportErrors->setWhatsThis(tr("Exports all errors as a new vocabulary document"));
  quizExportErrors->setToolTip(quizExportErrors->whatsThis());
  quizExportErrors->setStatusTip(quizExportErrors->whatsThis());
  connect(quizExportErrors, SIGNAL(triggered(bool)), this, SLOT(slotCreateErrorListDocument()));

  connect(configShowSearchBar, SIGNAL(triggered(bool)), this, SLOT(slotConfigShowSearch()));
  configShowSearchBar->setWhatsThis(tr("Toggle display of the search bar"));
  configShowSearchBar->setToolTip(configShowSearchBar->whatsThis());
  configShowSearchBar->setStatusTip(configShowSearchBar->whatsThis());
  
  QMenu *toolbarMenu = new QMenu;
  toolbarMenu->addAction(toolBarMain->toggleViewAction());
  toolbarMenu->addAction(toolBarQuiz->toggleViewAction());
  toolbarMenu->addAction(toolBarCharacters->toggleViewAction());
  configToolbars->setMenu(toolbarMenu);

  connect(configShowStatusbar, SIGNAL(triggered(bool)), this, SLOT(slotConfigShowStatusbar()));
  configShowStatusbar->setWhatsThis(tr("Toggle display of the statusbar"));
  configShowStatusbar->setToolTip(configShowStatusbar->whatsThis());
  configShowStatusbar->setStatusTip(configShowStatusbar->whatsThis());

  connect(configConfigureShortcuts, SIGNAL(triggered(bool)), wqApp, SLOT(slotUpgradeToKWordQuiz/*slotConfigConfigureShortcuts*/()));
  configConfigureShortcuts->setWhatsThis(tr("Configure the shortcuts used for actions"));
  configConfigureShortcuts->setToolTip(configConfigureShortcuts->whatsThis());
  configConfigureShortcuts->setStatusTip(configConfigureShortcuts->whatsThis());
  configConfigureShortcuts->setVisible(false);

  connect(configConfigureToolbars, SIGNAL(triggered(bool)), wqApp, SLOT(slotUpgradeToKWordQuiz/*slotConfigToolbars*/()));
  configConfigureToolbars->setWhatsThis(tr("Configure the toolbars"));
  configConfigureToolbars->setToolTip(configConfigureToolbars->whatsThis());
  configConfigureToolbars->setStatusTip(configConfigureToolbars->whatsThis());
  configConfigureToolbars->setVisible(false);

  connect(configConfigureNotifications, SIGNAL(triggered(bool)), wqApp, SLOT(slotUpgradeToKWordQuiz/*slotConfigureNotifications*/()));
  configConfigureNotifications->setWhatsThis(tr("Configures sound and other notifications for certain events"));
  configConfigureNotifications->setToolTip(configConfigureNotifications->whatsThis());
  configConfigureNotifications->setStatusTip(configConfigureNotifications->whatsThis());
  configConfigureNotifications->setVisible(false);

  connect(configConfigureWordQuiz, SIGNAL(triggered(bool)), this, SLOT(slotConfigure()));
  configConfigureWordQuiz->setWhatsThis(tr("Specifies preferences for the vocabulary editor and quiz sessions"));
  configConfigureWordQuiz->setToolTip(configConfigureWordQuiz->whatsThis());
  configConfigureWordQuiz->setToolTip(configConfigureWordQuiz->whatsThis());

  connect(helpHandbook, SIGNAL(triggered(bool)), wqApp, SLOT(slotHelpHandbook()));
  helpHandbook->setWhatsThis(tr("Display the WordQuiz Handbook"));
  helpHandbook->setToolTip(helpHandbook->whatsThis());
  helpHandbook->setToolTip(helpHandbook->whatsThis());

  connect(helpWhatsThis, SIGNAL(triggered(bool)), wqApp, SLOT(slotHelpWhatsThis()));
  helpWhatsThis->setWhatsThis(tr("Get help for the item under the cursor"));
  helpWhatsThis->setToolTip(helpWhatsThis->whatsThis());
  helpWhatsThis->setToolTip(helpWhatsThis->whatsThis());

  connect(helpReportBug, SIGNAL(triggered(bool)), wqApp, SLOT(slotUpgradeToKWordQuiz/*slotHelpReportBug*/()));
  helpReportBug->setWhatsThis(tr("Report a bug in WordQuiz to the maintainer"));
  helpReportBug->setToolTip(helpReportBug->whatsThis());
  helpReportBug->setToolTip(helpReportBug->whatsThis());
  helpReportBug->setVisible(false);

  connect(helpSwitchLanguage, SIGNAL(triggered(bool)), wqApp, SLOT(slotUpgradeToKWordQuiz/*slotHelpSwitchLanguage*/()));
  helpSwitchLanguage->setWhatsThis(tr("Switch the language used in WordQuiz"));
  helpSwitchLanguage->setToolTip(helpSwitchLanguage->whatsThis());
  helpSwitchLanguage->setToolTip(helpSwitchLanguage->whatsThis());
  helpSwitchLanguage->setVisible(false);

  connect(helpAboutWordQuiz, SIGNAL(triggered(bool)), wqApp, SLOT(slotAbout()));
  helpAboutWordQuiz->setWhatsThis(tr("Display information about WordQuiz"));
  helpAboutWordQuiz->setToolTip(helpAboutWordQuiz->whatsThis());
  helpAboutWordQuiz->setToolTip(helpAboutWordQuiz->whatsThis());

  connect(helpAboutQt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
  helpAboutQt->setWhatsThis(tr("Display information about Qt"));
  helpAboutQt->setToolTip(helpAboutQt->whatsThis());
  helpAboutQt->setToolTip(helpAboutQt->whatsThis());

  connect(helpCheckForUpdates, SIGNAL(triggered(bool)), wqApp, SLOT(slotCheckForUpdates()));
  helpCheckForUpdates->setWhatsThis(tr("Check for updates"));
  helpCheckForUpdates->setToolTip(helpCheckForUpdates->whatsThis());

  m_charActionGroup = new QActionGroup(this);
  m_charActionGroup->setExclusive(false);
  connect(m_charActionGroup, SIGNAL(triggered(QAction *)), this, SLOT(slotCharActionGroupTriggered(QAction *)));
  m_charActionGroup->addAction(actionChar1);
  m_charActionGroup->addAction(actionChar2);
  m_charActionGroup->addAction(actionChar3);
  m_charActionGroup->addAction(actionChar4);
  m_charActionGroup->addAction(actionChar5);
  m_charActionGroup->addAction(actionChar6);
  m_charActionGroup->addAction(actionChar7);
  m_charActionGroup->addAction(actionChar8);
  m_charActionGroup->addAction(actionChar9);

  for (int i = 1; i <= 9; i++) {
    QAction * a = m_charActionGroup->actions()[i - 1];
    a->setData(i);
    a->setCheckable(false);
    a->setText(tr("Special Character %1").arg(QString::number(i)));
    a->setShortcut(QKeySequence(QString("Ctrl+%1").arg(QString::number(i))));
    a->setWhatsThis(tr("Inserts this character"));
    a->setToolTip(a->whatsThis());
    a->setStatusTip(a->whatsThis());
  }

  updateSpecialCharIcons();
}


void WordQuizApp::initDocument()
{
  m_doc = new KEduVocDocument(this);
  ///@todo New kvtml documents have a locale setting which should contain the language they are written in.
  /* it would be nice to support that somehow. get hot new stuff for KHangman and KAnagram already uses this. */
  m_doc->appendIdentifier();
  m_doc->identifier(0).setName(tr("Column 1"));
  // m_doc->identifier(0).setLocale("en");
  m_doc->appendIdentifier();
  m_doc->identifier(1).setName(tr("Column 2"));
  // m_doc->identifier(1).setLocale("en");

  for (int i=0; i<20; i++)
  {
    m_doc->lesson()->appendEntry(new KEduVocExpression());
  }
}

void WordQuizApp::initModel()
{
  m_tableModel = new KWQTableModel(this);
  m_tableModel->setDocument(m_doc);
  m_tableModel->setHeaderData(0, Qt::Horizontal, QSize(250, 25), Qt::SizeHintRole);
  m_tableModel->setHeaderData(1, Qt::Horizontal, QSize(250, 25), Qt::SizeHintRole);
  m_sortFilterModel = new KWQSortFilterModel(this);
  m_sortFilterModel->setSourceModel(m_tableModel);
}

void WordQuizApp::initView()
{
  m_contentsWidget = new QListWidget;
  m_contentsWidget->setFocusPolicy(Qt::NoFocus);
  m_contentsWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

#ifdef Q_WS_MAC
  // Set up the left-hand side blue side bar.
  m_contentsWidget->setFrameStyle(QFrame::NoFrame);
  m_contentsWidget->setAttribute(Qt::WA_MacShowFocusRect, false);
  m_contentsWidget->setAutoFillBackground(true);

  // Set the palette.
  QPalette palette = m_contentsWidget->palette();
  //QColor macSidebarColor(219, 222, 232);
  //QColor macSidebarColorInactive(237, 237, 237);
  QColor macSidebarHighlightColor(83, 126, 201);
  QColor macSidebarHighlightColorInactive(158, 170, 202);

  QLinearGradient activeGradient = QLinearGradient(0, 0, 0, 1);
  activeGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
  activeGradient.setColorAt(0.0, QColor(233, 237, 242));
  activeGradient.setColorAt(1.0, QColor(209, 216, 224));
  palette.setBrush(QPalette::Active, QPalette::Base, QBrush(activeGradient));
  palette.setColor(QPalette::Active, QPalette::Highlight, macSidebarHighlightColor);
  QLinearGradient inactiveGradient = QLinearGradient(0, 0, 0, 1);
  inactiveGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
  inactiveGradient.setColorAt(0.0, QColor(248, 248, 248));
  inactiveGradient.setColorAt(1.0, QColor(235, 235, 235));
  palette.setBrush(QPalette::Inactive, QPalette::Base, inactiveGradient);
  palette.setColor(QPalette::Inactive, QPalette::Highlight, macSidebarHighlightColorInactive);
  m_contentsWidget->setPalette(palette);
  m_contentsWidget->setFixedWidth(160);
  m_contentsWidget->setSpacing(0);
  m_contentsWidget->setGridSize(QSize(22, 22));
#else
  m_contentsWidget->setViewMode(QListView::IconMode);
  m_contentsWidget->setIconSize(QSize(96, 48));
  m_contentsWidget->setMovement(QListView::Static);
  m_contentsWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_contentsWidget->setWrapping(true);
  m_contentsWidget->setFixedWidth(116);
  m_contentsWidget->setSpacing(6);
#endif

  toolBarMain->addWidget(new Spacer());
  m_searchWrapper = new SearchWrapper();
  toolBarMain->addWidget(m_searchWrapper);
  connect(m_searchWrapper->searchBox(), SIGNAL(textChanged(const QString&)), this, SLOT(slotEditFind(const QString&)));

  m_pageWidget = new QStackedWidget;
  
  QVBoxLayout *editorLayout = new QVBoxLayout();
  editorLayout->setMargin(0);
  editorLayout->setSpacing(0);
  editorLayout->setContentsMargins(0, 0, 0, 0);

  m_tableView = new KWQTableView(m_undoStack, this);
  editorLayout->addWidget(m_tableView);
  m_tableView->setModel(m_sortFilterModel);
  m_tableView->setColumnWidth(0, qvariant_cast<QSize>(m_tableModel->headerData(0, Qt::Horizontal, Qt::SizeHintRole)).width());
  m_tableView->setColumnWidth(1, qvariant_cast<QSize>(m_tableModel->headerData(1, Qt::Horizontal, Qt::SizeHintRole)).width());
  setCaption(m_doc->url().toLocalFile(), false);
  
  QAction *a = new QAction(this);
  a->setSeparator(true);
  QAction *b = new QAction(this);
  b->setSeparator(true);
  QAction *c = new QAction(this);
  c->setSeparator(true);
  m_tableView->addAction(menuEdit->actions().at(0));
  m_tableView->addAction(a);
  m_tableView->addAction(editCut);
  m_tableView->addAction(editCopy);
  m_tableView->addAction(editPaste);
  m_tableView->addAction(b);
  m_tableView->addAction(editInsert);
  m_tableView->addAction(editDelete);
  m_tableView->addAction(c);
  m_tableView->addAction(vocabLanguages);
  m_tableView->setContextMenuPolicy(Qt::ActionsContextMenu);

  connect(m_tableModel, SIGNAL(modelReset()), m_tableView, SLOT(slotModelReset()));
  configShowSearchBar->setVisible(false);

#ifdef Q_WS_MAC
  m_tableView->setFrameStyle(QFrame::NoFrame);
#endif

  m_editorView = new QWidget(this);
  m_editorView->setLayout(editorLayout);
  m_flashView = new FlashView(this/*, actionCollection()*/);
  m_multipleView = new MultipleView(this/*, actionCollection()*/);
  m_qaView = new QAView(this/*, actionCollection()*/);

  m_pageWidget->addWidget(m_editorView);
  m_pageWidget->addWidget(m_flashView);
  m_pageWidget->addWidget(m_multipleView);
  m_pageWidget->addWidget(m_qaView);

  m_flashView->addAction(quizCheck);
  m_flashView->addAction(flashKnow);
  m_flashView->addAction(flashDontKnow);
  m_flashView->addAction(qaHint);
  m_flashView->addAction(quizRestart);
  m_flashView->addAction(quizRepeatErrors);
  m_flashView->addAction(quizExportErrors);
  m_flashView->addAction(quizPlayAudio);

  m_multipleView->addAction(quizCheck);
  m_multipleView->addAction(flashKnow);
  m_multipleView->addAction(flashDontKnow);
  m_multipleView->addAction(qaHint);
  m_multipleView->addAction(quizRestart);
  m_multipleView->addAction(quizRepeatErrors);
  m_multipleView->addAction(quizExportErrors);

  m_qaView->addAction(quizCheck);
  m_qaView->addAction(flashKnow);
  m_qaView->addAction(flashDontKnow);
  m_qaView->addAction(qaHint);
  m_qaView->addAction(qaMarkLastCorrect);
  m_qaView->addAction(quizRestart);
  m_qaView->addAction(quizRepeatErrors);
  m_qaView->addAction(quizExportErrors);
  m_qaView->addAction(quizPlayAudio);

  updateActions();

  createIcons();
  m_contentsWidget->setCurrentRow(0);
  m_tableView->setFocus();

  QHBoxLayout *horizontalLayout = new QHBoxLayout;
  horizontalLayout->addWidget(m_contentsWidget);

#ifdef Q_WS_MAC
  horizontalLayout->setContentsMargins(0, 0, 0, 0);
  horizontalLayout->setSpacing(0);
  QFrame *vline = new QFrame(this);
  vline->setFrameShape(QFrame::VLine);
  palette = vline->palette();
  QColor macLineColor(145, 145, 145);
  palette.setColor(QPalette::WindowText, macLineColor);
  vline->setPalette(palette);
  horizontalLayout->addWidget(vline, 1);
#else
  horizontalLayout->setContentsMargins(0, 6, 0, 0);
  horizontalLayout->setSpacing(6);
#endif

  horizontalLayout->addWidget(m_pageWidget, 2);
  
  setCentralWidget(new QWidget());
  centralWidget()->setLayout(horizontalLayout);
}

void WordQuizApp::createIcons()
{
  Qt::Alignment myAlignment = Qt::AlignHCenter;
#ifdef Q_WS_MAC
  myAlignment = Qt::AlignLeft;
#endif

  QListWidgetItem *editorButton = new QListWidgetItem(m_contentsWidget);
#ifdef Q_WS_MAC
  editorButton->setIcon(QIcon(":/kwordquiz/src/pics/ox16-action-editor.png"));
#else
  editorButton->setIcon(QIcon(":/src/pics/list-action-editor.png"));
#endif
  editorButton->setText(tr("Editor"));
  editorButton->setTextAlignment(myAlignment);
  editorButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  QListWidgetItem *flashcardButton = new QListWidgetItem(m_contentsWidget);
#ifdef Q_WS_MAC
  flashcardButton->setIcon(QIcon(":/kwordquiz/src/pics/ox16-action-flash.png"));
#else
  flashcardButton->setIcon(QIcon(":/src/pics/list-action-flash.png"));
#endif
  flashcardButton->setText(tr("Flashcard"));
  flashcardButton->setTextAlignment(myAlignment);
  flashcardButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  QListWidgetItem *multipleButton = new QListWidgetItem(m_contentsWidget);
#ifdef Q_WS_MAC
  multipleButton->setIcon(QIcon(":/kwordquiz/src/pics/ox16-action-multiple.png"));
  multipleButton->setText(tr("Multiple Choice"));
#else
  multipleButton->setIcon(QIcon(":/src/pics/list-action-multiple.png"));
  multipleButton->setText(tr("Multiple\nChoice"));
#endif

  multipleButton->setTextAlignment(myAlignment);
  multipleButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
  
  QListWidgetItem *qaButton = new QListWidgetItem(m_contentsWidget);
#ifdef Q_WS_MAC
  qaButton->setIcon(QIcon(":/kwordquiz/src/pics/ox16-action-qa.png"));
  qaButton->setText(tr("Question & Answer"));
#else
  qaButton->setIcon(QIcon(":/src/pics/list-action-qa.png"));
  qaButton->setText(tr("Question &\nAnswer"));
#endif

  qaButton->setTextAlignment(myAlignment);
  qaButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  QAction *quizEditor = new QAction(this);
  quizEditor->setShortcut(QKeySequence(Qt::Key_F6));
  connect(quizEditor, SIGNAL(triggered(bool)), this, SLOT(slotQuizEditor()));

  QAction *quizFlash =  new QAction(this);
  quizFlash->setShortcut(QKeySequence(Qt::Key_F7));
  connect(quizFlash, SIGNAL(triggered(bool)), this, SLOT(slotQuizFlash()));

  QAction *quizMultiple =  new QAction(this);
  quizMultiple->setShortcut(QKeySequence(Qt::Key_F8));
  connect(quizMultiple, SIGNAL(triggered(bool)), this, SLOT(slotQuizMultiple()));

  QAction *quizQA =  new QAction(this);
  quizQA->setShortcut(QKeySequence(Qt::Key_F9));
  connect(quizQA, SIGNAL(triggered(bool)), this, SLOT(slotQuizQA()));
  
  m_contentsWidget->addAction(quizEditor);
  m_contentsWidget->addAction(quizFlash);
  m_contentsWidget->addAction(quizMultiple);
  m_contentsWidget->addAction(quizQA);

#ifndef Q_WS_MAC
  QFont f = m_contentsWidget->font();
  f.setWeight(QFont::Bold);
  m_contentsWidget->setFont(f);
#endif

  connect(m_contentsWidget, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
          this, SLOT(slotCurrentPageChanged(QListWidgetItem *, QListWidgetItem*)));
}

void WordQuizApp::saveOptions()
{
  //fileOpenRecent->saveEntries(KGlobal::config()->group( "Recent Files") );
  Prefs::setMainWindowSize(size());
  Prefs::setMainWindowPosition(pos());
  Prefs::setMainWindowState(saveState());
  Prefs::self()->writePrefs();
}

void WordQuizApp::readOptions()
{
  updateRecentFileActions(); //fileOpenRecent->loadEntries(KGlobal::config()->group( "Recent Files") );
}


void WordQuizApp::openUrl(const QUrl& url)
{
  if(!url.isEmpty()) {
    /*if (m_dirWatch->contains(url.path()))
    {
      KMainWindow* w;
      if(!memberList().isEmpty())
      {
        for (int i = 0; i < memberList().size(); ++i)
        {
          w = memberList().at(i);
          KWordQuizApp *a =(KWordQuizApp *) w;
          if(a->document()->url().path() == url.path())
          {
            if (w->isMinimized())
              KWindowSystem::unminimizeWindow(w->winId());
            w->activateWindow();
            w->raise();
            break;
          }
        }
      }
    }
    else
    {*/
      if (m_tableModel->isEmpty()){
        openDocumentFile(url);
      }
      else
      {
        WordQuizApp *new_window= new WordQuizApp();
        new_window->show();
        new_window->openDocumentFile(url);
      }
    }
  //}
}

void WordQuizApp::openDocumentFile(const QUrl& url)
{
  if (!url.isEmpty()) {
    int result = m_doc->open(url);
    if (result == KEduVocDocument::NoError) {
      while (m_doc->identifierCount() < 2) { //if we opened a TAB-less CSV, there
        m_doc->appendIdentifier(); //may be 0 or 1 identifiers, we need at least 2
      }

      m_tableModel->reset();
      //m_dirWatch->addFile(url.path());
      setCaption(m_doc->url().toLocalFile(), false);
      addRecentFile(m_doc->url().toLocalFile()); // fileOpenRecent->addUrl(url);
      slotModeActionGroupTriggered(m_modeActionGroup->actions()[Prefs::mode() - 1]);
      m_undoStack->clear();
      slotDockActivated();
      switch (Prefs::startSession()) {
        case Prefs::EnumStartSession::Flashcard:
          slotQuizFlash();
          break;
        case Prefs::EnumStartSession::MultipleChoice:
          slotQuizMultiple();
          break;
        case Prefs::EnumStartSession::QA:
          slotQuizQA();
          break;
        default:
          slotQuizEditor();
          break;
      }
    }
    else
      QMessageBox::critical(this, tr("WordQuiz"), KEduVocDocument::errorDescription(result));
  }
}

void WordQuizApp::closeEvent(QCloseEvent *event)
 {
     if (m_reallyClose) {
         saveOptions();
         event->accept();
     } else {
         queryClose();
         event->ignore();
     }
 } 

bool WordQuizApp::queryClose()
{
  bool completed=true;
  if (m_doc->isModified()) {
    m_messageBox = new QMessageBox(QCoreApplication::applicationName(),
                tr("The document has been modified.\nDo you want to save your changes?"),
                QMessageBox::Warning,
                QMessageBox::Save | QMessageBox::Default,
                QMessageBox::Discard,
                QMessageBox::Cancel | QMessageBox::Escape,
                this, Qt::Sheet);

    
    connect(m_messageBox, SIGNAL(finished(int)), this, SLOT(finishClose(int)));
    m_messageBox->show();
  }
    
    
    
    /*int want_save = QMessageBox::warning(this, tr("WordQuiz"),
                   tr("The document has been modified.\nDo you want to save your changes?"),
                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                   QMessageBox::Save);*/
    //int want_save = KMessageBox::warningYesNoCancel(this, i18n("The current document has been modified.\nDo you want to save it?"), QString(), KStandardGuiItem::save(), KStandardGuiItem::discard());


  //if (completed)
    //if (m_dirWatch->contains(m_doc->url().path()))
      //m_dirWatch->removeFile(m_doc->url().path());
  return completed;
}

void WordQuizApp::finishClose(int response) {
  switch (response) {
    case QMessageBox::Save:
      if (m_doc->url().toString() == tr("Untitled")) {
        m_reallyClose = saveDocAsFileName(m_doc);
      } else {
        m_reallyClose = (m_doc->saveAs(m_doc->url(), KEduVocDocument::Automatic, QString("kwordquiz %1").arg(QCoreApplication::applicationVersion())) == KEduVocDocument::NoError);
      }
      break;

    case QMessageBox::Discard:
      m_reallyClose = true;
      break;
    
    case QMessageBox::Cancel:
    default:
      m_reallyClose = false;
#ifdef Q_WS_MAC
      wqApp->setQuitOnLastWindowClosed(false);
#endif
      break;
  }

  if (m_messageBox) {
    disconnect(m_messageBox, SIGNAL(finished(int )), this, SLOT(finishClose(int )));
    m_messageBox->deleteLater();
    m_messageBox = 0;
  }

  if (m_reallyClose)
    close();
}

bool WordQuizApp::queryExit()
{
  //saveOptions();
  return true;
}

void WordQuizApp::slotFileNew()
{
  if (!m_tableModel->isEmpty()){
    WordQuizApp *new_window= new WordQuizApp();
    new_window->show();
  }
}


void WordQuizApp::slotFileOpen()
{
  /*QCheckBox * cb = new QCheckBox(i18n("&Merge selected files with the current document"), 0);
  cb -> setChecked(false);
  cb -> setEnabled(false);

  QString filter = KEduVocDocument::pattern(KEduVocDocument::Reading);
  KFileDialog *fd = new KFileDialog(KUrl(), filter, this, cb);
  fd->setOperationMode(KFileDialog::Opening);
  fd->setMode(KFile::Files | KFile::ExistingOnly);
  fd->setCaption(i18n("Open Vocabulary Document"));
*/
  QStringList filters = QString(KEduVocDocument::pattern(KEduVocDocument::Reading)).split('\n');
  QString qtFilters;
  foreach (const QString filter, filters) {
    QStringList temp = filter.split('|');
    qtFilters.append(QString(temp[1] + " (" + temp[0] + ");;"));
  }
  QStringList files = QFileDialog::getOpenFileNames (this, tr("Open Vocabulary Document"), Prefs::lastOpenDir(), qtFilters);
  if (!files.isEmpty())
  {
    QStringList l = files;
    /*bool append = ((cb->isChecked()) && (l.count() >= 1));

    if (append)
    {
      KUrl::List::iterator it;
      for(it = l.begin(); it != l.end(); ++it)
      {
        KEduVocDocument *new_doc = new KEduVocDocument(this);
        new_doc->open(*it);

        m_doc->merge(new_doc, false);
        delete (new_doc);
      }
      m_tableModel->reset();
    }
    else
    {*/

      foreach(const QString & file, files)
      {
          Prefs::setLastOpenDir(QFileInfo(file).path());
          openUrl(QUrl::fromLocalFile(file));
      }
    }
  //}

  //delete (fd); //deletes cb also
}


void WordQuizApp::slotFileOpenRecent(QAction *action)
{
  if (action) {
    openUrl(QUrl::fromLocalFile(action->data().toString()));
  }
}


void WordQuizApp::slotFileGHNS()
{
/*  KNS::Entry::List entries = KNS::Engine::download();
  // list of changed entries
  foreach(KNS::Entry* entry, entries) {
    // care only about installed ones
    if (entry->status() == KNS::Entry::Installed) {
      // check mime type and if kvtml, open it
      foreach(const QString &file, entry->installedFiles()) {
        KMimeType::Ptr mimeType = KMimeType::findByPath(file);
        if (mimeType->name() == "application/x-kvtml") {
          KProcess::startDetached("kwordquiz", QStringList() << file);
        }
      }
    }
  }
  qDeleteAll(entries);*/
}


void WordQuizApp::slotFileSave()
{
  bool success = false;
  if (m_doc->url().toString() == tr("Untitled")) {
    success = saveDocAsFileName(m_doc);
  } else {
    int saveStatus = m_doc->saveAs(m_doc->url(), KEduVocDocument::Automatic, QString("kwordquiz %1").arg(QCoreApplication::applicationVersion()));
    if (saveStatus == KEduVocDocument::NoError)
      success = true;
    else
      success = saveDocAsFileName(m_doc);
  }
  if (success)
    m_undoStack->setClean(); //emits cleanChanged()
}

void WordQuizApp::slotFileSaveAs()
{
  bool success = saveDocAsFileName(m_doc);
  if (success) {
    m_undoStack->setClean(); //emits cleanChanged()
    setCaption(m_doc->url().toLocalFile(), m_doc->isModified());  //if clean to begin with
  }
}


bool WordQuizApp::saveDocAsFileName(KEduVocDocument *document)
{
  bool success = false;
  int result = KEduVocDocument::Unknown;

  QStringList filters = QString(KEduVocDocument::pattern(KEduVocDocument::Writing)).split('\n');
  QString qtFilters;
  foreach (const QString filter, filters) {
    QStringList temp = filter.split('|');
    qtFilters.append(QString(temp[1] + " (" + temp[0] + ");;"));
  }
  qtFilters.append(QString(tr("HTML Document (*.html)")));
  QString selectedFilter;
  QString fileName = QFileDialog::getSaveFileName (this, tr("Save Vocabulary Document As"), QString(), qtFilters, &selectedFilter);

  if (!fileName.isEmpty())
  {
    if (!fileName.contains('.')) {
      if (selectedFilter.contains("*.csv"))
        fileName.append(".csv");
      else if (selectedFilter.contains("*.kvtml"))
        fileName.append(".kvtml");
      else if (selectedFilter.contains("*.html"))
        fileName.append(".html");
    }
    
    QUrl url = QUrl::fromLocalFile(fileName);
    if(!url.isEmpty()){
        if (selectedFilter.contains("*.html")) {
          if (m_tableView->doHtmlExport(url))
            result = KEduVocDocument::NoError;
          else
            result = KEduVocDocument::FileCannotWrite;
          success = false; //export only, do not consider really saved
        }
        else {
          result = document->saveAs(url, KEduVocDocument::Automatic, QString("kwordquiz %1").arg(QCoreApplication::applicationVersion()));
          if (result == KEduVocDocument::NoError) {
            addRecentFile(url.toLocalFile());
            success = true;
          }
        }

        if (result != KEduVocDocument::NoError) {
          QMessageBox::critical(this, QCoreApplication::applicationName(), KEduVocDocument::errorDescription(result));
          success = false;
        }
      }
    }

  return success;
}

void WordQuizApp::slotFileClose()
{
  close();
  /*if (memberList().count() > 1)
    close();
  else
    if (queryClose())
    {
      delete m_doc;
      initDocument();
      setCaption(m_doc->url().toString(), m_doc->isModified());
      m_tableModel->setDocument(m_doc);
      slotQuizEditor();
      QAction *a = m_modeActionGroup->actions()[Prefs::mode() - 1];
      slotModeActionGroupTriggered(a);
      m_tableView ->setFocus();
      m_undoStack->clear();
    }*/
}

void WordQuizApp::slotFilePrint()
{
  m_tableView->doPrint();
}

void WordQuizApp::slotFilePrintPreview()
{
  m_tableView->doPrintPreview();
}

void WordQuizApp::slotFileQuit()
{
  wqApp->setQuitOnLastWindowClosed(true);
  wqApp->closeAllWindows();
  /*if (!wqApp->quitOnLastWindowClosed()) {
    if (QApplication::topLevelWidgets().count() == 1) {
      if (QApplication::topLevelWidgets()[0]->isHidden()) {
        wqApp->quit();
      }
    }
  }*/
}

void WordQuizApp::slotEditCut()
{
  m_tableView->doEditCut();
}

void WordQuizApp::slotEditCopy()
{
  m_tableView->doEditCopy();
}

void WordQuizApp::slotEditPaste()
{
  m_tableView->doEditPaste();
}

void WordQuizApp::slotEditClear()
{
  m_tableView->doEditClear();
}

void WordQuizApp::slotEditInsert()
{
  m_tableView->doEditInsert();
}

void WordQuizApp::slotEditDelete()
{
  m_tableView->doEditDelete();
}

void WordQuizApp::slotEditMarkBlank()
{
  m_tableView->doEditMarkBlank();
}

void WordQuizApp::slotEditUnmarkBlank()
{
  m_tableView->doEditUnmarkBlank();
}

void WordQuizApp::slotEditFind(const QString & find)
{
  m_sortFilterModel->setFilterRegExp(QRegExp(find, Qt::CaseInsensitive));
}


void WordQuizApp::slotVocabLanguages()
{
  QDialog dlg(this, Qt::Sheet);
  Ui::dlgLanguages ui;
  ui.setupUi(&dlg);

  ui.column1TitleLineEdit->setText(m_tableModel->headerData(0, Qt::Horizontal, Qt::DisplayRole).toString());
  ui.column2TitleLineEdit->setText(m_tableModel->headerData(1, Qt::Horizontal, Qt::DisplayRole).toString());
  ui.column1TitleLineEdit->setFocus();

  QStringList list1 = Prefs::columnTitles1();
  QCompleter *completer1 = new QCompleter(list1, this);
  completer1->setCaseSensitivity(Qt::CaseInsensitive);
  ui.column1TitleLineEdit->setCompleter(completer1);

  QStringList list2 = Prefs::columnTitles2();
  QCompleter *completer2 = new QCompleter(list2, this);
  completer2->setCaseSensitivity(Qt::CaseInsensitive);
  ui.column2TitleLineEdit->setCompleter(completer2);

  QStringList layouts;
#ifdef Q_WS_MAC
  layouts = WQMacKeyboard::availableLayouts();
#elif defined(Q_WS_WIN)
  layouts = WQWinKeyboard::availableLayouts();
#endif
  if (layouts.isEmpty()) {
      ui.column1LayoutComboBox->setEnabled(false);
      ui.column2LayoutComboBox->setEnabled(false);
  } else {
      ui.column1LayoutComboBox->setEnabled(true);
      ui.column2LayoutComboBox->setEnabled(true);
      ui.column1LayoutComboBox->addItems(layouts);
      ui.column2LayoutComboBox->addItems(layouts);
      ui.column1LayoutComboBox->setCurrentIndex(layouts.indexOf(m_tableModel->headerData(0, Qt::Horizontal, KWQTableModel::KeyboardLayoutRole).toString()));
      ui.column2LayoutComboBox->setCurrentIndex(layouts.indexOf(m_tableModel->headerData(1, Qt::Horizontal, KWQTableModel::KeyboardLayoutRole).toString()));
  }
  QSize s = m_tableModel->headerData(0, Qt::Horizontal, Qt::SizeHintRole).toSize();
  ui.column1WidthSpinBox->setValue(s.width());

  s = m_tableModel->headerData(1, Qt::Horizontal, Qt::SizeHintRole).toSize();
  ui.column2WidthSpinBox->setValue(s.width());

  if (dlg.exec() == QDialog::Accepted) {
    QString title1 = ui.column1TitleLineEdit->text();
    QString title2 = ui.column2TitleLineEdit->text();

    ColumnDataList result;
    ColumnData cd;

    cd.identifier = ui.column1TitleLineEdit->text();
    cd.layout = ui.column1LayoutComboBox->currentText();
    cd.width = ui.column1WidthSpinBox->value();
    result.append(cd);

    cd.identifier = ui.column2TitleLineEdit->text();
    cd.layout = ui.column2LayoutComboBox->currentText();
    cd.width = ui.column2WidthSpinBox->value();
    result.append(cd);

    KWQCommandIdentifiers *kwqc = new KWQCommandIdentifiers(m_tableView, result);
    m_undoStack->push(kwqc);

    list1.removeAll(title1);
    list1.prepend(title1);
    Prefs::setColumnTitles1(list1);

    list2.removeAll(title2);
    list2.prepend(title2);
    Prefs::setColumnTitles2(list2);
  }
}

void WordQuizApp::slotVocabFont()
{
  bool ok;
  QFont font = QFontDialog::getFont(&ok, Prefs::editorFont(), this);
  if (ok) {
    KWQCommandFont *kwqc = new KWQCommandFont(m_tableView, Prefs::editorFont(), font);
    m_undoStack->push(kwqc);
  }
}


void WordQuizApp::slotVocabImage()
{
  m_tableView->doVocabImage();
}


void WordQuizApp::slotVocabSound()
{
  m_tableView->doVocabSound();
}




void WordQuizApp::slotVocabAdjustRows()
{
  foreach(const QModelIndex &index, m_tableView->selectionModel()->selectedIndexes())
    m_tableView->adjustRow(index.row());
}


void WordQuizApp::slotVocabShuffle()
{
  m_tableView->doVocabShuffle();
}

void WordQuizApp::slotModeChange()
{
  int newMode;
  if (Prefs::mode() < 5)
    newMode = Prefs::mode() + 1;
  else
    newMode = 1;

  QAction *a = m_modeActionGroup->actions()[newMode - 1];
  a->activate(QAction::Trigger);
}

void WordQuizApp::slotModeActionGroupTriggered(QAction *act)
{
  if (m_quiz != 0) {
    int ret = QMessageBox::warning(this, tr("WordQuiz"), tr("This will restart your quiz. Do you wish to continue?"),
              QMessageBox::Yes | QMessageBox::No, QMessageBox::No); 
    if (ret == QMessageBox::No)
      {
        m_modeActionGroup->actions()[0]->setChecked(Prefs::mode() == 1);
        m_modeActionGroup->actions()[1]->setChecked(Prefs::mode() == 2);
        m_modeActionGroup->actions()[2]->setChecked(Prefs::mode() == 3);
        m_modeActionGroup->actions()[3]->setChecked(Prefs::mode() == 4);
        m_modeActionGroup->actions()[4]->setChecked(Prefs::mode() == 5);
        return;
      }
  }
  
  act->setChecked(true);

  QString s1 = m_tableModel->headerData(0, Qt::Horizontal, Qt::DisplayRole).toString();
  QString s2 = m_tableModel->headerData(1, Qt::Horizontal, Qt::DisplayRole).toString();

  m_modeActionGroup->actions()[0]->setText(tr("&1 %1 -> %2 In Order").arg(s1).arg(s2));
  m_modeActionGroup->actions()[1]->setText(tr("&2 %1 -> %2 In Order").arg(s2).arg(s1));
  m_modeActionGroup->actions()[2]->setText(tr("&3 %1 -> %2 Randomly").arg(s1).arg(s2));
  m_modeActionGroup->actions()[3]->setText(tr("&4 %1 -> %2 Randomly").arg(s2).arg(s1));
  m_modeActionGroup->actions()[4]->setText(tr("&5 %1 <-> %2 Randomly").arg(s1).arg(s2));

  m_modeActionGroup->actions()[0]->setShortcut(QKeySequence());
  m_modeActionGroup->actions()[1]->setShortcut(QKeySequence());
  m_modeActionGroup->actions()[2]->setShortcut(QKeySequence());
  m_modeActionGroup->actions()[3]->setShortcut(QKeySequence());
  m_modeActionGroup->actions()[4]->setShortcut(QKeySequence());

  Prefs::setMode(act->data().toInt());
  mode0->setIcon(act->icon());

  switch (Prefs::mode()){
  case 1:
    m_modeLabel->setText(tr("%1 -> %2 In Order").arg(s1).arg(s2));
    break;
  case 2:
    m_modeLabel->setText(tr("%1 -> %2 In Order").arg(s2).arg(s1));
    break;
  case 3:
    m_modeLabel->setText(tr("%1 -> %2 Randomly").arg(s1).arg(s2));
    break;
  case 4:
    m_modeLabel->setText(tr("%1 -> %2 Randomly").arg(s2).arg(s1));
    break;
  case 5:
    m_modeLabel->setText(tr("%1 <-> %2 Randomly").arg(s1).arg(s2));
    break;
  }

  if (m_quiz !=0)
    slotCurrentPageChanged(m_contentsWidget->currentItem(), m_contentsWidget->currentItem());
}

void WordQuizApp::slotQuizEditor()
{
  m_contentsWidget->setCurrentRow(0);
}

void WordQuizApp::slotQuizFlash()
{
  m_contentsWidget->setCurrentRow(1);
}

void WordQuizApp::slotQuizMultiple()
{
  m_contentsWidget->setCurrentRow(2);
}

void WordQuizApp::slotQuizQA()
{
  m_contentsWidget->setCurrentRow(3);
}

void WordQuizApp::slotCurrentPageChanged(QListWidgetItem *current, QListWidgetItem *before)
{
  Q_UNUSED(before);
  if (m_quiz != 0) {
    delete(m_quiz);
    m_quiz = 0;
  }
  disconnect(quizCheck, 0, 0, 0);
  disconnect(flashKnow, 0, 0, 0);
  disconnect(flashDontKnow, 0, 0, 0);
  disconnect(quizRestart, 0, 0, 0);
  disconnect(quizRepeatErrors, 0, 0, 0);
  disconnect(qaHint, 0, 0, 0);
  disconnect(quizPlayAudio, 0, 0, 0);
  disconnect(qaMarkLastCorrect, 0, 0, 0);

  int row = m_contentsWidget->row(current);
  m_pageWidget->setCurrentIndex(row);
  
  if (row == 0) {
    m_tableView->setFocus();
  }

  else  if (row == 1) {
    if (Prefs::enableBlanks()) {
      if (!m_tableModel->checkSyntax())
        m_pageWidget->setCurrentWidget(m_editorView);
    }
    m_quiz = new KWQQuizModel(this);
    m_quiz->setSourceModel(m_sortFilterModel);
    connect(m_quiz, SIGNAL(checkingAnswer(int )), m_tableView, SLOT(slotCheckedAnswer(int )));
    m_quiz ->setQuizType(Prefs::EnumStartSession::Flashcard);
    m_quiz->setQuizMode(Prefs::mode());
    if (m_quiz->init())
    {
      connect(quizCheck, SIGNAL(triggered(bool)), m_flashView, SLOT(slotCheck()));
      connect(flashKnow, SIGNAL(triggered(bool)), m_flashView, SLOT(slotKnow()));
      connect(flashDontKnow, SIGNAL(triggered(bool)), m_flashView, SLOT(slotDontKnow()));
      connect(quizPlayAudio, SIGNAL(triggered(bool)), m_flashView, SLOT(slotAudioPlay()));
      connect(quizRestart, SIGNAL(triggered(bool)), m_flashView, SLOT(slotRestart()));
      connect(quizRepeatErrors, SIGNAL(triggered(bool)), m_flashView, SLOT(slotRepeat()));
      connect(this, SIGNAL(settingsChanged()), m_flashView, SLOT(slotApplySettings()));

      m_flashView->setQuiz(m_quiz);
      m_flashView->init();
      m_flashView->stackedWidget->setCurrentIndex(0);
    }
    else
    {
      delete(m_quiz);
      m_quiz = 0;
      m_flashView->stackedWidget->setCurrentIndex(1);;
    }
  }

  else if (row == 2) {
    if (Prefs::enableBlanks()) {
      if (!m_tableModel->checkSyntax())
        m_pageWidget->setCurrentWidget(m_editorView);
    }
    m_quiz = new KWQQuizModel(this);
    m_quiz->setSourceModel(m_sortFilterModel);
    connect(m_quiz, SIGNAL(checkingAnswer(int )), m_tableView, SLOT(slotCheckedAnswer(int )));
    m_quiz ->setQuizType(Prefs::EnumStartSession::MultipleChoice);
    m_quiz->setQuizMode(Prefs::mode());
    if (m_quiz->init())
    {
      connect(quizCheck, SIGNAL(triggered(bool)), m_multipleView, SLOT(slotCheck()));
      connect(quizRestart, SIGNAL(triggered(bool)), m_multipleView, SLOT(slotRestart()));
      connect(quizRepeatErrors, SIGNAL(triggered(bool)), m_multipleView, SLOT(slotRepeat()));
      connect(this, SIGNAL(settingsChanged()), m_multipleView, SLOT(slotApplySettings()));

      m_multipleView->setQuiz(m_quiz);
      m_multipleView->init();
      m_multipleView->stackedWidget->setCurrentIndex(0);
    }
    else
    {
      delete(m_quiz);
      m_quiz = 0;
      m_multipleView->stackedWidget->setCurrentIndex(1);
    }
  }

  else if (row == 3) {
    if (Prefs::enableBlanks()) {
      if (!m_tableModel->checkSyntax())
        m_pageWidget->setCurrentWidget(m_editorView);
    }
    m_quiz = new KWQQuizModel(this);
    m_quiz->setSourceModel(m_sortFilterModel);
    connect(m_quiz, SIGNAL(checkingAnswer(int )), m_tableView, SLOT(slotCheckedAnswer(int )));
    m_quiz ->setQuizType(Prefs::EnumStartSession::QA);
    m_quiz->setQuizMode(Prefs::mode());
    if (m_quiz->init())
    {
      connect(quizCheck, SIGNAL(triggered(bool)), m_qaView, SLOT(slotCheck()));
      connect(qaHint, SIGNAL(triggered(bool)), m_qaView, SLOT(slotHint()));
      connect(qaMarkLastCorrect, SIGNAL(triggered(bool)), m_qaView, SLOT(slotMarkLastCorrect()));
      connect(quizRestart, SIGNAL(triggered(bool)), m_qaView, SLOT(slotRestart()));
      connect(quizPlayAudio, SIGNAL(triggered(bool)), m_flashView, SLOT(slotAudioPlay()));
      connect(quizRepeatErrors, SIGNAL(triggered(bool)), m_qaView, SLOT(slotRepeat()));
      connect(this, SIGNAL(settingsChanged()), m_qaView, SLOT(slotApplySettings()));

      m_qaView->setQuiz(m_quiz);
      m_qaView->init();
      m_qaView->stackedWidget->setCurrentIndex(0);
    }
    else
    {
      delete(m_quiz);
      m_quiz = 0;
      m_qaView->stackedWidget->setCurrentIndex(1);
    }
  }

  updateActions();
}

/** Configure notifications */
void WordQuizApp::slotConfigureNotifications( )
{
  //qtport KNotifyConfigWidget::configure(this);
}

/** Configure kwordquiz */
void WordQuizApp::slotConfigure()
{
  WordQuizPrefs *dlg = new WordQuizPrefs(this);
  connect(dlg, SIGNAL(settingsChanged()), this, SLOT(slotApplyPreferences()));
  dlg->exec();
  delete dlg;
}

void WordQuizApp::slotApplyPreferences()
{
  editMarkBlank->setEnabled(Prefs::enableBlanks());
  editUnmarkBlank->setEnabled(Prefs::enableBlanks());
  m_tableView->reset();
  updateSpecialCharIcons();
  emit settingsChanged();
}

void WordQuizApp::updateSpecialCharIcons()
{
  for (int i = 0; i < 9; i++){
    QAction * a = m_charActionGroup->actions()[i];
    a->setIcon(QIcon(charIcon(Prefs::specialCharacters()[i])));
    a->setIconText(tr("Insert %1").arg(Prefs::specialCharacters()[i]));
    a->setWhatsThis(tr("Inserts the character %1").arg(Prefs::specialCharacters()[i]));
    a->setToolTip(a->whatsThis());
    a->setStatusTip(a->whatsThis());
  }
}

QIcon WordQuizApp::charIcon(const QChar & c)
{
  QRect r(4, 4, 120, 120);

  ///A font to draw the character with
  QFont font("sans");
  font.setPixelSize(100);
  font.setWeight(QFont::Bold);

  ///Create the pixmap
  QPixmap pm(128, 128);
  pm.fill(Qt::white);
  QPainter p;
  p.begin(&pm);
  p.setFont(font);
  p.setPen(Qt::blue);
  p.drawText(r, Qt::AlignCenter, (QString) c);
  p.end();

  ///Create transparency mask
  QBitmap bm(128, 128);
  bm.fill(Qt::color0);
  QPainter b;
  b.begin(&bm);
  b.setFont(font);
  b.setPen(Qt::color1);
  b.drawText(r, Qt::AlignCenter, (QString) c);
  b.end();

  ///Mask the pixmap
  pm.setMask(bm);

  return QIcon(pm);
}

void WordQuizApp::slotCharActionGroupTriggered(QAction *act)
{
  if (m_pageWidget->currentWidget() == m_qaView)
    m_qaView->slotSpecChar(Prefs::specialCharacters()[act->data().toInt() - 1]);
  else
    if (m_pageWidget->currentWidget() == m_editorView) {
      m_tableView->slotSpecChar(Prefs::specialCharacters()[act->data().toInt() - 1]);
    }
}

void WordQuizApp::updateActions()
{
  bool fEdit = (m_pageWidget->currentWidget() == m_editorView);
  bool fQuiz = !fEdit && (m_quiz != 0);

  fileSave->setEnabled(fEdit);
  fileSaveAs->setEnabled(fEdit);
  filePrint->setEnabled(fEdit);

  editCopy->setEnabled(fEdit);
  editCut->setEnabled(fEdit);
  editPaste->setEnabled(fEdit);
  editClear->setEnabled(fEdit);
  editInsert->setEnabled(fEdit);
  editDelete->setEnabled(fEdit);
  editMarkBlank->setEnabled(fEdit && Prefs::enableBlanks());
  editUnmarkBlank->setEnabled(fEdit && Prefs::enableBlanks());
  vocabLanguages->setEnabled(fEdit);
  vocabFont->setEnabled(fEdit);
  vocabImage->setEnabled(fEdit);
  vocabSound->setEnabled(fEdit);
  vocabAdjustRows->setEnabled(fEdit);
  vocabShuffle->setEnabled(fEdit);

  quizCheck->setEnabled(fQuiz);
  quizRestart->setEnabled(fQuiz);
  quizRepeatErrors->setEnabled(false);
  quizExportErrors->setEnabled(false);
  quizPlayAudio->setEnabled(false);
  flashKnow->setEnabled((m_pageWidget->currentWidget() == m_flashView) && fQuiz);
  flashDontKnow->setEnabled((m_pageWidget->currentWidget() == m_flashView) && fQuiz);

  qaHint->setEnabled((m_pageWidget->currentWidget() == m_qaView) && fQuiz);
  qaMarkLastCorrect->setVisible((m_pageWidget->currentWidget() == m_qaView) && fQuiz);
  configShowSearchBar->setEnabled(fEdit);

  //addToolBar(Qt::RightToolBarArea, toolBar("quizToolBar"));
  toolBarQuiz->setHidden(fEdit);
  m_searchWrapper->searchBox()->setEnabled(fEdit);
}

void WordQuizApp::slotConfigShowSearch()
{
  //
}

void WordQuizApp::slotConfigShowStatusbar()
{
  statusBar()->setVisible(statusBar()->isHidden());
  Prefs::setShowStatusbar(statusBar()->isVisible());
  configShowStatusbar->setChecked(statusBar()->isVisible());
}

void WordQuizApp::slotCleanChanged(bool clean)
{
  m_doc->setModified(!clean);
  setCaption(m_doc->url().toLocalFile(), m_doc->isModified());
}


void WordQuizApp::slotUndoTextChanged(const QString & undoText)
{
  //Q_UNUSED(undoText);
  menuEdit->actions().at(0)->setWhatsThis(tr("Undo %1").arg(undoText));
  menuEdit->actions().at(0)->setStatusTip(tr("Undo %1").arg(undoText));

  QAction *a = m_modeActionGroup->actions()[Prefs::mode() - 1];
  slotModeActionGroupTriggered(a);
}


void WordQuizApp::slotRedoTextChanged(const QString &redoText)
{
  //Q_UNUSED(redoText);
  menuEdit->actions().at(1)->setWhatsThis(tr("Redo %1").arg(redoText));
  menuEdit->actions().at(1)->setStatusTip(tr("Redo %1").arg(redoText));

  QAction *a = m_modeActionGroup->actions()[Prefs::mode() - 1];
  slotModeActionGroupTriggered(a);
}

void WordQuizApp::slotCreateErrorListDocument()
{
  if (m_quiz) {
    KEduVocDocument *errorDoc = new KEduVocDocument(this);
    errorDoc->appendIdentifier();
    errorDoc->identifier(0).setName(m_tableModel->headerData(0, Qt::Horizontal, Qt::DisplayRole).toString());
    errorDoc->appendIdentifier();
    errorDoc->identifier(1).setName(m_tableModel->headerData(1, Qt::Horizontal, Qt::DisplayRole).toString());

    foreach (int item, m_quiz->errorList()) {
      KEduVocExpression *errorExpr = new KEduVocExpression();
      errorDoc->lesson()->appendEntry(errorExpr);
      errorExpr->setTranslation(0, m_quiz->data(m_quiz->index(item, 0), Qt::DisplayRole).toString());
      errorExpr->setTranslation(1, m_quiz->data(m_quiz->index(item, 1), Qt::DisplayRole).toString());
    }
    saveDocAsFileName(errorDoc);
    //qtport if (m_dirWatch->contains(errorDoc->url().path()))
      //m_dirWatch->removeFile(errorDoc->url().path());
  }
}

void WordQuizApp::setCaption(const QString &fullFileName, bool modified)
{
  QString windowTitle;
  windowTitle.append(QFileInfo(fullFileName).fileName());
  if (modified)
     windowTitle.append(tr(" [modified]"));
  windowTitle.append(tr(" - WordQuiz"));
  setWindowTitle(windowTitle);
  setWindowModified(modified);
  m_reallyClose = !modified;
}


void WordQuizApp::updateRecentFileActions()
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

void WordQuizApp::addRecentFile(const QString &fileName)
{
  QStringList files = Prefs::recentFiles();
  files.removeAll(fileName);
  files.prepend(fileName);
  while (files.size() > 10)
    files.removeLast();

  Prefs::setRecentFiles(files);

  foreach (QWidget *widget, QApplication::topLevelWidgets()) {
      WordQuizApp *mainWin = qobject_cast<WordQuizApp *>(widget);
      if (mainWin)
          mainWin->updateRecentFileActions();
  }
}

void WordQuizApp::slotDockActivated() {
  show();
  raise();
  activateWindow();
}

