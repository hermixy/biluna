/****************************************************************************
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include "qg_exitdialog.h"

#include <QShortcut>
#include <QMessageBox>

/**
 * Constructor
 */
QG_ExitDialog::QG_ExitDialog(QWidget* parent)
        : QDialog(parent) {
    setupUi(this);

    connect(bLeave, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(bCancel, SIGNAL(clicked()),
            this, SLOT(reject()));
    connect(bSave, SIGNAL(clicked()),
            this, SLOT(slotSave()));
    connect(bSaveAs, SIGNAL(clicked()),
            this, SLOT(slotSaveAs()));
    
    makeLetterAccell( bLeave );
    makeLetterAccell( bSave );
    makeLetterAccell( bSaveAs );
    makeLetterAccell( bCancel );

    lIcon->setPixmap( QMessageBox::standardIcon( QMessageBox::Warning ) );
}


/**
 * Destructor
 */
QG_ExitDialog::~QG_ExitDialog() {}


/**
 * Provides single-key shortcuts in addition to the Alt-X ones.
 */
void QG_ExitDialog::makeLetterAccell(QPushButton *btn) {
    if (btn->shortcut().count()<1) {
        return;
    }
    QShortcut* a = new QShortcut(btn);
    /*a->connectItem( a->insertItem( btn->accel() & ~(Qt::MODIFIER_MASK|Qt::UNICODE_ACCEL) ),
                    btn, SLOT(animateClick()) );
    */
    a->setKey(btn->shortcut());
}


void QG_ExitDialog::setText(const QString& text) {
    lQuestion->setText(text);
    // ### Qt4 ###
    //resize(lQuestion->sizeHint().width()+32,
    //       lQuestion->sizeHint().height() + layButtons->sizeHint().height()+32);
}

void QG_ExitDialog::setTitle(const QString& text) {
    setWindowTitle(text);
}

void QG_ExitDialog::setForce(bool force) {
    bCancel->setDisabled(force);
}

void QG_ExitDialog::setSaveAsEnabled(bool set)
{
	if (set)
		bSaveAs->show();
	else
		bSaveAs->hide();
}

void QG_ExitDialog::slotSaveAs() {
    done(3);
}

void QG_ExitDialog::slotSave() {
    done(2);
}
