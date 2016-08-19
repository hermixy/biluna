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

#include "qg_dlgrotate2.h"

#include "rs_settings.h"

/**
 * Constructor
 */
QG_DlgRotate2::QG_DlgRotate2(QWidget* parent)
        : QDialog(parent) {
    setupUi(this);
    
    RS_SETTINGS->beginGroup("/Modify");
    copies = RS_SETTINGS->readEntry("/Rotate2Copies", "10");
    numberMode = RS_SETTINGS->readNumEntry("/Rotate2Mode", 0);
    useCurrentLayer =
        (bool)RS_SETTINGS->readNumEntry("/Rotate2UseCurrentLayer", 0);
    useCurrentAttributes =
        (bool)RS_SETTINGS->readNumEntry("/MoveRotate2UseCurrentAttributes", 0);
    angle1 = RS_SETTINGS->readEntry("/Rotate2Angle1", "30.0");
    angle2 = RS_SETTINGS->readEntry("/Rotate2Angle2", "-30.0");
    RS_SETTINGS->endGroup();

    switch (numberMode) {
    case 0:
        rbMove->setChecked(true);
        break;
    case 1:
        rbCopy->setChecked(true);
        break;
    case 2:
        rbMultiCopy->setChecked(true);
        break;
    default:
        break;
    }
    leNumber->setText(copies);
    leAngle1->setText(angle1);
    leAngle2->setText(angle2);
    cbCurrentAttributes->setChecked(useCurrentAttributes);
    cbCurrentLayer->setChecked(useCurrentLayer);
}


/**
 * Destructor
 */
QG_DlgRotate2::~QG_DlgRotate2() {
    RS_SETTINGS->beginGroup("/Modify");
    RS_SETTINGS->writeEntry("/Rotate2Copies", leNumber->text());
    if (rbMove->isChecked()) {
        numberMode = 0;
    } else if (rbCopy->isChecked()) {
        numberMode = 1;
    } else {
        numberMode = 2;
    }
    RS_SETTINGS->writeEntry("/Rotate2Mode", numberMode);
    RS_SETTINGS->writeEntry("/Rotate2Angle1", leAngle1->text());
    RS_SETTINGS->writeEntry("/Rotate2Angle2", leAngle2->text());
    RS_SETTINGS->writeEntry("/Rotate2UseCurrentLayer",
                            (int)cbCurrentLayer->isChecked());
    RS_SETTINGS->writeEntry("/Rotate2UseCurrentAttributes",
                            (int)cbCurrentAttributes->isChecked());
    RS_SETTINGS->endGroup();
}


void QG_DlgRotate2::setData(RS_Rotate2Data* d) {
    data = d;
}

void QG_DlgRotate2::updateData() {
    if (rbMove->isChecked()) {
        data->number = 0;
    } else if (rbCopy->isChecked()) {
        data->number = 1;
    } else {
        data->number = (int)RS_Math::eval(leNumber->text(), 1);
    }
    data->angle1 = RS_Math::deg2rad(RS_Math::eval(leAngle1->text()));
    data->angle2 = RS_Math::deg2rad(RS_Math::eval(leAngle2->text()));
    data->useCurrentAttributes = cbCurrentAttributes->isChecked();
    data->useCurrentLayer = cbCurrentLayer->isChecked();
}

