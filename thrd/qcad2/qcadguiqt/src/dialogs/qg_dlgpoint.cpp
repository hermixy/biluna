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

#include "qg_dlgpoint.h"

#include "rs_point.h"
#include "rs_graphic.h"

/**
 * Constructor
 */
QG_DlgPoint::QG_DlgPoint(QWidget* parent)
        : QDialog(parent) {
    setupUi(this);
    
}


/**
 * Destructor
 */
QG_DlgPoint::~QG_DlgPoint() {
}

void QG_DlgPoint::setPoint(RS_Point& p) {
    point = &p;

    wPen->setPen(point->getPen(false), true, false, "Pen");
    RS_Graphic* graphic = point->getGraphic();
    if (graphic!=NULL) {
        cbLayer->init(*(graphic->getLayerList()), false, false);
    }
    RS_Layer* lay = point->getLayer(false);
    if (lay!=NULL) {
        cbLayer->setLayer(*lay);
    }

    QString s;
    s.setNum(point->getPos().x);
    lePosX->setText(s);
    s.setNum(point->getPos().y);
    lePosY->setText(s);
}

void QG_DlgPoint::updatePoint() {
    point->setPos(RS_Vector(RS_Math::eval(lePosX->text()),
                            RS_Math::eval(lePosY->text())));
    point->setPen(wPen->getPen());
    point->setLayer(cbLayer->currentText());
}

