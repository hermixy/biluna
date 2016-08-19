/****************************************************************************
** $Id: qg_colorbox.cpp 2510 2005-07-26 20:28:53Z andrew $
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

#include "qg_coordinatewidget.h"

#include "rs_graphic.h"
#include "rs_settings.h"
#include "rs_vector.h"

/**
 * Constructor
 */
QG_CoordinateWidget::QG_CoordinateWidget(QWidget* parent)
        : QWidget(parent) {
    setupUi(this);
    
    lAbsCart->setText("");
    lRelCart->setText("");
    lAbsPol->setText("");
    lRelPol->setText("");
    
    int fsize;
#ifdef __APPLE__
    fsize = 9;
#else
    fsize = 9;
#endif
    
    RS_SETTINGS->beginGroup("/Appearance");
    fsize = RS_SETTINGS->readNumEntry("/StatusBarFontSize", fsize);
    RS_SETTINGS->endGroup();

    QFont font = lAbsCart->font();
    font.setPixelSize(fsize);
    
    lAbsCart->setFont(font);
    lAbsPol->setFont(font);
    lRelCart->setFont(font);
    lRelPol->setFont(font);
    
    graphic = NULL;
    prec = 4;
    format = RS2::Decimal;
    aprec = 2;
    aformat = RS2::DegreesDecimal;
}


/**
 * Destructor
 */
QG_CoordinateWidget::~QG_CoordinateWidget() {}


void QG_CoordinateWidget::setGraphic(RS_Graphic* graphic) {
    RS_DEBUG->print("QG_CoordinateWidget::setGraphic: %d", (int)graphic);
    if (graphic==NULL) {
        RS_DEBUG->print("QG_CoordinateWidget::setGraphic to NULL");
    }
    this->graphic = graphic;
    
    setCoordinates(RS_Vector(0.0,0.0), RS_Vector(0.0,0.0), true);
}



void QG_CoordinateWidget::setCoordinates(const RS_Vector& abs,
                                         const RS_Vector& rel, bool updateFormat) {

    RS_DEBUG->print("QG_CoordinateWidget::setCoordinates");
    
    if (graphic==NULL) {
        return;
    }

    RS_DEBUG->print("QG_CoordinateWidget::setCoordinates: 001");
    if (updateFormat) {
        format = graphic->getLinearFormat();
        prec = graphic->getLinearPrecision();
        aformat = graphic->getAngleFormat();
        aprec = graphic->getAnglePrecision();
    }
    RS_DEBUG->print("QG_CoordinateWidget::setCoordinates: 002");

    RS_String absX;
    RS_String absY;
    RS_String relX;
    RS_String relY;
    
    RS_DEBUG->print("QG_CoordinateWidget::setCoordinates: 003");
    // abs / rel coordinates:
    if (abs.valid) {
        RS_DEBUG->print("QG_CoordinateWidget::setCoordinates: 003a");
        RS_DEBUG->print("QG_CoordinateWidget::setCoordinates: 003a0: %d", (int)graphic->getUnit());
        RS_DEBUG->print("QG_CoordinateWidget::setCoordinates: 003a1");
        absX = RS_Units::formatLinear(abs.x,
                                     graphic->getUnit(),
                                     format, prec);
        RS_DEBUG->print("QG_CoordinateWidget::setCoordinates: 003b");
        absY = RS_Units::formatLinear(abs.y,
                                     graphic->getUnit(),
                                     format, prec);
        RS_DEBUG->print("QG_CoordinateWidget::setCoordinates: 003c");
    }
    else {
        absX = "invalid";
        absY = "invalid";
    }
    RS_DEBUG->print("QG_CoordinateWidget::setCoordinates: 003d");
    lAbsCart->setText(absX + " , " + absY);
    
    RS_DEBUG->print("QG_CoordinateWidget::setCoordinates: 004");
    if (rel.valid) {
        relX = RS_Units::formatLinear(rel.x,
                                     graphic->getUnit(),
                                     format, prec);
        relY = RS_Units::formatLinear(rel.y,
                                     graphic->getUnit(),
                                     format, prec);
        lRelCart->setText(relX + " , " + relY);        
    }
    else {
        lRelCart->setText("");
    }
    
    RS_DEBUG->print("QG_CoordinateWidget::setCoordinates: 005");
    if (abs.valid) {
        // polar coordinates:
        QString rStr = RS_Units::formatLinear(abs.magnitude(),
                                               graphic->getUnit(),
                                               format, prec);
        QString aStr = RS_Units::formatAngle(abs.angle(),
                                               aformat, aprec);
        
        lAbsPol->setText(rStr + " < " + aStr);
    }
    else {
        lAbsPol->setText("");
    }
    
    RS_DEBUG->print("QG_CoordinateWidget::setCoordinates: 006");
    
    if (rel.valid) {
        QString rStr = RS_Units::formatLinear(rel.magnitude(),
                                      graphic->getUnit(),
                                      format, prec);
        QString aStr = RS_Units::formatAngle(rel.angle(),
                                     aformat, aprec);
        lRelPol->setText(rStr + " < " + aStr);
    }
    else {
        lRelPol->setText("");
    }
    
    RS_DEBUG->print("QG_CoordinateWidget::setCoordinates: OK");
}
