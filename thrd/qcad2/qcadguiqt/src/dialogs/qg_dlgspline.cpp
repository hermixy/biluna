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

#include "qg_dlgspline.h"

#include "rs_spline.h"
#include "rs_graphic.h"

/**
 * Constructor
 */
QG_DlgSpline::QG_DlgSpline(QWidget* parent)
        : QDialog(parent) {
    setupUi(this);
    
    cbDegree->addItem("2");
    cbDegree->addItem("3");
}


/**
 * Destructor
 */
QG_DlgSpline::~QG_DlgSpline() {
}

void QG_DlgSpline::setSpline(RS_Spline& e) {
    spline = &e;
    //pen = spline->getPen();
    wPen->setPen(spline->getPen(false), true, false, "Pen");
    RS_Graphic* graphic = spline->getGraphic();
    if (graphic!=NULL) {
        cbLayer->init(*(graphic->getLayerList()), false, false);
    }
    RS_Layer* lay = spline->getLayer(false);
    if (lay!=NULL) {
        cbLayer->setLayer(*lay);
    }
    
    QString s;
    s.setNum(spline->getDegree());
    cbDegree->setCurrentIndex(cbDegree->findText(s));

    cbClosed->setChecked(spline->isClosed());
}

void QG_DlgSpline::updateSpline() {
    spline->setDegree(RS_Math::mround(RS_Math::eval(cbDegree->currentText())));
    spline->setClosed(cbClosed->isChecked());
    spline->setPen(wPen->getPen());
    spline->setLayer(cbLayer->currentText());
    spline->update();
}

