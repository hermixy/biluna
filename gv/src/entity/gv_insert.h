/****************************************************************************
** $Id: gv_insert.h 9471 2008-03-28 11:12:04Z andrew $
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


#ifndef GV_INSERT_H
#define GV_INSERT_H

#include "gv_entitycontainer.h"

#include "gv_block.h"
#include "gv_graphic.h"

#define GV_INSERT_NAME QObject::tr(GV_Insert::propertyName)
#define GV_INSERT_INSERTIONPOINT_X QObject::tr(GV_Insert::propertyInsertionPointX)
#define GV_INSERT_INSERTIONPOINT_Y QObject::tr(GV_Insert::propertyInsertionPointY)
#define GV_INSERT_SCALEFACTOR_X QObject::tr(GV_Insert::propertyScaleFactorX)
#define GV_INSERT_SCALEFACTOR_Y QObject::tr(GV_Insert::propertyScaleFactorY)
#define GV_INSERT_ANGLE QObject::tr(GV_Insert::propertyAngle)
#define GV_INSERT_COLS QObject::tr(GV_Insert::propertyCols)
#define GV_INSERT_ROWS QObject::tr(GV_Insert::propertyRows)
#define GV_INSERT_SPACING_X QObject::tr(GV_Insert::propertySpacingX)
#define GV_INSERT_SPACING_Y QObject::tr(GV_Insert::propertySpacingY)
        


/**
 * Holds the data that defines an insert.
 */
class GV_InsertData {
public:
    /**
     * Default constructor.
     */
    GV_InsertData() {}

    /**
     * @param name The name of the block used as an identifier.
     * @param insertionPoint Insertion point of the block.
     * @param scaleFactor Scale factor in x / y.
     * @param angle Rotation angle.
     * @param cols Number of cols if we insert a whole array.
     * @param rows Number of rows if we insert a whole array.
     * @param spacing Spacing between rows and cols.
     * @param blockSource Source for the block to insert if other than parent.
     *    Normally blocks are requested from the entity's parent but the
     *    block can also come from another resource. GV_Text uses that
     *    to share the blocks (letters) from a font.
     * @param updateMode GV2::Update will update the insert entity instantly
     *    GV2::NoUpdate will not update the insert. You can update
     *      it later manually using the update() method. This is 
     *    often the case since you might want to adjust attributes
     *    after creating an insert entity.
     */
    GV_InsertData(const QString& name,
                  const GV_Vector& insertionPoint,
                  const GV_Vector& scaleFactor,
                  double angle,
                  int cols, int rows, const GV_Vector& spacing,
                  GV_BlockList* blockSource = NULL,
                  GV2::UpdateMode updateMode = GV2::Update) {
        this->name = name;
        this->insertionPoint = insertionPoint;
        this->scaleFactor = scaleFactor;
        this->angle = angle;
        this->cols = cols;
        this->rows = rows;
        this->spacing = spacing;
        this->blockSource = blockSource;
        this->updateMode = updateMode;

    }

    friend class GV_Insert;

    QString name;
    GV_Vector insertionPoint;
    GV_Vector scaleFactor;
    double angle;
    int cols, rows;
    GV_Vector spacing;
    GV_BlockList* blockSource;
    GV2::UpdateMode updateMode;
};



/**
 * An insert inserts a block into the drawing at a certain location
 * with certain attributes (angle, scale, ...). 
 * Inserts don't really contain other entities internally. They just
 * refer to a block. However, to the outside world they act exactly 
 * like EntityContainer.
 *
 * @author Andrew Mustun
 */
class GV_Insert : public GV_EntityContainer {
public:
    GV_Insert(GV_EntityContainer* parentContainer,
              const GV_InsertData& d);
    virtual ~GV_Insert();

    virtual GV_Entity* clone() {
        GV_Insert* i = new GV_Insert(parentContainer, data);
        i->copyPropertiesFrom(this);
        i->entities = entities;
        i->entities.setAutoDelete(entities.autoDelete());
        i->initId();
        i->detach();
        return i;
    }

    /** @return GV2::EntityInsert */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityInsert;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityInsert || GV_EntityContainer::isOfType(t));
    }
    virtual QString typeName() const {
        return QObject::tr("Insert");
    }

    /** @return Copy of data that defines the insert. **/
    GV_InsertData getData() const {
        return data;
    }

    /**
     * Reimplementation of reparent. Invalidates block cache pointer.
     */
    virtual void reparent(GV_EntityContainer* parentContainer) {
        GV_Entity::reparent(parentContainer);
        block = NULL;
    }

    GV_Block* getBlockForInsert();

    virtual void update();

    QString getName() const {
        return data.name;
    }

    void setName(const QString& newName) {
        data.name = newName;
        update();
    }

    GV_Vector getInsertionPoint() const {
        return data.insertionPoint;
    }
    void setInsertionPoint(const GV_Vector& i) {
        data.insertionPoint = i;
    }

    GV_Vector getScale() const {
        return data.scaleFactor;
    }

    void setScale(const GV_Vector& s) {
        data.scaleFactor = s;
    }

    double getAngle() const {
        return data.angle;
    }
    void setAngle(double a) {
        data.angle = a;
    }

    int getCols() const {
        return data.cols;
    }
    void setCols(int c) {
        data.cols = c;
    }

    int getRows() const {
        return data.rows;
    }
    void setRows(int r) {
        data.rows = r;
    }

    GV_Vector getSpacing() const {
        return data.spacing;
    }
    void setSpacing(const GV_Vector& s) {
        data.spacing = s;
    }
    
    virtual bool isVisible();

    virtual GV_VectorList getRefPoints(GV2::RefMode refMode=GV2::RefAll);
    virtual GV_Vector getNearestRef(const GV_Vector& coord,
                                     double* dist = NULL,
                                     GV2::RefMode refMode=GV2::RefAll);

    virtual void move(const GV_Vector& offset);
    virtual void rotate(const GV_Vector& center, double angle);
    virtual void scale(const GV_Vector& center, const GV_Vector& factor);
    virtual void mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2);
    virtual void moveRef(const GV_Vector& ref, const GV_Vector& offset);
    
    virtual QVariant getProperty(const QString& name, 
        const QVariant& def=QVariant());
    virtual QStringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const QString& name, const QVariant& value);

    static const char* propertyName;
    static const char* propertyInsertionPointX;
    static const char* propertyInsertionPointY;
    static const char* propertyScaleFactorX;
    static const char* propertyScaleFactorY;
    static const char* propertyAngle;
    static const char* propertyCols;
    static const char* propertyRows;
    static const char* propertySpacingX;
    static const char* propertySpacingY;

protected:
    GV_InsertData data;
    GV_Block* block;
};


#endif
