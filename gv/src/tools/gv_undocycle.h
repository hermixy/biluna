/****************************************************************************
** $Id: gv_undocycle.h 4861 2007-03-07 15:29:43Z andrew $
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


#ifndef GV_UNDOLISTITEM_H
#define GV_UNDOLISTITEM_H

#include <iostream>

#include "gv.h"
#include "gv_entity.h"
#include "gv_ptrlist.h"
#include "gv_undoable.h"

/**
 * An Undo Cycle represents an action that was triggered and can 
 * be undone. It stores all the pointers to the Undoables affected by 
 * the action. Undoables are entities in a container that can be
 * created and deleted.
 *
 * Undo Cycles are stored within classes derrived from GV_Undo.
 *
 * @see GV_Undoable
 * @see GV_Undo
 *
 * @author Andrew Mustun
 */
class GV_UndoCycle {
public:
    /**
     * @param type Type of undo item.
     */
    GV_UndoCycle(/*GV2::UndoType type*/) 
        : iter(undoables) {
        //this->type = type;
        undoables.setAutoDelete(false);
    }

    /**
     * Adds an Undoable to this Undo Cycle. Every Cycle can contain one or
     * more Undoables.
     */
    void addUndoable(GV_Undoable* u) {
        undoables.append(u);
    }

    /**
     * Removes an undoable from the list.
     */
    void removeUndoable(GV_Undoable* u) {
        undoables.removeAt(undoables.indexOf(u));
    }

    /**
     * Iteration through undoable elements in this item.
     */
    GV_Undoable* getFirstUndoable() {
        iter.toFront();
        return getNextUndoable();
    }

    /**
     * Iteration through undoable elements in this item.
     */
    GV_Undoable* getNextUndoable() {
        if (iter.hasNext()) {
            return iter.next();
        }
        else {
            return NULL;
        }
    }

    friend class GV_Undo;

private:
    //! List of entity id's that were affected by this action
    GV_PtrList<GV_Undoable> undoables;
    GV_MutableListIterator<GV_Undoable*> iter;
};

#endif
