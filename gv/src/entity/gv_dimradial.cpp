/****************************************************************************
** $Id: gv_dimradial.cpp 8945 2008-02-07 23:37:50Z andrew $
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


#include "gv_dimradial.h"
//#include "gv_constructionline.h"
#include "gv_text.h"
#include "gv_solid.h"
#include "gv_graphic.h"


const char* GV_DimRadial::propertyDefinitionPoint1X = QT_TRANSLATE_NOOP("QObject", "Point 1|X");
const char* GV_DimRadial::propertyDefinitionPoint1Y = QT_TRANSLATE_NOOP("QObject", "Point 1|Y");
const char* GV_DimRadial::propertyDefinitionPoint2X = QT_TRANSLATE_NOOP("QObject", "Point 2|X");
const char* GV_DimRadial::propertyDefinitionPoint2Y = QT_TRANSLATE_NOOP("QObject", "Point 2|Y");
const char* GV_DimRadial::propertyLeader = QT_TRANSLATE_NOOP("QObject", "Leader");


/**
 * Constructor.
 *
 * @para parentContainer Parent Entity Container.
 * @para d Common dimension geometrical data.
 * @para ed Extended geometrical data for radial dimension.
 */
GV_DimRadial::GV_DimRadial(GV_EntityContainer* parentContainer,
                           const GV_DimensionData& d,
                           const GV_DimRadialData& ed)
        : GV_Entity(parentContainer), GV_Dimension(parentContainer, d), edata(ed) {}



/**
 * @return Automatically created label for the default 
 * measurement of this dimension.
 */
QString GV_DimRadial::getMeasuredLabel() {

    // Definitive dimension line:
    double dist = data.definitionPoint.distanceTo(edata.definitionPoint);

    GV_Graphic* graphic = getGraphic();

    QString ret;
    if (graphic!=NULL) {
        ret = GV_Units::formatLinear(
            dist, graphic->getUnit(),
            graphic->getLinearFormat(), graphic->getLinearPrecision(),
            false,
            graphic->showLeadingZeroes(), graphic->showTrailingZeroes());
    } else {
        ret = QString("%1").arg(dist);
    }

    return ret;
}


GV_VectorList GV_DimRadial::getRefPoints(GV2::RefMode /*refMode*/) {
    GV_VectorList ret;
    ret << edata.definitionPoint;
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
 * Updates the sub entities of this dimension. Called when the 
 * dimension or the position, alignment, .. changes.
 *
 * @param autoText Automatically reposition the text label
 */
void GV_DimRadial::update(bool autoText) {

    RB_DEBUG->print("GV_DimRadial::update");

    clear();

    if (isUndone()) {
        return;
    }

    // dimension line:
    //updateCreateDimensionLine(data.definitionPoint, edata.definitionPoint,
    //false, true);

    GV_Vector p1 = data.definitionPoint;
    GV_Vector p2 = edata.definitionPoint;
    double angle = p1.angleTo(p2);

    // text height (DIMTXT)
    double dimtxt = getTextHeight();
    // text distance to line (DIMGAP)
    double dimgap = getDimensionLineGap();

    // length of dimension line:
    double length = p1.distanceTo(p2);

    GV_TextData textData;

    textData = GV_TextData(GV_Vector(0.0,0.0),
                           dimtxt, 30.0,
                           GV2::VAlignMiddle,
                           GV2::HAlignCenter,
                           GV2::LeftToRight,
                           GV2::Exact,
                           1.0,
                           getLabel(),
                           "standard",
                           0.0);

    GV_Text* text = new GV_Text(this, textData);
    double textWidth = text->getSize().x;

    // do we have to put the arrow / text outside of the arc?
    bool outsideArrow = (length<getArrowSize()*2+textWidth);
    double arrowAngle;

    if (outsideArrow) {
        length += getArrowSize()*2 + textWidth;
        arrowAngle = angle+M_PI;
    } else {
        arrowAngle = angle;
    }

    // create arrow:
    addEntity(createArrow(p2, arrowAngle));

    GV_Vector p3;
    p3.setPolar(length, angle);
    p3 += p1;

    // Create dimension line:
    GV_Line* dimensionLine = new GV_Line(this, GV_LineData(p1, p3));
    dimensionLine->setPen(GV_Pen(GV2::FlagInvalid));
    dimensionLine->setLayer(NULL);
    addEntity(dimensionLine);

    GV_Vector distV;
    double textAngle;

    // rotate text so it's readable from the bottom or right (ISO)
    // quadrant 1 & 4
    if (angle>M_PI/2.0*3.0+0.001 ||
            angle<M_PI/2.0+0.001) {

        distV.setPolar(dimgap + dimtxt/2.0, angle+M_PI/2.0);
        textAngle = angle;
    }
    // quadrant 2 & 3
    else {
        distV.setPolar(dimgap + dimtxt/2.0, angle-M_PI/2.0);
        textAngle = angle+M_PI;
    }

    // move text label:
    GV_Vector textPos;

    if (data.middleOfText.valid && !autoText) {
        textPos = data.middleOfText;
    } else {
        if (outsideArrow) {
            textPos.setPolar(length-textWidth/2.0-getArrowSize(), angle);
        } else {
            textPos.setPolar(length/2.0, angle);
        }
        textPos+=p1;
        // move text away from dimension line:
        textPos += distV;
        //data.middleOfText = textPos;
        autoTextPos = textPos;
    }

    text->rotate(GV_Vector(0.0,0.0), textAngle);
    text->move(textPos);

    text->setPen(GV_Pen(GV2::FlagInvalid));
    text->setLayer(NULL);
    addEntity(text);

    calculateBorders();
}



void GV_DimRadial::move(const GV_Vector& offset) {
    GV_Dimension::move(offset);

    edata.definitionPoint.move(offset);
    update();
}



void GV_DimRadial::rotate(const GV_Vector& center, double angle) {
    GV_Dimension::rotate(center, angle);

    edata.definitionPoint.rotate(center, angle);
    update();
}



void GV_DimRadial::scale(const GV_Vector& center, const GV_Vector& factor) {
    GV_Dimension::scale(center, factor);

    edata.definitionPoint.scale(center, factor);
    edata.leader*=factor.x;
    update();
}



void GV_DimRadial::mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) {
    GV_Dimension::mirror(axisPoint1, axisPoint2);

    edata.definitionPoint.mirror(axisPoint1, axisPoint2);
    update();
}


void GV_DimRadial::moveRef(const GV_Vector& ref, const GV_Vector& offset) {

    if (ref.distanceTo(edata.definitionPoint)<1.0e-4) {
        double d = data.definitionPoint.distanceTo(edata.definitionPoint);
        double a = data.definitionPoint.angleTo(edata.definitionPoint + offset);
        
        GV_Vector v;
        v.setPolar(d, a);
        edata.definitionPoint = data.definitionPoint + v;
        update(true);
    }
    else if (ref.distanceTo(data.middleOfText)<1.0e-4) {
        data.middleOfText.move(offset);
        update(false);
    }
    else if (ref.distanceTo(autoTextPos)<1.0e-4) {
        data.middleOfText = autoTextPos + offset;
        update(false);
    }
}


QVariant GV_DimRadial::getProperty(const QString& name, const QVariant& def) {
    if (name==GV_DIMRADIAL_DEFINITIONPOINT1_X) {
        return data.definitionPoint.x;
    } 
    else if (name==GV_DIMRADIAL_DEFINITIONPOINT1_Y) {
        return data.definitionPoint.y;
    } 
    else if (name==GV_DIMRADIAL_DEFINITIONPOINT2_X) {
        return edata.definitionPoint.x;
    } 
    else if (name==GV_DIMRADIAL_DEFINITIONPOINT2_Y) {
        return edata.definitionPoint.y;
    } 
    else if (name==GV_DIMRADIAL_LEADER) {
        return edata.leader;
    } else {
        return GV_Dimension::getProperty(name, def);
    }
}





QStringList GV_DimRadial::getPropertyNames(bool includeGeometry) {
    QStringList ret = GV_Dimension::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret << GV_DIMRADIAL_DEFINITIONPOINT1_X;
        ret << GV_DIMRADIAL_DEFINITIONPOINT1_Y;
        ret << GV_DIMRADIAL_DEFINITIONPOINT2_X;
        ret << GV_DIMRADIAL_DEFINITIONPOINT2_Y;
        //ret << GV_DIMRADIAL_LEADER;
    }
    // this entity renamed the definition point from GV_Dimension:
    ret.removeAll(GV_DIM_DEFINITIONPOINT_X);
    ret.removeAll(GV_DIM_DEFINITIONPOINT_Y);

    return ret;
}



void GV_DimRadial::setProperty(const QString& name, const QVariant& value) {
    if (name==GV_DIMRADIAL_DEFINITIONPOINT1_X) {
        data.definitionPoint.x = GV_Math::eval(value.toString());
        update(true);
    }
    if (name==GV_DIMRADIAL_DEFINITIONPOINT1_Y) {
        data.definitionPoint.y = GV_Math::eval(value.toString());
        update(true);
    }
    if (name==GV_DIMRADIAL_DEFINITIONPOINT2_X) {
        edata.definitionPoint.x = GV_Math::eval(value.toString());
        update(true);
    }
    if (name==GV_DIMRADIAL_DEFINITIONPOINT2_Y) {
        edata.definitionPoint.y = GV_Math::eval(value.toString());
        update(true);
    }
    if (name==GV_DIMRADIAL_LEADER) {
        edata.leader = GV_Math::eval(value.toString());
        update(true);
    }
    
    GV_Dimension::setProperty(name, value);
}


