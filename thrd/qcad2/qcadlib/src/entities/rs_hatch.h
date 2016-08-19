/****************************************************************************
** $Id: rs_hatch.h 8935 2008-02-07 20:52:34Z andrew $
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


#ifndef RS_HATCH_H
#define RS_HATCH_H

#include "rs_entity.h"
#include "rs_entitycontainer.h"

#define RS_HATCH_SOLID QObject::tr(RS_Hatch::propertySolid)
#define RS_HATCH_SCALE QObject::tr(RS_Hatch::propertyScale)
#define RS_HATCH_ANGLE QObject::tr(RS_Hatch::propertyAngle)
#define RS_HATCH_PATTERN QObject::tr(RS_Hatch::propertyPattern)

/**
 * Holds the data that defines a hatch entity.
 */
class RS_HatchData {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    RS_HatchData() {}

    /**
     * @param solid true: solid fill, false: pattern.
     * @param scale Pattern scale or spacing.
     * @param pattern Pattern name.
     */
    RS_HatchData(bool solid,
                 double scale,
                 double angle,
                 const RS_String& pattern) {
        this->solid = solid;
        this->scale = scale;
        this->angle = angle;
        this->pattern = pattern;
    }

public:
    bool solid;
    double scale;
    double angle;
    RS_String pattern;
};



/**
 * Class for a hatch entity.
 *
 * @todo: redesign with generated hatch in 'visual' entity.
 *
 * @author Andrew Mustun
 */
class RS_Hatch : public virtual RS_EntityContainer {
public:
    RS_Hatch(RS_EntityContainer* parentContainer,
            const RS_HatchData& d);
    virtual ~RS_Hatch() {}

    virtual RS_Entity* clone();

    /** @return RS2::EntityHatch */
    virtual RS2::EntityType rtti() const {
        return RS2::EntityHatch;
    }
    virtual bool isOfType(RS2::EntityType t) const {
        return (t==RS2::EntityHatch || RS_EntityContainer::isOfType(t));
    }
    virtual RS_String typeName() const {
        return QObject::tr("Hatch");
    }
    
    /**
     * @return true: if this is a hatch with lines (hatch pattern),
     *         false: if this is filled with a solid color.
     */
    virtual bool isContainer() const {
        if (isSolid()) {
            return false;
        }
        else {
            return true;
        }
    }

    /** @return Copy of data that defines the hatch. */
    RS_HatchData getData() const {
        return data;
    }
    
    virtual RS_VectorList getRefPoints(RS2::RefMode refMode=RS2::RefAll);
    virtual RS_Vector getNearestSelectedRef(const RS_Vector& coord,
                                     double* dist = NULL,
                                     RS2::RefMode refMode=RS2::RefAll);

    void initFromVectorList(const RS_VectorList& vectorList);
    
    bool validate();
    
    int countLoops();

    /** @return true if this is a solid fill. false if it is a pattern hatch. */
    bool isSolid() const {
        return data.solid;
    }
    void setSolid(bool solid) {
        data.solid = solid;
    }

    RS_String getPattern() {
        return data.pattern;
    }
    void setPattern(const RS_String& pattern) {
        data.pattern = pattern;
    }
    
    double getScale() {
        return data.scale;
    }
    void setScale(double scale) {
        data.scale = scale;
    }
    
    double getAngle() {
        return data.angle;
    }
    void setAngle(double angle) {
        data.angle = angle;
    }

    //virtual void calculateBorders();
    virtual void update();
    //void activateContour(bool on);
    
    virtual void draw(RS_Painter* painter, RS_GraphicView* view, 
        double patternOffset=0.0);

    virtual double getLength() {
        return -1.0;
    }
    
    virtual double getDistanceToPoint(const RS_Vector& coord,
                                      RS_Entity** entity = NULL,
                                      RS2::ResolveLevel level = RS2::ResolveNone,
                                      double solidDist = RS_MAXDOUBLE,
                                      bool visually = false);

    virtual void move(const RS_Vector& offset);
    virtual void rotate(const RS_Vector& center, double angle);
    virtual void scale(const RS_Vector& center, const RS_Vector& factor);
    virtual void mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2);
    virtual void stretch(const RS_Vector& firstCorner,
                         const RS_Vector& secondCorner,
                         const RS_Vector& offset);
    
    virtual RS_Variant getProperty(const RS_String& name, 
        const RS_Variant& def=RS_Variant());
    virtual RS_StringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const RS_String& name, const RS_Variant& value);
    //virtual RS_StringList getPropertyChoices(const RS_String& name);
    virtual RS_PropertyAttributes getPropertyAttributes(const RS_String& propertyName);

    static const char* propertySolid;
    static const char* propertyScale;
    static const char* propertyAngle;
    static const char* propertyPattern;

protected:
    RS_HatchData data;
    RS_EntityContainer* hatchPattern;
    bool updateRunning;
    bool needOptimization;
};

#endif
