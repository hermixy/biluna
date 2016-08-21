/****************************************************************************
** $Id: rs_infoarea.h 4008 2006-12-27 19:05:42Z andrew $
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

#ifndef RS_INFOAREA_H
#define RS_INFOAREA_H

#include "rs_vector.h"
#include "rs_valuevector.h"



/**
 * Class for getting information about an area. 
 *
 * @author Guenter Szolderits
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_InfoArea {
public: 
    RS_InfoArea();
    ~RS_InfoArea();

    void reset();
    void addPoint(const RS_Vector& p);
    //void setLastPoint(const RS_Vector& p);
    void calculate();
    void close();
    bool isValid();
    bool isClosed();
    double getArea() const { 
        return area; 
    }
    double getCircumference() const { 
        return circumference; 
    }
    int count() const { 
        return thePoints.count(); 
    }
    RS_Vector at(int i) const {
        if (i>=0 && i<thePoints.count()) {
            return thePoints.at(i);
        }
        else {
            return RS_Vector(false);
        }
    }

private:
    double calcSubArea(const RS_Vector& p1, const RS_Vector& p2);

    RS_ValueVector<RS_Vector> thePoints;
    double baseY;
    double area;
    double circumference;
};

#endif
