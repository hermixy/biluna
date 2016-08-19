/****************************************************************************
** $Id: rs_preview.h 8349 2008-01-21 16:15:34Z andrew $
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


#ifndef RS_PREVIEW_H
#define RS_PREVIEW_H

#include "rs_entitycontainer.h"

class RS_Entity;
class RS_Vector;
class RS_Pen;
class RS_InfoArea;

/**
 * This class supports previewing. The RS_Snapper class uses
 * an instance of RS_Preview to preview entities, ranges, 
 * lines, arcs, ... on the fly.
 *
 * @author Andrew Mustun
 */
class RS_Preview : public RS_EntityContainer {
public:
    RS_Preview(RS_EntityContainer* parent=NULL);
    ~RS_Preview();
    
    /** @return RS2::EntityContainerPreview */
    virtual RS2::EntityType rtti() const {
        return RS2::EntityPreview;
    }
    
    /**
     * @return Always NULL as a preview is not on any layer
     * even if the parent is.
     */
    virtual RS_Layer* getLayer(bool /*resolve*/ = true) const {
        return NULL;
    }
    
    void addPreviewEntity(RS_Entity* entity, 
        RS_EntityContainer* subContainer=NULL);
    void addRectangle(const RS_Vector& v1, const RS_Vector& v2,
        RS_EntityContainer* subContainer=NULL,
        bool forSelection = false);
    void addCloneOf(RS_Entity* entity);
    void addAllFrom(RS_EntityContainer& container, 
        RS_EntityContainer* subContainer = NULL);
    void addSelectionFrom(RS_EntityContainer& srcContainer, 
        RS_EntityContainer* subContainer = NULL);
    void addStretchablesFrom(RS_EntityContainer& container,
           const RS_Vector& v1, const RS_Vector& v2,
           RS_EntityContainer* subContainer = NULL);
    void addMeasuringLine(const RS_Vector& p1, const RS_Vector& p2, 
        RS_GraphicView& graphicView);
    void addMeasuringArc(const RS_Vector& center, 
        const RS_Vector& p1, const RS_Vector& p2,
        const RS_Vector& position,
        RS_GraphicView& graphicView);
    void addMeasuringArea(RS_InfoArea& area,
        RS_GraphicView& graphicView);
    void addMeasuringLabel(const RS_String& label, 
        const RS_Vector& position, double direction,
        RS_GraphicView& graphicView);

    static RS_Pen getPreviewPen() {
        return previewPen;
    }
    static RS_Pen getPreviewInfoPen() {
        return previewInfoPen;
    }

    /*
    void setOverridePen(const RS_Pen& p) {
        overridePen = p;
    }

    void disableOverridePen() {
        overridePen = RS_Pen(RS2::FlagInvalid);
    }
    */

    void moveSnapMarkerToFront() {
        moveEntityToFront(snapMarker);
    }

    void setSnapMarker(RS_Entity* sm) {
        snapMarker = sm;
        addEntity(snapMarker);
    }

private:
    int maxEntities;
    RS_Entity* snapMarker;
    static RS_Pen previewPen;
    static RS_Pen previewInfoPen;
    static RS_Pen previewLabelPen;
    //RS_Pen overridePen;
};

#endif
