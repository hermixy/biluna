/****************************************************************************
** $Id: gv_dimension.h 8945 2008-02-07 23:37:50Z andrew $
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


#ifndef GV_DIMENSION_H
#define GV_DIMENSION_H

#include "gv_entitycontainer.h"
        
        
#define GV_DIM_DEFINITIONPOINT_X QObject::tr(GV_Dimension::propertyDefinitionPointX)
#define GV_DIM_DEFINITIONPOINT_Y QObject::tr(GV_Dimension::propertyDefinitionPointY)
#define GV_DIM_TEXTPOSITION_X QObject::tr(GV_Dimension::propertyTextPositionX)
#define GV_DIM_TEXTPOSITION_Y QObject::tr(GV_Dimension::propertyTextPositionY)
#define GV_DIM_TEXT QObject::tr(GV_Dimension::propertyText)

/**
 * Holds the data that is common to all dimension entities.
 */
class GV_DimensionData /*: public GV_Flags*/ {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    GV_DimensionData() {}

    /**
     * Constructor with initialisation.
     *
     * @param definitionPoint Definition point.
     * @param middleOfText Middle point of dimension text.
     * @param valign Vertical alignment.
     * @param halign Horizontal alignment.
     * @param lineSpacingStyle Line spacing style.
     * @param lineSpacingFactor Line spacing factor.
     * @param text Text string entered explicitly by user or null
     *         or "<>" for the actual measurement or " " (one blank space).
     *         for supressing the text.
     * @param style Dimension style name.
     * @param angle Rotation angle of dimension text away from 
     *         default orientation.
     */
    GV_DimensionData(const GV_Vector& definitionPoint,
                     const GV_Vector& middleOfText,
                     GV2::VAlign valign,
                     GV2::HAlign halign,
                     GV2::TextLineSpacingStyle lineSpacingStyle,
                     double lineSpacingFactor,
                     QString text,
                     QString style,
                     double angle) {
        this->definitionPoint = definitionPoint;
        this->middleOfText = middleOfText;
        this->valign = valign;
        this->halign = halign;
        this->lineSpacingStyle = lineSpacingStyle;
        this->lineSpacingFactor = lineSpacingFactor;
        this->text = text;
        this->style = style;
        this->angle = angle;
    }

public:
    /** Definition point */
    GV_Vector definitionPoint;
    /** Middle point of dimension text */
    GV_Vector middleOfText;
    /** Vertical alignment */
    GV2::VAlign valign;
    /** Horizontal alignment */
    GV2::HAlign halign;
    /** Line spacing style */
    GV2::TextLineSpacingStyle lineSpacingStyle;
    /** Line spacing factor */
    double lineSpacingFactor;
    /**
     * Text string entered explicitly by user or null 
     * or "<>" for the actual measurement or " " (one blank space) 
     * for supressing the text. 
     */
    QString text;
    /** Dimension style name */
    QString style;
    /** Rotation angle of dimension text away from default orientation */
    double angle;
};



/**
 * Abstract base class for dimension entity classes.
 *
 * @author Andrew Mustun
 */
class GV_Dimension : public GV_EntityContainer {
public:
    GV_Dimension(GV_EntityContainer* parentContainer,
                 const GV_DimensionData& d);
    virtual ~GV_Dimension() {}

    /** @return Copy of data that defines the dimension. */
    GV_DimensionData getData() const {
        return data;
    }
    
    /** @return GV2::EntityDimension */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityDimension;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityDimension || GV_EntityContainer::isOfType(t));
    }
    virtual QString typeName() const {
        return QObject::tr("Dimension");
    }
    
    GV_Vector getNearestRef(const GV_Vector& coord, double* dist, GV2::RefMode refMode=GV2::RefAll);
    GV_Vector getNearestSelectedRef(const GV_Vector& coord, double* dist, GV2::RefMode refMode=GV2::RefAll);

    QString getLabel(bool resolve=true);
    void setLabel(const QString& l);

    /**
     * Needs to be implemented by the dimension class to return the
     * measurement of the dimension (e.g. 10.5 or 15'14").
     */
    virtual QString getMeasuredLabel() = 0;

    /**
     * Must be overwritten by implementing dimension entity class
     * to update the subentities which make up the dimension entity.
     */
    virtual void update(bool autoText=false) = 0;

    void updateCreateDimensionLine(const GV_Vector& p1, const GV_Vector& p2,
              bool arrow1=true, bool arrow2=true, bool autoText=false);

    GV_Entity* createArrow(const GV_Vector& p, double angle);

    GV_Vector getDefinitionPoint() {
        return data.definitionPoint;
    }

    GV_Vector getMiddleOfText() {
        return data.middleOfText;
    }

    bool hasCustomTextPosition() {
        return data.middleOfText.valid;
    }

    GV2::VAlign getVAlign() {
        return data.valign;
    }

    GV2::HAlign getHAlign() {
        return data.halign;
    }

    GV2::TextLineSpacingStyle getLineSpacingStyle() {
        return data.lineSpacingStyle;
    }

    double getLineSpacingFactor() {
        return data.lineSpacingFactor;
    }

    QString getText() {
        return data.text;
    }

    QString getStyle() {
        return data.style;
    }

    double getAngle() {
        return data.angle;
    }

    bool useArchTick();
    double getArrowSize();
    double getExtensionLineExtension();
    double getExtensionLineOffset();
    double getDimensionLineGap();
    double getTextHeight();
    
    virtual double getLength() {
        return -1.0;
    }

    /**
     * Override size of text, arrows, ... The value corresponds to the
     * text height. Arrow size and other sizes are derrived from it.
     */
    void overrideAttributeSize(double f) {
        attributeSize = f;
    }

    virtual void move(const GV_Vector& offset);
    virtual void rotate(const GV_Vector& center, double angle);
    virtual void scale(const GV_Vector& center, const GV_Vector& factor);
    virtual void mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2);
    
    virtual QVariant getProperty(const QString& name, 
        const QVariant& def=QVariant());
    virtual QStringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const QString& name, const QVariant& value);

    static const char* propertyDefinitionPointX;
    static const char* propertyDefinitionPointY;
    static const char* propertyTextPositionX;
    static const char* propertyTextPositionY;
    static const char* propertyText;

protected:
    /** Data common to all dimension entities. */
    GV_DimensionData data;
    double attributeSize;
    GV_Vector autoTextPos;
};

#endif
