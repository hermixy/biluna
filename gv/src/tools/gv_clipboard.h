/****************************************************************************
** $Id: gv_clipboard.h 5722 2007-06-27 23:00:45Z andrew $
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


#ifndef GV_CLIPBOARD_H
#define GV_CLIPBOARD_H

#include <iostream>
#include "gv_graphic.h"

#define GV_CLIPBOARD GV_Clipboard::instance()

class GV_Block;
class GV_Layer;
class GV_Entity;

/**
 * QCad internal clipboard. We don't use the system clipboard for
 * better portaility.
 * Implemented as singleton.
 *
 * @author Andrew Mustun
 */
class GV_Clipboard {
protected:
    GV_Clipboard() {
        graphic.setUndoEnabled(false);
    }

public:
    /**
     * @return Instance to the unique clipboard object.
     */
    static GV_Clipboard* instance() {
        if (uniqueInstance==NULL) {
            uniqueInstance = new GV_Clipboard();
        }
        return uniqueInstance;
    }

    void clear();

    void addBlock(GV_Block* b);
    bool hasBlock(const QString& name);
    int  countBlocks() {
        return graphic.countBlocks();
    }
    GV_Block* blockAt(int i) {
        return graphic.blockAt(i);
    }
    
    void addLayer(GV_Layer* l);
    bool hasLayer(const QString& name);
    int  countLayers() {
        return graphic.countLayers();
    }
    GV_Layer* layerAt(int i) {
        return graphic.layerAt(i);
    }

    void addEntity(GV_Entity* e);

    uint count() {
        return graphic.count();
    }
    GV_Entity* entityAt(uint i) {
        return graphic.entityAt(i);
    }
    GV_Entity* firstEntity() {
        return graphic.firstEntity();
    }
    
    GV_Entity* nextEntity() {
        return graphic.nextEntity();
    }

    GV_Graphic* getGraphic() {
        return &graphic;
    }

protected:
    static GV_Clipboard* uniqueInstance;

    GV_Graphic graphic;
};

#endif

