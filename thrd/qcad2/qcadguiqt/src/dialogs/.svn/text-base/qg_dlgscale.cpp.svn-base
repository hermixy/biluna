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

#include "qg_dlgscale.h"

#include "rs_settings.h"

/**
 * Constructor
 */
QG_DlgScale::QG_DlgScale(QWidget* parent)
        : QDialog(parent) {
    setupUi(this);
    
    bool useCurrentAttributes;
    bool useCurrentLayer;
    bool keepProportional;
    int numberMode;
    QString copies;
    QString factorX;
    QString factorY;
    
    RS_SETTINGS->beginGroup("/Modify");
    copies = RS_SETTINGS->readEntry("/ScaleCopies", "10");
    numberMode = RS_SETTINGS->readNumEntry("/ScaleMode", 0);
    factorX = RS_SETTINGS->readEntry("/ScaleFactorX", "0.5");
    factorY = RS_SETTINGS->readEntry("/ScaleFactorY", "0.5");
    keepProportional =
        (bool)RS_SETTINGS->readNumEntry("/ScaleKeepProportional", 1);
    useCurrentLayer =
        (bool)RS_SETTINGS->readNumEntry("/ScaleUseCurrentLayer", 0);
    useCurrentAttributes =
        (bool)RS_SETTINGS->readNumEntry("/ScaleUseCurrentAttributes", 0);
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
    leFactorX->setText(factorX);
    if (keepProportional) {
        leFactorY->setText(factorX);
    }
    else {
        leFactorY->setText(factorY);
    }
    cbProportional->setChecked(keepProportional);
    cbCurrentAttributes->setChecked(useCurrentAttributes);
    cbCurrentLayer->setChecked(useCurrentLayer);

    connect(leFactorX, SIGNAL(textEdited(const QString&)),
        this, SLOT(slotFactorXChanged(const QString&)));
}


/**
 * Destructor
 */
QG_DlgScale::~QG_DlgScale() {
    RS_SETTINGS->beginGroup("/Modify");
    RS_SETTINGS->writeEntry("/ScaleCopies", leNumber->text());
    RS_SETTINGS->writeEntry("/ScaleFactorX", leFactorX->text());
    RS_SETTINGS->writeEntry("/ScaleFactorY", leFactorY->text());
    int numberMode;
    if (rbMove->isChecked()) {
        numberMode = 0;
    } else if (rbCopy->isChecked()) {
        numberMode = 1;
    } else {
        numberMode = 2;
    }
    RS_SETTINGS->writeEntry("/ScaleMode", numberMode);
    RS_SETTINGS->writeEntry("/ScaleKeepProportional",
                            (int)cbProportional->isChecked());
    RS_SETTINGS->writeEntry("/ScaleUseCurrentLayer",
                            (int)cbCurrentLayer->isChecked());
    RS_SETTINGS->writeEntry("/ScaleUseCurrentAttributes",
                            (int)cbCurrentAttributes->isChecked());
    RS_SETTINGS->endGroup();
}


void QG_DlgScale::setData(RS_ScaleData* d) {
    data = d;
}

void QG_DlgScale::updateData() {
    if (rbMove->isChecked()) {
        data->number = 0;
    } else if (rbCopy->isChecked()) {
        data->number = 1;
    } else {
        data->number = (int)RS_Math::eval(leNumber->text(), 1);
    }
    data->factor.x = RS_Math::eval(leFactorX->text());
    if (cbProportional->isChecked()) {
        data->factor.y = data->factor.x;
    }
    else {
        data->factor.y = RS_Math::eval(leFactorY->text());
    }
    data->useCurrentAttributes = cbCurrentAttributes->isChecked();
    data->useCurrentLayer = cbCurrentLayer->isChecked();
}

void QG_DlgScale::slotFactorXChanged(const QString&) {
    if (cbProportional->isChecked()) {
        leFactorY->setText(leFactorX->text());
    }
}
