/****************************************************************************
** $Id: gv_dimdiametric.cpp 7930 2008-01-17 21:23:04Z andrew $
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


#include "gv_dimdiametric.h"
#include "gv_solid.h"
#include "gv_graphic.h"
#include "gv_units.h"

const char* GV_DimDiametric::propertyDefinitionPoint1X = QT_TRANSLATE_NOOP("QObject", "Point 1|X");
const char* GV_DimDiametric::propertyDefinitionPoint1Y = QT_TRANSLATE_NOOP("QObject", "Point 1|Y");
const char* GV_DimDiametric::propertyDefinitionPoint2X = QT_TRANSLATE_NOOP("QObject", "Point 2|X");
const char* GV_DimDiametric::propertyDefinitionPoint2Y = QT_TRANSLATE_NOOP("QObject", "Point 2|Y");
const char* GV_DimDiametric::propertyLeader = QT_TRANSLATE_NOOP("QObject", "Leader");

/**
 * Constructor.
 *
 * @para parentContainer Parent Entity Container.
 * @para d Common dimension geometrical data.
 * @para ed Extended geometrical data for diametric dimension.
 */
GV_DimDiametric::GV_DimDiametric(GV_EntityContainer* parentContainer,
                           const GV_DimensionData& d,
                           const GV_DimDiametricData& ed)
        : GV_Entity(parentContainer), GV_Dimension(parentContainer, d), edata(ed) {

    calculateBorders();
}



/**
 * @return Automatically created label for the default 
 * measurement of this dimension.
 */
QString GV_DimDiametric::getMeasuredLabel() {

    // Definitive dimension line:
    double dist = data.definitionPoint.distanceTo(edata.definitionPoint);

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



GV_VectorList GV_DimDiametric::getRefPoints(GV2::RefMode /*refMode*/) {
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
void GV_DimDiametric::update(bool autoText) {

    RB_DEBUG->print("GV_DimDiametric::update");

    clear();

    if (isUndone()) {
        return;
    }

    // dimension line:
    updateCreateDimensionLine(data.definitionPoint, edata.definitionPoint,
        true, true, autoText);

    calculateBorders();
}



void GV_DimDiametric::move(const GV_Vector& offset) {
    GV_Dimension::move(offset);

    edata.definitionPoint.move(offset);
    update();
}



void GV_DimDiametric::rotate(const GV_Vector& center, double angle) {
    GV_Dimension::rotate(center, angle);

    edata.definitionPoint.rotate(center, angle);
    update();
}



void GV_DimDiametric::scale(const GV_Vector& center, const GV_Vector& factor) {
    GV_Dimension::scale(center, factor);

    edata.definitionPoint.scale(center, factor);
    edata.leader*=factor.x;
    update();
}



void GV_DimDiametric::mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) {
    GV_Dimension::mirror(axisPoint1, axisPoint2);

    edata.definitionPoint.mirror(axisPoint1, axisPoint2);
    update();
}



void GV_DimDiametric::moveRef(const GV_Vector& ref, const GV_Vector& offset) {

    if (ref.distanceTo(edata.definitionPoint)<1.0e-4) {
        GV_Vector c = (edata.definitionPoint + data.definitionPoint)/2.0;
        double d = c.distanceTo(edata.definitionPoint);
        double a = c.angleTo(edata.definitionPoint + offset);
        
        GV_Vector v;
        v.setPolar(d, a);
        edata.definitionPoint = c + v;
        data.definitionPoint = c - v;
        update(true);
    }
    else if (ref.distanceTo(data.definitionPoint)<1.0e-4) {
        GV_Vector c = (edata.definitionPoint + data.definitionPoint)/2.0;
        double d = c.distanceTo(data.definitionPoint);
        double a = c.angleTo(data.definitionPoint + offset);
        
        GV_Vector v;
        v.setPolar(d, a);
        data.definitionPoint = c + v;
        edata.definitionPoint = c - v;
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



QVariant GV_DimDiametric::getProperty(const QString& name, const QVariant& def) {
    if (name==GV_DIMDIAMETRIC_DEFINITIONPOINT1_X) {
        return data.definitionPoint.x;
    } 
    else if (name==GV_DIMDIAMETRIC_DEFINITIONPOINT1_Y) {
        return data.definitionPoint.y;
    } 
    else if (name==GV_DIMDIAMETRIC_DEFINITIONPOINT2_X) {
        return edata.definitionPoint.x;
    } 
    else if (name==GV_DIMDIAMETRIC_DEFINITIONPOINT2_Y) {
        return edata.definitionPoint.y;
    } 
    else if (name==GV_DIMDIAMETRIC_LEADER) {
        return edata.leader;
    } else {
        return GV_Dimension::getProperty(name, def);
    }
}





QStringList GV_DimDiametric::getPropertyNames(bool includeGeometry) {
    QStringList ret = GV_Dimension::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret << GV_DIMDIAMETRIC_DEFINITIONPOINT1_X;
        ret << GV_DIMDIAMETRIC_DEFINITIONPOINT1_Y;
        ret << GV_DIMDIAMETRIC_DEFINITIONPOINT2_X;
        ret << GV_DIMDIAMETRIC_DEFINITIONPOINT2_Y;
        //ret << GV_DIMDIAMETRIC_LEADER;
    }
    // this entity renamed the definition point from GV_Dimension:
    ret.removeAll(GV_DIM_DEFINITIONPOINT_X);
    ret.removeAll(GV_DIM_DEFINITIONPOINT_Y);

    return ret;
}



void GV_DimDiametric::setProperty(const QString& name, const QVariant& value) {
    if (name==GV_DIMDIAMETRIC_DEFINITIONPOINT1_X) {
        data.definitionPoint.x = GV_Math::eval(value.toString());
        update(true);
    }
    if (name==GV_DIMDIAMETRIC_DEFINITIONPOINT1_Y) {
        data.definitionPoint.y = GV_Math::eval(value.toString());
        update(true);
    }
    if (name==GV_DIMDIAMETRIC_DEFINITIONPOINT2_X) {
        edata.definitionPoint.x = GV_Math::eval(value.toString());
        update(true);
    }
    if (name==GV_DIMDIAMETRIC_DEFINITIONPOINT2_Y) {
        edata.definitionPoint.y = GV_Math::eval(value.toString());
        update(true);
    }
    if (name==GV_DIMDIAMETRIC_LEADER) {
        edata.leader = GV_Math::eval(value.toString());
        update(true);
    }
    
    GV_Dimension::setProperty(name, value);
}


