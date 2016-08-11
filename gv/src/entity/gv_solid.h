/****************************************************************************
** $Id: gv_solid.h 8991 2008-02-11 15:41:52Z andrew $
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


#ifndef GV_SOLID_H
#define GV_SOLID_H

#include "gv_atomicentity.h"
#include "gv_vector.h"

// cheap implementation of a 3d face:
#define GV_3dFaceData GV_SolidData
#define GV_3dFace GV_Solid

#define GV_SOLID_POINT1_X   QObject::tr(GV_Solid::propertyPoint1X)
#define GV_SOLID_POINT1_Y   QObject::tr(GV_Solid::propertyPoint1Y)
#define GV_SOLID_POINT2_X   QObject::tr(GV_Solid::propertyPoint2X)
#define GV_SOLID_POINT2_Y   QObject::tr(GV_Solid::propertyPoint2Y)
#define GV_SOLID_POINT3_X   QObject::tr(GV_Solid::propertyPoint3X)
#define GV_SOLID_POINT3_Y   QObject::tr(GV_Solid::propertyPoint3Y)
#define GV_SOLID_POINT4_X   QObject::tr(GV_Solid::propertyPoint4X)
#define GV_SOLID_POINT4_Y   QObject::tr(GV_Solid::propertyPoint4Y)



/**
 * Holds the data that defines a solid.
 */
class GV_SolidData {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    GV_SolidData() {
        for (int i=0; i<4; ++i) {
            corner[i] = GV_Vector(false);
        }
        thickness = 0.0;
    }

    /**
     * Constructor for a solid with 3 corners.
     */
    GV_SolidData(const GV_Vector& corner1,
                 const GV_Vector& corner2,
                 const GV_Vector& corner3,
                 double thickness = 0.0) {

        corner[0] = corner1;
        corner[1] = corner2;
        corner[2] = corner3;
        corner[3] = corner3;
        corner[3].valid = false;
        this->thickness = thickness;
    }

    /**
     * Constructor for a solid with 4 corners.
     */
    GV_SolidData(const GV_Vector& corner1,
                 const GV_Vector& corner2,
                 const GV_Vector& corner3,
                 const GV_Vector& corner4,
                 double thickness=0.0) {

        corner[0] = corner1;
        corner[1] = corner2;
        corner[2] = corner3;
        corner[3] = corner4;
        this->thickness = thickness;
    }

    friend class GV_Solid;

private:
    GV_Vector corner[4];
    double thickness;
};



/**
 * Class for a solid entity (e.g. dimension arrows).
 *
 * @author Andrew Mustun
 */
class GV_Solid : public GV_AtomicEntity {
public:
    GV_Solid(GV_EntityContainer* parentContainer,
             const GV_SolidData& d);

    virtual GV_Entity* clone() {
        GV_Solid* s = new GV_Solid(parentContainer, data);
        s->copyPropertiesFrom(this);
        s->initId();
        return s;
    }

    /** @return GV_ENTITY_POINT */
    virtual GV2::EntityType rtti() const {
        return GV2::EntitySolid;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntitySolid || GV_AtomicEntity::isOfType(t));
    }
    virtual QString typeName() const {
        return QObject::tr("Solid");
    }
    
    /** 
     * @return Start point of the entity. 
     */
    virtual GV_Vector getStartpoint() const {
        return GV_Vector(false);
    }
    /** 
     * @return End point of the entity. 
     */
    virtual GV_Vector getEndpoint() const {
        return GV_Vector(false);
    }


    /** @return Copy of data that defines the point. */
    GV_SolidData getData() const {
        return data;
    }

    double getThickness() const {
        return data.thickness;
    }

    /** @return true if this is a triangle. */
    bool isTriangle() {
        return (!data.corner[3].valid || 
            data.corner[2].distanceTo(data.corner[3]) < 1.0e-6);
    }

    GV2::ContourOrientation getOrientation();
    virtual void reverse();

    GV_Vector getCorner(int num) const;
    void setCorner(int num, const GV_Vector& v);
    
    virtual GV_VectorList getRefPoints(GV2::RefMode refMode=GV2::RefAll);
    virtual GV_Vector getNearestRef(const GV_Vector& coord,
                                     double* dist = NULL,
                                     GV2::RefMode refMode=GV2::RefAll);
    virtual GV_Vector getNearestSelectedRef(const GV_Vector& coord,
                                     double* dist = NULL,
                                     GV2::RefMode refMode=GV2::RefAll);

    void shapeArrow(const GV_Vector& point,
                    double angle,
                    double arrowSize);

    virtual GV_Vector getNearestEndpoint(const GV_Vector& coord,
                                         double* dist = NULL);
    virtual GV_Vector getNearestPointOnEntity(const GV_Vector& coord,
            bool onEntity = true, double* dist = NULL, GV_Entity** entity = NULL);
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
    virtual GV2::Side getSideOfPoint(const GV_Vector& ) {
        return GV2::NoSide;
    }

    virtual void move(const GV_Vector& offset);
    virtual void rotate(const GV_Vector& center, double angle);
    virtual void scale(const GV_Vector& center, const GV_Vector& factor);
    virtual void mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2);
    virtual void moveRef(const GV_Vector& ref, const GV_Vector& offset);
    virtual void stretch(const GV_Vector& firstCorner,
                         const GV_Vector& secondCorner,
                         const GV_Vector& offset);
    virtual void isoProject(GV2::IsoProjectionType type, double segmentLength);

    virtual void draw(GV_Painter* painter, GV_GraphicView* view, double patternOffset=0.0);

    /** Recalculates the borders of this entity. */
    virtual void calculateBorders(bool visibleOnly=false);
    
    virtual QVariant getProperty(const QString& name, 
        const QVariant& def=QVariant());
    virtual QStringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const QString& name, const QVariant& value);
    
    static const char* propertyPoint1X;
    static const char* propertyPoint1Y;
    static const char* propertyPoint2X;
    static const char* propertyPoint2Y;
    static const char* propertyPoint3X;
    static const char* propertyPoint3Y;
    static const char* propertyPoint4X;
    static const char* propertyPoint4Y;

protected:
    GV_SolidData data;
    
};

#endif
