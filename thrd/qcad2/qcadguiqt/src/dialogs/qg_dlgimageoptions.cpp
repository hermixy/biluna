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

#include "qg_dlgimageoptions.h"

#include "rs_settings.h"
#include "rs_math.h"


/**
 * Constructor
 */
QG_DlgImageOptions::QG_DlgImageOptions(QWidget* parent)
        : QDialog(parent) {
    setupUi(this);
    
    connect(okButton, SIGNAL(clicked()),
        this, SLOT(ok()));
    connect(cancelButton, SIGNAL(clicked()),
        this, SLOT(reject()));
    connect(leWidth, SIGNAL(textChanged(const QString&)),
        this, SLOT(sizeChanged()));
    connect(leHeight, SIGNAL(textChanged(const QString&)),
        this, SLOT(sizeChanged()));
    connect(cbResolution, SIGNAL(textChanged(const QString&)),
        this, SLOT(resolutionChanged()));

    graphicSize = RS_Vector(0.0,0.0);
    updateEnabled = true;

    RS_SETTINGS->beginGroup("/ExportImage");
    leWidth->setText(RS_SETTINGS->readEntry("/Width", "640"));
    leHeight->setText(RS_SETTINGS->readEntry("/Height", "480"));
    if (RS_SETTINGS->readEntry("/BlackBackground", "0")=="1") {
        rbBlack->setChecked(true);
    }
    /*if (RS_SETTINGS->readEntry("/Blackwhite", "0")=="1") {
        rbBlackwhite->setChecked(true);
    }*/
    RS_SETTINGS->endGroup();
}


/**
 * Destructor
 */
QG_DlgImageOptions::~QG_DlgImageOptions() {}

void QG_DlgImageOptions::setGraphicSize(const RS_Vector& s) {
    graphicSize = s;
}


void QG_DlgImageOptions::ok() {
    RS_SETTINGS->beginGroup("/ExportImage");
    RS_SETTINGS->writeEntry("/Width", leWidth->text());
    RS_SETTINGS->writeEntry("/Height", leHeight->text());
    RS_SETTINGS->writeEntry("/BlackBackground", (int)rbBlack->isChecked());
    //RS_SETTINGS->writeEntry("/Blackwhite", (int)rbBlackwhite->isChecked());
    RS_SETTINGS->endGroup();

    accept();
}

void QG_DlgImageOptions::sizeChanged() {
    if (updateEnabled) {
        updateEnabled = false;
        cbResolution->setEditText("auto");
        updateEnabled = true;
    }
}

void  QG_DlgImageOptions::resolutionChanged() {
    if (updateEnabled) {
        updateEnabled = false;
        bool ok = false;
        double res = RS_Math::eval(cbResolution->currentText(), &ok);
        if (!ok) {
            res = 1.0;
        }
        int w = RS_Math::mround(res * graphicSize.x);
        int h = RS_Math::mround(res * graphicSize.y);
        leWidth->setText(QString("%1").arg(w));
        leHeight->setText(QString("%1").arg(h));
        updateEnabled = true;
    }
}

QSize QG_DlgImageOptions::getSize() {
    return QSize(RS_Math::mround(RS_Math::eval(leWidth->text())),
                 RS_Math::mround(RS_Math::eval(leHeight->text())));
}

bool QG_DlgImageOptions::isBackgroundBlack() {
    return rbBlack->isChecked();
}
