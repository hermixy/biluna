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

#include "qg_arcoptions.h"

#include "rs_actiondrawarc.h"
#include "rs_settings.h"

/**
 * Constructor
 */
QG_ArcOptions::QG_ArcOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);

    connect(rbPos, SIGNAL(toggled(bool)),
            this, SLOT(updateDirection(bool)));
    connect(rbNeg, SIGNAL(toggled(bool)),
            this, SLOT(updateDirection(bool)));
}


/**
 * Destructor
 */
QG_ArcOptions::~QG_ArcOptions() {
    RS_DEBUG->print("QG_ArcOptions::~QG_ArcOptions");
    RS_SETTINGS->beginGroup("/Draw");
    RS_SETTINGS->writeEntry("/ArcReversed", (int)rbNeg->isChecked());
    RS_SETTINGS->endGroup();
}


void QG_ArcOptions::setAction(RS_ActionInterface* a, bool update) {
    action = dynamic_cast<RS_ActionDrawArc*>(a);
    if (action!=NULL) {
        bool reversed;
        if (update) {
            reversed = action->isReversed();
        } else {
            RS_SETTINGS->beginGroup("/Draw");
            reversed = RS_SETTINGS->readNumEntry("/ArcReversed", 0);
            RS_SETTINGS->endGroup();
            action->setReversed(reversed);
        }
        rbNeg->setChecked(reversed);
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, "QG_ArcOptions::setAction: wrong action type");
        action = NULL;
    }
}

void QG_ArcOptions::updateDirection(bool /*pos*/) {
    if (action!=NULL) {
        action->setReversed(!(rbPos->isChecked()));
    }
}
