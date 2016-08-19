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

#include "qg_roundoptions.h"

#include "rs_actionmodifyround.h"
#include "rs_settings.h"

/**
 * Constructor
 */
QG_RoundOptions::QG_RoundOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);

    connect(leRadius, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateData()));
    connect(cbTrim, SIGNAL(toggled(bool)),
            this, SLOT(updateData()));
}


/**
 * Destructor
 */
QG_RoundOptions::~QG_RoundOptions() {
    RS_SETTINGS->beginGroup("/Modify");
    RS_SETTINGS->writeEntry("/RoundRadius", leRadius->text());
    RS_SETTINGS->writeEntry("/RoundTrim", (int)cbTrim->isChecked());
    RS_SETTINGS->endGroup();
}


void QG_RoundOptions::setAction(RS_ActionInterface* a, bool update) {
    action = dynamic_cast<RS_ActionModifyRound*>(a);
    if (action!=NULL) {
        QString sr;
        QString st;
        if (update) {
            sr = QString("%1").arg(action->getRadius());
            st = QString("%1").arg((int)action->isTrimOn());
        } else {
            RS_SETTINGS->beginGroup("/Modify");
            sr = RS_SETTINGS->readEntry("/RoundRadius", "1.0");
            st = RS_SETTINGS->readEntry("/RoundTrim", "1");
            RS_SETTINGS->endGroup();
        }
        leRadius->setText(sr);
        cbTrim->setChecked(st=="1");
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_RoundOptions::setAction: wrong action type");
        action = NULL;
    }
}


void QG_RoundOptions::updateData() {
    if (action!=NULL) {
        action->setTrim(cbTrim->isChecked());
        action->setRadius(RS_Math::eval(leRadius->text()));
    }
}
