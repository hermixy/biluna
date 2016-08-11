/****************************************************************************
** $Id: gv_hatch.h 8935 2008-02-07 20:52:34Z andrew $
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


#ifndef GV_HATCH_H
#define GV_HATCH_H

#include "gv_entity.h"
#include "gv_entitycontainer.h"

#define GV_HATCH_SOLID QObject::tr(GV_Hatch::propertySolid)
#define GV_HATCH_SCALE QObject::tr(GV_Hatch::propertyScale)
#define GV_HATCH_ANGLE QObject::tr(GV_Hatch::propertyAngle)
#define GV_HATCH_PATTERN QObject::tr(GV_Hatch::propertyPattern)

/**
 * Holds the data that defines a hatch entity.
 */
class GV_HatchData {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    GV_HatchData() {}

    /**
     * @param solid true: solid fill, false: pattern.
     * @param scale Pattern scale or spacing.
     * @param pattern Pattern name.
     */
    GV_HatchData(bool solid,
                 double scale,
                 double angle,
                 const QString& pattern) {
        this->solid = solid;
        this->scale = scale;
        this->angle = angle;
        this->pattern = pattern;
    }

public:
    bool solid;
    double scale;
    double angle;
    QString pattern;
};



/**
 * Class for a hatch entity.
 *
 * @todo: redesign with generated hatch in 'visual' entity.
 *
 * @author Andrew Mustun
 */
class GV_Hatch : public virtual GV_EntityContainer {
public:
    GV_Hatch(GV_EntityContainer* parentContainer,
            const GV_HatchData& d);
    virtual ~GV_Hatch() {}

    virtual GV_Entity* clone();

    /** @return GV2::EntityHatch */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityHatch;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityHatch || GV_EntityContainer::isOfType(t));
    }
    virtual QString typeName() const {
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
    GV_HatchData getData() const {
        return data;
    }
    
    virtual GV_VectorList getRefPoints(GV2::RefMode refMode=GV2::RefAll);
    virtual GV_Vector getNearestSelectedRef(const GV_Vector& coord,
                                     double* dist = NULL,
                                     GV2::RefMode refMode=GV2::RefAll);

    void initFromVectorList(const GV_VectorList& vectorList);
    
    bool validate();
    
    int countLoops();

    /** @return true if this is a solid fill. false if it is a pattern hatch. */
    bool isSolid() const {
        return data.solid;
    }
    void setSolid(bool solid) {
        data.solid = solid;
    }

    QString getPattern() {
        return data.pattern;
    }
    void setPattern(const QString& pattern) {
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
    
    virtual void draw(GV_Painter* painter, GV_GraphicView* view, 
        double patternOffset=0.0);

    virtual double getLength() {
        return -1.0;
    }
    
    virtual double getDistanceToPoint(const GV_Vector& coord,
                                      GV_Entity** entity = NULL,
                                      GV2::ResolveLevel level = GV2::ResolveNone,
                                      double solidDist = GV_MAXDOUBLE,
                                      bool visually = false);

    virtual void move(const GV_Vector& offset);
    virtual void rotate(const GV_Vector& center, double angle);
    virtual void scale(const GV_Vector& center, const GV_Vector& factor);
    virtual void mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2);
    virtual void stretch(const GV_Vector& firstCorner,
                         const GV_Vector& secondCorner,
                         const GV_Vector& offset);
    
    virtual QVariant getProperty(const QString& name, 
        const QVariant& def=QVariant());
    virtual QStringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const QString& name, const QVariant& value);
    //virtual QStringList getPropertyChoices(const QString& name);
    virtual GV_PropertyAttributes getPropertyAttributes(const QString& propertyName);

    static const char* propertySolid;
    static const char* propertyScale;
    static const char* propertyAngle;
    static const char* propertyPattern;

protected:
    GV_HatchData data;
    GV_EntityContainer* hatchPattern;
    bool updateRunning;
    bool needOptimization;
};

#endif
