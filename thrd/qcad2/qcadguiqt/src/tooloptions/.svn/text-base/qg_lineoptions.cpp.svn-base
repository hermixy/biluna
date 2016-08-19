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

#include "qg_lineoptions.h"

#include "rs_actiondrawline.h"

/**
 * Constructor
 */
QG_LineOptions::QG_LineOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);

    connect(bClose, SIGNAL(clicked()),
            this, SLOT(close()));
    connect(bUndo, SIGNAL(clicked()),
            this, SLOT(undo()));
}


/**
 * Destructor
 */
QG_LineOptions::~QG_LineOptions() {
    RS_DEBUG->print("QG_LineOptions::~QG_LineOptions");
}


void QG_LineOptions::setAction(RS_ActionInterface* a, bool /*update*/) {
    action = dynamic_cast<RS_ActionDrawLine*>(a);
    if (action!=NULL) {
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_LineOptions::setAction: wrong action type");
        action = NULL;
    }
}

void QG_LineOptions::close() {
    if (action!=NULL) {
        action->close();
    }
}

void QG_LineOptions::undo() {
    if (action!=NULL) {
        action->undo();
    }
}
