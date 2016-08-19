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

#include "qg_lineangleoptions.h"

#include "rs_actiondrawlineangle.h"
#include "rs_settings.h"

/**
 * Constructor
 */
QG_LineAngleOptions::QG_LineAngleOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);

    connect(leAngle, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateAngle(const QString&)));
    connect(leLength, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateLength(const QString&)));
    connect(cbSnapReference, SIGNAL(activated(int)),
            this, SLOT(updateSnapReference(int)));
}


/**
 * Destructor
 */
QG_LineAngleOptions::~QG_LineAngleOptions() {
    RS_DEBUG->print("QG_LineAngleOptions::~QG_LineAngleOptions");
    
    if (action!=NULL) {
        RS_SETTINGS->beginGroup("/Draw");
        if (!action->hasFixedAngle()) {
            RS_SETTINGS->writeEntry("/LineAngleAngle", RS_Math::rad2deg(action->getAngle()));
        }
        RS_SETTINGS->writeEntry("/LineAngleLength", action->getLength());
        RS_SETTINGS->writeEntry("/LineAngleSnapReference", action->getSnapReference());
        RS_SETTINGS->endGroup();
    }
}


void QG_LineAngleOptions::setAction(RS_ActionInterface* a, bool update) {
    //if (a!=NULL && a->rtti()==RS2::ActionDrawLineAngle) {
    if (dynamic_cast<RS_ActionDrawLineAngle*>(a)!=NULL) {
        action = (RS_ActionDrawLineAngle*)a;
        if (action->hasFixedAngle()) {
            lAngle->hide();
            leAngle->hide();
        }

        QString sa;
        QString sl;
        int sp;

        // settings from action:
        if (update) {
            sa = QString("%1").arg(RS_Math::rad2deg(action->getAngle()));
            sl = QString("%1").arg(action->getLength());
            sp = action->getSnapReference();
        }
        // settings from config file:
        else {
            RS_SETTINGS->beginGroup("/Draw");
            if (!action->hasFixedAngle()) {
                sa = RS_SETTINGS->readEntry("/LineAngleAngle", "30.0");
            } else {
                sa = QString("%1").arg(action->getAngle());
            }
            sl = RS_SETTINGS->readEntry("/LineAngleLength", "10.0");
            sp = RS_SETTINGS->readNumEntry("/LineAngleSnapReference", 0);
            RS_SETTINGS->endGroup();
            action->setSnapReference(sp);
        }

        leAngle->setText(sa);
        leLength->setText(sl);
        cbSnapReference->setCurrentIndex(sp);
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_LineAngleOptions::setAction: wrong action type");
        this->action = NULL;
    }
}

void QG_LineAngleOptions::updateAngle(const QString& a) {
    if (action!=NULL && !action->hasFixedAngle()) {
        action->setAngle(RS_Math::deg2rad(RS_Math::eval(a)));
    }
}

void QG_LineAngleOptions::updateLength(const QString& l) {
    if (action!=NULL) {
        action->setLength(RS_Math::eval(l));
    }
}

void QG_LineAngleOptions::updateSnapReference(int sp) {
    if (action!=NULL) {
        action->setSnapReference(sp);
    }
}
