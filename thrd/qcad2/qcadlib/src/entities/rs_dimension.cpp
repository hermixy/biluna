/****************************************************************************
** $Id: rs_dimension.cpp 8945 2008-02-07 23:37:50Z andrew $
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


#include "rs_dimension.h"
#include "rs_solid.h"
#include "rs_text.h"
#include "rs_units.h"
    
    
const char* RS_Dimension::propertyDefinitionPointX = QT_TRANSLATE_NOOP("QObject", "Def. Point|X");
const char* RS_Dimension::propertyDefinitionPointY = QT_TRANSLATE_NOOP("QObject", "Def. Point|Y");
const char* RS_Dimension::propertyTextPositionX = QT_TRANSLATE_NOOP("QObject", "Label Pos.|X");
const char* RS_Dimension::propertyTextPositionY = QT_TRANSLATE_NOOP("QObject", "Label Pos.|Y");
const char* RS_Dimension::propertyText = QT_TRANSLATE_NOOP("QObject", "Label");



/**
 * Constructor.
 */
RS_Dimension::RS_Dimension(RS_EntityContainer* parentContainer,
                           const RS_DimensionData& d)
        : RS_Entity(parentContainer), RS_EntityContainer(parentContainer), data(d) {

    attributeSize = -1.0;
}



RS_Vector RS_Dimension::getNearestRef(const RS_Vector& coord,
                                      double* dist,
                                      RS2::RefMode refMode) {

    return RS_Entity::getNearestRef(coord, dist, refMode);
}


RS_Vector RS_Dimension::getNearestSelectedRef(const RS_Vector& coord,
        double* dist, RS2::RefMode refMode) {

    return RS_Entity::getNearestSelectedRef(coord, dist, refMode);
}



/**
 * @return Dimension text. Either a text the user defined or 
 *         the measured text.
 *
 * @param resolve false: return plain value. true: return measured 
 *      label if appropriate.
 * @see getMeasuredLabel
 */
RS_String RS_Dimension::getLabel(bool resolve) {
    if (!resolve) {
        return data.text;
    }

    RS_String ret="";

    // One space suppresses the text:
    if (data.text==" ") {
        ret = "";
    }

    // No text prints actual measurement:
    else if (data.text=="") {
        ret = getMeasuredLabel();
    }

    // Others print the text 
    // (<> is replaced by the measurement)
    else {
        ret = data.text;
        ret = ret.replace(RS_String("<>"), getMeasuredLabel());
    }

    return ret;
}


/**
 * Sets a new text for the label.
 */
void RS_Dimension::setLabel(const RS_String& l) {
    data.text = l;
}



/**
 * Creates a dimensioning line (line with one, two or no arrows and a text).
 *
 * @param forceAutoText Automatically reposition the text label.
 */
void RS_Dimension::updateCreateDimensionLine(const RS_Vector& p1,
        const RS_Vector& p2, bool arrow1, bool arrow2, bool forceAutoText) {

    // text height (DIMTXT)
    double dimtxt = getTextHeight();
    // text distance to line (DIMGAP)
    double dimgap = getDimensionLineGap();

    // length of dimension line:
    double distance = p1.distanceTo(p2);

    // do we have to put the arrows outside of the line?
    bool outsideArrows = (distance<getArrowSize()*2.5);

    // arrow angles:
    double arrowAngle1, arrowAngle2;

    // Create dimension line:
    RS_Line* dimensionLine = new RS_Line(this, RS_LineData(p1, p2));
    dimensionLine->setPen(RS_Pen(RS2::FlagInvalid));
    dimensionLine->setLayer(NULL);
    addEntity(dimensionLine);

    if (outsideArrows==false) {
        arrowAngle1 = dimensionLine->getAngle2();
        arrowAngle2 = RS_Math::correctAngle(arrowAngle1+M_PI);
    } else {
        arrowAngle1 = dimensionLine->getAngle1();
        arrowAngle2 = RS_Math::correctAngle(arrowAngle1+M_PI);
       
        // extend dimension line outside arrows
        RS_Vector dir;
        dir.setPolar(getArrowSize()*2, arrowAngle2);
        dimensionLine->setStartpoint(p1 + dir);
        dimensionLine->setEndpoint(p2 - dir);
    }

    // Arrows:
    //RS_SolidData sd;
    //RS_Solid* arrow;

    if (arrow1) {
        addEntity(createArrow(p1, arrowAngle1));
    }

    if (arrow2) {
        addEntity(createArrow(p2, arrowAngle2));
    }

    // Text label:
    RS_TextData textData;
    RS_Vector textPos;

    double dimAngle1 = dimensionLine->getAngle1();
    double textAngle;
    bool corrected=false;
    textAngle = RS_Math::makeAngleReadable(dimAngle1, true, &corrected);

    if (data.middleOfText.valid && !forceAutoText) {
        textPos = data.middleOfText;
        autoTextPos = RS_Vector(false);
    } else {
        textPos = dimensionLine->getMiddlepoint();

        RS_Vector distV;

        // rotate text so it's readable from the bottom or right (ISO)
        // quadrant 1 & 4
        if (corrected) {
            distV.setPolar(dimgap + dimtxt/2.0, dimAngle1-M_PI/2.0);
        } else {
            distV.setPolar(dimgap + dimtxt/2.0, dimAngle1+M_PI/2.0);
        }

        // move text away from dimension line:
        textPos+=distV;
        data.middleOfText = textPos;
        data.middleOfText.valid = false;
        autoTextPos = textPos;
    }

    textData = RS_TextData(textPos,
                           dimtxt, 30.0,
                           RS2::VAlignMiddle,
                           RS2::HAlignCenter,
                           RS2::LeftToRight,
                           RS2::Exact,
                           1.0,
                           getLabel(),
                           "standard",
                           textAngle);

    RS_Text* text = new RS_Text(this, textData);

    // move text to the side:
    if (!hasCustomTextPosition()) {
        RS_Vector distH;
        if (text->getUsedTextWidth()>distance) {
            distH.setPolar(text->getUsedTextWidth()/2.0
                           +distance/2.0+dimgap, textAngle);
            text->move(distH);
            autoTextPos+=distH;
            data.middleOfText+=distH;
        }
    }
    text->setPen(RS_Pen(RS2::FlagInvalid));
    text->setLayer(NULL);
    addEntity(text);
}
    
    
    
RS_Entity* RS_Dimension::createArrow(const RS_Vector& p, double angle) {

    if (useArchTick()) {
        double sz = getArrowSize();
        RS_Vector p1(sz/2, sz/2);

        RS_Line* line = new RS_Line(this, RS_LineData(p1, -p1));
        line->rotate(RS_Vector(0,0), angle);
        line->move(p);
        line->setPen(RS_Pen(RS2::FlagInvalid));
        line->setLayer(NULL);
        return line;
    }
    else {
        RS_Solid* arrow = new RS_Solid(this, RS_SolidData());
        arrow->shapeArrow(p,
                          angle,
                          getArrowSize());
        arrow->setPen(RS_Pen(RS2::FlagInvalid));
        arrow->setLayer(NULL);
        return arrow;
    }
}

    

/**
 * @return True to use arch ticks instead of arrows.
 */
bool RS_Dimension::useArchTick() {
    return (getGraphicVariableString("$DIMBLK", "")=="ArchTick");
}


/**
 * @return arrow size in drawing units.
 */
double RS_Dimension::getArrowSize() {
    if (attributeSize>0.0) {
        return attributeSize;
    }
    else {
        return getGraphicVariable("$DIMASZ", 2.5, 40);
    }
}



/**
 * @return extension line overlength in drawing units.
 */
double RS_Dimension::getExtensionLineExtension() {
    if (attributeSize>0.0) {
        return attributeSize/2.0;
    }
    else {
        return getGraphicVariable("$DIMEXE", 1.25, 40);
    }
}



/**
 * @return extension line offset from entities in drawing units.
 */
double RS_Dimension::getExtensionLineOffset() {
    if (attributeSize>0.0) {
        return attributeSize/4.0;
    }
    else {
        return getGraphicVariable("$DIMEXO", 0.625, 40);
    }
}



/**
 * @return extension line gap to text in drawing units.
 */
double RS_Dimension::getDimensionLineGap() {
    if (attributeSize>0.0) {
        return attributeSize/4.0;
    }
    else {
        return getGraphicVariable("$DIMGAP", 0.625, 40);
    }
}



/**
 * @return Dimension lables text height.
 */
double RS_Dimension::getTextHeight() {
    if (attributeSize>0.0) {
        return attributeSize;
    }
    else {
        return getGraphicVariable("$DIMTXT", 2.5, 40);
    }
}







void RS_Dimension::move(const RS_Vector& offset) {
    data.definitionPoint.move(offset);
    data.middleOfText.move(offset);
}



void RS_Dimension::rotate(const RS_Vector& center, double angle) {
    data.definitionPoint.rotate(center, angle);
    data.middleOfText.rotate(center, angle);
    data.angle = RS_Math::correctAngle(data.angle+angle);
}



void RS_Dimension::scale(const RS_Vector& center, const RS_Vector& factor) {
    data.definitionPoint.scale(center, factor);
    data.middleOfText.scale(center, factor);
}



void RS_Dimension::mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2) {
    data.definitionPoint.mirror(axisPoint1, axisPoint2);
    data.middleOfText.mirror(axisPoint1, axisPoint2);
}



RS_Variant RS_Dimension::getProperty(const RS_String& name, const RS_Variant& def) {
    if (name==RS_DIM_DEFINITIONPOINT_X) {
        return data.definitionPoint.x;
    } else if (name==RS_DIM_DEFINITIONPOINT_Y) {
        return data.definitionPoint.y;
    } else if (name==RS_DIM_TEXTPOSITION_X) {
        return data.middleOfText.x;
    } else if (name==RS_DIM_TEXTPOSITION_Y) {
        return data.middleOfText.y;
    } else if (name==RS_DIM_TEXT) {
        return data.text;
    } else {
        return RS_Entity::getProperty(name, def);
    }
}



RS_StringList RS_Dimension::getPropertyNames(bool includeGeometry) {
    RS_StringList ret = RS_Entity::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret << RS_DIM_TEXT;
        ret << RS_DIM_TEXTPOSITION_X;
        ret << RS_DIM_TEXTPOSITION_Y;
        ret << RS_DIM_DEFINITIONPOINT_X;
        ret << RS_DIM_DEFINITIONPOINT_Y;
    }
    return ret;
}



void RS_Dimension::setProperty(const RS_String& name, const RS_Variant& value) {
    if (name==RS_DIM_DEFINITIONPOINT_X) {
        data.definitionPoint.x = RS_Math::eval(value.toString());
        update(true);
    }
    else if (name==RS_DIM_DEFINITIONPOINT_Y) {
        data.definitionPoint.y = RS_Math::eval(value.toString());
        update(true);
    }
    else if (name==RS_DIM_TEXTPOSITION_X) {
        data.middleOfText.x = RS_Math::eval(value.toString());
        update(false);
    }
    else if (name==RS_DIM_TEXTPOSITION_Y) {
        data.middleOfText.y = RS_Math::eval(value.toString());
        update(false);
    }
    else if (name==RS_DIM_TEXT) {
        data.text = value.toString();
        update(false);
    } else {
        RS_Entity::setProperty(name, value);
    }
}

