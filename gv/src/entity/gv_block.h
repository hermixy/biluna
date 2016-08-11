/****************************************************************************
** $Id: gv_block.h 6363 2007-09-14 16:45:12Z andrew $
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


#ifndef GV_BLOCK_H
#define GV_BLOCK_H

#include "gv_document.h"

/**
 * Holds the data that defines a block.
 */
class GV_BlockData {
public:
    GV_BlockData() {}

    GV_BlockData(const QString& name,
               const GV_Vector& basePoint,
               bool frozen) {

        this->name = name;
        this->basePoint = basePoint;
        this->frozen = frozen;
    }

    bool isValid() {
        return (!name.isEmpty() && basePoint.valid);
    }

public:
    /**
     * Block name. Acts as an id.
     */
    QString name;
    /*
     * Base point of the Block. Usually 0/0 since blocks can be moved around 
     * using the insertion point of Insert entities.
     */
    GV_Vector basePoint;

    //! Frozen flag
    bool frozen;
};



/**
 * A block is a group of entities. A block unlike an other entity
 * container has a base point which defines the offset of the
 * block. Note that although technically possible, a block should
 * never be part of the entity tree of a graphic. Blocks are 
 * stored in a seperate list inside the graphic document (a block list).
 * The graphic can contain GV_Insert entities that refer to such 
 * blocks.
 *
 * blocks are documents and can therefore be handled by graphic views.
 *
 * @author Andrew Mustun
 */
class GV_Block : public GV_Document {

    friend class GV_BlockList;

public:
    /**
     * @param parent The graphic this block belongs to.
     * @param blockData defining data of the block.
     */
    GV_Block(GV_EntityContainer* parentContainer, const GV_BlockData& d);

    virtual ~GV_Block();
    
    virtual GV_Entity* clone();

    /** @return GV2::EntityBlock */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityBlock;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityBlock || GV_Document::isOfType(t));
    }

    /**
     * @return Name of this block (the name is an Id for this block).
     */
    QString getName() const {
        return data.name;
    }

    /**
     * @return base point of this block.
     */
    GV_Vector getBasePoint() const {
        return data.basePoint;
    }

    virtual GV_LayerList* getLayerList();
    virtual GV_BlockList* getBlockList();

    /**
     * Reimplementation from GV_Document. Does nothing.
     */
    virtual void newDoc() {
        // do nothing
    }

    /**
     * Reimplementation from GV_Document. Saves the parent graphic document.
     */
    virtual bool save();

    /**
     * Reimplementation from GV_Document. Does nothing.
     */
    virtual bool saveAs(const QString& filename, GV2::FormatType type);

    /**
     * Reimplementation from GV_Document. Does nothing.
     */
    virtual bool open(const QString& , GV2::FormatType, bool = true) {
        // do nothing
        return false;
    }

    /** 
     * sets a new name for the block. Only called by blocklist to
     * assure that block names stay unique.
     */
    void setName(const QString& n) {
        data.name = n;
    }
    
    /**
     * @retval true if this block is frozen (invisible)
     * @retval false if this block isn't frozen (visible)
     */
    bool isFrozen() const {
        return data.frozen;
    }

    /**
     * Toggles the visibility of this block.
     * Freezes the block if it's not frozen, thaws the block otherwise
     */
    void toggle() {
        data.frozen = !data.frozen;
    }

    /**
     * (De-)freezes this block.
     *
     * @param freeze true: freeze, false: defreeze
     */
    void freeze(bool freeze) {
        data.frozen = freeze;
    }
    
    virtual void setModified(bool m);


protected:
    //! Block data
    GV_BlockData data;
};


#endif
