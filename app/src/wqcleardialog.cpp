/*
    This file is part of WordQuiz
    Copyright (C) 2010 Peter Hedlund <peter@peterandlinda.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include "wqcleardialog.h"
#include "ui_wqcleardialog.h"

#include "prefs.h"

WQClearDialog::WQClearDialog(QWidget *parent) : QDialog(parent), ui(new Ui::WQClearDialog)
{
    ui->setupUi(this);

    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->addButton(ui->clearAllCheckBox);
    m_buttonGroup->addButton(ui->clearTextCheckBox);
    m_buttonGroup->addButton(ui->clearImageCheckBox);
    m_buttonGroup->addButton(ui->clearSoundCheckBox);
    m_buttonGroup->setExclusive(false);

    connect(m_buttonGroup, SIGNAL(buttonClicked(int )), this, SLOT(buttonGroupChanged(int)));

    updateCheckBoxes();
}

WQClearDialog::~WQClearDialog()
{
    delete m_buttonGroup;
    delete ui;
}

void WQClearDialog::buttonGroupChanged(int id)
{
    Q_UNUSED(id);
    Prefs::setClearAll(ui->clearAllCheckBox->isChecked());
    Prefs::setClearText(ui->clearTextCheckBox->isChecked());
    Prefs::setClearImageLink(ui->clearImageCheckBox->isChecked());
    Prefs::setClearSoundLink(ui->clearSoundCheckBox->isChecked());
    updateCheckBoxes();
}

void WQClearDialog::updateCheckBoxes()
{
    bool all = Prefs::clearAll();
    ui->clearAllCheckBox->setChecked(all);
    ui->clearTextCheckBox->setChecked(Prefs::clearText());
    ui->clearImageCheckBox->setChecked(Prefs::clearImageLink());
    ui->clearSoundCheckBox->setChecked(Prefs::clearSoundLink());

    ui->clearTextCheckBox->setEnabled(!all);
    ui->clearImageCheckBox->setEnabled(!all);
    ui->clearSoundCheckBox->setEnabled(!all);
}

void WQClearDialog::accept()
{
    Prefs::setClearAll(ui->clearAllCheckBox->isChecked());
    Prefs::setClearText(ui->clearTextCheckBox->isChecked());
    Prefs::setClearImageLink(ui->clearImageCheckBox->isChecked());
    Prefs::setClearSoundLink(ui->clearSoundCheckBox->isChecked());
    QDialog::accept();
}


