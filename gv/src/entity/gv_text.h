/****************************************************************************
** $Id: gv_text.h 7857 2008-01-12 19:23:21Z andrew $
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


#ifndef GV_TEXT_H
#define GV_TEXT_H

#include "gv_entity.h"
#include "gv_entitycontainer.h"

#define GV_TEXT_INSERTIONPOINT_X   QObject::tr(GV_Text::propertyInsertionPointX)
#define GV_TEXT_INSERTIONPOINT_Y   QObject::tr(GV_Text::propertyInsertionPointY)
#define GV_TEXT_HEIGHT             QObject::tr(GV_Text::propertyHeight)
#define GV_TEXT_VALIGN             QObject::tr(GV_Text::propertyVAlign)
#define GV_TEXT_HALIGN             QObject::tr(GV_Text::propertyHAlign)
#define GV_TEXT_LINESPACINGFACTOR  QObject::tr(GV_Text::propertyLineSpacingFactor)
#define GV_TEXT_TEXT               QObject::tr(GV_Text::propertyText)
#define GV_TEXT_STYLE              QObject::tr(GV_Text::propertyStyle)
#define GV_TEXT_ANGLE              QObject::tr(GV_Text::propertyAngle)

/**
 * Holds the data that defines a text entity.
 */
class GV_TextData {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    GV_TextData() {}

    /**
     * Constructor with initialisation.
     *
     * @param insertionPoint Insertion point
     * @param height Nominal (initial) text height
     * @param width Reference rectangle width
     * @param valign Vertical alignment
     * @param halign Horizontal alignment
     * @param drawingDirection Drawing direction
     * @param lineSpacingStyle Line spacing style
     * @param lineSpacingFactor Line spacing factor
     * @param text Text string
     * @param style Text style name
     * @param angle Rotation angle
     * @param updateMode GV2::Update will update the text entity instantly
     *    GV2::NoUpdate will not update the entity. You can update
     *    it later manually using the update() method. This is
     *    often the case since you might want to adjust attributes
     *    after creating a text entity.
     */
    GV_TextData(const GV_Vector& insertionPoint,
                double height,
                double width,
                GV2::VAlign valign,
                GV2::HAlign halign,
                GV2::TextDrawingDirection drawingDirection,
                GV2::TextLineSpacingStyle lineSpacingStyle,
                double lineSpacingFactor,
                const QString& text,
                const QString& style,
                double angle,
                GV2::UpdateMode updateMode = GV2::Update) {
        this->insertionPoint = insertionPoint;
        this->height = height;
        this->width = width;
        this->valign = valign;
        this->halign = halign;
        this->drawingDirection = drawingDirection;
        this->lineSpacingStyle = lineSpacingStyle;
        this->lineSpacingFactor = lineSpacingFactor;
        this->style = style;
        this->angle = angle;
        this->text = text;
        this->updateMode = updateMode;
    }

    friend class GV_Text;

public:
    /** Insertion point */
    GV_Vector insertionPoint;
    /** Nominal (initial) text height */
    double height;
    /** Reference rectangle width */
    double width;
    /** Vertical alignment */
    GV2::VAlign valign;
    /** Horizontal alignment */
    GV2::HAlign halign;
    /** Drawing direction */
    GV2::TextDrawingDirection drawingDirection;
    /** Line spacing style */
    GV2::TextLineSpacingStyle lineSpacingStyle;
    /** Line spacing factor */
    double lineSpacingFactor;
    /** Text string */
    QString text;
    /** Text style name */
    QString style;
    /** Rotation angle */
    double angle;
    /** Update mode */
    GV2::UpdateMode updateMode;
};



/**
 * Class for a text entity.
 * Please note that text strings can contain special 
 * characters such as %%c for a diameter sign as well as unicode
 * characters. Line feeds are stored as real line feeds in the string.
 *
 * @author Andrew Mustun
 */
class GV_Text : public GV_EntityContainer {
public:
    GV_Text(GV_EntityContainer* parentContainer,
            const GV_TextData& d);
    virtual ~GV_Text() {}

    virtual GV_Entity* clone() {
        GV_Text* t = new GV_Text(parentContainer, data);
        t->copyPropertiesFrom(this);
        t->usedTextWidth = usedTextWidth;
        t->usedTextHeight = usedTextHeight;
        t->lineInsertionPoints = lineInsertionPoints;
        t->entities = entities;
        t->entities.setAutoDelete(entities.autoDelete());
        t->initId();
        t->detach();
        return t;
    }

    /**    @return GV2::EntityText */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityText;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityText || GV_EntityContainer::isOfType(t));
    }
    virtual QString typeName() const {
        return QObject::tr("Text");
    }

    /** @return Copy of data that defines the text. */
    GV_TextData getData() const {
        return data;
    }

    virtual void calculateBorders(bool visibleOnly=false);
    void update();
    void updateAddLine(GV_EntityContainer* textLine, int lineCounter);

    int getNumberOfLines();

    GV_Vector getInsertionPoint() {
        return data.insertionPoint;
    }
    void setInsertionPoint(const GV_Vector& ip) {
        data.insertionPoint = ip;
    }
    double getHeight() {
        return data.height;
    }
    void setHeight(double h) {
        data.height = h;
    }
    double getWidth() {
        return data.width;
    }
    void setAlignment(int a);
    int getAlignment();
    GV2::VAlign getVAlign() {
        return data.valign;
    }
    void setVAlign(GV2::VAlign va) {
        data.valign = va;
    }
    GV2::HAlign getHAlign() {
        return data.halign;
    }
    void setHAlign(GV2::HAlign ha) {
        data.halign = ha;
    }
    GV2::TextDrawingDirection getDrawingDirection() {
        return data.drawingDirection;
    }
    GV2::TextLineSpacingStyle getLineSpacingStyle() {
        return data.lineSpacingStyle;
    }
    void setLineSpacingFactor(double f) {
        data.lineSpacingFactor = f;
    }
    double getLineSpacingFactor() {
        return data.lineSpacingFactor;
    }
    void setText(const QString& t);
    QString getText() {
        return data.text;
    }
    void setStyle(const QString& s) {
        data.style = s;
    }
    QString getStyle() {
        return data.style;
    }
    void setAngle(double a) {
        data.angle = a;
    }
    double getAngle() {
        return data.angle;
    }
    double getUsedTextWidth() {
        return usedTextWidth;
    }
    double getUsedTextHeight() {
        return usedTextHeight;
    }

    virtual double getLength() {
        return -1.0;
    }
    
    virtual GV_VectorList getRefPoints(GV2::RefMode refMode=GV2::RefAll);
    virtual GV_Vector getNearestRef(const GV_Vector& coord,
                                     double* dist = NULL,
                                     GV2::RefMode refMode=GV2::RefAll);
    virtual GV_Vector getNearestSelectedRef(const GV_Vector& coord,
                                     double* dist = NULL,
                                     GV2::RefMode refMode=GV2::RefAll);
    
    virtual GV_VectorList getLineInsertionPoints();

    virtual void move(const GV_Vector& offset);
    virtual void rotate(const GV_Vector& center, double angle);
    virtual void scale(const GV_Vector& center, const GV_Vector& factor);
    virtual void mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2);
    virtual void moveRef(const GV_Vector& ref, const GV_Vector& offset);
    virtual bool hasEndpointsWithinWindow(const GV_Vector& v1, const GV_Vector& v2);
    virtual void stretch(const GV_Vector& firstCorner,
                         const GV_Vector& secondCorner,
                         const GV_Vector& offset);

    virtual QVariant getProperty(const QString& name, 
        const QVariant& def=QVariant());
    virtual QStringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const QString& name, const QVariant& value);
    //virtual QStringList getPropertyChoices(const QString& name);
    virtual GV_PropertyAttributes getPropertyAttributes(
        const QString& propertyName);

    static QString vAlignToText(GV2::VAlign va) {
        switch(va) {
        default:
        case GV2::VAlignTop:
            return QObject::tr("Top");
        case GV2::VAlignMiddle:
            return QObject::tr("Middle");
        case GV2::VAlignBottom:
            return QObject::tr("Bottom");
        }
    }
    
    static GV2::VAlign textToVAlign(const QString& t) {
        if (t==vAlignToText(GV2::VAlignTop)) {
            return GV2::VAlignTop;
        }
        else if (t==vAlignToText(GV2::VAlignMiddle)) {
            return GV2::VAlignMiddle;
        }
        else if (t==vAlignToText(GV2::VAlignBottom)) {
            return GV2::VAlignBottom;
        }
        
        return GV2::VAlignTop;
    }
    
    static QString hAlignToText(GV2::HAlign va) {
        switch(va) {
        default:
        case GV2::HAlignLeft:
            return QObject::tr("Left");
        case GV2::HAlignCenter:
            return QObject::tr("Center");
        case GV2::HAlignRight:
            return QObject::tr("Right");
        }
    }
    
    static GV2::HAlign textToHAlign(const QString& t) {
        if (t==hAlignToText(GV2::HAlignLeft)) {
            return GV2::HAlignLeft;
        }
        else if (t==hAlignToText(GV2::HAlignCenter)) {
            return GV2::HAlignCenter;
        }
        else if (t==hAlignToText(GV2::HAlignRight)) {
            return GV2::HAlignRight;
        }

        return GV2::HAlignLeft;
    }

protected:
    GV_TextData data;

    /**
     * Text width used by the current contents of this text entity. 
     * This property is updated by the update method.
     * @see update
     */
    double usedTextWidth;
    /**
     * Text height used by the current contents of this text entity.
     * This property is updated by the update method.
     * @see update
     */
    double usedTextHeight;

    /**
     * Line insertion points. One for every text line. Used to split up 
     * text into text lines.
     */
    GV_VectorList lineInsertionPoints;

    static const char* propertyInsertionPointX;
    static const char* propertyInsertionPointY;
    static const char* propertyHeight;
    static const char* propertyVAlign;
    static const char* propertyHAlign;
    static const char* propertyLineSpacingFactor;
    static const char* propertyText;
    static const char* propertyStyle;
    static const char* propertyAngle;
};

#endif
