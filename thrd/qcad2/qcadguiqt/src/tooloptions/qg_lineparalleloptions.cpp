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

#include "qg_lineparalleloptions.h"

#include "rs_actiondrawlineparallel.h"
#include "rs_settings.h"

/**
 * Constructor
 */
QG_LineParallelOptions::QG_LineParallelOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);

    connect(leDist, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateDist(const QString&)));
    connect(sbNumber, SIGNAL(valueChanged(int)),
            this, SLOT(updateNumber(int)));
}


/**
 * Destructor
 */
QG_LineParallelOptions::~QG_LineParallelOptions() {
    RS_DEBUG->print("QG_LineParallelOptions::~QG_LineParallelOptions");
    RS_SETTINGS->beginGroup("/Draw");
    RS_SETTINGS->writeEntry("/LineParallelDistance", leDist->text());
    RS_SETTINGS->writeEntry("/LineParallelNumber", sbNumber->text());
    RS_SETTINGS->endGroup();
}


void QG_LineParallelOptions::setAction(RS_ActionInterface* a, bool update) {
    action = dynamic_cast<RS_ActionDrawLineParallel*>(a);
    if (action!=NULL) {
        QString sd;
        QString sn;
        if (update) {
            sd = QString("%1").arg(action->getDistance());
            sn = QString("%1").arg(action->getNumber());
        } else {
            RS_SETTINGS->beginGroup("/Draw");
            sd = RS_SETTINGS->readEntry("/LineParallelDistance", "1.0");
            sn = RS_SETTINGS->readEntry("/LineParallelNumber", "1");
            RS_SETTINGS->endGroup();
        }
        leDist->setText(sd);
        sbNumber->setValue(sn.toInt());
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_LineParallelOptions::setAction: wrong action type");
        action = NULL;
    }
}

void QG_LineParallelOptions::updateDist(const QString& d) {
    if (action!=NULL) {
        action->setDistance(RS_Math::eval(d));
    }
}

void QG_LineParallelOptions::updateNumber(int n) {
    if (action!=NULL) {
        action->setNumber(n);
    }
}
