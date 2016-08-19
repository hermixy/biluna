/****************************************************************************
** $Id: rs_dimaligned.cpp 7802 2008-01-03 10:30:18Z andrew $
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


#include "rs_dimordinate.h"

#include "rs_graphic.h"
#include "rs_units.h"
#include "rs_text.h"

const char* RS_DimOrdinate::propertyDefPoint1X = QT_TRANSLATE_NOOP("QObject", "Def. Point 1|X");
const char* RS_DimOrdinate::propertyDefPoint1Y = QT_TRANSLATE_NOOP("QObject", "Def. Point 1|Y");
const char* RS_DimOrdinate::propertyDefPoint2X = QT_TRANSLATE_NOOP("QObject", "Def. Point 2|X");
const char* RS_DimOrdinate::propertyDefPoint2Y = QT_TRANSLATE_NOOP("QObject", "Def. Point 2|Y");
const char* RS_DimOrdinate::propertyType = QT_TRANSLATE_NOOP("QObject", "Ordinate Type");

const char* RS_DimOrdinate::valueTypeX = QT_TRANSLATE_NOOP("QObject", "X Datum");
const char* RS_DimOrdinate::valueTypeY = QT_TRANSLATE_NOOP("QObject", "Y Datum");



/**
 * Constructor.
 *
 * @para parentContainer Parent Entity Container.
 * @para d Common dimension geometrical data.
 * @para ed Extended geometrical data for aligned dimension.
 */
RS_DimOrdinate::RS_DimOrdinate(RS_EntityContainer* parentContainer,
                             const RS_DimensionData& d,
                             const RS_DimOrdinateData& ed)
        : RS_Entity(parentContainer), RS_Dimension(parentContainer, d), edata(ed) {

    calculateBorders();
}



RS_VectorList RS_DimOrdinate::getRefPoints(RS2::RefMode /*refMode*/) {
    RS_VectorList ret;
    ret << data.definitionPoint;
    ret << edata.defPoint1;
    ret << edata.defPoint2;
    if (data.middleOfText.valid) {
        ret << data.middleOfText;
    }
    else {
        ret << autoTextPos;
    }
    return ret;
}



/**
 * @return Automatically creted label for the default 
 * measurement of this dimension.
 */
RS_String RS_DimOrdinate::getMeasuredLabel() {
    //double dist = edata.defPoint1.distanceTo(edata.defPoint2);
    // TODO:
    double dist;
    
    if (isXType()) {
        dist = fabs(edata.defPoint1.x-data.definitionPoint.x);
    }
    else {
        dist = fabs(edata.defPoint1.y-data.definitionPoint.y);
    }

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



/**
 * Updates the sub entities of this dimension. Called when the 
 * text or the position, alignment, .. changes.
 *
 * @param autoText Automatically reposition the text label
 */
void RS_DimOrdinate::update(bool autoText) {

    RS_DEBUG->print("RS_DimOrdinate::update");

    clear();

    if (isUndone()) {
        return;
    }
    
    // distance from entities (DIMEXO)
    double dimexo = getExtensionLineOffset();

    RS_Vector knee1;
    RS_Vector knee2;
    RS_Vector textOffset;   // normal vector in direction of text offset

    // vertical, measuring X
    if (isXType()) {
        knee1.x = edata.defPoint1.x;
        knee2.x = edata.defPoint2.x;

        if (edata.defPoint1.y < edata.defPoint2.y) {
            knee1.y = edata.defPoint2.y - 0.36*2;
            if (knee1.y < edata.defPoint1.y + 0.36) {
                knee1.y = edata.defPoint1.y + 0.36;
            }
            
            knee2.y = edata.defPoint2.y - 0.36;
            textOffset = RS_Vector(0,1);
        }
        else {
            knee1.y = edata.defPoint2.y + 0.36*2;
            if (knee1.y > edata.defPoint1.y - 0.36) {
                knee1.y = edata.defPoint1.y - 0.36;
            }
            
            knee2.y = edata.defPoint2.y + 0.36;
            textOffset = RS_Vector(0,-1);
        }
    }
    
    // horizontal, measuring Y
    else {
        knee1.y = edata.defPoint1.y;
        knee2.y = edata.defPoint2.y;

        if (edata.defPoint1.x < edata.defPoint2.x) {
            knee1.x = edata.defPoint2.x - 0.36*2;
            if (knee1.x < edata.defPoint1.x + 0.36) {
                knee1.x = edata.defPoint1.x + 0.36;
            }

            knee2.x = edata.defPoint2.x - 0.36;
            textOffset = RS_Vector(1,0);
        }
        else {
            knee1.x = edata.defPoint2.x + 0.36*2;
            if (knee1.x > edata.defPoint1.x - 0.36) {
                knee1.x = edata.defPoint1.x - 0.36;
            }

            knee2.x = edata.defPoint2.x + 0.36;
            textOffset = RS_Vector(-1,0);
        }
    }

    RS_Line* line;

    if (edata.defPoint1.distanceTo(knee1) > dimexo) {
        line = new RS_Line(this, RS_LineData(edata.defPoint1 + textOffset*dimexo, knee1));
        line->setPen(RS_Pen(RS2::FlagInvalid));
        line->setLayer(NULL);
        addEntity(line);
    }
    
    line = new RS_Line(this, RS_LineData(knee1, knee2));
    line->setPen(RS_Pen(RS2::FlagInvalid));
    line->setLayer(NULL);
    addEntity(line);
        
    line = new RS_Line(this, RS_LineData(knee2, edata.defPoint2));
    line->setPen(RS_Pen(RS2::FlagInvalid));
    line->setLayer(NULL);
    addEntity(line);

    // Text label:
    // text height (DIMTXT)
    double textHeight = getTextHeight();
    // text distance to line (DIMGAP)
    double dimgap = getDimensionLineGap();
    
    RS_Text* text = new RS_Text(this, 
        RS_TextData(RS_Vector(0,0),
                    textHeight, 30.0,
                    RS2::VAlignMiddle,
                    RS2::HAlignCenter,
                    RS2::LeftToRight,
                    RS2::Exact,
                    1.0,
                    getLabel(),
                    "standard",
                    0.0)
    );
    double textWidth = text->getSize().x;
    
    RS_Vector textPos;
    if (data.middleOfText.valid && !autoText) {
        textPos = data.middleOfText;
        autoTextPos = RS_Vector(false);
    } else {
        if (!isXType()) {
            textPos = edata.defPoint2 + textOffset * (textWidth/2.0+dimgap);
        }
        else {
            textPos = edata.defPoint2 + textOffset * (textHeight/2.0+dimgap);
        }
        data.middleOfText = textPos;
        data.middleOfText.valid = false;
        autoTextPos = textPos;
    }
    text->move(textPos);
    text->setPen(RS_Pen(RS2::FlagInvalid));
    text->setLayer(NULL);
    addEntity(text);

    /*

    // distance from entities (DIMEXO)
    double dimexo = getExtensionLineOffset();
    // definition line definition (DIMEXE)
    double dimexe = getExtensionLineExtension();

    // angle from extension endpoints towards dimension line
    double extAngle = edata.defPoint1.angleTo(edata.defPoint2);
    //edata.defPoint2.angleTo(data.definitionPoint);
    RS2::Side side = 
        RS_Math::getSideOfPoint(
            edata.defPoint1, 
            edata.defPoint2, 
            data.definitionPoint);
    if (side==RS2::RightHand) {
        extAngle -= M_PI/2.0;
    }
    else if (side==RS2::LeftHand) {
        extAngle += M_PI/2.0;
    }
    else {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "RS_DimOrdinate::update: "
            "cannot determine side of definition point");
    }
    // extension lines length
    double extLength = 
        RS_Math::getDistanceToPoint(
            edata.defPoint1, 
            edata.defPoint2, 
            data.definitionPoint);
    //double extLength = edata.defPoint2.distanceTo(data.definitionPoint);

    RS_Vector v1, v2, e1;
    RS_LineData ld;
    RS_Line* line;

    // from entity to inner point of extension line:
    v1.setPolar(dimexo, extAngle);
    // from entity to outer point of extension line:
    v2.setPolar(dimexe, extAngle);
    e1.setPolar(1.0, extAngle);

    refDefinitionPoint1 = edata.defPoint1 + e1*extLength;
    refDefinitionPoint2 = edata.defPoint2 + e1*extLength;
    data.definitionPoint = refDefinitionPoint1;

    // Extension line 1:
    ld = RS_LineData(edata.defPoint1 + v1,
                     edata.defPoint1 + e1*extLength + v2);
    line = new RS_Line(this, ld);
    line->setPen(RS_Pen(RS2::FlagInvalid));
    line->setLayer(NULL);
    addEntity(line);

    // Extension line 2:
    ld = RS_LineData(edata.defPoint2 + v1,
                     edata.defPoint2 + e1*extLength + v2);
    line = new RS_Line(this, ld);
    line->setPen(RS_Pen(RS2::FlagInvalid));
    line->setLayer(NULL);
    addEntity(line);

    // Dimension line:
    updateCreateDimensionLine(edata.defPoint1 + e1*extLength,
                              edata.defPoint2 + e1*extLength,
                              true, true, autoText);

    */

    calculateBorders();
}



bool RS_DimOrdinate::hasEndpointsWithinWindow(const RS_Vector& v1, const RS_Vector& v2) {
    return (edata.defPoint1.isInWindow(v1, v2) ||
            edata.defPoint2.isInWindow(v1, v2));
}


void RS_DimOrdinate::move(const RS_Vector& offset) {
    RS_Dimension::move(offset);

    edata.defPoint1.move(offset);
    edata.defPoint2.move(offset);
    update();
}



void RS_DimOrdinate::rotate(const RS_Vector& center, double angle) {
    RS_Dimension::rotate(center, angle);

    edata.defPoint1.rotate(center, angle);
    edata.defPoint2.rotate(center, angle);
    update();
}



void RS_DimOrdinate::scale(const RS_Vector& center, const RS_Vector& factor) {
    RS_Dimension::scale(center, factor);

    edata.defPoint1.scale(center, factor);
    edata.defPoint2.scale(center, factor);
    update();
}


void RS_DimOrdinate::mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2) {
    RS_Dimension::mirror(axisPoint1, axisPoint2);

    edata.defPoint1.mirror(axisPoint1, axisPoint2);
    edata.defPoint2.mirror(axisPoint1, axisPoint2);
    update();
}


void RS_DimOrdinate::stretch(const RS_Vector& firstCorner,
                        const RS_Vector& secondCorner,
                        const RS_Vector& offset) {
                        
    //e->calculateBorders();
    if (getMin().isInWindow(firstCorner, secondCorner) &&
        getMax().isInWindow(firstCorner, secondCorner)) {

        move(offset);
    }
    else {    
        if (edata.defPoint1.isInWindow(firstCorner,
                                      secondCorner)) {
            edata.defPoint1.move(offset);
        } 
        if (edata.defPoint2.isInWindow(firstCorner,
                                      secondCorner)) {
            edata.defPoint2.move(offset);
        } 
        // TODO: move def point data.definitionPoint ?
    }
    update(true);
}



void RS_DimOrdinate::moveRef(const RS_Vector& ref, const RS_Vector& offset) {
    if (ref.distanceTo(data.definitionPoint)<1.0e-4) {
        data.definitionPoint.move(offset);
        update(true);
    }
    else if (ref.distanceTo(data.middleOfText)<1.0e-4) {
        RS_DEBUG->print("RS_DimOrdinate::moveRef: moving middle of text");
        data.middleOfText.move(offset);
        update(false);
    }
    else if (ref.distanceTo(autoTextPos)<1.0e-4) {
        RS_DEBUG->print("RS_DimOrdinate::moveRef: moving autoTextPos");
        autoTextPos += offset;
        data.middleOfText = autoTextPos;
        update(false);
    }
    else if (ref.distanceTo(edata.defPoint1)<1.0e-4) {
        edata.defPoint1.move(offset);
        update(true);
    }
    else if (ref.distanceTo(edata.defPoint2)<1.0e-4) {
        edata.defPoint2.move(offset);
        update(true);
    }
    else {
        RS_DEBUG->print("RS_DimOrdinate::moveRef: moving nothing");
    }
}



RS_Variant RS_DimOrdinate::getProperty(const RS_String& name, const RS_Variant& def) {
    if (name==RS_DIMORDINATE_DEFINITIONPOINT1_X) {
        return edata.defPoint1.x;
    } 
    else if (name==RS_DIMORDINATE_DEFINITIONPOINT1_Y) {
        return edata.defPoint1.y;
    }
    else if (name==RS_DIMORDINATE_DEFINITIONPOINT2_X) {
        return edata.defPoint2.x;
    }
    else if (name==RS_DIMORDINATE_DEFINITIONPOINT2_Y) {
        return edata.defPoint2.y;
    }
    else if (name==RS_DIMORDINATE_TYPE) {
        if (edata.xType) {
            return RS_DIMORDINATE_TYPE_X;
        }
        else {
            return RS_DIMORDINATE_TYPE_Y;
        }
    } else {
        return RS_Dimension::getProperty(name, def);
    }
}



RS_StringList RS_DimOrdinate::getPropertyNames(bool includeGeometry) {
    RS_StringList ret = RS_Dimension::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret << RS_DIMORDINATE_DEFINITIONPOINT1_X;
        ret << RS_DIMORDINATE_DEFINITIONPOINT1_Y;
        ret << RS_DIMORDINATE_DEFINITIONPOINT2_X;
        ret << RS_DIMORDINATE_DEFINITIONPOINT2_Y;
        ret << RS_DIMORDINATE_TYPE;
    }
    return ret;
}



void RS_DimOrdinate::setProperty(const RS_String& name, const RS_Variant& value) {
    if (name==RS_DIMORDINATE_DEFINITIONPOINT1_X) {
        edata.defPoint1.x = RS_Math::eval(value.toString());
        update(true);
    }
    else if (name==RS_DIMORDINATE_DEFINITIONPOINT1_Y) {
        edata.defPoint1.y = RS_Math::eval(value.toString());
        update(true);
    }
    else if (name==RS_DIMORDINATE_DEFINITIONPOINT2_X) {
        edata.defPoint2.x = RS_Math::eval(value.toString());
        update(true);
    }
    else if (name==RS_DIMORDINATE_DEFINITIONPOINT2_Y) {
        edata.defPoint2.y = RS_Math::eval(value.toString());
        update(true);
    } 
    else if (name==RS_DIMORDINATE_TYPE) {
        edata.xType = (value==RS_DIMORDINATE_TYPE_X);
        update(true);
    } 
    else {
        RS_Dimension::setProperty(name, value);
    }
}



RS_PropertyAttributes RS_DimOrdinate::getPropertyAttributes(const RS_String& propertyName) {
    RS_PropertyAttributes ret = 
        RS_Dimension::getPropertyAttributes(propertyName);
    
    if (propertyName==RS_DIMORDINATE_TYPE) {
        ret.choices 
            << RS_DIMORDINATE_TYPE_X
            << RS_DIMORDINATE_TYPE_Y;
    }

    return ret;
}
