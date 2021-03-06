/****************************************************************************
** $Id: rs_painter.cpp 8332 2008-01-20 13:04:08Z andrew $
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


#include "gv_painter.h"


void GV_Painter::createArc(GV_PointArray& pa,
                             const GV_Vector& cp, double radius,
                             double a1, double a2,
                             bool reversed) {

    if (radius<1.0e-6) {
//        RB_DEBUG->print(RB_Debug::D_WARNING,
//            "GV_Painter::createArc: invalid radius: %f", radius);
        return;
    }

    double cix;           // Next point on circle
    double ciy;           //
    double aStep;         // Angle Step (rad)
    double a;             // Current Angle (rad)


    if (pdfOutput) {
        if(fabs(0.1/radius)<=1.0) {
            aStep=asin(0.1/radius);
        } else {
            aStep=0.1;
        }
    }
    else {
        if(fabs(2.0/radius)<=1.0) {
            aStep=asin(2.0/radius);
        } else {
            aStep=1.0;
        }
    }

    pa.moveTo(offset.x + cp.x+cos(a1)*radius,
              offset.y + cp.y-sin(a1)*radius);
    if(!reversed) {
        // Arc Counterclockwise:
        if(a1>a2-1.0e-10) {
            a2+=2*M_PI;
        }
        for(a=a1+aStep; a<=a2; a+=aStep) {
            cix = offset.x + cp.x+cos(a)*radius;
            ciy = offset.y + cp.y-sin(a)*radius;
            pa.lineTo(cix, ciy);
        }
    } else {
        // Arc Clockwise:
        if(a1<a2+1.0e-10) {
            a2-=2*M_PI;
        }
        for(a=a1-aStep; a>=a2; a-=aStep) {
            cix = offset.x + cp.x+cos(a)*radius;
            ciy = offset.y + cp.y-sin(a)*radius;
            pa.lineTo(cix, ciy);
        }
    }
    pa.lineTo(offset.x + cp.x+cos(a2)*radius,
              offset.y + cp.y-sin(a2)*radius);
}
    
    
    
void GV_Painter::createEllipseArc(GV_PointArray& pa,
                   const GV_Vector& cp,
                   double radius1, double radius2,
                   double angle,
                   double a1, double a2,
                   bool reversed) {

    if (radius1<1.0e-6 || radius2<1.0e-6) {
//        RB_DEBUG->print(RB_Debug::D_WARNING,
//            "GV_Painter::createArc: invalid radius: %f or %f", radius1, radius2);
        return;
    }

    //double cix;           // Next point on ellipse
    //double ciy;           //
    double aStep;         // Angle Step (rad)
    double a;             // Current Angle (rad)
    
    aStep=0.01;

    /*
    if (pdfOutput) {
        if(fabs(0.1/radius1)<=1.0) {
            aStep=asin(0.1/radius1);
        } else {
            aStep=0.1;
        }
    }
    else {
        if(fabs(2.0/radius)<=1.0) {
            aStep=asin(2.0/radius);
        } else {
            aStep=1.0;
        }
    }
    */

    GV_Vector vp;
    GV_Vector vc(cp.x, cp.y);
    vp.set(cp.x+cos(a1)*radius1,
           cp.y-sin(a1)*radius2);
    vp.rotate(vc, -angle);
    pa.moveTo(offset.x + vp.x,
           offset.y + vp.y);
    if(!reversed) {
        // Arc Counterclockwise:
        if(a1>a2-GV_TOLERANCE) {
            a2+=2*M_PI;
        }
        for(a=a1+aStep; a<=a2; a+=aStep) {
            vp.set(cp.x+cos(a)*radius1,
                   cp.y-sin(a)*radius2);
            vp.rotate(vc, -angle);
            pa.lineTo(offset.x + vp.x,
                      offset.y + vp.y);
        }
    } else {
        // Arc Clockwise:
        if(a1<a2+GV_TOLERANCE) {
            a2-=2*M_PI;
        }
        for(a=a1-aStep; a>=a2; a-=aStep) {
            vp.set(cp.x+cos(a)*radius1,
                   cp.y-sin(a)*radius2);
            vp.rotate(vc, -angle);
            pa.lineTo(offset.x + vp.x,
                   offset.y + vp.y);
        }
    }
    vp.set(cp.x+cos(a2)*radius1,
           cp.y-sin(a2)*radius2);
    vp.rotate(vc, -angle);
    pa.lineTo(offset.x + vp.x,
           offset.y + vp.y);

}



void GV_Painter::drawRect(const GV_Vector& p1, const GV_Vector& p2) {
    drawLine(GV_Vector(p1.x, p1.y), GV_Vector(p2.x, p1.y));
    drawLine(GV_Vector(p2.x, p1.y), GV_Vector(p2.x, p2.y));
    drawLine(GV_Vector(p2.x, p2.y), GV_Vector(p1.x, p2.y));
    drawLine(GV_Vector(p1.x, p2.y), GV_Vector(p1.x, p1.y));
}


void GV_Painter::drawHandle(const GV_Vector& p, const GV_Color& c, int size) {
    RB_DEBUG->print("GV_Painter::drawHandle");

    if (size<0) {
        size = 6;
    }

    fillRect((int)(p.x-size+1), (int)(p.y-size+1), 
        2*size-2, 2*size-2, 
        c);
    /*
    drawRect(p-GV_Vector(size/2,size/2), p+GV_Vector(size/2,size/2));
    setPen(GV_Color(0,0,0));
    drawPoint(p);
    */
}


