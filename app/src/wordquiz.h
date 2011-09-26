/***************************************************************************
                               wordquiz.h
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

#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H
//
#include <QtGui/QMainWindow>
#include <QtGui/QMessageBox>

#include <keduvocdocument.h>

#include "kwqtableview.h"
#include "kwqtablemodel.h"
#include "kwqsortfiltermodel.h"
#include "version.h"
#include "searchwidgetcontainer.h"

#include "ui_mainwindow.h"

class QLabel;
class QListWidget;
class QListWidgetItem;
class QStackedWidget;
class QLineEdit;
class QUndoStack;
class QActionGroup;

class KWQQuizModel;
class FlashView;
class MultipleView;
class QAView;

class WordQuizApp : public QMainWindow, public Ui::MainWindow
{
Q_OBJECT
  public:
    WordQuizApp( QWidget * parent = 0, Qt::WFlags f = 0 );
    
    /**
     * opens a file specified by commandline option
     * @param url the URL to be opened
     */
    void openDocumentFile(const QUrl& url=QUrl());
    void openUrl(const QUrl& url);
    /**
     * @param document the @see KEduVocDocument to save
     * @return whether the saving was successful
     */
    bool saveDocAsFileName(KEduVocDocument * document);

    void updateRecentFileActions();

  protected:
    /** save general Options like all bar positions and status as well as the geometry and the recent file list to the configuration
     * file
     */
    void saveOptions();
    /** read general Options again and initialize all variables like the recent file list
     */
    void readOptions();
    /** initializes the KActions of the application */

    void initActions();
    /** sets up the statusbar for the main window by initialzing a statuslabel.
     */
    void initStatusBar();
    /** initializes the document object of the main window that is connected to the view in initView().
     * @see initView();
     */
    void initDocument();
    /** creates a data model that reads and writes data from the document and displays it in a view
     * @see initDocument();
     * @see initView();
     */
    void initModel();
    /** creates the centerwidget of the KXmlGuiWindow instance and sets it as the view
     */
    void initView();
    /** queryClose is called by KXmlGuiWindow on each closeEvent of a window. Against the
     * default implementation (only returns true), this calles saveModified() on the document object to ask if the document shall
     * be saved if Modified; on cancel the closeEvent is rejected.
     * @see KXmlGuiWindow#queryClose
     * @see KXmlGuiWindow#closeEvent
     */
    virtual bool queryClose();
    /** queryExit is called by KXmlGuiWindow when the last window of the application is going to be closed during the closeEvent().
     * Against the default implementation that just returns true, this calls saveOptions() to save the settings of the last window's
     * properties.
     * @see KXmlGuiWindow#queryExit
     * @see KXmlGuiWindow#closeEvent
     */
    virtual bool queryExit();

    void closeEvent(QCloseEvent *event);

  signals:
    void settingsChanged();

  private slots:
    /** open a new application window by creating a new instance of KWordQuizApp */
    void slotFileNew();
    /** open a file and load it into the document*/
    void slotFileOpen();
    /** opens a file from the recent files menu */
    void slotFileOpenRecent(QAction *);
    /** download vocabularies from the Internet */
    void slotFileGHNS();
    /** save a document */
    void slotFileSave();
    /** save a document by a new filename*/
    void slotFileSaveAs();
    /** asks for saving if the file is modified, then closes the actual file and window*/
    void slotFileClose();
    /** print the actual file */
    void slotFilePrint();
    /** preview the appearance of the vocabulary as printed */
    void slotFilePrintPreview();
    /** closes all open windows by calling close() on each memberList item until the list is empty, then quits the application.
     * If queryClose() returns false because the user canceled the saveModified() dialog, the closing breaks.*/
    void slotFileQuit();

    /** put the marked text/object into the clipboard and remove it from the document */
    void slotEditCut();
    /** put the marked text/object into the clipboard */
    void slotEditCopy();
    /** paste the clipboard into the document */
    void slotEditPaste();
    /** clear the grid selection */
    void slotEditClear();
    /** insert a new row */
    void slotEditInsert();
    /** delete selected row(s) */
    void slotEditDelete();
    /** mark word as blank */
    void slotEditMarkBlank();
    /** unmark word as blank */
    void slotEditUnmarkBlank();
    /** find and replace text */
    void slotEditFind(const QString &);

    /** define vocabulary languages */
    void slotVocabLanguages();
    /** define vocabulary fonts */
    void slotVocabFont();
    /** link an image to the current entry */
    void slotVocabImage();
    /** link a sound to the current entry */
    void slotVocabSound();
    /** automatically adjust row heights */
    void slotVocabAdjustRows();
    /** shuffle the vocabulary */
    void slotVocabShuffle();

    void slotModeChange();
    void slotModeActionGroupTriggered(QAction *);

    void slotCurrentPageChanged(QListWidgetItem *current, QListWidgetItem *before);
    /** editor session */
    void slotQuizEditor();
    /** flashcard session */
    void slotQuizFlash();
    /** multiple choice session */
    void slotQuizMultiple();
    /** question and answer session */
    void slotQuizQA();
    /** create a new vocabulary document from quiz errors */
    void slotCreateErrorListDocument();

    void slotConfigShowSearch();
    void slotConfigShowStatusbar();
    /** configure notifications */
    void slotConfigureNotifications();
    /** configure kwordquiz */
    void slotConfigure();
    /** applies changes from the preferences dialog */
    void slotApplyPreferences();

    void slotCharActionGroupTriggered(QAction *);

    void slotCleanChanged(bool);
    void slotUndoTextChanged(const QString &);
    void slotRedoTextChanged(const QString &);

    void slotDockActivated();
    void finishClose(int );

  private:
    void createIcons();
    
    void setCaption(const QString &fileName, bool modified);
    void addRecentFile(const QString &fileName);
    void updateSpecialCharIcons();
    QIcon charIcon(const QChar &);
    void updateActions();

    QLabel * m_modeLabel;
    /** m_doc represents your actual document and is created only once. It keeps
     * information such as filename and does the serialization of your files.
     */
    KEduVocDocument * m_doc;
    KWQTableModel *m_tableModel;
    KWQSortFilterModel *m_sortFilterModel;
    KWQTableView *m_tableView;
    FlashView *m_flashView;
    MultipleView *m_multipleView;
    QAView *m_qaView;
    
    QListWidget *m_contentsWidget;
    QStackedWidget *m_pageWidget;
    QUndoStack *m_undoStack;
    QWidget *m_editorView;
    QWidget *m_editorPage;
    QActionGroup *m_modeActionGroup;
    QActionGroup *m_charActionGroup;
    QActionGroup *m_recentFilesActionGroup;
    
    KWQQuizModel *m_quiz;
    QMessageBox *m_messageBox;
    bool m_reallyClose;
    SearchWrapper *m_searchWrapper;
};
#endif
