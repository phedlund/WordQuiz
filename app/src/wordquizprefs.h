/***************************************************************************
                          wordquizprefs.h  -  description
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

#ifndef WORDQUIZPREFS_H
#define WORDQUIZPREFS_H

#include "ui_dlgconfig.h"

#include <QtGui/QDialog>
#include <QtGui/QButtonGroup>

#include "kcolorbutton.h"
#include "kfontrequester.h"

/**
  *@author Peter Hedlund
*/

class WordQuizPrefs : public QDialog, Ui::dlgConfig
{
  Q_OBJECT
public:
  WordQuizPrefs(QWidget *parent);

signals:
  void settingsChanged();

protected slots:
  void updateButtons();
  void updateSettings();
  void updateWidgets();
  void updateWidgetsDefault();

  void slotFlipButtonClicked();
  void slotFontChanged(const QFont &);
  void slotTextColorChanged(const QColor &);
  void slotCardColorChanged(const QColor &);
  void slotFrameColorChanged(const QColor &);
  void slotCurrentChanged(int);

  void slotCharListSelectionChanged();
  void slotSelectSpecChar();
  void slotSpecChar(const QString &);

  void buttonGroupClick(int );

  void dialogButtonClicked(QAbstractButton * button);

protected:
  bool hasChanged();
  bool isDefault();

private:
  QButtonGroup *m_startSessionGroup;
  QButtonGroup *m_enterMoveGroup;
  QButtonGroup *m_keepDiscardGroup;

  QString m_selectedCharacter;
};

#endif
