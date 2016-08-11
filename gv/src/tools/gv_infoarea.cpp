/****************************************************************************
** $Id: gv_infoarea.cpp 4008 2006-12-27 19:05:42Z andrew $
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

#include "gv_infoarea.h"


#include "gv_infoarea.h"
#include "gv_math.h"
#include "rb_debug.h"


/**
 * Constructor.
 */
GV_InfoArea::GV_InfoArea() {
}



/**
 * Destructor.
 */
GV_InfoArea::~GV_InfoArea() {}



/** 
 * Adds a point to the internal list
 *
 * @param p coordinate of the point
 */
void GV_InfoArea::addPoint(const GV_Vector& p) {
    if (thePoints.empty()) {
        baseY = p.y;
    }
    
    thePoints.append(p);
}



/**
 * Changes the last point of the area. Typically used by
 * dynamic previews of areas.
 */
/*void GV_InfoArea::setLastPoint(const GV_Vector& p) {
    if (thePoints.count()>0) {
        thePoints[thePoints.count()-1] = p;
    }
}*/


/**
 * Resets the points.
 */
void GV_InfoArea::reset() {
    thePoints.clear();
    area = 0.0;
    circumference = 0.0;
}



/**
 * Closes the polygon if it is not closed already.
 */
void GV_InfoArea::close() {
    if (isValid() && isClosed()==false) {
        
        thePoints.append(thePoints.first());

        RB_DEBUG->print("GV_InfoArea::close: closed");
    }
}



/**
 * @retval true If the area is closed (i.e. start point and end point are
 *   identical)
 * @retval false Otherwise.
 */
bool GV_InfoArea::isClosed() {
    return (thePoints.first().distanceTo(thePoints.last())<1.0e-4);
}



/**
 * @retval true If the area is defined (i.e. there are at least 3 points)
 * @retval false If there are only two or less points.
 */
bool GV_InfoArea::isValid() {
//    RB_DEBUG->print("GV_InfoArea::isValid: count: %d", thePoints.count());
    return (thePoints.count()>2);
}



/**
 * Calculates the area and the circumference of the area.
 */
void GV_InfoArea::calculate() {
    area = 0.0;
    circumference = 0.0;
    GV_Vector ptFirst; 
    GV_Vector p1;
    GV_Vector p2;
    int i = thePoints.count()-1;

    // at least 3 points needed for an area
    if (isValid()) {
        ptFirst = thePoints.at(i);
        i--;
        //thePoints.pop_back();
        
        p1 = ptFirst;
        //while (!thePoints.empty()) {
        while (i>=0) {
            //p2 = thePoints.last();
            p2 = thePoints.at(i);
            //thePoints.pop_back();
            i--;
            
            area += calcSubArea(p1, p2);
            circumference += p1.distanceTo(p2);
            p1 = p2;
        }
        area += calcSubArea(p1, ptFirst);
        circumference += p1.distanceTo(ptFirst);
    }

    area = fabs(area);

//    RB_DEBUG->print("GV_InfoArea::calculate: area: %f", area);
//    RB_DEBUG->print("GV_InfoArea::calculate: circumference: %f", circumference);
}



/**
 * Calculates a sub area.
 * 
 * @param p1 first point
 * @param p2 second point
 */
double GV_InfoArea::calcSubArea(const GV_Vector& p1, const GV_Vector& p2) {
    double width = p2.x - p1.x;
    double height = (p1.y - baseY) + (p2.y - baseY);

    return width * height / 2.0;
}


