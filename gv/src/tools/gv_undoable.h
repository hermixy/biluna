/****************************************************************************
** $Id: gv_undoable.h 4861 2007-03-07 15:29:43Z andrew $
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


#ifndef GV_UNDOABLE_H
#define GV_UNDOABLE_H

#include "gv_flags.h"

class GV_UndoCycle;



/**
 * Base class for something that can be added and deleted and every 
 * addition and deletion can be undone.
 *
 * @see GV_Undo
 * @author Andrew Mustun
 */
class GV_Undoable : public GV_Flags {
public:
    GV_Undoable();
    virtual ~GV_Undoable();

    /**
     * Runtime type identification for undoables.
     * Note that this is voluntarily. The default implementation 
     * returns GV2::UndoableUnknown.
     */
    virtual GV2::UndoableType undoRtti() const {
        return GV2::UndoableUnknown;
    }

    virtual void setUndoCycle(GV_UndoCycle* cycle);
    virtual void changeUndoState();
    virtual void setUndoState(bool undone);
    virtual bool isUndone() const;

    /**
     * Can be overwriten by the implementing class to be notified
     * when the undo state changes (the undoable becomes visible / invisible).
     */
    virtual void undoStateChanged(bool /*undone*/) {}

private:
    GV_UndoCycle* cycle;
};

#endif
