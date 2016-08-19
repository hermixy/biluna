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

#include "qg_polylineequidistantoptions.h"

#include "rs_actionpolylineequidistant.h"
#include "rs_settings.h"

/**
 * Constructor
 */
QG_PolylineEquidistantOptions::QG_PolylineEquidistantOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);

    connect(leDist, SIGNAL(textChanged(const QString&)),
        this, SLOT(updateDist(const QString&)));
    connect(sbNumber, SIGNAL(valueChanged(int)),
        this, SLOT(updateNumber(int)));
    connect(cbUseArcs, SIGNAL(toggled(bool)),
        this, SLOT(updateUseArcs(bool)));
}


/**
 * Destructor
 */
QG_PolylineEquidistantOptions::~QG_PolylineEquidistantOptions() {
    RS_SETTINGS->beginGroup("/Draw");
    RS_SETTINGS->writeEntry("/PolylineEquidistantDistance", leDist->text());
    RS_SETTINGS->writeEntry("/PolylineEquidistantNumber", sbNumber->text());
    RS_SETTINGS->writeEntry("/PolylineEquidistantUseArcs", cbUseArcs->isChecked());
    RS_SETTINGS->endGroup();
}


void QG_PolylineEquidistantOptions::setAction(RS_ActionInterface* a, bool update) {
    action = dynamic_cast<RS_ActionPolylineEquidistant*>(a);
    if (action!=NULL) {
        QString sd;
        QString sn;
        QString su;
        if (update) {
            sd = QString("%1").arg(action->getDistance());
            sn = QString("%1").arg(action->getNumber());
            su = QString("%1").arg((int)action->getUseArcs());
        } else {
            RS_SETTINGS->beginGroup("/Draw");
            sd = RS_SETTINGS->readEntry("/PolylineEquidistantDistance", "1.0");
            sn = RS_SETTINGS->readEntry("/PolylineEquidistantNumber", "1");
            su = RS_SETTINGS->readEntry("/PolylineEquidistantUseArcs", "1");
            RS_SETTINGS->endGroup();
        }
        leDist->setText(sd);
        sbNumber->setValue(sn.toInt());
        cbUseArcs->setChecked((bool)su.toInt());

        updateDist(sd);
        updateNumber(sn.toInt());
        updateUseArcs((bool)su.toInt());
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_PolylineEquidistantOptions::setAction: wrong action type");
        action = NULL;
    }
}

void QG_PolylineEquidistantOptions::updateDist(const QString& d) {
    RS_DEBUG->print("QG_PolylineEquidistantOptions::updateDist");
    if (action!=NULL) {
        action->setDistance(RS_Math::eval(d));
    }
}

void QG_PolylineEquidistantOptions::updateNumber(int n) {
    RS_DEBUG->print("QG_PolylineEquidistantOptions::updateNumber");
    if (action!=NULL) {
        action->setNumber(n);
    }
}

void QG_PolylineEquidistantOptions::updateUseArcs(bool u) {
    RS_DEBUG->print("QG_PolylineEquidistantOptions::updateUseArcs");
    if (action!=NULL) {
        action->setUseArcs(u);
    }
}
