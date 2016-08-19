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

#include "qg_lineparallelthroughoptions.h"

#include "rs_actiondrawlineparallelthrough.h"
#include "rs_settings.h"

/**
 * Constructor
 */
QG_LineParallelThroughOptions::QG_LineParallelThroughOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);

    connect(sbNumber, SIGNAL(valueChanged(int)),
            this, SLOT(updateNumber(int)));
}


/**
 * Destructor
 */
QG_LineParallelThroughOptions::~QG_LineParallelThroughOptions() {
    RS_SETTINGS->beginGroup("/Draw");
    RS_SETTINGS->writeEntry("/LineParallelThroughNumber", sbNumber->text());
    RS_SETTINGS->endGroup();
}


void QG_LineParallelThroughOptions::setAction(RS_ActionInterface* a, bool update) {
    action = dynamic_cast<RS_ActionDrawLineParallelThrough*>(a);
    if (action!=NULL) {
        QString sn;
        if (update) {
            sn = QString("%1").arg(action->getNumber());
        } else {
            RS_SETTINGS->beginGroup("/Draw");
            sn = RS_SETTINGS->readEntry("/LineParallelThroughNumber", "1");
            RS_SETTINGS->endGroup();
        }
        sbNumber->setValue(sn.toInt());
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_LineParallelThroughOptions::setAction: wrong action type");
        action = NULL;
    }
}

void QG_LineParallelThroughOptions::updateNumber(int n) {
    if (action!=NULL) {
        action->setNumber(n);
    }
}
