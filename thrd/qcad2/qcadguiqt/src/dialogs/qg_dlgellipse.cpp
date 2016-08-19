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

#include "qg_dlgellipse.h"

#include "rs_ellipse.h"
#include "rs_graphic.h"

/**
 * Constructor
 */
QG_DlgEllipse::QG_DlgEllipse(QWidget* parent)
        : QDialog(parent) {
    setupUi(this);
    
}


/**
 * Destructor
 */
QG_DlgEllipse::~QG_DlgEllipse() {
}

void QG_DlgEllipse::setEllipse(RS_Ellipse& e) {
    ellipse = &e;
    //pen = ellipse->getPen();
    wPen->setPen(ellipse->getPen(false), true, false, "Pen");
    RS_Graphic* graphic = ellipse->getGraphic();
    if (graphic!=NULL) {
        cbLayer->init(*(graphic->getLayerList()), false, false);
    }
    RS_Layer* lay = ellipse->getLayer(false);
    if (lay!=NULL) {
        cbLayer->setLayer(*lay);
    }
    QString s;
    s.setNum(ellipse->getCenter().x);
    leCenterX->setText(s);
    s.setNum(ellipse->getCenter().y);
    leCenterY->setText(s);
    s.setNum(ellipse->getMajorP().magnitude());
    leMajor->setText(s);
    s.setNum(ellipse->getMajorP().magnitude()*ellipse->getRatio());
    leMinor->setText(s);
    s.setNum(RS_Math::rad2deg(ellipse->getMajorP().angle()));
    leRotation->setText(s);
    s.setNum(RS_Math::rad2deg(ellipse->getAngle1()));
    leAngle1->setText(s);
    s.setNum(RS_Math::rad2deg(ellipse->getAngle2()));
    leAngle2->setText(s);
    //cbReversed->setChecked(ellipse->isReversed());
}

void QG_DlgEllipse::updateEllipse() {
    ellipse->setCenter(RS_Vector(RS_Math::eval(leCenterX->text()),
                                  RS_Math::eval(leCenterY->text())));
    RS_Vector v;
    v.setPolar(RS_Math::eval(leMajor->text()), 
               RS_Math::deg2rad(RS_Math::eval(leRotation->text())));
    ellipse->setMajorP(v);
    if (RS_Math::eval(leMajor->text())>1.0e-6) {
        ellipse->setRatio(RS_Math::eval(leMinor->text())/RS_Math::eval(leMajor->text()));
    }
    else {
        ellipse->setRatio(1.0);
    }
    ellipse->setAngle1(RS_Math::deg2rad(RS_Math::eval(leAngle1->text())));
    ellipse->setAngle2(RS_Math::deg2rad(RS_Math::eval(leAngle2->text())));
    //ellipse->setReversed(cbReversed->isChecked());
    ellipse->setReversed(false);
    ellipse->setPen(wPen->getPen());
    ellipse->setLayer(cbLayer->currentText());
    ellipse->calculateBorders();
}
