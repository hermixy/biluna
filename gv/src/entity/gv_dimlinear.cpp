/****************************************************************************
** $Id: gv_dimlinear.cpp 7930 2008-01-17 21:23:04Z andrew $
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
** not clear to you. Updated for Biluna 2016-08-11 Updated for Biluna 2016-08-11
**
**********************************************************************/


#include "gv_dimlinear.h"
#include "gv_constructionline.h"
#include "gv_solid.h"
#include "gv_graphic.h"

const char* GV_DimLinear::propertyDefinitionPointX = QT_TRANSLATE_NOOP("QObject", "Line Pos.|X");
const char* GV_DimLinear::propertyDefinitionPointY = QT_TRANSLATE_NOOP("QObject", "Line Pos.|Y");
const char* GV_DimLinear::propertyExtensionPoint1X = QT_TRANSLATE_NOOP("QObject", "Extension|X1");
const char* GV_DimLinear::propertyExtensionPoint1Y = QT_TRANSLATE_NOOP("QObject", "Extension|Y1");
const char* GV_DimLinear::propertyExtensionPoint2X = QT_TRANSLATE_NOOP("QObject", "Extension|X2");
const char* GV_DimLinear::propertyExtensionPoint2Y = QT_TRANSLATE_NOOP("QObject", "Extension|Y2");
const char* GV_DimLinear::propertyAngle = QT_TRANSLATE_NOOP("QObject", "Angle");


/**
 * Constructor.
 *
 * @para parentContainer Parent Entity Container.
 * @para d Common dimension geometrical data.
 * @para ed Extended geometrical data for linear dimension.
 */
GV_DimLinear::GV_DimLinear(GV_EntityContainer* parentContainer,
                           const GV_DimensionData& d,
                           const GV_DimLinearData& ed)
        : GV_Entity(parentContainer), GV_Dimension(parentContainer, d), edata(ed) {

    calculateBorders();
}



GV_VectorList GV_DimLinear::getRefPoints(GV2::RefMode /*refMode*/) {
    GV_VectorList ret;
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
QString GV_DimLinear::getMeasuredLabel() {
    // direction of dimension line
    GV_Vector dirDim;
    dirDim.setPolar(100.0, edata.angle);

    // construction line for dimension line
    GV_ConstructionLine dimLine(NULL,
                                GV_ConstructionLineData(data.definitionPoint,
                                                        data.definitionPoint + dirDim));

    GV_Vector dimP1 = dimLine.getNearestPointOnEntity(edata.extensionPoint1);
    GV_Vector dimP2 = dimLine.getNearestPointOnEntity(edata.extensionPoint2);

    // Definitive dimension line:
    double dist = dimP1.distanceTo(dimP2);

    GV_Graphic* graphic = getGraphic();

    QString ret;
    if (graphic!=NULL) {
        ret = GV_Units::formatLinear(dist, graphic->getUnit(), 
            graphic->getLinearFormat(), graphic->getLinearPrecision(),
            false,
            graphic->showLeadingZeroes(), graphic->showTrailingZeroes());
    }
    else {
        ret = QString("%1").arg(dist);
    }

    return ret;
}



bool GV_DimLinear::hasEndpointsWithinWindow(const GV_Vector& v1, const GV_Vector& v2) {
    return (edata.extensionPoint1.isInWindow(v1, v2) ||
            edata.extensionPoint2.isInWindow(v1, v2));
}



/**
 * Updates the sub entities of this dimension. Called when the 
 * text or the position, alignment, .. changes.
 *
 * @param autoText Automatically reposition the text label
 */
void GV_DimLinear::update(bool autoText) {

    RB_DEBUG->print("GV_DimLinear::update");

    clear();

    if (isUndone()) {
        return;
    }

    // distance from entities (DIMEXO)
    double dimexo = getExtensionLineOffset();
    // extension line extension (DIMEXE)
    double dimexe = getExtensionLineExtension();

    GV_LineData ld;
    double extAngle = edata.angle + (M_PI/2.0);

    // direction of dimension line
    GV_Vector dirDim;
    dirDim.setPolar(100.0, edata.angle);
    // direction of extension lines
    GV_Vector dirExt;
    dirExt.setPolar(100.0, extAngle);

    // construction line for dimension line
    GV_ConstructionLine dimLine(
        NULL,
        GV_ConstructionLineData(data.definitionPoint,
                                data.definitionPoint + dirDim));

    GV_Vector dimP1 = dimLine.getNearestPointOnEntity(edata.extensionPoint1);
    GV_Vector dimP2 = dimLine.getNearestPointOnEntity(edata.extensionPoint2);

    // Definitive dimension line:
    updateCreateDimensionLine(dimP1, dimP2, true, true, autoText);
    /*
    ld = GV_LineData(data.definitionPoint, dimP1);
    GV_Line* dimensionLine = new GV_Line(this, ld);
       addEntity(dimensionLine);
    */
    GV_Vector vDimexo1, vDimexe1, vDimexo2, vDimexe2;
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
    ld = GV_LineData(edata.extensionPoint1+vDimexo1,
                     dimP1+vDimexe1);
    GV_Line* line = new GV_Line(this, ld);
    line->setPen(GV_Pen(GV2::FlagInvalid));
    line->setLayer(NULL);
    addEntity(line);
    ld = GV_LineData(edata.extensionPoint2+vDimexo2,
                     dimP2+vDimexe2);
    //data.definitionPoint+vDimexe2);
    line = new GV_Line(this, ld);
    line->setPen(GV_Pen(GV2::FlagInvalid));
    line->setLayer(NULL);
    addEntity(line);

    calculateBorders();
}



void GV_DimLinear::move(const GV_Vector& offset) {
    GV_Dimension::move(offset);

    edata.extensionPoint1.move(offset);
    edata.extensionPoint2.move(offset);
    update();
}



void GV_DimLinear::rotate(const GV_Vector& center, double angle) {
    GV_Dimension::rotate(center, angle);

    edata.extensionPoint1.rotate(center, angle);
    edata.extensionPoint2.rotate(center, angle);
    edata.angle = GV_Math::correctAngle(edata.angle+angle);
    update();
}



void GV_DimLinear::scale(const GV_Vector& center, const GV_Vector& factor) {
    GV_Dimension::scale(center, factor);

    edata.extensionPoint1.scale(center, factor);
    edata.extensionPoint2.scale(center, factor);
    update();
}



void GV_DimLinear::mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) {
    GV_Dimension::mirror(axisPoint1, axisPoint2);

    edata.extensionPoint1.mirror(axisPoint1, axisPoint2);
    edata.extensionPoint2.mirror(axisPoint1, axisPoint2);

    GV_Vector vec;
    vec.setPolar(1.0, edata.angle);
    vec.mirror(GV_Vector(0.0,0.0), axisPoint2-axisPoint1);
    edata.angle = vec.angle();

    update();
}



void GV_DimLinear::stretch(const GV_Vector& firstCorner,
                           const GV_Vector& secondCorner,
                           const GV_Vector& offset) {

    //e->calculateBorders();
    if (getMin().isInWindow(firstCorner, secondCorner) &&
            getMax().isInWindow(firstCorner, secondCorner)) {

        move(offset);
    } else {
        //GV_Vector v = data.definitionPoint - edata.extensionPoint2;
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

        double diff = GV_Math::getAngleDifference(ang1, ang2);
        if (diff>M_PI) {
            diff-=2*M_PI;
        }

        if (fabs(diff)>M_PI/2) {
            ang2 = GV_Math::correctAngle(ang2+M_PI);
        }

        GV_Vector v;
        v.setPolar(len, ang2);
        data.definitionPoint = edata.extensionPoint2 + v;
        */
    }
    update(true);
}



void GV_DimLinear::moveRef(const GV_Vector& ref, const GV_Vector& offset) {

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



QVariant GV_DimLinear::getProperty(const QString& name, const QVariant& def) {
    if (name==GV_DIMLINEAR_DEFINITIONPOINT_X) {
        return data.definitionPoint.x;
    } 
    else if (name==GV_DIMLINEAR_DEFINITIONPOINT_Y) {
        return data.definitionPoint.y;
    } 
    if (name==GV_DIMLINEAR_EXTENSIONPOINT1_X) {
        return edata.extensionPoint1.x;
    } 
    else if (name==GV_DIMLINEAR_EXTENSIONPOINT1_Y) {
        return edata.extensionPoint1.y;
    } 
    else if (name==GV_DIMLINEAR_EXTENSIONPOINT2_X) {
        return edata.extensionPoint2.x;
    } 
    else if (name==GV_DIMLINEAR_EXTENSIONPOINT2_Y) {
        return edata.extensionPoint2.y;
    } 
    else if (name==GV_DIMLINEAR_ANGLE) {
        return GV_Math::rad2deg(edata.angle);
    } else {
        return GV_Dimension::getProperty(name, def);
    }
}





QStringList GV_DimLinear::getPropertyNames(bool includeGeometry) {
    QStringList ret = GV_Dimension::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret << GV_DIMLINEAR_DEFINITIONPOINT_X;
        ret << GV_DIMLINEAR_DEFINITIONPOINT_Y;
        ret << GV_DIMLINEAR_EXTENSIONPOINT1_X;
        ret << GV_DIMLINEAR_EXTENSIONPOINT1_Y;
        ret << GV_DIMLINEAR_EXTENSIONPOINT2_X;
        ret << GV_DIMLINEAR_EXTENSIONPOINT2_Y;
        ret << GV_DIMLINEAR_ANGLE;
    }
    // this entity renamed the definition point from GV_Dimension:
    ret.removeAll(GV_DIM_DEFINITIONPOINT_X);
    ret.removeAll(GV_DIM_DEFINITIONPOINT_Y);

    return ret;
}



void GV_DimLinear::setProperty(const QString& name, const QVariant& value) {
    if (name==GV_DIMLINEAR_DEFINITIONPOINT_X) {
        data.definitionPoint.x = GV_Math::eval(value.toString());
        update(true);
    }
    if (name==GV_DIMLINEAR_DEFINITIONPOINT_Y) {
        data.definitionPoint.y = GV_Math::eval(value.toString());
        update(true);
    }
    if (name==GV_DIMLINEAR_EXTENSIONPOINT1_X) {
        edata.extensionPoint1.x = GV_Math::eval(value.toString());
        update(true);
    }
    if (name==GV_DIMLINEAR_EXTENSIONPOINT1_Y) {
        edata.extensionPoint1.y = GV_Math::eval(value.toString());
        update(true);
    }
    if (name==GV_DIMLINEAR_EXTENSIONPOINT2_X) {
        edata.extensionPoint2.x = GV_Math::eval(value.toString());
        update(true);
    }
    if (name==GV_DIMLINEAR_EXTENSIONPOINT2_Y) {
        edata.extensionPoint2.y = GV_Math::eval(value.toString());
        update(true);
    }
    if (name==GV_DIMLINEAR_ANGLE) {
        edata.angle = GV_Math::deg2rad(GV_Math::eval(value.toString()));
        update(true);
    }
    
    GV_Dimension::setProperty(name, value);
}
