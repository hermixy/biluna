/****************************************************************************
** $Id: gv_undo.h 7067 2007-11-13 09:36:48Z andrew $
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


#ifndef GV_UNDO_H
#define GV_UNDO_H

#include "gv_undocycle.h"
#include "gv_ptrlist.h"

/**
 * Undo / redo functionality. The internal undo list consists of
 * GV_UndoCycle entries.
 *
 * @see GV_UndoCycle
 * @author Andrew Mustun
 */
class GV_Undo {
public:
    GV_Undo();
    virtual ~GV_Undo();

    void addUndoCycle(GV_UndoCycle* i);

    virtual void undo();
    virtual void redo();

    virtual GV_UndoCycle* getUndoCycle();
    virtual GV_UndoCycle* getRedoCycle();

    virtual int countUndoCycles();
    virtual int countRedoCycles();

    virtual void startUndoCycle();
    virtual void addUndoable(GV_Undoable* u);
    virtual void endUndoCycle();

    /**
     * Must be overwritten by the implementing class and delete
     * the given Undoable (unrecoverable). This method is called
     * for Undoables that are no longer in the undo buffer.
     */
    virtual void removeUndoable(GV_Undoable* u) = 0;
    
    virtual void removeUndoablePointers(GV_Undoable* u);

    void setUndoEnabled(bool on) {
        undoEnabled = on;
    }

    bool isUndoEnabled() {
        return undoEnabled;
    }

protected:
    //! List of undo list items. every item is something that can be undone.
    GV_PtrList<GV_UndoCycle> undoList;

    /**
     * Index that points to the current position in the undo list.
     * The item it points on will be undone the next time undo is called.
     * The item after will be redone (if there is an item) when redo 
     * is called.
     */
    int undoPointer;

    /**
     * Current undo cycle.
     */
    GV_UndoCycle* currentCycle;

    bool undoEnabled;

};


#endif

