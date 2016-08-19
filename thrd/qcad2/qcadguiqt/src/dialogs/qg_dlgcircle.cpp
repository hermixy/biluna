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

#include "qg_dlgcircle.h"

#include "rs_circle.h"
#include "rs_graphic.h"

/**
 * Constructor
 */
QG_DlgCircle::QG_DlgCircle(QWidget* parent)
        : QDialog(parent) {
    setupUi(this);
    
}


/**
 * Destructor
 */
QG_DlgCircle::~QG_DlgCircle() {
}

void QG_DlgCircle::setCircle(RS_Circle& c) {
    circle = &c;
    //pen = circle->getPen();
    wPen->setPen(circle->getPen(false), true, false, "Pen");
    RS_Graphic* graphic = circle->getGraphic();
    if (graphic!=NULL) {
        cbLayer->init(*(graphic->getLayerList()), false, false);
    }
    RS_Layer* lay = circle->getLayer(false);
    if (lay!=NULL) {
        cbLayer->setLayer(*lay);
    }
    QString s;
    s.setNum(circle->getCenter().x);
    leCenterX->setText(s);
    s.setNum(circle->getCenter().y);
    leCenterY->setText(s);
    s.setNum(circle->getRadius());
    leRadius->setText(s);
}

void QG_DlgCircle::updateCircle() {
    circle->setCenter(RS_Vector(RS_Math::eval(leCenterX->text()),
                                  RS_Math::eval(leCenterY->text())));
    circle->setRadius(RS_Math::eval(leRadius->text()));
    circle->setPen(wPen->getPen());
    circle->setLayer(cbLayer->currentText());
    circle->calculateBorders();
}

