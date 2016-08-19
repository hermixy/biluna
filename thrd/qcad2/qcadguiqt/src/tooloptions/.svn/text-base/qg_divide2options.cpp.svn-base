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

#include "qg_divide2options.h"

#include "rs_actionmodifydivide2.h"
#include "rs_settings.h"

/**
 * Constructor
 */
QG_Divide2Options::QG_Divide2Options(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);

    connect(cbRemove, SIGNAL(toggled(bool)),
            this, SLOT(updateData()));
}


/**
 * Destructor
 */
QG_Divide2Options::~QG_Divide2Options() {
    RS_SETTINGS->beginGroup("/Modify");
    RS_SETTINGS->writeEntry("/Divide2Remove", (int)cbRemove->isChecked());
    RS_SETTINGS->endGroup();
}


void QG_Divide2Options::setAction(RS_ActionInterface* a, bool update) {
    action = dynamic_cast<RS_ActionModifyDivide2*>(a);
    if (action!=NULL) {
        QString st;
        if (update) {
            st = QString("%1").arg((int)action->isRemoveOn());
        } else {
            RS_SETTINGS->beginGroup("/Modify");
            st = RS_SETTINGS->readEntry("/Divide2Remove", "1");
            RS_SETTINGS->endGroup();
        }
        cbRemove->setChecked(st=="1");
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_Divide2Options::setAction: wrong action type");
        action = NULL;
    }
}


void QG_Divide2Options::updateData() {
    if (action!=NULL) {
        action->setRemove(cbRemove->isChecked());
    }
}
