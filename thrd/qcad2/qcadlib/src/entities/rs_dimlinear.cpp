/****************************************************************************
** $Id: rs_dimlinear.cpp 7930 2008-01-17 21:23:04Z andrew $
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


#include "rs_dimlinear.h"
#include "rs_constructionline.h"
#include "rs_solid.h"
#include "rs_graphic.h"

const char* RS_DimLinear::propertyDefinitionPointX = QT_TRANSLATE_NOOP("QObject", "Line Pos.|X");
const char* RS_DimLinear::propertyDefinitionPointY = QT_TRANSLATE_NOOP("QObject", "Line Pos.|Y");
const char* RS_DimLinear::propertyExtensionPoint1X = QT_TRANSLATE_NOOP("QObject", "Extension|X1");
const char* RS_DimLinear::propertyExtensionPoint1Y = QT_TRANSLATE_NOOP("QObject", "Extension|Y1");
const char* RS_DimLinear::propertyExtensionPoint2X = QT_TRANSLATE_NOOP("QObject", "Extension|X2");
const char* RS_DimLinear::propertyExtensionPoint2Y = QT_TRANSLATE_NOOP("QObject", "Extension|Y2");
const char* RS_DimLinear::propertyAngle = QT_TRANSLATE_NOOP("QObject", "Angle");


/**
 * Constructor.
 *
 * @para parentContainer Parent Entity Container.
 * @para d Common dimension geometrical data.
 * @para ed Extended geometrical data for linear dimension.
 */
RS_DimLinear::RS_DimLinear(RS_EntityContainer* parentContainer,
                           const RS_DimensionData& d,
                           const RS_DimLinearData& ed)
        : RS_Entity(parentContainer), RS_Dimension(parentContainer, d), edata(ed) {

    calculateBorders();
}



RS_VectorList RS_DimLinear::getRefPoints(RS2::RefMode /*refMode*/) {
    RS_VectorList ret;
    ret << edata.extensionPoint1;
    ret << edata.extensionPoint2;
    ret << data.definitionPoint;
    if (data.middleOfText.valid) {
        ret << data.middleOfText;
    }
    else {
        ret << autoTextPos;
    }
    return ret;
}


/**
 * @return Automatically created label for the default 
 * measurement of this dimension.
 */
RS_String RS_DimLinear::getMeasuredLabel() {
    // direction of dimension line
    RS_Vector dirDim;
    dirDim.setPolar(100.0, edata.angle);

    // construction line for dimension line
    RS_ConstructionLine dimLine(NULL,
                                RS_ConstructionLineData(data.definitionPoint,
                                                        data.definitionPoint + dirDim));

    RS_Vector dimP1 = dimLine.getNearestPointOnEntity(edata.extensionPoint1);
    RS_Vector dimP2 = dimLine.getNearestPointOnEntity(edata.extensionPoint2);

    // Definitive dimension line:
    double dist = dimP1.distanceTo(dimP2);

    RS_Graphic* graphic = getGraphic();

    RS_String ret;
    if (graphic!=NULL) {
        ret = RS_Units::formatLinear(dist, graphic->getUnit(), 
            graphic->getLinearFormat(), graphic->getLinearPrecision(),
            false,
            graphic->showLeadingZeroes(), graphic->showTrailingZeroes());
    }
    else {
        ret = RS_String("%1").arg(dist);
    }

    return ret;
}



bool RS_DimLinear::hasEndpointsWithinWindow(const RS_Vector& v1, const RS_Vector& v2) {
    return (edata.extensionPoint1.isInWindow(v1, v2) ||
            edata.extensionPoint2.isInWindow(v1, v2));
}



/**
 * Updates the sub entities of this dimension. Called when the 
 * text or the position, alignment, .. changes.
 *
 * @param autoText Automatically reposition the text label
 */
void RS_DimLinear::update(bool autoText) {

    RS_DEBUG->print("RS_DimLinear::update");

    clear();

    if (isUndone()) {
        return;
    }

    // distance from entities (DIMEXO)
    double dimexo = getExtensionLineOffset();
    // extension line extension (DIMEXE)
    double dimexe = getExtensionLineExtension();

    RS_LineData ld;
    double extAngle = edata.angle + (M_PI/2.0);

    // direction of dimension line
    RS_Vector dirDim;
    dirDim.setPolar(100.0, edata.angle);
    // direction of extension lines
    RS_Vector dirExt;
    dirExt.setPolar(100.0, extAngle);

    // construction line for dimension line
    RS_ConstructionLine dimLine(
        NULL,
        RS_ConstructionLineData(data.definitionPoint,
                                data.definitionPoint + dirDim));

    RS_Vector dimP1 = dimLine.getNearestPointOnEntity(edata.extensionPoint1);
    RS_Vector dimP2 = dimLine.getNearestPointOnEntity(edata.extensionPoint2);

    // Definitive dimension line:
    updateCreateDimensionLine(dimP1, dimP2, true, true, autoText);
    /*
    ld = RS_LineData(data.definitionPoint, dimP1);
    RS_Line* dimensionLine = new RS_Line(this, ld);
       addEntity(dimensionLine);
    */
    RS_Vector vDimexo1, vDimexe1, vDimexo2, vDimexe2;
    vDimexe1.setPolar(dimexe, edata.extensionPoint1.angleTo(dimP1));
    vDimexo1.setPolar(dimexo, edata.extensionPoint1.angleTo(dimP1));

    vDimexe2.setPolar(dimexe, edata.extensionPoint2.angleTo(dimP2));
    vDimexo2.setPolar(dimexo, edata.extensionPoint2.angleTo(dimP2));
    
    if ((edata.extensionPoint1-dimP1).magnitude()<1e-6) {
        vDimexe1.setPolar(dimexe,
                          data.definitionPoint.angleTo(dimP1)-M_PI/2.0);
        vDimexo1.setPolar(dimexo,
                          data.definitionPoint.angleTo(dimP1)-M_PI/2.0);
    }
    if ((edata.extensionPoint2-dimP2).magnitude()<1e-6) {
        vDimexe2.setPolar(dimexe,
                          data.definitionPoint.angleTo(dimP2)-M_PI/2.0);
        vDimexo2.setPolar(dimexo,
                          data.definitionPoint.angleTo(dimP2)-M_PI/2.0);
    }
    
    // extension lines:
    ld = RS_LineData(edata.extensionPoint1+vDimexo1,
                     dimP1+vDimexe1);
    RS_Line* line = new RS_Line(this, ld);
    line->setPen(RS_Pen(RS2::FlagInvalid));
    line->setLayer(NULL);
    addEntity(line);
    ld = RS_LineData(edata.extensionPoint2+vDimexo2,
                     dimP2+vDimexe2);
    //data.definitionPoint+vDimexe2);
    line = new RS_Line(this, ld);
    line->setPen(RS_Pen(RS2::FlagInvalid));
    line->setLayer(NULL);
    addEntity(line);

    calculateBorders();
}



void RS_DimLinear::move(const RS_Vector& offset) {
    RS_Dimension::move(offset);

    edata.extensionPoint1.move(offset);
    edata.extensionPoint2.move(offset);
    update();
}



void RS_DimLinear::rotate(const RS_Vector& center, double angle) {
    RS_Dimension::rotate(center, angle);

    edata.extensionPoint1.rotate(center, angle);
    edata.extensionPoint2.rotate(center, angle);
    edata.angle = RS_Math::correctAngle(edata.angle+angle);
    update();
}



void RS_DimLinear::scale(const RS_Vector& center, const RS_Vector& factor) {
    RS_Dimension::scale(center, factor);

    edata.extensionPoint1.scale(center, factor);
    edata.extensionPoint2.scale(center, factor);
    update();
}



void RS_DimLinear::mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2) {
    RS_Dimension::mirror(axisPoint1, axisPoint2);

    edata.extensionPoint1.mirror(axisPoint1, axisPoint2);
    edata.extensionPoint2.mirror(axisPoint1, axisPoint2);

    RS_Vector vec;
    vec.setPolar(1.0, edata.angle);
    vec.mirror(RS_Vector(0.0,0.0), axisPoint2-axisPoint1);
    edata.angle = vec.angle();

    update();
}



void RS_DimLinear::stretch(const RS_Vector& firstCorner,
                           const RS_Vector& secondCorner,
                           const RS_Vector& offset) {

    //e->calculateBorders();
    if (getMin().isInWindow(firstCorner, secondCorner) &&
            getMax().isInWindow(firstCorner, secondCorner)) {

        move(offset);
    } else {
        //RS_Vector v = data.definitionPoint - edata.extensionPoint2;
        //double len = edata.extensionPoint2.distanceTo(data.definitionPoint);
        //double ang1 = edata.extensionPoint1.angleTo(edata.extensionPoint2)
        //              + M_PI/2;

        if (edata.extensionPoint1.isInWindow(firstCorner,
                                            secondCorner)) {
            edata.extensionPoint1.move(offset);
        }
        if (edata.extensionPoint2.isInWindow(firstCorner,
                                            secondCorner)) {
            edata.extensionPoint2.move(offset);
        }

        /*
        double ang2 = edata.extensionPoint1.angleTo(edata.extensionPoint2)
                      + M_PI/2;

        double diff = RS_Math::getAngleDifference(ang1, ang2);
        if (diff>M_PI) {
            diff-=2*M_PI;
        }

        if (fabs(diff)>M_PI/2) {
            ang2 = RS_Math::correctAngle(ang2+M_PI);
        }

        RS_Vector v;
        v.setPolar(len, ang2);
        data.definitionPoint = edata.extensionPoint2 + v;
        */
    }
    update(true);
}



void RS_DimLinear::moveRef(const RS_Vector& ref, const RS_Vector& offset) {

    if (ref.distanceTo(data.definitionPoint)<1.0e-4) {
        data.definitionPoint += offset;
        update(true);
    }
    else if (ref.distanceTo(data.middleOfText)<1.0e-4) {
        data.middleOfText += offset;
        update(false);
    }
    else if (ref.distanceTo(edata.extensionPoint1)<1.0e-4) {
        edata.extensionPoint1 += offset;
        update(true);
    }
    else if (ref.distanceTo(edata.extensionPoint2)<1.0e-4) {
        edata.extensionPoint2 += offset;
        update(true);
    }
    else if (ref.distanceTo(autoTextPos)<1.0e-4) {
        data.middleOfText = autoTextPos + offset;
        update(false);
    }
}



RS_Variant RS_DimLinear::getProperty(const RS_String& name, const RS_Variant& def) {
    if (name==RS_DIMLINEAR_DEFINITIONPOINT_X) {
        return data.definitionPoint.x;
    } 
    else if (name==RS_DIMLINEAR_DEFINITIONPOINT_Y) {
        return data.definitionPoint.y;
    } 
    if (name==RS_DIMLINEAR_EXTENSIONPOINT1_X) {
        return edata.extensionPoint1.x;
    } 
    else if (name==RS_DIMLINEAR_EXTENSIONPOINT1_Y) {
        return edata.extensionPoint1.y;
    } 
    else if (name==RS_DIMLINEAR_EXTENSIONPOINT2_X) {
        return edata.extensionPoint2.x;
    } 
    else if (name==RS_DIMLINEAR_EXTENSIONPOINT2_Y) {
        return edata.extensionPoint2.y;
    } 
    else if (name==RS_DIMLINEAR_ANGLE) {
        return RS_Math::rad2deg(edata.angle);
    } else {
        return RS_Dimension::getProperty(name, def);
    }
}





RS_StringList RS_DimLinear::getPropertyNames(bool includeGeometry) {
    RS_StringList ret = RS_Dimension::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret << RS_DIMLINEAR_DEFINITIONPOINT_X;
        ret << RS_DIMLINEAR_DEFINITIONPOINT_Y;
        ret << RS_DIMLINEAR_EXTENSIONPOINT1_X;
        ret << RS_DIMLINEAR_EXTENSIONPOINT1_Y;
        ret << RS_DIMLINEAR_EXTENSIONPOINT2_X;
        ret << RS_DIMLINEAR_EXTENSIONPOINT2_Y;
        ret << RS_DIMLINEAR_ANGLE;
    }
    // this entity renamed the definition point from RS_Dimension:
    ret.removeAll(RS_DIM_DEFINITIONPOINT_X);
    ret.removeAll(RS_DIM_DEFINITIONPOINT_Y);

    return ret;
}



void RS_DimLinear::setProperty(const RS_String& name, const RS_Variant& value) {
    if (name==RS_DIMLINEAR_DEFINITIONPOINT_X) {
        data.definitionPoint.x = RS_Math::eval(value.toString());
        update(true);
    }
    if (name==RS_DIMLINEAR_DEFINITIONPOINT_Y) {
        data.definitionPoint.y = RS_Math::eval(value.toString());
        update(true);
    }
    if (name==RS_DIMLINEAR_EXTENSIONPOINT1_X) {
        edata.extensionPoint1.x = RS_Math::eval(value.toString());
        update(true);
    }
    if (name==RS_DIMLINEAR_EXTENSIONPOINT1_Y) {
        edata.extensionPoint1.y = RS_Math::eval(value.toString());
        update(true);
    }
    if (name==RS_DIMLINEAR_EXTENSIONPOINT2_X) {
        edata.extensionPoint2.x = RS_Math::eval(value.toString());
        update(true);
    }
    if (name==RS_DIMLINEAR_EXTENSIONPOINT2_Y) {
        edata.extensionPoint2.y = RS_Math::eval(value.toString());
        update(true);
    }
    if (name==RS_DIMLINEAR_ANGLE) {
        edata.angle = RS_Math::deg2rad(RS_Math::eval(value.toString()));
        update(true);
    }
    
    RS_Dimension::setProperty(name, value);
}
