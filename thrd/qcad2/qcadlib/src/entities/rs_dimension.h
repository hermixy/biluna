/****************************************************************************
** $Id: rs_dimension.h 8945 2008-02-07 23:37:50Z andrew $
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


#ifndef RS_DIMENSION_H
#define RS_DIMENSION_H

#include "rs_entitycontainer.h"
        
        
#define RS_DIM_DEFINITIONPOINT_X QObject::tr(RS_Dimension::propertyDefinitionPointX)
#define RS_DIM_DEFINITIONPOINT_Y QObject::tr(RS_Dimension::propertyDefinitionPointY)
#define RS_DIM_TEXTPOSITION_X QObject::tr(RS_Dimension::propertyTextPositionX)
#define RS_DIM_TEXTPOSITION_Y QObject::tr(RS_Dimension::propertyTextPositionY)
#define RS_DIM_TEXT QObject::tr(RS_Dimension::propertyText)

/**
 * Holds the data that is common to all dimension entities.
 */
class CAD_EXPORT RS_DimensionData /*: public RS_Flags*/ {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    RS_DimensionData() {}

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
    RS_DimensionData(const RS_Vector& definitionPoint,
                     const RS_Vector& middleOfText,
                     RS2::VAlign valign,
                     RS2::HAlign halign,
                     RS2::TextLineSpacingStyle lineSpacingStyle,
                     double lineSpacingFactor,
                     RS_String text,
                     RS_String style,
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
    RS_Vector definitionPoint;
    /** Middle point of dimension text */
    RS_Vector middleOfText;
    /** Vertical alignment */
    RS2::VAlign valign;
    /** Horizontal alignment */
    RS2::HAlign halign;
    /** Line spacing style */
    RS2::TextLineSpacingStyle lineSpacingStyle;
    /** Line spacing factor */
    double lineSpacingFactor;
    /**
     * Text string entered explicitly by user or null 
     * or "<>" for the actual measurement or " " (one blank space) 
     * for supressing the text. 
     */
    RS_String text;
    /** Dimension style name */
    RS_String style;
    /** Rotation angle of dimension text away from default orientation */
    double angle;
};



/**
 * Abstract base class for dimension entity classes.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_Dimension : public RS_EntityContainer {
public:
    RS_Dimension(RS_EntityContainer* parentContainer,
                 const RS_DimensionData& d);
    virtual ~RS_Dimension() {}

    /** @return Copy of data that defines the dimension. */
    RS_DimensionData getData() const {
        return data;
    }
    
    /** @return RS2::EntityDimension */
    virtual RS2::EntityType rtti() const {
        return RS2::EntityDimension;
    }
    virtual bool isOfType(RS2::EntityType t) const {
        return (t==RS2::EntityDimension || RS_EntityContainer::isOfType(t));
    }
    virtual RS_String typeName() const {
        return QObject::tr("Dimension");
    }
    
    RS_Vector getNearestRef(const RS_Vector& coord, double* dist, RS2::RefMode refMode=RS2::RefAll);
    RS_Vector getNearestSelectedRef(const RS_Vector& coord, double* dist, RS2::RefMode refMode=RS2::RefAll);

    RS_String getLabel(bool resolve=true);
    void setLabel(const RS_String& l);

    /**
     * Needs to be implemented by the dimension class to return the
     * measurement of the dimension (e.g. 10.5 or 15'14").
     */
    virtual RS_String getMeasuredLabel() = 0;

    /**
     * Must be overwritten by implementing dimension entity class
     * to update the subentities which make up the dimension entity.
     */
    virtual void update(bool autoText=false) = 0;

    void updateCreateDimensionLine(const RS_Vector& p1, const RS_Vector& p2,
              bool arrow1=true, bool arrow2=true, bool autoText=false);

    RS_Entity* createArrow(const RS_Vector& p, double angle);

    RS_Vector getDefinitionPoint() {
        return data.definitionPoint;
    }

    RS_Vector getMiddleOfText() {
        return data.middleOfText;
    }

    bool hasCustomTextPosition() {
        return data.middleOfText.valid;
    }

    RS2::VAlign getVAlign() {
        return data.valign;
    }

    RS2::HAlign getHAlign() {
        return data.halign;
    }

    RS2::TextLineSpacingStyle getLineSpacingStyle() {
        return data.lineSpacingStyle;
    }

    double getLineSpacingFactor() {
        return data.lineSpacingFactor;
    }

    RS_String getText() {
        return data.text;
    }

    RS_String getStyle() {
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

    virtual void move(const RS_Vector& offset);
    virtual void rotate(const RS_Vector& center, double angle);
    virtual void scale(const RS_Vector& center, const RS_Vector& factor);
    virtual void mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2);
    
    virtual RS_Variant getProperty(const RS_String& name, 
        const RS_Variant& def=RS_Variant());
    virtual RS_StringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const RS_String& name, const RS_Variant& value);

    static const char* propertyDefinitionPointX;
    static const char* propertyDefinitionPointY;
    static const char* propertyTextPositionX;
    static const char* propertyTextPositionY;
    static const char* propertyText;

protected:
    /** Data common to all dimension entities. */
    RS_DimensionData data;
    double attributeSize;
    RS_Vector autoTextPos;
};

#endif
