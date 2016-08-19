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

#include "qg_dlgdimlinear.h"

#include "rs_dimlinear.h"
#include "rs_graphic.h"

/**
 * Constructor
 */
QG_DlgDimLinear::QG_DlgDimLinear(QWidget* parent)
        : QDialog(parent) {
    setupUi(this);
    
}


/**
 * Destructor
 */
QG_DlgDimLinear::~QG_DlgDimLinear() {
}

void QG_DlgDimLinear::setDim(RS_DimLinear& d) {
    dim = &d;
    wPen->setPen(dim->getPen(false), true, false, "Pen");
    RS_Graphic* graphic = dim->getGraphic();
    if (graphic!=NULL) {
        cbLayer->init(*(graphic->getLayerList()), false, false);
    }
    RS_Layer* lay = dim->getLayer(false);
    if (lay!=NULL) {
        cbLayer->setLayer(*lay);
    }
    
    wLabel->setLabel(dim->getLabel(false));
    leAngle->setText(QString("%1").arg(RS_Math::rad2deg(dim->getAngle())));
}

void QG_DlgDimLinear::updateDim() {
    dim->setLabel(wLabel->getLabel());
    dim->setAngle(RS_Math::deg2rad(RS_Math::eval(leAngle->text(), 0.0)));
    dim->setPen(wPen->getPen());
    dim->setLayer(cbLayer->currentText());
}


