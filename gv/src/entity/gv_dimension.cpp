/****************************************************************************
** $Id: gv_dimension.cpp 8945 2008-02-07 23:37:50Z andrew $
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


#include "gv_dimension.h"
#include "gv_solid.h"
#include "gv_text.h"
#include "gv_units.h"
    
    
const char* GV_Dimension::propertyDefinitionPointX = QT_TRANSLATE_NOOP("QObject", "Def. Point|X");
const char* GV_Dimension::propertyDefinitionPointY = QT_TRANSLATE_NOOP("QObject", "Def. Point|Y");
const char* GV_Dimension::propertyTextPositionX = QT_TRANSLATE_NOOP("QObject", "Label Pos.|X");
const char* GV_Dimension::propertyTextPositionY = QT_TRANSLATE_NOOP("QObject", "Label Pos.|Y");
const char* GV_Dimension::propertyText = QT_TRANSLATE_NOOP("QObject", "Label");



/**
 * Constructor.
 */
GV_Dimension::GV_Dimension(GV_EntityContainer* parentContainer,
                           const GV_DimensionData& d)
        : GV_Entity(parentContainer), GV_EntityContainer(parentContainer), data(d) {

    attributeSize = -1.0;
}



GV_Vector GV_Dimension::getNearestRef(const GV_Vector& coord,
                                      double* dist,
                                      GV2::RefMode refMode) {

    return GV_Entity::getNearestRef(coord, dist, refMode);
}


GV_Vector GV_Dimension::getNearestSelectedRef(const GV_Vector& coord,
        double* dist, GV2::RefMode refMode) {

    return GV_Entity::getNearestSelectedRef(coord, dist, refMode);
}



/**
 * @return Dimension text. Either a text the user defined or 
 *         the measured text.
 *
 * @param resolve false: return plain value. true: return measured 
 *      label if appropriate.
 * @see getMeasuredLabel
 */
QString GV_Dimension::getLabel(bool resolve) {
    if (!resolve) {
        return data.text;
    }

    QString ret="";

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
        ret = ret.replace(QString("<>"), getMeasuredLabel());
    }

    return ret;
}


/**
 * Sets a new text for the label.
 */
void GV_Dimension::setLabel(const QString& l) {
    data.text = l;
}



/**
 * Creates a dimensioning line (line with one, two or no arrows and a text).
 *
 * @param forceAutoText Automatically reposition the text label.
 */
void GV_Dimension::updateCreateDimensionLine(const GV_Vector& p1,
        const GV_Vector& p2, bool arrow1, bool arrow2, bool forceAutoText) {

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
    GV_Line* dimensionLine = new GV_Line(this, GV_LineData(p1, p2));
    dimensionLine->setPen(GV_Pen(GV2::FlagInvalid));
    dimensionLine->setLayer(NULL);
    addEntity(dimensionLine);

    if (outsideArrows==false) {
        arrowAngle1 = dimensionLine->getAngle2();
        arrowAngle2 = GV_Math::correctAngle(arrowAngle1+M_PI);
    } else {
        arrowAngle1 = dimensionLine->getAngle1();
        arrowAngle2 = GV_Math::correctAngle(arrowAngle1+M_PI);
       
        // extend dimension line outside arrows
        GV_Vector dir;
        dir.setPolar(getArrowSize()*2, arrowAngle2);
        dimensionLine->setStartpoint(p1 + dir);
        dimensionLine->setEndpoint(p2 - dir);
    }

    // Arrows:
    //GV_SolidData sd;
    //GV_Solid* arrow;

    if (arrow1) {
        addEntity(createArrow(p1, arrowAngle1));
    }

    if (arrow2) {
        addEntity(createArrow(p2, arrowAngle2));
    }

    // Text label:
    GV_TextData textData;
    GV_Vector textPos;

    double dimAngle1 = dimensionLine->getAngle1();
    double textAngle;
    bool corrected=false;
    textAngle = GV_Math::makeAngleReadable(dimAngle1, true, &corrected);

    if (data.middleOfText.valid && !forceAutoText) {
        textPos = data.middleOfText;
        autoTextPos = GV_Vector(false);
    } else {
        textPos = dimensionLine->getMiddlepoint();

        GV_Vector distV;

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

    textData = GV_TextData(textPos,
                           dimtxt, 30.0,
                           GV2::VAlignMiddle,
                           GV2::HAlignCenter,
                           GV2::LeftToRight,
                           GV2::Exact,
                           1.0,
                           getLabel(),
                           "standard",
                           textAngle);

    GV_Text* text = new GV_Text(this, textData);

    // move text to the side:
    if (!hasCustomTextPosition()) {
        GV_Vector distH;
        if (text->getUsedTextWidth()>distance) {
            distH.setPolar(text->getUsedTextWidth()/2.0
                           +distance/2.0+dimgap, textAngle);
            text->move(distH);
            autoTextPos+=distH;
            data.middleOfText+=distH;
        }
    }
    text->setPen(GV_Pen(GV2::FlagInvalid));
    text->setLayer(NULL);
    addEntity(text);
}
    
    
    
GV_Entity* GV_Dimension::createArrow(const GV_Vector& p, double angle) {

    if (useArchTick()) {
        double sz = getArrowSize();
        GV_Vector p1(sz/2, sz/2);

        GV_Line* line = new GV_Line(this, GV_LineData(p1, -p1));
        line->rotate(GV_Vector(0,0), angle);
        line->move(p);
        line->setPen(GV_Pen(GV2::FlagInvalid));
        line->setLayer(NULL);
        return line;
    }
    else {
        GV_Solid* arrow = new GV_Solid(this, GV_SolidData());
        arrow->shapeArrow(p,
                          angle,
                          getArrowSize());
        arrow->setPen(GV_Pen(GV2::FlagInvalid));
        arrow->setLayer(NULL);
        return arrow;
    }
}

    

/**
 * @return True to use arch ticks instead of arrows.
 */
bool GV_Dimension::useArchTick() {
    return (getGraphicVariableString("$DIMBLK", "")=="ArchTick");
}


/**
 * @return arrow size in drawing units.
 */
double GV_Dimension::getArrowSize() {
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
double GV_Dimension::getExtensionLineExtension() {
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
double GV_Dimension::getExtensionLineOffset() {
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
double GV_Dimension::getDimensionLineGap() {
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
double GV_Dimension::getTextHeight() {
    if (attributeSize>0.0) {
        return attributeSize;
    }
    else {
        return getGraphicVariable("$DIMTXT", 2.5, 40);
    }
}







void GV_Dimension::move(const GV_Vector& offset) {
    data.definitionPoint.move(offset);
    data.middleOfText.move(offset);
}



void GV_Dimension::rotate(const GV_Vector& center, double angle) {
    data.definitionPoint.rotate(center, angle);
    data.middleOfText.rotate(center, angle);
    data.angle = GV_Math::correctAngle(data.angle+angle);
}



void GV_Dimension::scale(const GV_Vector& center, const GV_Vector& factor) {
    data.definitionPoint.scale(center, factor);
    data.middleOfText.scale(center, factor);
}



void GV_Dimension::mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) {
    data.definitionPoint.mirror(axisPoint1, axisPoint2);
    data.middleOfText.mirror(axisPoint1, axisPoint2);
}



QVariant GV_Dimension::getProperty(const QString& name, const QVariant& def) {
    if (name==GV_DIM_DEFINITIONPOINT_X) {
        return data.definitionPoint.x;
    } else if (name==GV_DIM_DEFINITIONPOINT_Y) {
        return data.definitionPoint.y;
    } else if (name==GV_DIM_TEXTPOSITION_X) {
        return data.middleOfText.x;
    } else if (name==GV_DIM_TEXTPOSITION_Y) {
        return data.middleOfText.y;
    } else if (name==GV_DIM_TEXT) {
        return data.text;
    } else {
        return GV_Entity::getProperty(name, def);
    }
}



QStringList GV_Dimension::getPropertyNames(bool includeGeometry) {
    QStringList ret = GV_Entity::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret << GV_DIM_TEXT;
        ret << GV_DIM_TEXTPOSITION_X;
        ret << GV_DIM_TEXTPOSITION_Y;
        ret << GV_DIM_DEFINITIONPOINT_X;
        ret << GV_DIM_DEFINITIONPOINT_Y;
    }
    return ret;
}



void GV_Dimension::setProperty(const QString& name, const QVariant& value) {
    if (name==GV_DIM_DEFINITIONPOINT_X) {
        data.definitionPoint.x = GV_Math::eval(value.toString());
        update(true);
    }
    else if (name==GV_DIM_DEFINITIONPOINT_Y) {
        data.definitionPoint.y = GV_Math::eval(value.toString());
        update(true);
    }
    else if (name==GV_DIM_TEXTPOSITION_X) {
        data.middleOfText.x = GV_Math::eval(value.toString());
        update(false);
    }
    else if (name==GV_DIM_TEXTPOSITION_Y) {
        data.middleOfText.y = GV_Math::eval(value.toString());
        update(false);
    }
    else if (name==GV_DIM_TEXT) {
        data.text = value.toString();
        update(false);
    } else {
        GV_Entity::setProperty(name, value);
    }
}

