/***************************************************************************
 *   Copyright (C) 2001 by Bernd Gehrmann                                  *
 *   bernd@kdevelop.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <kbuttonbox.h>
#include <klocale.h>

#include "addfilegroupdlg.h"


AddFileGroupDialog::AddFileGroupDialog(QWidget *parent, const char *name)
    : QDialog(parent, name, true)
{
    setCaption(("Add file group"));
    
    QLabel *title_label = new QLabel(i18n("&Title:"), this);
    title_edit = new QLineEdit(this);
    title_edit->setFocus();
    title_label->setBuddy(title_edit);

    QLabel *pattern_label = new QLabel(i18n("&Pattern:"), this);
    pattern_edit = new QLineEdit(this);
    pattern_label->setBuddy(pattern_edit);
    QFontMetrics fm(pattern_edit->fontMetrics());
    pattern_edit->setMinimumWidth(fm.width('X')*35);
    
    QVBoxLayout *layout = new QVBoxLayout(this, 10);
    
    QGridLayout *grid = new QGridLayout(2, 2);
    layout->addLayout(grid);
    grid->addWidget(title_label, 0, 0);
    grid->addWidget(title_edit, 0, 1);
    grid->addWidget(pattern_label, 1, 0);
    grid->addWidget(pattern_edit, 1, 1);

    QFrame *frame = new QFrame(this);
    frame->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    layout->addWidget(frame, 0);

    KButtonBox *buttonbox = new KButtonBox(this);
    buttonbox->addStretch();
    QPushButton *ok = buttonbox->addButton(i18n("&OK"));
    QPushButton *cancel = buttonbox->addButton(i18n("Cancel"));
    ok->setDefault(true);
    connect( ok, SIGNAL(clicked()), this, SLOT(accept()) );
    connect( cancel, SIGNAL(clicked()), this, SLOT(reject()) );
    buttonbox->layout();
    layout->addWidget(buttonbox, 0);
}


AddFileGroupDialog::~AddFileGroupDialog()
{}

#include "addfilegroupdlg.moc"
