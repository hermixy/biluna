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

#include "qg_arctangentialoptions.h"

#include "rs_actiondrawarctangential.h"
#include "rs_settings.h"

/**
 * Constructor
 */
QG_ArcTangentialOptions::QG_ArcTangentialOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);

    connect(leRadius, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateRadius(const QString&)));
}


/**
 * Destructor
 */
QG_ArcTangentialOptions::~QG_ArcTangentialOptions() {
    RS_SETTINGS->beginGroup("/Draw");
    RS_SETTINGS->writeEntry("/ArcTangentialRadius", leRadius->text());    
    RS_SETTINGS->endGroup();
}


void QG_ArcTangentialOptions::setAction(RS_ActionInterface* a, bool update) {
    action = dynamic_cast<RS_ActionDrawArcTangential*>(a);
    if (action!=NULL) {
        QString sr;
        if (update) {
            sr = QString("%1").arg(action->getRadius());
        } else {
            RS_SETTINGS->beginGroup("/Draw");
            sr = RS_SETTINGS->readEntry("/ArcTangentialRadius", "1.0");
            RS_SETTINGS->endGroup();
            action->setRadius(sr.toDouble());
        }
        leRadius->setText(sr);
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_ArcTangentialOptions::setAction: wrong action type");
        action = NULL;
    }
}

void QG_ArcTangentialOptions::updateRadius(const QString& s) {
    if (action!=NULL) {
        action->setRadius(RS_Math::eval(s));
    }
}
