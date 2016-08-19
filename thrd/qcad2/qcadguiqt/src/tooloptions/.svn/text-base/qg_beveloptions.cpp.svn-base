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

#include "qg_beveloptions.h"

#include "rs_actionmodifybevel.h"
#include "rs_settings.h"

/**
 * Constructor
 */
QG_BevelOptions::QG_BevelOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);

    connect(leLength1, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateData()));
    connect(leLength2, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateData()));
    connect(cbTrim, SIGNAL(toggled(bool)),
            this, SLOT(updateData()));
}


/**
 * Destructor
 */
QG_BevelOptions::~QG_BevelOptions() {
    RS_SETTINGS->beginGroup("/Modify");
    RS_SETTINGS->writeEntry("/BevelLength1", leLength1->text());
    RS_SETTINGS->writeEntry("/BevelLength2", leLength2->text());
    RS_SETTINGS->writeEntry("/BevelTrim", (int)cbTrim->isChecked());
    RS_SETTINGS->endGroup();
}


void QG_BevelOptions::setAction(RS_ActionInterface* a, bool update) {
    action = dynamic_cast<RS_ActionModifyBevel*>(a);
    if (action!=NULL) {
        QString sd1;
        QString sd2;
        QString st;
        if (update) {
            sd1 = QString("%1").arg(action->getLength1());
            sd2 = QString("%1").arg(action->getLength2());
            st = QString("%1").arg((int)action->isTrimOn());
        } else {
            RS_SETTINGS->beginGroup("/Modify");
            sd1 = RS_SETTINGS->readEntry("/BevelLength1", "1.0");
            sd2 = RS_SETTINGS->readEntry("/BevelLength2", "1.0");
            st = RS_SETTINGS->readEntry("/BevelTrim", "1");
            RS_SETTINGS->endGroup();
        }
        leLength1->setText(sd1);
        leLength2->setText(sd2);
        cbTrim->setChecked(st=="1");
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_BevelOptions::setAction: wrong action type");
        action = NULL;
    }
}


void QG_BevelOptions::updateData() {
    if (action!=NULL) {
        action->setTrim(cbTrim->isChecked());
        action->setLength1(RS_Math::eval(leLength1->text()));
        action->setLength2(RS_Math::eval(leLength2->text()));
    }
}
