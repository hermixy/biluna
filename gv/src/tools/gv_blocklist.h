/****************************************************************************
** $Id: gv_blocklist.h 4861 2007-03-07 15:29:43Z andrew $
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
** not clear to you. Updated for Biluna 2016-08-11
**
**********************************************************************/


#ifndef GV_BLOCKLIST_H
#define GV_BLOCKLIST_H


#include "gv_block.h"
#include "gv_blocklistlistener.h"
#include "gv_entity.h"
#include "gv_ptrlist.h"

/**
 * List of blocks.
 *
 * @see GV_Block
 *
 * @author Andrew Mustun
 */
class GV_BlockList {
public:
    GV_BlockList(bool owner=false);
    virtual ~GV_BlockList() {
        RB_DEBUG->print("GV_BlockList::~GV_BlockList");
        clear();
    }

    void clear();
    /**
     * @return Number of blocks available.
     */
    int count() {
        return blocks.count();
    }

    /**
     * @return Block at given position or NULL if 'i' is out of range.
     */
    GV_Block* at(int i) {
        if (i>=0 && i<blocks.count()) {
            return blocks[i];
        }
        else {
            return NULL;
        }
    }

    void activate(const QString& name);
    void activate(GV_Block* block);
    //! @return The active block of NULL if no block is activated.
    GV_Block* getActive() {
        return activeBlock;
    }

    virtual bool add(GV_Block* block, bool notify=true);
    virtual void addNotification(GV_Block* block);
    virtual void remove(GV_Block* block);
    virtual bool rename(GV_Block* block, const QString& name);
    //virtual void editBlock(GV_Block* block, const GV_Block& source);
    GV_Block* find(const QString& name);
    QString newName(const QString& suggestion = "");
    void toggle(const QString& name);
    void toggle(GV_Block* block);
    void freezeAll(bool freeze);

    /**
     * @return First block of the list.
     */
    //GV_Block* firstBlock() {
    //    return blocks.first();
    //}

    /**
     * @return Next block from the list after 
     * calling firstBlock() or nextBlock().
     */
    //GV_Block* nextBlock() {
    //    return blocks.next();
    //}

    void addListener(GV_BlockListListener* listener);
    void removeListener(GV_BlockListListener* listener);
    
    /**
     * Sets the layer lists modified status to 'm'.
     */
    void setModified(bool m) {
        modified = m;
    }
    
    /**
     * @retval true The layer list has been modified.
     * @retval false The layer list has not been modified.
     */
    virtual bool isModified() const {
        return modified;
    }

private:
    //! Is the list owning the blocks?
    bool owner;
    //! Blocks in the graphic
    GV_PtrList<GV_Block> blocks;
    //! List of registered BlockListListeners
    GV_PtrList<GV_BlockListListener> blockListListeners;
    //! Currently active block
    GV_Block* activeBlock;
    /** Flag set if the layer list was modified and not yet saved. */
    bool modified;
};

#endif
