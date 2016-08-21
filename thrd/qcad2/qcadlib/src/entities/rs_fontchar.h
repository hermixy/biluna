/****************************************************************************
** $Id: rs_fontchar.h 7930 2008-01-17 21:23:04Z andrew $
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


#ifndef RS_FONTCHAR_H
#define RS_FONTCHAR_H

#include "rs_block.h"


/**
 * A character in a font is represented by this special block class.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_FontChar : public RS_Block {
public:
    /**
     * @param parentContainer The font this block belongs to.
     * @param name The name of the letter (a unicode char) used as 
     *        an identifier.
     * @param basePoint Base point (offset) of the letter (usually 0/0).
     */
    RS_FontChar(RS_EntityContainer* parentContainer,
                const RS_String& name,
                RS_Vector basePoint)
            : RS_Entity(parentContainer), RS_Block(parentContainer, RS_BlockData(name, basePoint, false)) {}

    virtual ~RS_FontChar() {
        RS_DEBUG->print("RS_FontChar::~RS_FontChar");
    }

    /** @return RS2::EntityFontChar */
    virtual RS2::EntityType rtti() const {
        return RS2::EntityFontChar;
    }
    virtual bool isOfType(RS2::EntityType t) const {
        return (t==RS2::EntityFontChar || RS_Block::isOfType(t));
    }
};


#endif
