/****************************************************************************
** $Id: rs_solid.h 8991 2008-02-11 15:41:52Z andrew $
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


#ifndef RS_SOLID_H
#define RS_SOLID_H

#include "rs_atomicentity.h"
#include "rs_vector.h"

// cheap implementation of a 3d face:
#define RS_3dFaceData RS_SolidData
#define RS_3dFace RS_Solid

#define RS_SOLID_POINT1_X   QObject::tr(RS_Solid::propertyPoint1X)
#define RS_SOLID_POINT1_Y   QObject::tr(RS_Solid::propertyPoint1Y)
#define RS_SOLID_POINT2_X   QObject::tr(RS_Solid::propertyPoint2X)
#define RS_SOLID_POINT2_Y   QObject::tr(RS_Solid::propertyPoint2Y)
#define RS_SOLID_POINT3_X   QObject::tr(RS_Solid::propertyPoint3X)
#define RS_SOLID_POINT3_Y   QObject::tr(RS_Solid::propertyPoint3Y)
#define RS_SOLID_POINT4_X   QObject::tr(RS_Solid::propertyPoint4X)
#define RS_SOLID_POINT4_Y   QObject::tr(RS_Solid::propertyPoint4Y)



/**
 * Holds the data that defines a solid.
 */
class CAD_EXPORT RS_SolidData {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    RS_SolidData() {
        for (int i=0; i<4; ++i) {
            corner[i] = RS_Vector(false);
        }
        thickness = 0.0;
    }

    /**
     * Constructor for a solid with 3 corners.
     */
    RS_SolidData(const RS_Vector& corner1,
                 const RS_Vector& corner2,
                 const RS_Vector& corner3,
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
    RS_SolidData(const RS_Vector& corner1,
                 const RS_Vector& corner2,
                 const RS_Vector& corner3,
                 const RS_Vector& corner4,
                 double thickness=0.0) {

        corner[0] = corner1;
        corner[1] = corner2;
        corner[2] = corner3;
        corner[3] = corner4;
        this->thickness = thickness;
    }

    friend class RS_Solid;

private:
    RS_Vector corner[4];
    double thickness;
};



/**
 * Class for a solid entity (e.g. dimension arrows).
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_Solid : public RS_AtomicEntity {
public:
    RS_Solid(RS_EntityContainer* parentContainer,
             const RS_SolidData& d);

    virtual RS_Entity* clone() {
        RS_Solid* s = new RS_Solid(parentContainer, data);
        s->copyPropertiesFrom(this);
        s->initId();
        return s;
    }

    /** @return RS_ENTITY_POINT */
    virtual RS2::EntityType rtti() const {
        return RS2::EntitySolid;
    }
    virtual bool isOfType(RS2::EntityType t) const {
        return (t==RS2::EntitySolid || RS_AtomicEntity::isOfType(t));
    }
    virtual RS_String typeName() const {
        return QObject::tr("Solid");
    }
    
    /** 
     * @return Start point of the entity. 
     */
    virtual RS_Vector getStartpoint() const {
        return RS_Vector(false);
    }
    /** 
     * @return End point of the entity. 
     */
    virtual RS_Vector getEndpoint() const {
        return RS_Vector(false);
    }


    /** @return Copy of data that defines the point. */
    RS_SolidData getData() const {
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

    RS2::ContourOrientation getOrientation();
    virtual void reverse();

    RS_Vector getCorner(int num) const;
    void setCorner(int num, const RS_Vector& v);
    
    virtual RS_VectorList getRefPoints(RS2::RefMode refMode=RS2::RefAll);
    virtual RS_Vector getNearestRef(const RS_Vector& coord,
                                     double* dist = NULL,
                                     RS2::RefMode refMode=RS2::RefAll);
    virtual RS_Vector getNearestSelectedRef(const RS_Vector& coord,
                                     double* dist = NULL,
                                     RS2::RefMode refMode=RS2::RefAll);

    void shapeArrow(const RS_Vector& point,
                    double angle,
                    double arrowSize);

    virtual RS_Vector getNearestEndpoint(const RS_Vector& coord,
                                         double* dist = NULL);
    virtual RS_Vector getNearestPointOnEntity(const RS_Vector& coord,
            bool onEntity = true, double* dist = NULL, RS_Entity** entity = NULL);
    virtual RS_Vector getNearestCenter(const RS_Vector& coord,
                                       double* dist = NULL);
    virtual RS_Vector getNearestMiddle(const RS_Vector& coord,
                                       double* dist = NULL);
    virtual RS_Vector getNearestDist(double distance,
                                     const RS_Vector& coord,
                                     double* dist = NULL);

    virtual double getDistanceToPoint(const RS_Vector& coord,
                                      RS_Entity** entity=NULL,
                                      RS2::ResolveLevel level=RS2::ResolveNone,
                                      double solidDist = RS_MAXDOUBLE,
                                      bool visually = false);
    virtual RS2::Side getSideOfPoint(const RS_Vector& ) {
        return RS2::NoSide;
    }

    virtual void move(const RS_Vector& offset);
    virtual void rotate(const RS_Vector& center, double angle);
    virtual void scale(const RS_Vector& center, const RS_Vector& factor);
    virtual void mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2);
    virtual void moveRef(const RS_Vector& ref, const RS_Vector& offset);
    virtual void stretch(const RS_Vector& firstCorner,
                         const RS_Vector& secondCorner,
                         const RS_Vector& offset);
    virtual void isoProject(RS2::IsoProjectionType type, double segmentLength);

    virtual void draw(RS_Painter* painter, RS_GraphicView* view, double patternOffset=0.0);

    /** Recalculates the borders of this entity. */
    virtual void calculateBorders(bool visibleOnly=false);
    
    virtual RS_Variant getProperty(const RS_String& name, 
        const RS_Variant& def=RS_Variant());
    virtual RS_StringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const RS_String& name, const RS_Variant& value);
    
    static const char* propertyPoint1X;
    static const char* propertyPoint1Y;
    static const char* propertyPoint2X;
    static const char* propertyPoint2Y;
    static const char* propertyPoint3X;
    static const char* propertyPoint3Y;
    static const char* propertyPoint4X;
    static const char* propertyPoint4Y;

protected:
    RS_SolidData data;
    
};

#endif
