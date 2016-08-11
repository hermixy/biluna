/****************************************************************************
** $Id: gv_line.h 8878 2008-02-04 21:39:25Z andrew $
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


#ifndef GV_LINE_H
#define GV_LINE_H

#include "gv_atomicentity.h"

#define GV_LINE_STARTPOINT_X   QObject::tr(GV_Line::propertyStartpointX)
#define GV_LINE_STARTPOINT_Y   QObject::tr(GV_Line::propertyStartpointY)
#define GV_LINE_ENDPOINT_X   QObject::tr(GV_Line::propertyEndpointX)
#define GV_LINE_ENDPOINT_Y   QObject::tr(GV_Line::propertyEndpointY)

/**
 * Holds the data that defines a line.
 */
class GV_LineData {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    GV_LineData() {}

    GV_LineData(const GV_Vector& startpoint,
                const GV_Vector& endpoint) {

        this->startpoint = startpoint;
        this->endpoint = endpoint;
    }

    GV_Vector startpoint;
    GV_Vector endpoint;
};


/**
 * Class for a line entity.
 *
 * @author Andrew Mustun
 */
class GV_Line : public virtual GV_AtomicEntity {
public:
    GV_Line(GV_EntityContainer* parentContainer,
            const GV_LineData& d);
    virtual ~GV_Line();
    virtual GV_Entity* clone();

    /** @return GV2::EntityLine */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityLine;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityLine || GV_AtomicEntity::isOfType(t));
    }
    
    virtual QString typeName() const {
        return QObject::tr("Line");
    }
    
    /** @return true */
    virtual bool isEdge() const {
        return true;
    }
    
    /** @return Copy of data that defines the line. */
    GV_LineData getData() const {
        return data;
    }
    
    /** Sets a new line data. */
    void setData(const GV_LineData& d) {
        data = d;
        update();
    }
    
    virtual GV_VectorList getRefPoints(GV2::RefMode refMode=GV2::RefAll);

    /** @return Startpoint of the entity */
    virtual GV_Vector getStartpoint() const {
        return data.startpoint;
    }
    /** @return Endpoint of the entity */
    virtual GV_Vector getEndpoint() const {
        return data.endpoint;
    }

    /** Sets the startpoint */
    void setStartpoint(const GV_Vector& s) {
        moveStartpoint(s);
    }
    /** Sets the endpoint */
    void setEndpoint(const GV_Vector& e) {
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
    virtual void moveStartpoint(const GV_Vector& pos);
    virtual void moveEndpoint(const GV_Vector& pos);
    virtual GV2::Ending getTrimPoint(const GV_Vector& coord, 
              const GV_Vector& trimPoint);
    virtual void reverse();
    /** @return the center point of the line. */
    GV_Vector getMiddlepoint() {
        return (data.startpoint + data.endpoint)/2.0;
    }
    virtual bool hasEndpointsWithinWindow(const GV_Vector& v1, const GV_Vector& v2);

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

    virtual GV_Vector getNearestEndpoint(const GV_Vector& coord,
                                         double* dist = NULL);
    virtual GV_Vector getNearestPointOnEntity(const GV_Vector& coord,
            bool onEntity=true, double* dist = NULL, GV_Entity** entity=NULL);
    virtual GV_Vector getNearestCenter(const GV_Vector& coord,
                                       double* dist = NULL);
    virtual GV_Vector getNearestMiddle(const GV_Vector& coord,
                                       double* dist = NULL);
    virtual GV_Vector getNearestDist(double distance,
                                     const GV_Vector& coord,
                                     double* dist = NULL);
    virtual GV_Vector getNearestDist(double distance,
                                     bool startp);
    virtual double getDistanceToPoint(const GV_Vector& coord,
                                      GV_Entity** entity=NULL,
                                      GV2::ResolveLevel level=GV2::ResolveNone,
                                      double solidDist = GV_MAXDOUBLE,
                                      bool visually = false);
    virtual GV2::Side getSideOfPoint(const GV_Vector& coord);

    virtual void move(const GV_Vector& offset);
    virtual void rotate(const GV_Vector& center, double angle);
    virtual void scale(const GV_Vector& center, const GV_Vector& factor);
    virtual void mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2);
    virtual void stretch(const GV_Vector& firstCorner,
                         const GV_Vector& secondCorner,
                         const GV_Vector& offset);
    virtual void isoProject(GV2::IsoProjectionType type, double segmentLength);
    virtual void moveRef(const GV_Vector& ref, const GV_Vector& offset);

    virtual void draw(GV_Painter* painter, GV_GraphicView* view, double patternOffset=0.0);

    virtual void calculateBorders(bool visibleOnly=false);
    
    virtual QVariant getProperty(const QString& name, 
        const QVariant& def=QVariant());
    virtual QStringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const QString& name, const QVariant& value);
    
    static const char* propertyStartpointX;
    static const char* propertyStartpointY;
    static const char* propertyEndpointX;
    static const char* propertyEndpointY;

protected:
    GV_LineData data;
};


#endif
