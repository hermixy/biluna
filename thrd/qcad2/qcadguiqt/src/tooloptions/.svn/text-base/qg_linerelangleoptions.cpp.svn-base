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

#include "qg_linerelangleoptions.h"

#include "rs_actiondrawlinerelangle.h"
#include "rs_settings.h"

/**
 * Constructor
 */
QG_LineRelAngleOptions::QG_LineRelAngleOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);

    connect(leAngle, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateAngle(const QString&)));
    connect(leLength, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateLength(const QString&)));
}


/**
 * Destructor
 */
QG_LineRelAngleOptions::~QG_LineRelAngleOptions() {
    RS_DEBUG->print("QG_LineRelAngleOptions::~QG_LineRelAngleOptions");
    
    if (action!=NULL) {
        RS_SETTINGS->beginGroup("/Draw");
        if (!action->hasFixedAngle()) {
            RS_SETTINGS->writeEntry("/LineRelAngleAngle", 
                RS_Math::rad2deg(action->getAngle()));
        }
        RS_SETTINGS->writeEntry("/LineRelAngleLength", action->getLength());
        RS_SETTINGS->endGroup();
    }
}


void QG_LineRelAngleOptions::setAction(RS_ActionInterface* a, bool update) {
    action = dynamic_cast<RS_ActionDrawLineRelAngle*>(a);
    if (action!=NULL) {
        if (action->hasFixedAngle()) {
            /*
            lAngle->setDisabled(true);
            leAngle->setDisabled(true);
            */
            lAngle->hide();
            leAngle->hide();
        }

        QString sa;
        QString sl;

        // settings from action:
        if (update) {
            sa = QString("%1").arg(RS_Math::rad2deg(action->getAngle()));
            sl = QString("%1").arg(action->getLength());
        }
        // settings from config file:
        else {
            RS_SETTINGS->beginGroup("/Draw");
            if (!action->hasFixedAngle()) {
                sa = RS_SETTINGS->readEntry("/LineRelAngleAngle", "30.0");
            } else {
                sa = QString("%1").arg(RS_Math::rad2deg(action->getAngle()));
            }
            sl = RS_SETTINGS->readEntry("/LineRelAngleLength", "10.0");
            RS_SETTINGS->endGroup();
        }

        leAngle->setText(sa);
        leLength->setText(sl);
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_LineRelAngleOptions::setAction: wrong action type");
        this->action = NULL;
    }
}

void QG_LineRelAngleOptions::updateAngle(const QString& a) {
    if (action!=NULL && !action->hasFixedAngle()) {
        action->setAngle(RS_Math::deg2rad(RS_Math::eval(a)));
    }
}

void QG_LineRelAngleOptions::updateLength(const QString& l) {
    if (action!=NULL) {
        action->setLength(RS_Math::eval(l));
    }
}
