/****************************************************************************
** $Id: rs_document.cpp 7930 2008-01-17 21:23:04Z andrew $
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


#include "rs_document.h"


/**
 * Constructor.
 *
 * @param parentContainer Parent of the document. Often that's NULL but
 *        for blocks it's the blocklist.
 */
RS_Document::RS_Document(RS_EntityContainer* parentContainer)
        : RS_Entity(parentContainer), RS_EntityContainer(parentContainer), RS_Undo() {

    RS_DEBUG->print("RS_Document::RS_Document() ");

    filename = "";
    originalFilename = "";
    formatType = RS2::FormatUnknown;
    setModified(false);
    RS_Color col(RS2::FlagByLayer);
    activePen = RS_Pen(col, RS2::WidthByLayer, RS2::LineByLayer);
    
    RS_DEBUG->print("RS_Document::RS_Document(): OK");
}


RS_Document::~RS_Document() {
    RS_DEBUG->print("RS_Document::~RS_Document");
    RS_DEBUG->print("RS_Document::~RS_Document: OK");
}

    
/**
 * Sets the documents modified status to 'm'.
 */
void RS_Document::setModified(bool m) {
    modified = m;
    if (m) {
        modifiedSinceRedraw = true;
    }
}


/**
 * Sets the documents modified since redraw status to 'm'.
 */
void RS_Document::setModifiedSinceRedraw(bool m) {
    modifiedSinceRedraw = m;
}

/**
 * @retval true The document has been modified since it was last saved.
 * @retval false The document has not been modified since it was last saved.
 */
bool RS_Document::isModified() const {
    return modified;
}

/**
 * @retval true The document has been modified since it was last drawn.
 * @retval false The document has not been modified since it was last drawn.
 */
bool RS_Document::isModifiedSinceRedraw() const {
    return modifiedSinceRedraw;
}
