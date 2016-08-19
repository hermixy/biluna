/****************************************************************************
**
** Copyright (C) 2001-2008 RibbonSoft. All rights reserved.
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

#include "qg_editpasteoptions.h"

#include "rs_actioneditpaste.h"
#include "rs_settings.h"



/**
 * Constructor
 */
QG_EditPasteOptions::QG_EditPasteOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);
    
    bFlipHorizontal->setIcon(QIcon(":/qcad/images/fliphorizontal.png"));
    bFlipVertical->setIcon(QIcon(":/qcad/images/flipvertical.png"));

    connect(leRotation, SIGNAL(textChanged(const QString&)),
        this, SLOT(updateData()));
    connect(leFactor, SIGNAL(textChanged(const QString&)),
        this, SLOT(updateData()));
    connect(bFlipHorizontal, SIGNAL(toggled(bool)),
        this, SLOT(updateData()));
    connect(bFlipVertical, SIGNAL(toggled(bool)),
        this, SLOT(updateData()));
    connect(cbUseCurrentLayer, SIGNAL(toggled(bool)),
        this, SLOT(updateData()));
}


/**
 * Destructor
 */
QG_EditPasteOptions::~QG_EditPasteOptions() {
    RS_SETTINGS->beginGroup("/EditPaste");
    RS_SETTINGS->writeEntry("/EditPasteRotation", leRotation->text());
    RS_SETTINGS->writeEntry("/EditPasteFactor", leFactor->text());
    RS_SETTINGS->writeEntry("/EditPasteFlipHorizontal", bFlipHorizontal->isChecked());
    RS_SETTINGS->writeEntry("/EditPasteFlipVertical", bFlipVertical->isChecked());
    RS_SETTINGS->writeEntry("/EditPasteUseCurrentLayer", cbUseCurrentLayer->isChecked());
    RS_SETTINGS->endGroup();
}


void QG_EditPasteOptions::setAction(RS_ActionInterface* a, bool update) {
    action = dynamic_cast<RS_ActionEditPaste*>(a);
    if (action!=NULL) {
        QString sRotation;
        QString sFactor;
        QString sFlipHorizontal;
        QString sFlipVertical;
        QString sUseCurrentLayer;
        if (update) {
            sRotation = QString("%1").arg(RS_Math::rad2deg(action->getRotation()));
            sFactor = QString("%1").arg(action->getFactor());
            sFlipHorizontal = QString("%1").arg(action->getFlipHorizontal());
            sFlipVertical = QString("%1").arg(action->getFlipVertical());
            sUseCurrentLayer = QString("%1").arg(action->getUseCurrentLayer());
        } else {
            RS_SETTINGS->beginGroup("/EditPaste");
            sRotation = RS_SETTINGS->readEntry("/EditPasteRotation", "0.0");
            sFactor = RS_SETTINGS->readEntry("/EditPasteFactor", "1.0");
            sFlipHorizontal = RS_SETTINGS->readEntry("/EditPasteFlipHorizontal", "0");
            sFlipVertical = RS_SETTINGS->readEntry("/EditPasteFlipVertical", "0");
            sUseCurrentLayer = RS_SETTINGS->readEntry("/EditPasteUseCurrentLayer", "0");
            RS_SETTINGS->endGroup();
        }
        leRotation->setText(sRotation);
        leFactor->setText(sFactor);
        if (sFlipHorizontal=="1") {
            bFlipHorizontal->setChecked(true);
        }
        if (sFlipVertical=="1") {
            bFlipVertical->setChecked(true);
        }
        if (sUseCurrentLayer=="1") {
            cbUseCurrentLayer->setChecked(true);
        }
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR,
                        "QG_EditPasteOptions::setAction: wrong action type");
        action = NULL;
    }
}



void QG_EditPasteOptions::updateData() {
    RS_DEBUG->print("QG_EditPasteOptions::updateData");

    if (action!=NULL) {
        action->setRotation(RS_Math::eval(leRotation->text()));
        action->setFactor(RS_Math::eval(leFactor->text()));
        action->setFlipHorizontal(bFlipHorizontal->isChecked());
        action->setFlipVertical(bFlipVertical->isChecked());
        action->setUseCurrentLayer(cbUseCurrentLayer->isChecked());
    }
}
