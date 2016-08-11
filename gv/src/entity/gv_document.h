/****************************************************************************
** $Id: gv_document.h 7877 2008-01-14 17:44:08Z andrew $
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


#ifndef GV_DOCUMENT_H
#define GV_DOCUMENT_H

#include "gv_layerlist.h"
#include "gv_entitycontainer.h"
#include "gv_undo.h"
#include <QString>

class GV_BlockList;

/**
 * Base class for documents. Documents can be either graphics or 
 * blocks and are typically shown in graphic views. Documents hold
 * an active pen for drawing in the Document, a file name and they
 * know whether they have been modified or not.
 *
 * @author Andrew Mustun
 */
class GV_Document : public GV_EntityContainer, public GV_Undo {

public:
    GV_Document(GV_EntityContainer* parentContainer=NULL);
    virtual ~GV_Document();
    
    /** @return GV2::EntityDocument */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityDocument;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityDocument || GV_EntityContainer::isOfType(t));
    }

    virtual GV_LayerList* getLayerList() = 0;
    virtual GV_BlockList* getBlockList() = 0;

    virtual void newDoc() = 0;
    virtual bool save() = 0;
    virtual bool saveAs(const QString& filename, GV2::FormatType type) = 0;
    virtual bool open(const QString& filename, GV2::FormatType type, bool clean=true) = 0;
    

    /**
     * @return true for all document entities (e.g. Graphics or Blocks).
     */
    virtual bool isDocument() const {
        return true;
    }

    /**
     * Removes an entity from the entiy container. Implementation
     * from GV_Undo.
     */
    virtual void removeUndoable(GV_Undoable* u) {
        if (u!=NULL && u->undoRtti()==GV2::UndoableEntity) {
            removeEntity((GV_Entity*)u);
        }
    }

    /**
     * @return Currently active drawing pen.
     */
    GV_Pen getActivePen() const {
        return activePen;
    }

    /**
     * Sets the currently active drawing pen to p.
     */
    void setActivePen(GV_Pen p) {
        activePen = p;
    }

    /**
     * @return File name of the document currently loaded.
     * Note, that the default file name is empty.
     */
    QString getFilename() const {
        return filename;
    }
    
    /**
     * Sets file name for the document currently loaded.
     */
    void setFilename(const QString& fn) {
        filename = fn;
    }
    
    /**
     * @return File name of the original document.
     * Note, that the default file name is empty.
     */
    QString getOriginalFilename() const {
        return originalFilename;
    }
    
    /**
     * Sets file name for the document currently loaded.
     */
    void setOriginalFilename(const QString& fn) {
        originalFilename = fn;
    }
    
    /**
     * Sets format type for the document currently loaded.
     */
    void setFormatType(GV2::FormatType ft) {
        formatType = ft;
    }

    /**
     * @return The format type of the current drawing.
     */
    GV2::FormatType getFormatType() {
        return formatType;
    }

    virtual void setModified(bool m);
    virtual bool isModified() const;
    virtual bool isModifiedSinceRedraw() const;
    virtual void setModifiedSinceRedraw(bool m);
    
    /**
     * Overwritten to set modified flag before starting an undo cycle.
     */
    virtual void startUndoCycle() {
        setModified(true);
        GV_Undo::startUndoCycle();
    }
    
    /**
     * Overwritten to set modified flag when undo is triggered.
     */
    virtual void undo() {
        setModified(true);
        GV_Undo::undo();
    }
    
    /**
     * Overwritten to set modified flag when redo is triggered.
     */
    virtual void redo() {
        setModified(true);
        GV_Undo::redo();
    }

protected:
    /** Flag set if the document was modified and not yet saved. */
    bool modified;
    bool modifiedSinceRedraw;
    /** Active pen. */
    GV_Pen activePen;
    /** File name of the document or empty for a new document. */
    QString filename;
    /** File name of the document that was originally loaded. */
    QString originalFilename;
    /** Format type */
    GV2::FormatType formatType;
};


#endif
