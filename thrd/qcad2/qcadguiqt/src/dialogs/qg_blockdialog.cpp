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

#include "qg_blockdialog.h"

#include <QMessageBox>

#include "rs_block.h"
#include "rs_blocklist.h"

/**
 * Constructor
 */
QG_BlockDialog::QG_BlockDialog(QWidget* parent)
        : QDialog(parent) {
    setupUi(this);
    
}


/**
 * Destructor
 */
QG_BlockDialog::~QG_BlockDialog() {
}


void QG_BlockDialog::setBlockList(RS_BlockList* l) {
    RS_DEBUG->print("QG_BlockDialog::setBlockList");
    
    blockList = l;
    if (blockList!=NULL) {
        RS_Block* block = blockList->getActive();
        if (block!=NULL) {
            leName->setText(block->getName());
        } else {
            RS_DEBUG->print(RS_Debug::D_ERROR, 
                "QG_BlockDialog::setBlockList: No block active.");
        }
    }
}

RS_BlockData QG_BlockDialog::getBlockData() {
    return RS_BlockData(leName->text(), RS_Vector(0.0,0.0), false);
}

void QG_BlockDialog::validate() {
    QString name = leName->text();

    if (!name.isEmpty()) {
        if (blockList!=NULL && blockList->find(name)==NULL) {
            accept();
        } else {
            QMessageBox::warning( this, tr("Renaming Block"),
                                  tr("Could not name block. A block named \"%1\" "
                                     "already exists.").arg(leName->text()),
                                  QMessageBox::Ok,
                                  Qt::NoButton);
        }
    }
    //else {
    //reject();
    //}
}

void QG_BlockDialog::cancel() {
    leName->setText("");
    reject();
}
