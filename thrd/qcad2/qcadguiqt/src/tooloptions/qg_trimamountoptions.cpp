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

#include "qg_trimamountoptions.h"

#include "rs_actionmodifytrimamount.h"
#include "rs_settings.h"

/**
 * Constructor
 */
QG_TrimAmountOptions::QG_TrimAmountOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);


    connect(leDist, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateDist(const QString&)));
}


/**
 * Destructor
 */
QG_TrimAmountOptions::~QG_TrimAmountOptions() {
    RS_SETTINGS->beginGroup("/Modify");
    RS_SETTINGS->writeEntry("/TrimAmount", leDist->text());
    RS_SETTINGS->endGroup();
}


void QG_TrimAmountOptions::setAction(RS_ActionInterface* a, bool update) {
    action = dynamic_cast<RS_ActionModifyTrimAmount*>(a);
    if (action!=NULL) {
        QString sd;
        // settings from action:
        if (update) {
            sd = QString("%1").arg(action->getDistance());
        }
        // settings from config file:
        else {
            RS_SETTINGS->beginGroup("/Modify");
            sd = RS_SETTINGS->readEntry("/TrimAmount", "1.0");
            RS_SETTINGS->endGroup();
        }

        leDist->setText(sd);
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_ModifyTrimAmountOptions::setAction: wrong action type");
        this->action = NULL;
    }
}


void QG_TrimAmountOptions::updateDist(const QString& d) {
    if (action!=NULL) {
        action->setDistance(RS_Math::eval(d, 1.0));
    }
}
