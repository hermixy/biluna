/****************************************************************************
** $Id: rs_painterqt.h 7877 2008-01-14 17:44:08Z andrew $
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


#ifndef RS_PAINTERQT_H
#define RS_PAINTERQT_H

#include <QPainter>

#include "rs_color.h"
#include "rs_graphicview.h"
#include "rs_painter.h"

/**
 * The Qt implementation of a painter. It can draw objects such as
 * lines or arcs in a widget. All coordinates are screen coordinates
 * and have nothing to do with the graphic view.
 */
class CAD_EXPORT RS_PainterQt: public QPainter, public RS_Painter {

public:
    RS_PainterQt(QPaintDevice* pd);
    RS_PainterQt();
    virtual ~RS_PainterQt();

    virtual void moveTo(double x, double y);
    virtual void lineTo(double x, double y);
    virtual void drawGridPoint(const RS_Vector& p);
    virtual void drawPoint(const RS_Vector& p);
    virtual void drawLine(const RS_Vector& p1, const RS_Vector& p2);
    //virtual void drawRect(const RS_Vector& p1, const RS_Vector& p2);
    /*
    virtual void drawArc(const RS_Vector& cp, double radius,
                         double a1, double a2,
                         const RS_Vector& p1, const RS_Vector& p2,
                         bool reversed);
    */
    
    virtual void drawArc(const RS_Vector& cp, double radius,
                         double a1, double a2,
                         bool reversed);
    /*
    virtual void drawArcMac(const RS_Vector& cp, double radius,
                         double a1, double a2,
                         bool reversed);
    */
    virtual void drawCircle(const RS_Vector&, double radius);
    virtual void drawEllipse(const RS_Vector& cp,
                             double radius1, double radius2,
                             double angle,
                             double a1, double a2,
                             bool reversed);
    virtual void drawImg(RS_Img& img, const RS_Vector& pos, 
            double angle, const RS_Vector& factor,
            int sx, int sy, int sw, int sh);
    virtual void drawTextH(int x1, int y1, int x2, int y2,
                           const QString& text);
    virtual void drawTextV(int x1, int y1, int x2, int y2,
                           const QString& text);

    virtual void fillRect(int x1, int y1, int w, int h,
                          const RS_Color& col);

    virtual void fillTriangle(const RS_Vector& p1,
                              const RS_Vector& p2,
                              const RS_Vector& p3);

    virtual void drawPolygon(const RS_PointArray& a);
    virtual void setBackground(const RS_Color& color);
    virtual void erase();
    virtual int getWidth();
    virtual int getHeight();

    virtual void setPreviewPen();
    virtual RS_Pen getPen();
    virtual void setPen(const RS_Pen& pen);
    virtual void setPen(const RS_Color& color);
    virtual void setPen(int r, int g, int b);
    virtual void setOpacity(double alpha);
    virtual void disablePen();
    virtual void setBrush(const RS_Color& color);
    virtual void setBrush(const RS_Pixmap& pixmap);
    virtual void disableBrush();

    virtual void setXORMode();
    virtual void setNormalMode();

    virtual void setClipRect(int x, int y, int w, int h);
    virtual void resetClipping();

protected:
    RS_Pen lpen;
    double posX;
    double posY;
};

#endif

