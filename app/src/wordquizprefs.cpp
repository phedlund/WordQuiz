/***************************************************************************
                          wordquizprefs.cpp  -  description
                             -------------------
    begin                : Sun Apr 13 2008
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

#include "wordquizprefs.h"

#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtGui/QCheckBox>
#include <QtGui/QListWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDesktopServices>
#include <QtGui/QScrollArea>

#include "prefs.h"
#include "ui_dlgcharacter.h"

WordQuizPrefs::WordQuizPrefs(QWidget *parent) : QDialog(parent)
{
  setupUi(this);

  m_contentsWidget->setViewMode(QListView::IconMode);
  m_contentsWidget->setIconSize(QSize(96, 48));
  m_contentsWidget->setMovement(QListView::Static);
  m_contentsWidget->setFixedWidth(112);
  m_contentsWidget->setSpacing(6);

  QListWidgetItem *generalButton = new QListWidgetItem(m_contentsWidget);
  generalButton->setIcon(QIcon(":/src/pics/list-app-kwordquiz.png"));
  generalButton->setText(tr("General"));
  generalButton->setTextAlignment(Qt::AlignHCenter);
  generalButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  QListWidgetItem *editorButton = new QListWidgetItem(m_contentsWidget);
  editorButton->setIcon(QIcon(":/src/pics/list-action-editor.png"));
  editorButton->setText(tr("Editor"));
  editorButton->setTextAlignment(Qt::AlignHCenter);
  editorButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  QListWidgetItem *quizButton = new QListWidgetItem(m_contentsWidget);
  quizButton->setIcon(QIcon(":/src/pics/list-action-qa.png"));
  quizButton->setText(tr("Quiz"));
  quizButton->setTextAlignment(Qt::AlignHCenter);
  quizButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  QListWidgetItem *flashcardButton = new QListWidgetItem(m_contentsWidget);
  flashcardButton->setIcon(QIcon(":/src/pics/list-action-flash.png"));
  flashcardButton->setText(tr("Flashcard\nAppearance"));
  flashcardButton->setTextAlignment(Qt::AlignHCenter);
  flashcardButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  QListWidgetItem *characterButton = new QListWidgetItem(m_contentsWidget);
  characterButton->setIcon(QIcon(":/src/pics/list-accessories-character-map.png"));
  characterButton->setText(tr("Special\nCharacters"));
  characterButton->setTextAlignment(Qt::AlignHCenter);
  characterButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  QFont f = m_contentsWidget->font();
  f.setWeight(QFont::Bold);
  m_contentsWidget->setFont(f);

  //General
  m_startSessionGroup = new QButtonGroup(this);
  m_startSessionGroup->addButton(optEditor, Prefs::EnumStartSession::Editor);
  m_startSessionGroup->addButton(optFlashcard, Prefs::EnumStartSession::Flashcard);
  m_startSessionGroup->addButton(optMultipleChoice, Prefs::EnumStartSession::MultipleChoice);
  m_startSessionGroup->addButton(optQA, Prefs::EnumStartSession::QA);
  connect(m_startSessionGroup, SIGNAL(buttonClicked(int )), this, SLOT(buttonGroupClick(int )));

  //Editor
  m_enterMoveGroup = new QButtonGroup(this);
  m_enterMoveGroup->addButton(optDown, 0);
  m_enterMoveGroup->addButton(optRight, 1);
  m_enterMoveGroup->addButton(optNoMove, 2);
  connect(m_enterMoveGroup, SIGNAL(buttonClicked(int )), this, SLOT(buttonGroupClick(int )));
  connect(kcfg_EnableBlanks, SIGNAL(clicked()), this, SLOT(updateButtons()));

  //Quiz
  m_keepDiscardGroup = new QButtonGroup(this);
  m_keepDiscardGroup->addButton(optCorrect, 0);
  m_keepDiscardGroup->addButton(optError, 1);
  spinFlipDelay->setEnabled(kcfg_AutoFlip->isChecked());
  optError->setEnabled(kcfg_AutoFlip->isChecked());
  optCorrect->setEnabled(kcfg_AutoFlip->isChecked());
  connect(kcfg_AutoFlip, SIGNAL(clicked()), this, SLOT(updateButtons()));
  connect(spinFlipDelay, SIGNAL(valueChanged(int )), this, SLOT(buttonGroupClick(int)));
  connect(m_keepDiscardGroup, SIGNAL(buttonClicked(int )), this, SLOT(buttonGroupClick(int )));
  connect(kcfg_AutoCheck, SIGNAL(clicked()), this, SLOT(updateButtons()));
  connect(kcfg_HintError, SIGNAL(clicked()), this, SLOT(updateButtons()));
  connect(kcfg_Percent, SIGNAL(clicked()), this, SLOT(updateButtons()));

  //Flashcard
  connect(flipButton, SIGNAL(clicked()), this, SLOT(slotFlipButtonClicked()));
  connect(m_frontFont, SIGNAL(fontSelected(const QFont&)), this, SLOT(slotFontChanged(const QFont&)));
  connect(m_frontTextColor, SIGNAL(changed(const QColor&)), this, SLOT(slotTextColorChanged(const QColor&)));
  connect(m_frontCardColor, SIGNAL(changed(const QColor&)), this, SLOT(slotCardColorChanged(const QColor&)));
  connect(m_frontFrameColor, SIGNAL(changed(const QColor&)), this, SLOT(slotFrameColorChanged(const QColor&)));
  connect(m_backFont, SIGNAL(fontSelected(const QFont&)), this, SLOT(slotFontChanged(const QFont&)));
  connect(m_backTextColor, SIGNAL(changed(const QColor&)), this, SLOT(slotTextColorChanged(const QColor&)));
  connect(m_backCardColor, SIGNAL(changed(const QColor&)), this, SLOT(slotCardColorChanged(const QColor&)));
  connect(m_backFrameColor, SIGNAL(changed(const QColor&)), this, SLOT(slotFrameColorChanged(const QColor&)));
  connect(widgetStack, SIGNAL(currentChanged(int)), this, SLOT(slotCurrentChanged(int)));

  widgetStack->setCurrentWidget(frontStackPage);
  
  //Special Characters
  QStringList sl;
  sl << tr("Action");
  sl << tr("Shortcut");
  sl << tr("Character");
  CharacterTree->setHeaderLabels(sl);

  QString sc = Prefs::specialCharacters();
  QTreeWidgetItem * twi;

  for (int i = 0; i < 9; i++)
  {
    twi = new QTreeWidgetItem(CharacterTree);
    twi->setText(0, tr("Special Character %1").arg(i + 1));
    twi->setText(1, tr("CTRL + %1").arg(QString::number(i + 1)));
    twi->setText(2, QString(sc[i]));
  }
  CharacterTree->resizeColumnToContents(0);
  CharacterTree->setItemSelected(CharacterTree->topLevelItem(0), true);
  CharacterTree->setCurrentItem(CharacterTree->topLevelItem(0));
  lblPreview->setText(CharacterTree->topLevelItem(0)->text(2));

  connect(CharacterTree, SIGNAL(itemSelectionChanged()), this, SLOT(slotCharListSelectionChanged()));
  connect(btnCharacter, SIGNAL(clicked()), this, SLOT(slotSelectSpecChar()));

  //Done
  connect(buttonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(dialogButtonClicked(QAbstractButton *)));
  updateWidgets();
  // initialize the buddies, etc.
  slotCurrentChanged(widgetStack->indexOf(frontStackPage));
  updateButtons();
}

bool WordQuizPrefs::hasChanged()
{
  QString sc = Prefs::specialCharacters();
  QString s;
  for (int i = 0; i < 9; i++)
    s.append(CharacterTree->topLevelItem(i)->text(2));

  return m_startSessionGroup->checkedId() != Prefs::startSession()    ||
         m_enterMoveGroup->checkedId()    != Prefs::enterMove()       ||
         kcfg_EnableBlanks->isChecked()   != Prefs::enableBlanks()    ||
         kcfg_AutoFlip->isChecked()       != Prefs::autoFlip()        ||
         spinFlipDelay->value()           != Prefs::flipDelay()       ||
         m_keepDiscardGroup->checkedId()  != Prefs::keepDiscard()     ||
         kcfg_AutoCheck->isChecked()      != Prefs::autoCheck()       ||
         kcfg_HintError->isChecked()      != Prefs::hintError()       ||
         kcfg_Percent->isChecked()        != Prefs::percent()         ||
         m_frontFont->font()              != Prefs::frontFont()       ||
         m_frontTextColor->color()        != Prefs::frontTextColor()  ||
         m_frontCardColor->color()        != Prefs::frontCardColor()  ||
         m_frontFrameColor->color()       != Prefs::frontFrameColor() ||
         m_backFont->font()               != Prefs::backFont()        ||
         m_backTextColor->color()         != Prefs::backTextColor()   ||
         m_backCardColor->color()         != Prefs::backCardColor()   ||
         m_backFrameColor->color()        != Prefs::backFrameColor()  ||
         sc.simplified()                  != s.simplified();
}

bool WordQuizPrefs::isDefault()
{
  QString s;
  for (int i = 0; i < 9; i++)
    s.append(CharacterTree->topLevelItem(i)->text(2));

  return m_startSessionGroup->checkedId() == Prefs::EnumStartSession::Editor &&
         m_enterMoveGroup->checkedId()    == 1                               &&
         kcfg_EnableBlanks->isChecked()   == false                           &&
         kcfg_AutoFlip->isChecked()       == false                           &&
         spinFlipDelay->value()           == 3                               &&
         m_keepDiscardGroup->checkedId()  == 1                               &&
         kcfg_AutoCheck->isChecked()      == false                           &&
         kcfg_HintError->isChecked()      == false                           &&
         kcfg_Percent->isChecked()        == false                           &&
         m_frontFont->font()              == QFont()                         &&
         m_frontTextColor->color()        == QColor(Qt::black)               &&
         m_frontCardColor->color()        == QColor(Qt::white)               &&
         m_frontFrameColor->color()       == QColor(Qt::red)                 &&
         m_backFont->font()               == QFont()                         &&
         m_backTextColor->color()         == QColor(Qt::black)               &&
         m_backCardColor->color()         == QColor(Qt::white)               &&
         m_backFrameColor->color()        == QColor(Qt::blue)                &&
         s.simplified()                   == QString("abcdefghi").simplified();
}

void WordQuizPrefs::updateButtons()
{
  buttonBox->button(QDialogButtonBox::Apply)->setEnabled(hasChanged());
  buttonBox->button(QDialogButtonBox::RestoreDefaults)->setEnabled(!isDefault());
}

void WordQuizPrefs::updateSettings( )
{
  //General
  Prefs::setStartSession(m_startSessionGroup->checkedId());

  //Editor
  Prefs::setEnterMove(m_enterMoveGroup->checkedId());
  Prefs::setEnableBlanks(kcfg_EnableBlanks->isChecked());

  //Quiz
  Prefs::setAutoFlip(kcfg_AutoFlip->isChecked());
  Prefs::setFlipDelay(spinFlipDelay->value());
  Prefs::setKeepDiscard(m_keepDiscardGroup->checkedId());
  Prefs::setAutoCheck(kcfg_AutoCheck->isChecked());
  Prefs::setHintError(kcfg_HintError->isChecked());
  Prefs::setPercent(kcfg_Percent->isChecked());

  //FlashCard
  Prefs::setFrontFont(m_frontFont->font());
  Prefs::setFrontTextColor(m_frontTextColor->color());
  Prefs::setFrontCardColor(m_frontCardColor->color());
  Prefs::setFrontFrameColor(m_frontFrameColor->color());
  Prefs::setBackFont(m_backFont->font());
  Prefs::setBackTextColor(m_backTextColor->color());
  Prefs::setBackCardColor(m_backCardColor->color());
  Prefs::setBackFrameColor(m_backFrameColor->color());

  //Special Characters
  QString s;
  for (int i = 0; i < 9; i++)
    s.append(CharacterTree->topLevelItem(i)->text(2));

  Prefs::setSpecialCharacters(s.simplified());

  //Done
  emit settingsChanged();
}

void WordQuizPrefs::updateWidgets()
{
  //General
  m_startSessionGroup->button(Prefs::startSession())->setChecked(true);

  //Editor
  m_enterMoveGroup->button(Prefs::enterMove())->setChecked(true);
  kcfg_EnableBlanks->setChecked(Prefs::enableBlanks());

  //Quiz
  kcfg_AutoFlip->setChecked(Prefs::autoFlip());
  spinFlipDelay->setValue(Prefs::flipDelay());
  m_keepDiscardGroup->button(Prefs::keepDiscard())->setChecked(true);
  kcfg_AutoCheck->setChecked(Prefs::autoCheck());
  kcfg_HintError->setChecked(Prefs::hintError());
  kcfg_Percent->setChecked(Prefs::percent());

  //FlashCard
  m_frontFont->setFont(Prefs::frontFont());
  m_frontTextColor->setColor(Prefs::frontTextColor());
  m_frontCardColor->setColor(Prefs::frontCardColor());
  m_frontFrameColor->setColor(Prefs::frontFrameColor());
  m_backFont->setFont(Prefs::backFont());
  m_backTextColor->setColor(Prefs::backTextColor());
  m_backCardColor->setColor(Prefs::backCardColor());
  m_backFrameColor->setColor(Prefs::backFrameColor());

  //Special Characters
  QString sc = Prefs::specialCharacters();
  for (int i = 0; i < 9; i++)
    CharacterTree->topLevelItem(i)->setText(2, QString(sc[i]));

  //Done
}

void WordQuizPrefs::updateWidgetsDefault()
{
  //General
  m_startSessionGroup->button(Prefs::EnumStartSession::Editor)->setChecked(true);

  //Editor
  m_enterMoveGroup->button(1)->setChecked(true);
  kcfg_EnableBlanks->setChecked(false);

  //Quiz
  kcfg_AutoFlip->setChecked(false);
  spinFlipDelay->setValue(3);
  m_keepDiscardGroup->button(1)->setChecked(true);
  kcfg_AutoCheck->setChecked(false);
  kcfg_HintError->setChecked(false);
  kcfg_Percent->setChecked(false);

  //FlashCard
  m_frontFont->setFont(QFont());
  m_frontTextColor->setColor(QColor(Qt::black));
  m_frontCardColor->setColor(QColor(Qt::white));
  m_frontFrameColor->setColor(QColor(Qt::red));
  m_backFont->setFont(QFont());
  m_backTextColor->setColor(QColor(Qt::black));
  m_backCardColor->setColor(QColor(Qt::white));
  m_backFrameColor->setColor(QColor(Qt::blue));

  //Special Characters
  QString sc("abcdefghi");
  for (int i = 0; i < 9; i++)
    CharacterTree->topLevelItem(i)->setText(2, QString(sc[i]));
}

void WordQuizPrefs::slotFlipButtonClicked( )
{
  if (widgetStack->currentWidget() == frontStackPage)
  {
    widgetStack->setCurrentWidget(backStackPage);
  }
  else
  {
    widgetStack->setCurrentWidget(frontStackPage);
  }
}

void WordQuizPrefs::slotFontChanged(const QFont & font)
{
  flashcard->setTextFont(font);
  updateButtons();
}

void WordQuizPrefs::slotTextColorChanged( const QColor & color)
{
  flashcard->setTextColor(color);
  updateButtons();
}

void WordQuizPrefs::slotCardColorChanged( const QColor & color)
{
  flashcard->setCardColor(color);
  updateButtons();
}

void WordQuizPrefs::slotFrameColorChanged( const QColor & color)
{
  flashcard->setFrameColor(color);
  updateButtons();
}

void WordQuizPrefs::slotCurrentChanged(int index)
{
  if (index == widgetStack->indexOf(backStackPage))
  {
    flashcard->setIdentifier(tr("Back"));
    flashcard->setText(tr("Answer"));
    flashcard->setTextFont(m_backFont->font());
    flashcard->setTextColor(m_backTextColor->color());
    flashcard->setCardColor(m_backCardColor->color());
    flashcard->setFrameColor(m_backFrameColor->color());

    fontLabel->setBuddy(m_backFont);
    textColorLabel->setBuddy(m_backTextColor);
    frameColorLabel->setBuddy(m_backFrameColor);
    cardColorLabel->setBuddy(m_backCardColor);
  }
  else
  {
    flashcard->setIdentifier(tr("Front"));
    flashcard->setText(tr("Question"));
    flashcard->setTextFont(m_frontFont->font());
    flashcard->setTextColor(m_frontTextColor->color());
    flashcard->setCardColor(m_frontCardColor->color());
    flashcard->setFrameColor(m_frontFrameColor->color());

    fontLabel->setBuddy(m_frontFont);
    textColorLabel->setBuddy(m_frontTextColor);
    frameColorLabel->setBuddy(m_frontFrameColor);
    cardColorLabel->setBuddy(m_frontCardColor);
  }
}

void WordQuizPrefs::slotCharListSelectionChanged( )
{
  if (CharacterTree->currentItem())
    lblPreview->setText(CharacterTree->currentItem()->text(2));
}

void WordQuizPrefs::slotSelectSpecChar()
{
  QString s = CharacterTree->currentItem()->text(2);
  QChar c = s[0];
  
  QDialog dlg;
  Ui::dlgCharacter ui;
  ui.setupUi(&dlg);

  QScrollArea* sa = new QScrollArea(ui.mainFrame);
  ui.widgetCharacter->setParent(0);
  sa->setWidget(ui.widgetCharacter);
  sa->setFrameStyle(QFrame::NoFrame);
  ui.mainFrame->layout()->addWidget(sa);
  ui.widgetCharacter->updateFont(Prefs::editorFont());
  ui.widgetCharacter->setFocus();
  connect(ui.widgetCharacter, SIGNAL(characterSelected(const QString &)), this, SLOT(slotSpecChar(const QString &)));
  if (dlg.exec() == QDialog::Accepted) {
     CharacterTree->currentItem()->setText(2, m_selectedCharacter);
     lblPreview->setText(CharacterTree->currentItem()->text(2));
     updateButtons();
  }
}

void WordQuizPrefs::slotSpecChar(const QString &c)
{
  m_selectedCharacter = c;
}

void WordQuizPrefs::buttonGroupClick(int id)
{
  Q_UNUSED(id);
  updateButtons();
}

void WordQuizPrefs::dialogButtonClicked(QAbstractButton * button)
{
  switch (buttonBox->buttonRole(button)) {
    case QDialogButtonBox::ApplyRole: {
      updateSettings();
      updateButtons();
      break;
    }
    case QDialogButtonBox::ResetRole: {
      updateWidgetsDefault();
      updateButtons();
      break;
    }
    case QDialogButtonBox::HelpRole: {
      QDesktopServices::openUrl(QUrl("http://docs.kde.org/development/en/kdeedu/kwordquiz/"));
      break;
    }
    case QDialogButtonBox::AcceptRole: {
      if (buttonBox->button(QDialogButtonBox::Apply)->isEnabled())
        updateSettings();
      break;
    }
    default: {
      //
    }
  }
}
