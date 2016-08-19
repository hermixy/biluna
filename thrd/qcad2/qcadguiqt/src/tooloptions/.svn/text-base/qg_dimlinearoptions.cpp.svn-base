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

#include "qg_dimlinearoptions.h"

#include "rs_actiondimlinear.h"
#include "rs_settings.h"

/**
 * Constructor
 */
QG_DimLinearOptions::QG_DimLinearOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);

    connect(leAngle, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateAngle(const QString&)));
    connect(bHor, SIGNAL(clicked()),
            this, SLOT(setHor()));
    connect(bVer, SIGNAL(clicked()),
            this, SLOT(setVer()));
}


/**
 * Destructor
 */
QG_DimLinearOptions::~QG_DimLinearOptions() {
    RS_SETTINGS->beginGroup("/Dimension");
    RS_SETTINGS->writeEntry("/Angle", leAngle->text());
    RS_SETTINGS->endGroup();
}


void QG_DimLinearOptions::setAction(RS_ActionInterface* a, bool update) {
    action = dynamic_cast<RS_ActionDimLinear*>(a);
    if (action!=NULL) {
        QString sa;
        if (update) {
            sa = QString("%1").arg(RS_Math::rad2deg(action->getAngle()));
        } else {
            RS_SETTINGS->beginGroup("/Dimension");
            sa = RS_SETTINGS->readEntry("/Angle", "0.0");
            RS_SETTINGS->endGroup();
        }
        leAngle->setText(sa);
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_DimLinearOptions::setAction: wrong action type");
        action = NULL;
    }
}


void QG_DimLinearOptions::updateAngle(const QString& a) {
    if (action!=NULL) {
        action->setAngle(RS_Math::deg2rad(RS_Math::eval(a)));
    }
}

void QG_DimLinearOptions::setHor() {
    leAngle->setText("0");
}

void QG_DimLinearOptions::setVer() {
    leAngle->setText("90");
}
