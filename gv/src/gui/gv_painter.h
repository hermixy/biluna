/****************************************************************************
** $Id: rs_painter.h 8332 2008-01-20 13:04:08Z andrew $
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


#ifndef GV_PAINTER_H
#define GV_PAINTER_H

#include "gv_color.h"
#include "gv_img.h"
#include "gv_math.h"
#include "gv_pen.h"
#include "gv_pixmap.h"
#include "gv_pointarray.h"
#include "gv_vector.h"


/**
 * This class is a common interface for a painter class. Such 
 * a class will in it's implementation be responsible to paint
 * lines, arcs, ... in widgets. All angles in rad.
 *
 * Note that this is just an interface used as a slot to 
 * communicate with the qcadlib from a GUI level. This 
 * does not contain any Qt or platform specific code.
 */
class GV_Painter {
public:
    GV_Painter() {
        drawingMode = GV2::ModeFull;
        offset = GV_Vector(0.0,0.0);
        pdfOutput = false;
        //overridePen = false;
    }
    virtual ~GV_Painter() {}

    /**
     * Sets the drawing mode.
     */
    void setDrawingMode(GV2::DrawingMode m) {
        drawingMode = m;
    }

    /**
     * @return Current drawing mode.
     */
    GV2::DrawingMode getDrawingMode() {
        return drawingMode;
    }

    virtual void moveTo(double x, double y) = 0;
    virtual void lineTo(double x, double y) = 0;
    
    virtual void drawGridPoint(const GV_Vector& p) = 0;
    virtual void drawPoint(const GV_Vector& p) = 0;
    virtual void drawLine(const GV_Vector& p1, const GV_Vector& p2) = 0;
    virtual void drawRect(const GV_Vector& p1, const GV_Vector& p2);
    /*virtual void drawArc(const GV_Vector& cp, double radius,
                         double a1, double a2,
                         const GV_Vector& p1, const GV_Vector& p2,
                         bool reversed) = 0;*/
    virtual void drawArc(const GV_Vector& cp, double radius,
                         double a1, double a2,
                         bool reversed) = 0;
    void createArc(GV_PointArray& pa,
                   const GV_Vector& cp, double radius,
                   double a1, double a2,
                   bool reversed);
    void createEllipseArc(GV_PointArray& pa,
                   const GV_Vector& cp,
                   double radius1, double radius2,
                   double angle,
                   double a1, double a2,
                   bool reversed);
    virtual void drawCircle(const GV_Vector& cp, double radius) = 0;
    virtual void drawEllipse(const GV_Vector& cp,
                             double radius1, double radius2,
                             double angle,
                             double angle1, double angle2,
                             bool reversed) = 0;
    virtual void drawImg(GV_Img& img, const GV_Vector& pos,
            double angle, const GV_Vector& factor,
            int sx, int sy, int sw, int sh) = 0;

    virtual void drawTextH(int x1, int y1, int x2, int y2,
                           const QString& text) = 0;
    virtual void drawTextV(int x1, int y1, int x2, int y2,
                           const QString& text) = 0;

    virtual void fillRect(int x1, int y1, int w, int h,
                          const GV_Color& col) = 0;

    virtual void fillTriangle(const GV_Vector& p1,
                              const GV_Vector& p2,
                              const GV_Vector& p3) = 0;

    virtual void drawHandle(const GV_Vector& p, const GV_Color& c, int size=-1);

    virtual void setPreviewPen() = 0;
    virtual GV_Pen getPen() = 0;
    virtual void setPen(const GV_Pen& pen) = 0;
    virtual void setPen(const GV_Color& color) = 0;
    virtual void setPen(int r, int g, int b) = 0;
    virtual void setOpacity(double alpha) = 0;
    virtual void disablePen() = 0;
    /*
    virtual void setOverridePen(const GV_Pen& pen) {
        setPen(pen);
        setBrush(pen.getColor());
        overridePen = true;
    }
    virtual void disableOverridePen() {
        overridePen = false;
    }
    */
    virtual void setBrush(const GV_Color& color) = 0;
    virtual void setBrush(const QPixmap& pixmap) = 0;
    virtual void disableBrush() = 0;
    virtual void drawPolygon(const GV_PointArray& a) = 0;
    virtual void setBackground(const GV_Color& color) = 0;
    virtual void erase() = 0;
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;

    virtual void setXORMode() = 0;
    virtual void setNormalMode() = 0;

    void setPreviewMode() {
        drawingMode = GV2::ModeXor;
        //setXORMode();
        //setPreviewPen();
    }
    inline bool isPreviewMode() {
        return drawingMode==GV2::ModeXor;
    }

    virtual void setOffset(const GV_Vector& o) {
        offset = o;
    }

    virtual void setClipRect(int x, int y, int w, int h) = 0;
    virtual void resetClipping() = 0;
    inline int toScreenX(double x) {
        return GV_Math::mround(offset.x + x);
    }
    inline int toScreenY(double y) {
        return GV_Math::mround(offset.y + y);
    }

    void setPdfOutput(bool on) {
        pdfOutput = on;
    }

protected:
    /**
     * Current drawing mode.
     */
    GV2::DrawingMode drawingMode;
    /**
     * A fixed offset added to all entities drawn (useful for previews).
     */
    GV_Vector offset;
    /**
     * Set to true if the painter draws to PDF.
     */
    bool pdfOutput;
    //bool overridePen;
};

#endif
