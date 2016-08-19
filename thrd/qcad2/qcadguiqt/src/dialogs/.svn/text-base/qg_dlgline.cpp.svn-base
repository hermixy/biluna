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

#include "qg_dlgline.h"

#include "rs_line.h"
#include "rs_graphic.h"

/**
 * Constructor
 */
QG_DlgLine::QG_DlgLine(QWidget* parent)
        : QDialog(parent) {
    setupUi(this);
    
}


/**
 * Destructor
 */
QG_DlgLine::~QG_DlgLine() {
}

void QG_DlgLine::setLine(RS_Line& l) {
    line = &l;
    //pen = line->getPen();
    wPen->setPen(line->getPen(false), true, false, tr("Pen"));
    RS_Graphic* graphic = line->getGraphic();
    if (graphic!=NULL) {
        cbLayer->init(*(graphic->getLayerList()), false, false);
    }
    RS_Layer* lay = line->getLayer(false);
    if (lay!=NULL) {
        cbLayer->setLayer(*lay);
    }
    QString s;
    s.setNum(line->getStartpoint().x);
    leStartX->setText(s);
    s.setNum(line->getStartpoint().y);
    leStartY->setText(s);
    s.setNum(line->getEndpoint().x);
    leEndX->setText(s);
    s.setNum(line->getEndpoint().y);
    leEndY->setText(s);
}

void QG_DlgLine::updateLine() {
    line->setStartpoint(RS_Vector(RS_Math::eval(leStartX->text()),
                                  RS_Math::eval(leStartY->text())));
    line->setEndpoint(RS_Vector(RS_Math::eval(leEndX->text()),
                                RS_Math::eval(leEndY->text())));
    line->setPen(wPen->getPen());
    line->setLayer(cbLayer->currentText());
}

