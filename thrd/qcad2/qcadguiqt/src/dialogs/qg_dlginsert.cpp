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

#include "qg_dlginsert.h"

#include "rs_insert.h"
#include "rs_graphic.h"

/**
 * Constructor
 */
QG_DlgInsert::QG_DlgInsert(QWidget* parent)
        : QDialog(parent) {
    setupUi(this);
    
}


/**
 * Destructor
 */
QG_DlgInsert::~QG_DlgInsert() {
}

void QG_DlgInsert::setInsert(RS_Insert& i) {
    insert = &i;
    //pen = insert->getPen();
    wPen->setPen(insert->getPen(false), true, false, "Pen");
    RS_Graphic* graphic = insert->getGraphic();
    if (graphic!=NULL) {
        cbLayer->init(*(graphic->getLayerList()), false, false);
    }
    RS_Layer* lay = insert->getLayer(false);
    if (lay!=NULL) {
        cbLayer->setLayer(*lay);
    }
    QString s;
    s.setNum(insert->getInsertionPoint().x);
    leInsertionPointX->setText(s);
    s.setNum(insert->getInsertionPoint().y);
    leInsertionPointY->setText(s);
    s.setNum(insert->getScale().x);
    leScale->setText(s);
    s.setNum(RS_Math::rad2deg(insert->getAngle()));
    leAngle->setText(s);
    s.setNum(insert->getRows());
    leRows->setText(s);
    s.setNum(insert->getCols());
    leCols->setText(s);
    s.setNum(insert->getSpacing().x);
    leRowSpacing->setText(s);
    s.setNum(insert->getSpacing().y);
    leColSpacing->setText(s);
    leBlock->setText(insert->getName());
}

void QG_DlgInsert::updateInsert() {
    insert->setInsertionPoint(RS_Vector(RS_Math::eval(leInsertionPointX->text()),
                                  RS_Math::eval(leInsertionPointY->text())));
    insert->setScale(RS_Vector(RS_Math::eval(leScale->text()), 
                                RS_Math::eval(leScale->text())));
    insert->setAngle(RS_Math::deg2rad(RS_Math::eval(leAngle->text())));
    insert->setRows(RS_Math::mround(RS_Math::eval(leRows->text())));
    insert->setCols(RS_Math::mround(RS_Math::eval(leCols->text())));
    insert->setSpacing(RS_Vector(RS_Math::eval(leRowSpacing->text()),
                                 RS_Math::eval(leColSpacing->text())));
    insert->setPen(wPen->getPen());
    insert->setLayer(cbLayer->currentText());
}

