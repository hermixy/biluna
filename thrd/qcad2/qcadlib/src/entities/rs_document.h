/****************************************************************************
** $Id: rs_document.h 7877 2008-01-14 17:44:08Z andrew $
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


#ifndef RS_DOCUMENT_H
#define RS_DOCUMENT_H

#include "rs_layerlist.h"
#include "rs_entitycontainer.h"
#include "rs_undo.h"
#include "rs_string.h"

class CAD_EXPORT RS_BlockList;

/**
 * Base class for documents. Documents can be either graphics or 
 * blocks and are typically shown in graphic views. Documents hold
 * an active pen for drawing in the Document, a file name and they
 * know whether they have been modified or not.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_Document : public RS_EntityContainer, public RS_Undo {

public:
    RS_Document(RS_EntityContainer* parentContainer=NULL);
    virtual ~RS_Document();
    
    /** @return RS2::EntityDocument */
    virtual RS2::EntityType rtti() const {
        return RS2::EntityDocument;
    }
    virtual bool isOfType(RS2::EntityType t) const {
        return (t==RS2::EntityDocument || RS_EntityContainer::isOfType(t));
    }

    virtual RS_LayerList* getLayerList() = 0;
    virtual RS_BlockList* getBlockList() = 0;

    virtual void newDoc() = 0;
    virtual bool save() = 0;
    virtual bool saveAs(const RS_String& filename, RS2::FormatType type) = 0;
    virtual bool open(const RS_String& filename, RS2::FormatType type, bool clean=true) = 0;
    

    /**
     * @return true for all document entities (e.g. Graphics or Blocks).
     */
    virtual bool isDocument() const {
        return true;
    }

    /**
     * Removes an entity from the entiy container. Implementation
     * from RS_Undo.
     */
    virtual void removeUndoable(RS_Undoable* u) {
        if (u!=NULL && u->undoRtti()==RS2::UndoableEntity) {
            removeEntity((RS_Entity*)u);
        }
    }

    /**
     * @return Currently active drawing pen.
     */
    RS_Pen getActivePen() const {
        return activePen;
    }

    /**
     * Sets the currently active drawing pen to p.
     */
    void setActivePen(RS_Pen p) {
        activePen = p;
    }

    /**
     * @return File name of the document currently loaded.
     * Note, that the default file name is empty.
     */
    RS_String getFilename() const {
        return filename;
    }
    
    /**
     * Sets file name for the document currently loaded.
     */
    void setFilename(const RS_String& fn) {
        filename = fn;
    }
    
    /**
     * @return File name of the original document.
     * Note, that the default file name is empty.
     */
    RS_String getOriginalFilename() const {
        return originalFilename;
    }
    
    /**
     * Sets file name for the document currently loaded.
     */
    void setOriginalFilename(const RS_String& fn) {
        originalFilename = fn;
    }
    
    /**
     * Sets format type for the document currently loaded.
     */
    void setFormatType(RS2::FormatType ft) {
        formatType = ft;
    }

    /**
     * @return The format type of the current drawing.
     */
    RS2::FormatType getFormatType() {
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
        RS_Undo::startUndoCycle();
    }
    
    /**
     * Overwritten to set modified flag when undo is triggered.
     */
    virtual void undo() {
        setModified(true);
        RS_Undo::undo();
    }
    
    /**
     * Overwritten to set modified flag when redo is triggered.
     */
    virtual void redo() {
        setModified(true);
        RS_Undo::redo();
    }

protected:
    /** Flag set if the document was modified and not yet saved. */
    bool modified;
    bool modifiedSinceRedraw;
    /** Active pen. */
    RS_Pen activePen;
    /** File name of the document or empty for a new document. */
    RS_String filename;
    /** File name of the document that was originally loaded. */
    RS_String originalFilename;
    /** Format type */
    RS2::FormatType formatType;
};


#endif
