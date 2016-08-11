/****************************************************************************
** $Id: gv_block.cpp 7930 2008-01-17 21:23:04Z andrew $
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


#include "gv_block.h"

#include "gv_graphic.h"

/**
 * @param parentContainer The graphic this block belongs to.
 * @param name The name of the block used as an identifier.
 * @param basePoint Base point (offset) of the block.
 */
GV_Block::GV_Block(GV_EntityContainer* parentContainer,
                   const GV_BlockData& d)
        : GV_Entity(parentContainer), GV_Document(parentContainer), data(d) {
    RB_DEBUG->print("GV_Block::GV_Block");

    pen = GV_Pen(GV_Color(128,128,128), GV2::Width01, GV2::SolidLine);
    
    RB_DEBUG->print("GV_Block::GV_Block: OK");
}



GV_Block::~GV_Block() {
    RB_DEBUG->print("GV_Block::~GV_Block");
    RB_DEBUG->print("GV_Block::~GV_Block: OK");
}




GV_Entity* GV_Block::clone() {
    GV_Block* blk = new GV_Block(parentContainer, data);
    blk->copyPropertiesFrom(this);
    blk->entities = entities;
    blk->entities.setAutoDelete(entities.autoDelete());
    blk->detach();
    blk->initId();
    return blk;
}



GV_LayerList* GV_Block::getLayerList() {
    GV_Graphic* g = getGraphic();
    if (g!=NULL) {
        return g->getLayerList();
    } else {
        return NULL;
    }
}



GV_BlockList* GV_Block::getBlockList() {
    GV_Graphic* g = getGraphic();
    if (g!=NULL) {
        return g->getBlockList();
    } else {
        return NULL;
    }
}


bool GV_Block::save() {
    GV_Graphic* g = getGraphic();
    if (g!=NULL) {
        return g->save();
    } else {
        return false;
    }
}


bool GV_Block::saveAs(const QString& filename, GV2::FormatType type) {
    GV_Graphic* g = getGraphic();
    if (g!=NULL) {
        return g->saveAs(filename, type);
    } else {
        return false;
    }
}



/**
 * Sets the parent documents modified status to 'm'.
 */
void GV_Block::setModified(bool m) {
    GV_Graphic* p = getGraphic();
    if (p) {
        p->setModified(m);
    }
    modified = m;
}
