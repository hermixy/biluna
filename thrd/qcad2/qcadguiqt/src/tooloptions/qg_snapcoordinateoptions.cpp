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

#include "qg_snapcoordinateoptions.h"

#include "rs_coordinateevent.h"
#include "rs_settings.h"
#include "rs_snapper.h"
#include "rs_math.h"

/**
 * Constructor
 */
QG_SnapCoordinateOptions::QG_SnapCoordinateOptions(QWidget* parent)
        : QG_SnapOptionsBase(parent) {
    setupUi(this);
    
    connect(leX, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateCoordinate()));
    connect(leY, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateCoordinate()));
    connect(cbRel, SIGNAL(stateChanged(int)),
            this, SLOT(updateCoordinate()));
    connect(bSet, SIGNAL(clicked()),
            this, SLOT(setCoordinate()));
    
    RS_SETTINGS->beginGroup("/Snap");
    QString x = RS_SETTINGS->readEntry("/CoordinateX", "0.0");
    QString y = RS_SETTINGS->readEntry("/CoordinateY", "0.0");
    QString rel = RS_SETTINGS->readEntry("/CoordinateRel", "0");
    RS_SETTINGS->endGroup();
    
    leX->setText(x);
    leY->setText(y);
    cbRel->setChecked(rel=="1");
}
    
    
void QG_SnapCoordinateOptions::setSnapper(RS_Snapper* s) {
    QG_SnapOptionsBase::setSnapper(s);
    updateCoordinate();
}


/**
 * Destructor
 */
QG_SnapCoordinateOptions::~QG_SnapCoordinateOptions() {
    RS_SETTINGS->beginGroup("/Snap");
    RS_SETTINGS->writeEntry("/CoordinateX", leX->text());
    RS_SETTINGS->writeEntry("/CoordinateY", leY->text());
    RS_SETTINGS->writeEntry("/CoordinateRel", (int)cbRel->isChecked());
    RS_SETTINGS->endGroup();
}


void QG_SnapCoordinateOptions::updateCoordinate() {
    if (snapper!=NULL) {
        coord.x = RS_Math::eval(leX->text(), 0.0);
        coord.y = RS_Math::eval(leY->text(), 0.0);
        snapper->setCoordinate(coord, cbRel->isChecked(), false);
    }
}


void QG_SnapCoordinateOptions::setCoordinate() {
    if (snapper!=NULL) {
        updateCoordinate();
        snapper->setCoordinate(coord, cbRel->isChecked(), true);
    }
}
