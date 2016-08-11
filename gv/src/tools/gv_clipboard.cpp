/****************************************************************************
** $Id: gv_clipboard.cpp 5737 2007-06-29 12:19:49Z andrew $
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


#include "gv_clipboard.h"
#include "gv_block.h"
#include "gv_layer.h"
#include "gv_entity.h"

GV_Clipboard* GV_Clipboard::uniqueInstance = NULL;



void GV_Clipboard::clear() {
    graphic.clear();
    graphic.clearBlocks();
    graphic.clearLayers();
    graphic.clearVariables();
}





void GV_Clipboard::addBlock(GV_Block* b) {
    if (b!=NULL) {
        graphic.addBlock(b, false);
    }
}


bool GV_Clipboard::hasBlock(const QString& name) {
    return (graphic.findBlock(name)!=NULL);
}


void GV_Clipboard::addLayer(GV_Layer* l) {
    if (l!=NULL) {
        graphic.addLayer(l);
    }
}



bool GV_Clipboard::hasLayer(const QString& name) {
    return (graphic.findLayer(name)!=NULL);
}



void GV_Clipboard::addEntity(GV_Entity* e) {
    if (e!=NULL) {
        // 2.1.3.6: keep entities in clipboard selected for standard
        //   transformations / property modifications
        e->setSelected(true, true);
        e->setUpdateEnabled(false);
        graphic.addEntity(e);
        e->reparent(&graphic);
    }
}

