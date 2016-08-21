/****************************************************************************
** $Id: rs_undocycle.h 4861 2007-03-07 15:29:43Z andrew $
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


#ifndef RS_UNDOLISTITEM_H
#define RS_UNDOLISTITEM_H

#include <iostream>

#include "rs.h"
#include "rs_entity.h"
#include "rs_ptrlist.h"
#include "rs_undoable.h"

/**
 * An Undo Cycle represents an action that was triggered and can 
 * be undone. It stores all the pointers to the Undoables affected by 
 * the action. Undoables are entities in a container that can be
 * created and deleted.
 *
 * Undo Cycles are stored within classes derrived from RS_Undo.
 *
 * @see RS_Undoable
 * @see RS_Undo
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_UndoCycle {
public:
    /**
     * @param type Type of undo item.
     */
    RS_UndoCycle(/*RS2::UndoType type*/) 
        : iter(undoables) {
        //this->type = type;
        undoables.setAutoDelete(false);
    }

    /**
     * Adds an Undoable to this Undo Cycle. Every Cycle can contain one or
     * more Undoables.
     */
    void addUndoable(RS_Undoable* u) {
        undoables.append(u);
    }

    /**
     * Removes an undoable from the list.
     */
    void removeUndoable(RS_Undoable* u) {
        undoables.removeAt(undoables.indexOf(u));
    }

    /**
     * Iteration through undoable elements in this item.
     */
    RS_Undoable* getFirstUndoable() {
        iter.toFront();
        return getNextUndoable();
    }

    /**
     * Iteration through undoable elements in this item.
     */
    RS_Undoable* getNextUndoable() {
        if (iter.hasNext()) {
            return iter.next();
        }
        else {
            return NULL;
        }
    }

    friend class RS_Undo;

private:
    //! List of entity id's that were affected by this action
    RS_PtrList<RS_Undoable> undoables;
    RS_MutableListIterator<RS_Undoable*> iter;
};

#endif
