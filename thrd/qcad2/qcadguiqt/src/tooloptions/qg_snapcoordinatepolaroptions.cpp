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

#include "qg_snapcoordinatepolaroptions.h"

#include "rs_coordinateevent.h"
#include "rs_settings.h"
#include "rs_snapper.h"
#include "rs_math.h"

/**
 * Constructor
 */
QG_SnapCoordinatePolarOptions::QG_SnapCoordinatePolarOptions(QWidget* parent)
        : QG_SnapOptionsBase(parent) {
    setupUi(this);
    
    connect(leRadius, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateCoordinate()));
    connect(leAngle, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateCoordinate()));
    connect(cbRel, SIGNAL(stateChanged(int)),
            this, SLOT(updateCoordinate()));
    connect(bSet, SIGNAL(clicked()),
            this, SLOT(setCoordinate()));
    
    RS_SETTINGS->beginGroup("/Snap");
    QString radius = RS_SETTINGS->readEntry("/CoordinatePolarRadius", "0.0");
    QString angle = RS_SETTINGS->readEntry("/CoordinatePolarAngle", "0.0");
    QString rel = RS_SETTINGS->readEntry("/CoordinatePolarRel", "0");
    RS_SETTINGS->endGroup();
    
    leRadius->setText(radius);
    leAngle->setText(angle);
    cbRel->setChecked(rel=="1");
}
    
    
void QG_SnapCoordinatePolarOptions::setSnapper(RS_Snapper* s) {
    QG_SnapOptionsBase::setSnapper(s);
    updateCoordinate();
}


/**
 * Destructor
 */
QG_SnapCoordinatePolarOptions::~QG_SnapCoordinatePolarOptions() {
    RS_SETTINGS->beginGroup("/Snap");
    RS_SETTINGS->writeEntry("/CoordinatePolarRadius", leRadius->text());
    RS_SETTINGS->writeEntry("/CoordinatePolarAngle", leAngle->text());
    RS_SETTINGS->writeEntry("/CoordinatePolarRel", (int)cbRel->isChecked());
    RS_SETTINGS->endGroup();
}


void QG_SnapCoordinatePolarOptions::updateCoordinate() {
    if (snapper!=NULL) {
        double r = RS_Math::eval(leRadius->text(), 0.0);
        double a = RS_Math::eval(leAngle->text(), 0.0);
        coord.setPolar(r, RS_Math::deg2rad(a));
        snapper->setCoordinate(coord, cbRel->isChecked(), false);
    }
}


void QG_SnapCoordinatePolarOptions::setCoordinate() {
    if (snapper!=NULL) {
        updateCoordinate();
        snapper->setCoordinate(coord, cbRel->isChecked(), true);
        //leX->clear();
        //leY->clear();
    }
}
