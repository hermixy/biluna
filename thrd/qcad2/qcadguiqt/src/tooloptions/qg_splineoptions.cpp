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

#include "qg_splineoptions.h"

#include "rs_actiondrawspline.h"
#include "rs_settings.h"

/**
 * Constructor
 */
QG_SplineOptions::QG_SplineOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);

    cbDegree->addItem("2");
    cbDegree->addItem("3");

    connect(bUndo, SIGNAL(clicked()),
            this, SLOT(undo()));
    connect(cbDegree, SIGNAL(activated(const QString&)),
            this, SLOT(setDegree(const QString&)));
    connect(cbClosed, SIGNAL(toggled(bool)),
            this, SLOT(setClosed(bool)));
}


/**
 * Destructor
 */
QG_SplineOptions::~QG_SplineOptions() {
    RS_SETTINGS->beginGroup("/Draw");
    RS_SETTINGS->writeEntry("/SplineDegree", cbDegree->currentText().toInt());
    RS_SETTINGS->writeEntry("/SplineClosed", (int)cbClosed->isChecked());
    RS_SETTINGS->endGroup();
}


void QG_SplineOptions::setAction(RS_ActionInterface* a, bool update) {
    action = dynamic_cast<RS_ActionDrawSpline*>(a);
    if (action!=NULL) {
        int degree;
        bool closed;
         if (update) {
            degree = action->getDegree();
            closed = action->isClosed();
        } else {
            RS_SETTINGS->beginGroup("/Draw");
            degree = RS_SETTINGS->readNumEntry("/SplineDegree", 3);
            closed = RS_SETTINGS->readNumEntry("/SplineClosed", 0);
            RS_SETTINGS->endGroup();
            action->setDegree(degree);
            action->setClosed(closed);
        }
        cbDegree->setCurrentIndex(cbDegree->findText(QString("%1").arg(degree)));
        cbClosed->setChecked(closed);
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_SplineOptions::setAction: wrong action type");
        action = NULL;
    }
}

void QG_SplineOptions::setClosed(bool c) {
    if (action!=NULL) {
        action->setClosed(c);
    }
}

void QG_SplineOptions::undo() {
    if (action!=NULL) {
        action->undo();
    }
}

void QG_SplineOptions::setDegree(const QString& deg) {
    if (action!=NULL) {
        action->setDegree(deg.toInt());
    }
}
