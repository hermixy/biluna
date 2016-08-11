/****************************************************************************
** $Id: gv_information.h 7816 2008-01-04 16:57:10Z andrew $
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

#ifndef GV_INFORMATION_H
#define GV_INFORMATION_H

#include "gv_entitycontainer.h"
#include "gv_line.h"
#include "gv_arc.h"



/**
 * Class for getting information about entities. This includes
 * also things like the end point of an element which is 
 * nearest to a given coordinate.
 * There's no interaction handled in this class.
 * This class is bound to an entity container.
 *
 * @author Andrew Mustun
 */
class GV_Information {
public:
    GV_Information(GV_EntityContainer& entityContainer);

    //static bool isDimension(GV2::EntityType type);
    static bool isTrimmable(GV_Entity* e);
    static bool isTrimmable(GV_Entity* e1, GV_Entity* e2);

    GV_Vector getNearestEndpoint(const GV_Vector& point,
                                 double* dist = NULL) const;
    GV_Vector getNearestPointOnEntity(const GV_Vector& point,
                                      bool onEntity=true, 
                                      double* dist = NULL,
                                      GV_Entity** entity=NULL) const;
    GV_Entity* getNearestEntity(const GV_Vector& point,
                                double* dist = NULL,
                                GV2::ResolveLevel level=GV2::ResolveAll) const;

    static bool hasIntersection(GV_Entity* e1,
            GV_Entity* e2,
            bool onEntities = false) {
        return getIntersection(e1, e2, onEntities).hasValid();
    }
    static GV_VectorList getIntersection(GV_Entity* e1,
            GV_Entity* e2,
            bool onEntities = false);

    static GV_VectorList getIntersectionLineLine(GV_Line* e1,
            GV_Line* e2);

    static GV_VectorList getIntersectionLineArc(GV_Line* line,
            GV_Arc* arc);

    static GV_VectorList getIntersectionArcArc(GV_Arc* e1,
            GV_Arc* e2);
    
    static GV_VectorList getIntersectionLineEllipse(GV_Line* line,
            GV_Ellipse* ellipse);

    static bool isPointInsideContour(const GV_Vector& point,
                                     GV_EntityContainer* contour,
                                     bool* onContour=NULL);

    static bool isOverlapping(const GV_Vector& r1v1, const GV_Vector& r1v2,
        const GV_Vector& r2v1, const GV_Vector& r2v2);
    
protected:
    GV_EntityContainer* container;
};



#endif
