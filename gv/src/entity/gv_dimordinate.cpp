/****************************************************************************
** $Id: gv_dimaligned.cpp 7802 2008-01-03 10:30:18Z andrew $
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


#include "gv_dimordinate.h"

#include "gv_graphic.h"
#include "gv_units.h"
#include "gv_text.h"

const char* GV_DimOrdinate::propertyDefPoint1X = QT_TRANSLATE_NOOP("QObject", "Def. Point 1|X");
const char* GV_DimOrdinate::propertyDefPoint1Y = QT_TRANSLATE_NOOP("QObject", "Def. Point 1|Y");
const char* GV_DimOrdinate::propertyDefPoint2X = QT_TRANSLATE_NOOP("QObject", "Def. Point 2|X");
const char* GV_DimOrdinate::propertyDefPoint2Y = QT_TRANSLATE_NOOP("QObject", "Def. Point 2|Y");
const char* GV_DimOrdinate::propertyType = QT_TRANSLATE_NOOP("QObject", "Ordinate Type");

const char* GV_DimOrdinate::valueTypeX = QT_TRANSLATE_NOOP("QObject", "X Datum");
const char* GV_DimOrdinate::valueTypeY = QT_TRANSLATE_NOOP("QObject", "Y Datum");



/**
 * Constructor.
 *
 * @para parentContainer Parent Entity Container.
 * @para d Common dimension geometrical data.
 * @para ed Extended geometrical data for aligned dimension.
 */
GV_DimOrdinate::GV_DimOrdinate(GV_EntityContainer* parentContainer,
                             const GV_DimensionData& d,
                             const GV_DimOrdinateData& ed)
        : GV_Entity(parentContainer), GV_Dimension(parentContainer, d), edata(ed) {

    calculateBorders();
}



GV_VectorList GV_DimOrdinate::getRefPoints(GV2::RefMode /*refMode*/) {
    GV_VectorList ret;
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
QString GV_DimOrdinate::getMeasuredLabel() {
    //double dist = edata.defPoint1.distanceTo(edata.defPoint2);
    // TODO:
    double dist;
    
    if (isXType()) {
        dist = fabs(edata.defPoint1.x-data.definitionPoint.x);
    }
    else {
        dist = fabs(edata.defPoint1.y-data.definitionPoint.y);
    }

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



/**
 * Updates the sub entities of this dimension. Called when the 
 * text or the position, alignment, .. changes.
 *
 * @param autoText Automatically reposition the text label
 */
void GV_DimOrdinate::update(bool autoText) {

    RB_DEBUG->print("GV_DimOrdinate::update");

    clear();

    if (isUndone()) {
        return;
    }
    
    // distance from entities (DIMEXO)
    double dimexo = getExtensionLineOffset();

    GV_Vector knee1;
    GV_Vector knee2;
    GV_Vector textOffset;   // normal vector in direction of text offset

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
            textOffset = GV_Vector(0,1);
        }
        else {
            knee1.y = edata.defPoint2.y + 0.36*2;
            if (knee1.y > edata.defPoint1.y - 0.36) {
                knee1.y = edata.defPoint1.y - 0.36;
            }
            
            knee2.y = edata.defPoint2.y + 0.36;
            textOffset = GV_Vector(0,-1);
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
            textOffset = GV_Vector(1,0);
        }
        else {
            knee1.x = edata.defPoint2.x + 0.36*2;
            if (knee1.x > edata.defPoint1.x - 0.36) {
                knee1.x = edata.defPoint1.x - 0.36;
            }

            knee2.x = edata.defPoint2.x + 0.36;
            textOffset = GV_Vector(-1,0);
        }
    }

    GV_Line* line;

    if (edata.defPoint1.distanceTo(knee1) > dimexo) {
        line = new GV_Line(this, GV_LineData(edata.defPoint1 + textOffset*dimexo, knee1));
        line->setPen(GV_Pen(GV2::FlagInvalid));
        line->setLayer(NULL);
        addEntity(line);
    }
    
    line = new GV_Line(this, GV_LineData(knee1, knee2));
    line->setPen(GV_Pen(GV2::FlagInvalid));
    line->setLayer(NULL);
    addEntity(line);
        
    line = new GV_Line(this, GV_LineData(knee2, edata.defPoint2));
    line->setPen(GV_Pen(GV2::FlagInvalid));
    line->setLayer(NULL);
    addEntity(line);

    // Text label:
    // text height (DIMTXT)
    double textHeight = getTextHeight();
    // text distance to line (DIMGAP)
    double dimgap = getDimensionLineGap();
    
    GV_Text* text = new GV_Text(this, 
        GV_TextData(GV_Vector(0,0),
                    textHeight, 30.0,
                    GV2::VAlignMiddle,
                    GV2::HAlignCenter,
                    GV2::LeftToRight,
                    GV2::Exact,
                    1.0,
                    getLabel(),
                    "standard",
                    0.0)
    );
    double textWidth = text->getSize().x;
    
    GV_Vector textPos;
    if (data.middleOfText.valid && !autoText) {
        textPos = data.middleOfText;
        autoTextPos = GV_Vector(false);
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
    text->setPen(GV_Pen(GV2::FlagInvalid));
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
    GV2::Side side = 
        GV_Math::getSideOfPoint(
            edata.defPoint1, 
            edata.defPoint2, 
            data.definitionPoint);
    if (side==GV2::RightHand) {
        extAngle -= M_PI/2.0;
    }
    else if (side==GV2::LeftHand) {
        extAngle += M_PI/2.0;
    }
    else {
        RB_DEBUG->print(RB_Debug::D_ERROR, 
            "GV_DimOrdinate::update: "
            "cannot determine side of definition point");
    }
    // extension lines length
    double extLength = 
        GV_Math::getDistanceToPoint(
            edata.defPoint1, 
            edata.defPoint2, 
            data.definitionPoint);
    //double extLength = edata.defPoint2.distanceTo(data.definitionPoint);

    GV_Vector v1, v2, e1;
    GV_LineData ld;
    GV_Line* line;

    // from entity to inner point of extension line:
    v1.setPolar(dimexo, extAngle);
    // from entity to outer point of extension line:
    v2.setPolar(dimexe, extAngle);
    e1.setPolar(1.0, extAngle);

    refDefinitionPoint1 = edata.defPoint1 + e1*extLength;
    refDefinitionPoint2 = edata.defPoint2 + e1*extLength;
    data.definitionPoint = refDefinitionPoint1;

    // Extension line 1:
    ld = GV_LineData(edata.defPoint1 + v1,
                     edata.defPoint1 + e1*extLength + v2);
    line = new GV_Line(this, ld);
    line->setPen(GV_Pen(GV2::FlagInvalid));
    line->setLayer(NULL);
    addEntity(line);

    // Extension line 2:
    ld = GV_LineData(edata.defPoint2 + v1,
                     edata.defPoint2 + e1*extLength + v2);
    line = new GV_Line(this, ld);
    line->setPen(GV_Pen(GV2::FlagInvalid));
    line->setLayer(NULL);
    addEntity(line);

    // Dimension line:
    updateCreateDimensionLine(edata.defPoint1 + e1*extLength,
                              edata.defPoint2 + e1*extLength,
                              true, true, autoText);

    */

    calculateBorders();
}



bool GV_DimOrdinate::hasEndpointsWithinWindow(const GV_Vector& v1, const GV_Vector& v2) {
    return (edata.defPoint1.isInWindow(v1, v2) ||
            edata.defPoint2.isInWindow(v1, v2));
}


void GV_DimOrdinate::move(const GV_Vector& offset) {
    GV_Dimension::move(offset);

    edata.defPoint1.move(offset);
    edata.defPoint2.move(offset);
    update();
}



void GV_DimOrdinate::rotate(const GV_Vector& center, double angle) {
    GV_Dimension::rotate(center, angle);

    edata.defPoint1.rotate(center, angle);
    edata.defPoint2.rotate(center, angle);
    update();
}



void GV_DimOrdinate::scale(const GV_Vector& center, const GV_Vector& factor) {
    GV_Dimension::scale(center, factor);

    edata.defPoint1.scale(center, factor);
    edata.defPoint2.scale(center, factor);
    update();
}


void GV_DimOrdinate::mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) {
    GV_Dimension::mirror(axisPoint1, axisPoint2);

    edata.defPoint1.mirror(axisPoint1, axisPoint2);
    edata.defPoint2.mirror(axisPoint1, axisPoint2);
    update();
}


void GV_DimOrdinate::stretch(const GV_Vector& firstCorner,
                        const GV_Vector& secondCorner,
                        const GV_Vector& offset) {
                        
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



void GV_DimOrdinate::moveRef(const GV_Vector& ref, const GV_Vector& offset) {
    if (ref.distanceTo(data.definitionPoint)<1.0e-4) {
        data.definitionPoint.move(offset);
        update(true);
    }
    else if (ref.distanceTo(data.middleOfText)<1.0e-4) {
        RB_DEBUG->print("GV_DimOrdinate::moveRef: moving middle of text");
        data.middleOfText.move(offset);
        update(false);
    }
    else if (ref.distanceTo(autoTextPos)<1.0e-4) {
        RB_DEBUG->print("GV_DimOrdinate::moveRef: moving autoTextPos");
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
        RB_DEBUG->print("GV_DimOrdinate::moveRef: moving nothing");
    }
}



QVariant GV_DimOrdinate::getProperty(const QString& name, const QVariant& def) {
    if (name==GV_DIMORDINATE_DEFINITIONPOINT1_X) {
        return edata.defPoint1.x;
    } 
    else if (name==GV_DIMORDINATE_DEFINITIONPOINT1_Y) {
        return edata.defPoint1.y;
    }
    else if (name==GV_DIMORDINATE_DEFINITIONPOINT2_X) {
        return edata.defPoint2.x;
    }
    else if (name==GV_DIMORDINATE_DEFINITIONPOINT2_Y) {
        return edata.defPoint2.y;
    }
    else if (name==GV_DIMORDINATE_TYPE) {
        if (edata.xType) {
            return GV_DIMORDINATE_TYPE_X;
        }
        else {
            return GV_DIMORDINATE_TYPE_Y;
        }
    } else {
        return GV_Dimension::getProperty(name, def);
    }
}



QStringList GV_DimOrdinate::getPropertyNames(bool includeGeometry) {
    QStringList ret = GV_Dimension::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret << GV_DIMORDINATE_DEFINITIONPOINT1_X;
        ret << GV_DIMORDINATE_DEFINITIONPOINT1_Y;
        ret << GV_DIMORDINATE_DEFINITIONPOINT2_X;
        ret << GV_DIMORDINATE_DEFINITIONPOINT2_Y;
        ret << GV_DIMORDINATE_TYPE;
    }
    return ret;
}



void GV_DimOrdinate::setProperty(const QString& name, const QVariant& value) {
    if (name==GV_DIMORDINATE_DEFINITIONPOINT1_X) {
        edata.defPoint1.x = GV_Math::eval(value.toString());
        update(true);
    }
    else if (name==GV_DIMORDINATE_DEFINITIONPOINT1_Y) {
        edata.defPoint1.y = GV_Math::eval(value.toString());
        update(true);
    }
    else if (name==GV_DIMORDINATE_DEFINITIONPOINT2_X) {
        edata.defPoint2.x = GV_Math::eval(value.toString());
        update(true);
    }
    else if (name==GV_DIMORDINATE_DEFINITIONPOINT2_Y) {
        edata.defPoint2.y = GV_Math::eval(value.toString());
        update(true);
    } 
    else if (name==GV_DIMORDINATE_TYPE) {
        edata.xType = (value==GV_DIMORDINATE_TYPE_X);
        update(true);
    } 
    else {
        GV_Dimension::setProperty(name, value);
    }
}



GV_PropertyAttributes GV_DimOrdinate::getPropertyAttributes(const QString& propertyName) {
    GV_PropertyAttributes ret = 
        GV_Dimension::getPropertyAttributes(propertyName);
    
    if (propertyName==GV_DIMORDINATE_TYPE) {
        ret.choices 
            << GV_DIMORDINATE_TYPE_X
            << GV_DIMORDINATE_TYPE_Y;
    }

    return ret;
}
