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

#include "qg_dlgattributes.h"

#include "rs_modification.h"

/**
 * Constructor
 */
QG_DlgAttributes::QG_DlgAttributes(QWidget* parent)
        : QDialog(parent) {
    setupUi(this);
    
}


/**
 * Destructor
 */
QG_DlgAttributes::~QG_DlgAttributes() {
}



void QG_DlgAttributes::setData(RS_AttributesData* data, RS_LayerList& layerList) {
    this->data = data;
    
    wPen->setPen(data->pen, true, true, "Pen");
    cbLayer->init(layerList, false, true);
}

void QG_DlgAttributes::updateData() {
    data->pen = wPen->getPen();
    data->layer = cbLayer->currentText();
    
    data->changeColor = !wPen->isColorUnchanged();
    data->changeLineType = !wPen->isLineTypeUnchanged();
    data->changeWidth = !wPen->isWidthUnchanged();
    
    data->changeLayer = !cbLayer->isUnchanged();
}

