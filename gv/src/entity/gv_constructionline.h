/****************************************************************************
** $Id: gv_constructionline.h 7898 2008-01-15 17:26:19Z andrew $
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


#ifndef GV_CONSTRUCTIONLINE_H
#define GV_CONSTRUCTIONLINE_H

#include "gv_atomicentity.h"
#include "gv_vector.h"
#include "gv_line.h"

/**
 * Holds the data that defines a construction line (a line
 * which is not limited to both directions).
 */
class GV_ConstructionLineData {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    GV_ConstructionLineData() {}

    GV_ConstructionLineData(const GV_Vector& point1,
                            const GV_Vector& point2) {

        this->point1 = point1;
        this->point2 = point2;
    }

    friend class GV_ConstructionLine;

private:
    GV_Vector point1;
    GV_Vector point2;
};


/**
 * Class for a construction line entity.
 *
 * @author Andrew Mustun
 */
class GV_ConstructionLine : public GV_AtomicEntity {
public:
    GV_ConstructionLine(GV_EntityContainer* parentContainer,
                        const GV_ConstructionLineData& d);

    virtual GV_Entity* clone();

    virtual ~GV_ConstructionLine();

    /** @return GV2::EntityConstructionLine */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityConstructionLine;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityConstructionLine || GV_AtomicEntity::isOfType(t));
    }
    
    /** 
     * @todo
     * @return Start point of the entity. 
     */
    virtual GV_Vector getStartpoint() const {
        return GV_Vector(false);
    }
    /** 
     * @todo
     * @return End point of the entity. 
     */
    virtual GV_Vector getEndpoint() const {
        return GV_Vector(false);
    }

    /** @return Copy of data that defines the line. */
    GV_ConstructionLineData getData() const {
        return data;
    }

    /** @return First definition point. */
    GV_Vector getPoint1() const {
        return data.point1;
    }
    /** @return Second definition point. */
    GV_Vector getPoint2() const {
        return data.point2;
    }

    virtual GV_Vector getNearestEndpoint(const GV_Vector& coord,
                                         double* dist = NULL);
    virtual GV_Vector getNearestPointOnEntity(const GV_Vector& coord,
            bool onEntity = true, double* dist = NULL, GV_Entity** entity=NULL);
    virtual GV_Vector getNearestCenter(const GV_Vector& coord,
                                       double* dist = NULL);
    virtual GV_Vector getNearestMiddle(const GV_Vector& coord,
                                       double* dist = NULL);
    virtual GV_Vector getNearestDist(double distance,
                                     const GV_Vector& coord,
                                     double* dist = NULL);
    virtual double getDistanceToPoint(const GV_Vector& coord,
                                      GV_Entity** entity=NULL,
                                      GV2::ResolveLevel level=GV2::ResolveNone,
                                      double solidDist = GV_MAXDOUBLE,
                                      bool visually = false);
    virtual GV2::Side getSideOfPoint(const GV_Vector& coord) {
        return GV_Line(NULL, GV_LineData(data.point1, data.point2))
            .getSideOfPoint(coord);
    }

    virtual void move(const GV_Vector& offset);
    virtual void rotate(const GV_Vector& center, double angle);
    virtual void scale(const GV_Vector& center, const GV_Vector& factor);
    virtual void mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2);

    virtual void draw(GV_Painter* /*painter*/, GV_GraphicView* /*view*/, 
        double /*patternOffset*/) {}

    virtual void calculateBorders(bool visibleOnly=false);

protected:
    GV_ConstructionLineData data;
};

#endif
