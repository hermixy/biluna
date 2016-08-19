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

#include "qg_imageoptions.h"

#include "rs_actiondrawimage.h"
#include "rs_settings.h"

/**
 * Constructor
 */
QG_ImageOptions::QG_ImageOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);

    connect(leAngle, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateData()));
    connect(leFactor, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateData()));
}


/**
 * Destructor
 */
QG_ImageOptions::~QG_ImageOptions() {
    RS_SETTINGS->beginGroup("/Image");
    RS_SETTINGS->writeEntry("/ImageAngle", leAngle->text());
    RS_SETTINGS->writeEntry("/ImageFactor", leFactor->text());
    RS_SETTINGS->endGroup();
}


void QG_ImageOptions::setAction(RS_ActionInterface* a, bool update) {
    action = dynamic_cast<RS_ActionDrawImage*>(a);
    if (action!=NULL) {
        QString sAngle;
        QString sFactor;
        if (update) {
            sAngle = QString("%1").arg(RS_Math::rad2deg(action->getAngle()));
            sFactor = QString("%1").arg(action->getFactor());
        } else {
            RS_SETTINGS->beginGroup("/Image");
            sAngle = RS_SETTINGS->readEntry("/ImageAngle", "0.0");
            sFactor = RS_SETTINGS->readEntry("/ImageFactor", "1.0");
            RS_SETTINGS->endGroup();
        }
    leAngle->setText(sAngle);
    leFactor->setText(sFactor);
        updateData();
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_ImageOptions::setAction: wrong action type");
        action = NULL;
    }
}


void QG_ImageOptions::updateData() {
    if (action!=NULL) {
        action->setAngle(RS_Math::deg2rad(RS_Math::eval(leAngle->text())));
        action->setFactor(RS_Math::eval(leFactor->text()));
    }
}
