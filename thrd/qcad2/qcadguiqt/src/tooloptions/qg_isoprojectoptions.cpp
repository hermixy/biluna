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

#include "qg_isoprojectoptions.h"

#include "rs_actionmodifyisoproject.h"
#include "rs_settings.h"

/**
 * Constructor
 */
QG_IsoProjectOptions::QG_IsoProjectOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);

    connect(cbViewDirection, SIGNAL(activated(int)),
            this, SLOT(updateViewDirection(int)));
    connect(leSegmentLength, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateSegmentLength(const QString&)));
}


/**
 * Destructor
 */
QG_IsoProjectOptions::~QG_IsoProjectOptions() {
    RS_DEBUG->print("QG_IsoProjectOptions::~QG_IsoProjectOptions");
    
    if (action!=NULL) {
        RS_SETTINGS->beginGroup("/Modify");
        RS_SETTINGS->writeEntry("/IsoProjectViewDirection", action->getViewDirection());
        RS_SETTINGS->writeEntry("/IsoProjectSegmentLength", action->getSegmentLength());
        RS_SETTINGS->endGroup();
    }
}


void QG_IsoProjectOptions::setAction(RS_ActionInterface* a, bool update) {
    action = dynamic_cast<RS_ActionModifyIsoProject*>(a);
    if (action!=NULL) {
        int vd;
        QString sl;

        // settings from action:
        if (update) {
            vd = (int)action->getViewDirection();
            sl = QString("%1").arg(action->getSegmentLength());
        }
        // settings from config file:
        else {
            RS_SETTINGS->beginGroup("/Modify");
            vd = RS_SETTINGS->readNumEntry("/IsoProjectViewDirection", 0);
            sl = RS_SETTINGS->readEntry("/IsoProjectSegmentLength", "0.5");
            RS_SETTINGS->endGroup();
            action->setViewDirection((RS2::IsoProjectionType)vd);
            action->setSegmentLength(RS_Math::eval(sl));
        }

        cbViewDirection->setCurrentIndex(vd);
        leSegmentLength->setText(sl);
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_IsoProjectOptions::setAction: wrong action type");
        this->action = NULL;
    }
}

void QG_IsoProjectOptions::updateViewDirection(int vd) {
    if (action!=NULL) {
        action->setViewDirection((RS2::IsoProjectionType)vd);
    }
}
    
    
void QG_IsoProjectOptions::updateSegmentLength(const QString& sl) {
    if (action!=NULL) {
        action->setSegmentLength(RS_Math::eval(sl));
    }
}
