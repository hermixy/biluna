/****************************************************************************
** $Id: gv_dimaligned.cpp 7930 2008-01-17 21:23:04Z andrew $
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


#include "gv_dimaligned.h"

#include "gv_graphic.h"
#include "gv_units.h"
#include "gv_constructionline.h"

const char* GV_DimAligned::propertyExtensionPoint1X = QT_TRANSLATE_NOOP("QObject", "Extension Point 1|X");
const char* GV_DimAligned::propertyExtensionPoint1Y = QT_TRANSLATE_NOOP("QObject", "Extension Point 1|Y");
const char* GV_DimAligned::propertyExtensionPoint2X = QT_TRANSLATE_NOOP("QObject", "Extension Point 2|X");
const char* GV_DimAligned::propertyExtensionPoint2Y = QT_TRANSLATE_NOOP("QObject", "Extension Point 2|Y");

/**
 * Constructor.
 *
 * @para parentContainer Parent Entity Container.
 * @para d Common dimension geometrical data.
 * @para ed Extended geometrical data for aligned dimension.
 */
GV_DimAligned::GV_DimAligned(GV_EntityContainer* parentContainer,
                             const GV_DimensionData& d,
                             const GV_DimAlignedData& ed)
        : GV_Entity(parentContainer), GV_Dimension(parentContainer, d), edata(ed) {

    calculateBorders();
}



/**
 * Sets a new text. The entities representing the 
 * text are updated.
 */
//void GV_DimAligned::setText(const QString& t) {
//    data.text = t;
//    update();
//}


GV_VectorList GV_DimAligned::getRefPoints(GV2::RefMode /*refMode*/) {
    GV_VectorList ret;
    ret << edata.extensionPoint1;
    ret << edata.extensionPoint2;
    if (data.middleOfText.valid) {
        ret << data.middleOfText;
    }
    else {
        ret << autoTextPos;
    }
    ret << refDefinitionPoint1;
    ret << refDefinitionPoint2;
    return ret;
}



/**
 * @return Automatically creted label for the default 
 * measurement of this dimension.
 */
QString GV_DimAligned::getMeasuredLabel() {
    double dist = edata.extensionPoint1.distanceTo(edata.extensionPoint2);

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
void GV_DimAligned::update(bool autoText) {

    RB_DEBUG->print("GV_DimAligned::update");

    clear();

    if (isUndone()) {
        return;
    }

    // distance from entities (DIMEXO)
    double dimexo = getExtensionLineOffset();
    // definition line definition (DIMEXE)
    double dimexe = getExtensionLineExtension();
    // text height (DIMTXT)
    //double dimtxt = getTextHeight();
    // text distance to line (DIMGAP)
    //double dimgap = getDimensionLineGap();

    // angle from extension endpoints towards dimension line
    double extAngle = edata.extensionPoint1.angleTo(edata.extensionPoint2);
    //edata.extensionPoint2.angleTo(data.definitionPoint);
    GV2::Side side = 
        GV_Math::getSideOfPoint(
            edata.extensionPoint1, 
            edata.extensionPoint2, 
            data.definitionPoint);
    if (side==GV2::RightHand) {
        extAngle -= M_PI/2.0;
    }
    else if (side==GV2::LeftHand) {
        extAngle += M_PI/2.0;
    }
    else {
        RB_DEBUG->print(RB_Debug::D_ERROR, 
            "GV_DimAligned::update: "
            "cannot determine side of definition point");
    }
    // extension lines length
    double extLength = 
        GV_Math::getDistanceToPoint(
            edata.extensionPoint1, 
            edata.extensionPoint2, 
            data.definitionPoint);
    //double extLength = edata.extensionPoint2.distanceTo(data.definitionPoint);

    GV_Vector v1, v2, e1;
    GV_LineData ld;
    GV_Line* line;

    // from entity to inner point of extension line:
    v1.setPolar(dimexo, extAngle);
    // from entity to outer point of extension line:
    v2.setPolar(dimexe, extAngle);
    e1.setPolar(1.0, extAngle);

    refDefinitionPoint1 = edata.extensionPoint1 + e1*extLength;
    refDefinitionPoint2 = edata.extensionPoint2 + e1*extLength;
    data.definitionPoint = refDefinitionPoint1;

    // Extension line 1:
    ld = GV_LineData(edata.extensionPoint1 + v1,
                     edata.extensionPoint1 + e1*extLength + v2);
    line = new GV_Line(this, ld);
    line->setPen(GV_Pen(GV2::FlagInvalid));
    line->setLayer(NULL);
    addEntity(line);

    // Extension line 2:
    ld = GV_LineData(edata.extensionPoint2 + v1,
                     edata.extensionPoint2 + e1*extLength + v2);
    line = new GV_Line(this, ld);
    line->setPen(GV_Pen(GV2::FlagInvalid));
    line->setLayer(NULL);
    addEntity(line);

    // Dimension line:
    updateCreateDimensionLine(edata.extensionPoint1 + e1*extLength,
                              edata.extensionPoint2 + e1*extLength,
                              true, true, autoText);

    calculateBorders();
}



bool GV_DimAligned::hasEndpointsWithinWindow(const GV_Vector& v1, const GV_Vector& v2) {
    return (edata.extensionPoint1.isInWindow(v1, v2) ||
            edata.extensionPoint2.isInWindow(v1, v2));
}


void GV_DimAligned::move(const GV_Vector& offset) {
    GV_Dimension::move(offset);

    edata.extensionPoint1.move(offset);
    edata.extensionPoint2.move(offset);
    update();
}



void GV_DimAligned::rotate(const GV_Vector& center, double angle) {
    GV_Dimension::rotate(center, angle);

    edata.extensionPoint1.rotate(center, angle);
    edata.extensionPoint2.rotate(center, angle);
    update();
}



void GV_DimAligned::scale(const GV_Vector& center, const GV_Vector& factor) {
    GV_Dimension::scale(center, factor);

    edata.extensionPoint1.scale(center, factor);
    edata.extensionPoint2.scale(center, factor);
    update();
}


void GV_DimAligned::mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) {
    GV_Dimension::mirror(axisPoint1, axisPoint2);

    edata.extensionPoint1.mirror(axisPoint1, axisPoint2);
    edata.extensionPoint2.mirror(axisPoint1, axisPoint2);
    update();
}


void GV_DimAligned::stretch(const GV_Vector& firstCorner,
                        const GV_Vector& secondCorner,
                        const GV_Vector& offset) {
                        
    //e->calculateBorders();
    if (getMin().isInWindow(firstCorner, secondCorner) &&
            getMax().isInWindow(firstCorner, secondCorner)) {

        move(offset);
    }
    else {    
        //GV_Vector v = data.definitionPoint - edata.extensionPoint2;
        double len = edata.extensionPoint2.distanceTo(data.definitionPoint);
        double ang1 = edata.extensionPoint1.angleTo(edata.extensionPoint2) 
                     + M_PI/2;
    
        if (edata.extensionPoint1.isInWindow(firstCorner,
                                      secondCorner)) {
            edata.extensionPoint1.move(offset);
        } 
        if (edata.extensionPoint2.isInWindow(firstCorner,
                                      secondCorner)) {
            edata.extensionPoint2.move(offset);
        } 
        
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
    }
    update(true);
}



void GV_DimAligned::moveRef(const GV_Vector& ref, const GV_Vector& offset) {
//    RB_DEBUG->print("GV_DimAligned::moveRef: %.3f/%.3f by %.3f/%.3f",
//        ref.x, ref.y, offset.x, offset.y);

    
//    RB_DEBUG->print("GV_DimAligned::moveRef: autoTextPos: %.3f/%.3f",
//        autoTextPos.x, autoTextPos.y);


    if (ref.distanceTo(refDefinitionPoint1)<1.0e-4) {
        data.definitionPoint = refDefinitionPoint1 + offset;
        update(true);
    }
    else if (ref.distanceTo(refDefinitionPoint2)<1.0e-4) {
        data.definitionPoint = refDefinitionPoint2 + offset;
        update(true);
    }
    else if (ref.distanceTo(data.middleOfText)<1.0e-4) {
        RB_DEBUG->print("GV_DimAligned::moveRef: moving middle of text");
        data.middleOfText.move(offset);
        update(false);
    }
    else if (ref.distanceTo(autoTextPos)<1.0e-4) {
        RB_DEBUG->print("GV_DimAligned::moveRef: moving autoTextPos");
        autoTextPos += offset;
        data.middleOfText = autoTextPos;
        update(false);
    }
    else if (ref.distanceTo(edata.extensionPoint1)<1.0e-4) {
        edata.extensionPoint1.move(offset);
        update(true);
    }
    else if (ref.distanceTo(edata.extensionPoint2)<1.0e-4) {
        edata.extensionPoint2.move(offset);
        update(true);
    }
    else {
        RB_DEBUG->print("GV_DimAligned::moveRef: moving nothing");
    }
}


QVariant GV_DimAligned::getProperty(const QString& name, const QVariant& def) {
    if (name==GV_DIMALIGNED_EXTENSIONPOINT1_X) {
        return edata.extensionPoint1.x;
    } 
    else if (name==GV_DIMALIGNED_EXTENSIONPOINT1_Y) {
        return edata.extensionPoint1.y;
    }
    else if (name==GV_DIMALIGNED_EXTENSIONPOINT2_X) {
        return edata.extensionPoint2.x;
    }
    else if (name==GV_DIMALIGNED_EXTENSIONPOINT2_Y) {
        return edata.extensionPoint2.y;
    } else {
        return GV_Dimension::getProperty(name, def);
    }
}



QStringList GV_DimAligned::getPropertyNames(bool includeGeometry) {
    QStringList ret = GV_Dimension::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret << GV_DIMALIGNED_EXTENSIONPOINT1_X;
        ret << GV_DIMALIGNED_EXTENSIONPOINT1_Y;
        ret << GV_DIMALIGNED_EXTENSIONPOINT2_X;
        ret << GV_DIMALIGNED_EXTENSIONPOINT2_Y;
    }
    return ret;
}



void GV_DimAligned::setProperty(const QString& name, const QVariant& value) {
    if (name==GV_DIMALIGNED_EXTENSIONPOINT1_X) {
        edata.extensionPoint1.x = GV_Math::eval(value.toString());
        update(true);
    }
    else if (name==GV_DIMALIGNED_EXTENSIONPOINT1_Y) {
        edata.extensionPoint1.y = GV_Math::eval(value.toString());
        update(true);
    }
    else if (name==GV_DIMALIGNED_EXTENSIONPOINT2_X) {
        edata.extensionPoint2.x = GV_Math::eval(value.toString());
        update(true);
    }
    else if (name==GV_DIMALIGNED_EXTENSIONPOINT2_Y) {
        edata.extensionPoint2.y = GV_Math::eval(value.toString());
        update(true);
    } 
    else {
        GV_Dimension::setProperty(name, value);
    }
}




