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

#include "qg_moverotateoptions.h"

#include "rs_actionmodifymoverotate.h"
#include "rs_settings.h"

/**
 * Constructor
 */
QG_MoveRotateOptions::QG_MoveRotateOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);

    connect(leAngle, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateAngle(const QString&)));
}


/**
 * Destructor
 */
QG_MoveRotateOptions::~QG_MoveRotateOptions() {
    RS_SETTINGS->beginGroup("/Modify");
    RS_SETTINGS->writeEntry("/MoveRotate", leAngle->text());
    RS_SETTINGS->endGroup();
}


void QG_MoveRotateOptions::setAction(RS_ActionInterface* a, bool update) {
    action = dynamic_cast<RS_ActionModifyMoveRotate*>(a);
    if (action!=NULL) {
        QString sa;
        if (update) {
            sa = QString("%1").arg(RS_Math::rad2deg(action->getAngle()));
        } else {
            RS_SETTINGS->beginGroup("/Modify");
            sa = RS_SETTINGS->readEntry("/MoveRotate", "30");
            RS_SETTINGS->endGroup();
            action->setAngle(RS_Math::deg2rad(sa.toDouble()));
        }
        leAngle->setText(sa);
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_CircleOptions::setAction: wrong action type");
        action = NULL;
    }
}

void QG_MoveRotateOptions::updateAngle(const QString& a) {
    if (action!=NULL) {
        action->setAngle(RS_Math::deg2rad(RS_Math::eval(a)));
    }
}
