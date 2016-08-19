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

#include "qg_dlgmirror.h"

#include "rs_settings.h"

/**
 * Constructor
 */
QG_DlgMirror::QG_DlgMirror(QWidget* parent)
        : QDialog(parent) {
    setupUi(this);
    
    RS_SETTINGS->beginGroup("/Modify");
    numberMode = RS_SETTINGS->readNumEntry("/MirrorMode", 0);
    useCurrentLayer =
        (bool)RS_SETTINGS->readNumEntry("/MirrorUseCurrentLayer", 0);
    useCurrentAttributes =
        (bool)RS_SETTINGS->readNumEntry("/MirrorUseCurrentAttributes", 0);
    RS_SETTINGS->endGroup();

    switch (numberMode) {
    case 0:
        rbMove->setChecked(true);
        break;
    case 1:
        rbCopy->setChecked(true);
        break;
    default:
        break;
    }
    cbCurrentAttributes->setChecked(useCurrentAttributes);
    cbCurrentLayer->setChecked(useCurrentLayer);
}


/**
 * Destructor
 */
QG_DlgMirror::~QG_DlgMirror() {
    RS_SETTINGS->beginGroup("/Modify");
    if (rbMove->isChecked()) {
        numberMode = 0;
    } else if (rbCopy->isChecked()) {
        numberMode = 1;
    } else {
        numberMode = 2;
    }
    RS_SETTINGS->writeEntry("/MirrorMode", numberMode);
    RS_SETTINGS->writeEntry("/MirrorUseCurrentLayer",
                            (int)cbCurrentLayer->isChecked());
    RS_SETTINGS->writeEntry("/MirrorUseCurrentAttributes",
                            (int)cbCurrentAttributes->isChecked());
    RS_SETTINGS->endGroup();
}


void QG_DlgMirror::setData(RS_MirrorData* d) {
    data = d;
}

void QG_DlgMirror::updateData() {
    if (rbMove->isChecked()) {
        data->copy = false;
    } else if (rbCopy->isChecked()) {
        data->copy = true;
    }
    data->useCurrentAttributes = cbCurrentAttributes->isChecked();
    data->useCurrentLayer = cbCurrentLayer->isChecked();
}

