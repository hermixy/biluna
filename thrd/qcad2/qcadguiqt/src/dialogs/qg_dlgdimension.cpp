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

#include "qg_dlgdimension.h"

#include "rs_dimension.h"
#include "rs_graphic.h"

/**
 * Constructor
 */
QG_DlgDimension::QG_DlgDimension(QWidget* parent)
        : QDialog(parent) {
    setupUi(this);
    
}


/**
 * Destructor
 */
QG_DlgDimension::~QG_DlgDimension() {
}

void QG_DlgDimension::setDim(RS_Dimension& d) {
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
}

void QG_DlgDimension::updateDim() {
    dim->setLabel(wLabel->getLabel());
    dim->setPen(wPen->getPen());
    dim->setLayer(cbLayer->currentText());
}


