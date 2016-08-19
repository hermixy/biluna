/****************************************************************************
** $Id: rs_line.h 8878 2008-02-04 21:39:25Z andrew $
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


#ifndef RS_LINE_H
#define RS_LINE_H

#include "rs_atomicentity.h"

#define RS_LINE_STARTPOINT_X   QObject::tr(RS_Line::propertyStartpointX)
#define RS_LINE_STARTPOINT_Y   QObject::tr(RS_Line::propertyStartpointY)
#define RS_LINE_ENDPOINT_X   QObject::tr(RS_Line::propertyEndpointX)
#define RS_LINE_ENDPOINT_Y   QObject::tr(RS_Line::propertyEndpointY)

/**
 * Holds the data that defines a line.
 */
class RS_LineData {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    RS_LineData() {}

    RS_LineData(const RS_Vector& startpoint,
                const RS_Vector& endpoint) {

        this->startpoint = startpoint;
        this->endpoint = endpoint;
    }

    RS_Vector startpoint;
    RS_Vector endpoint;
};


/**
 * Class for a line entity.
 *
 * @author Andrew Mustun
 */
class RS_Line : public virtual RS_AtomicEntity {
public:
    RS_Line(RS_EntityContainer* parentContainer,
            const RS_LineData& d);
    virtual ~RS_Line();
    virtual RS_Entity* clone();

    /** @return RS2::EntityLine */
    virtual RS2::EntityType rtti() const {
        return RS2::EntityLine;
    }
    virtual bool isOfType(RS2::EntityType t) const {
        return (t==RS2::EntityLine || RS_AtomicEntity::isOfType(t));
    }
    
    virtual RS_String typeName() const {
        return QObject::tr("Line");
    }
    
    /** @return true */
    virtual bool isEdge() const {
        return true;
    }
    
    /** @return Copy of data that defines the line. */
    RS_LineData getData() const {
        return data;
    }
    
    /** Sets a new line data. */
    void setData(const RS_LineData& d) {
        data = d;
        update();
    }
    
    virtual RS_VectorList getRefPoints(RS2::RefMode refMode=RS2::RefAll);

    /** @return Startpoint of the entity */
    virtual RS_Vector getStartpoint() const {
        return data.startpoint;
    }
    /** @return Endpoint of the entity */
    virtual RS_Vector getEndpoint() const {
        return data.endpoint;
    }

    /** Sets the startpoint */
    void setStartpoint(const RS_Vector& s) {
        moveStartpoint(s);
    }
    /** Sets the endpoint */
    void setEndpoint(const RS_Vector& e) {
        moveEndpoint(e);
    }
    /** 
     * @return Direction 1. The angle at which the line starts at 
     * the startpoint. 
     */
    double getDirection1() const {
        return getAngle1();
    }
    /** 
     * @return Direction 2. The angle at which the line starts at 
     * the endpoint.
     */
    double getDirection2() const {
        return getAngle2();
    }
    virtual void moveStartpoint(const RS_Vector& pos);
    virtual void moveEndpoint(const RS_Vector& pos);
    virtual RS2::Ending getTrimPoint(const RS_Vector& coord, 
              const RS_Vector& trimPoint);
    virtual void reverse();
    /** @return the center point of the line. */
    RS_Vector getMiddlepoint() {
        return (data.startpoint + data.endpoint)/2.0;
    }
    virtual bool hasEndpointsWithinWindow(const RS_Vector& v1, const RS_Vector& v2);

    /**
     * @return The length of the line.
     */
    virtual double getLength() {
        return data.startpoint.distanceTo(data.endpoint);
    }

    /**
     * @return The angle of the line (from start to endpoint).
     */
    virtual double getAngle1() const {
        return data.startpoint.angleTo(data.endpoint);
    }

    /**
     * @return The angle of the line (from end to startpoint).
     */
    virtual double getAngle2() const {
        return data.endpoint.angleTo(data.startpoint);
    }

    virtual RS_Vector getNearestEndpoint(const RS_Vector& coord,
                                         double* dist = NULL);
    virtual RS_Vector getNearestPointOnEntity(const RS_Vector& coord,
            bool onEntity=true, double* dist = NULL, RS_Entity** entity=NULL);
    virtual RS_Vector getNearestCenter(const RS_Vector& coord,
                                       double* dist = NULL);
    virtual RS_Vector getNearestMiddle(const RS_Vector& coord,
                                       double* dist = NULL);
    virtual RS_Vector getNearestDist(double distance,
                                     const RS_Vector& coord,
                                     double* dist = NULL);
    virtual RS_Vector getNearestDist(double distance,
                                     bool startp);
    virtual double getDistanceToPoint(const RS_Vector& coord,
                                      RS_Entity** entity=NULL,
                                      RS2::ResolveLevel level=RS2::ResolveNone,
                                      double solidDist = RS_MAXDOUBLE,
                                      bool visually = false);
    virtual RS2::Side getSideOfPoint(const RS_Vector& coord);

    virtual void move(const RS_Vector& offset);
    virtual void rotate(const RS_Vector& center, double angle);
    virtual void scale(const RS_Vector& center, const RS_Vector& factor);
    virtual void mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2);
    virtual void stretch(const RS_Vector& firstCorner,
                         const RS_Vector& secondCorner,
                         const RS_Vector& offset);
    virtual void isoProject(RS2::IsoProjectionType type, double segmentLength);
    virtual void moveRef(const RS_Vector& ref, const RS_Vector& offset);

    virtual void draw(RS_Painter* painter, RS_GraphicView* view, double patternOffset=0.0);

    virtual void calculateBorders(bool visibleOnly=false);
    
    virtual RS_Variant getProperty(const RS_String& name, 
        const RS_Variant& def=RS_Variant());
    virtual RS_StringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const RS_String& name, const RS_Variant& value);
    
    static const char* propertyStartpointX;
    static const char* propertyStartpointY;
    static const char* propertyEndpointX;
    static const char* propertyEndpointY;

protected:
    RS_LineData data;
};


#endif
