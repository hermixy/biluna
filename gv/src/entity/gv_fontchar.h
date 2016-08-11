/****************************************************************************
** $Id: gv_fontchar.h 7930 2008-01-17 21:23:04Z andrew $
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


#ifndef GV_FONTCHAR_H
#define GV_FONTCHAR_H

#include "gv_block.h"


/**
 * A character in a font is represented by this special block class.
 *
 * @author Andrew Mustun
 */
class GV_FontChar : public GV_Block {
public:
    /**
     * @param parentContainer The font this block belongs to.
     * @param name The name of the letter (a unicode char) used as 
     *        an identifier.
     * @param basePoint Base point (offset) of the letter (usually 0/0).
     */
    GV_FontChar(GV_EntityContainer* parentContainer,
                const QString& name,
                GV_Vector basePoint)
            : GV_Entity(parentContainer), GV_Block(parentContainer, GV_BlockData(name, basePoint, false)) {}

    virtual ~GV_FontChar() {
        RB_DEBUG->print("GV_FontChar::~GV_FontChar");
    }

    /** @return GV2::EntityFontChar */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityFontChar;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityFontChar || GV_Block::isOfType(t));
    }
};


#endif
