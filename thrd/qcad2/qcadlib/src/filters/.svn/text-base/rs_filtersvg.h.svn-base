/****************************************************************************
**
** Copyright (C) 2007 RibbonSoft. All rights reserved.
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


#ifndef RS_FILTERSVG_H
#define RS_FILTERSVG_H

#include "rs_filterinterface.h"

class RS_Text;
class RS_Hatch;
class RS_StaticGraphicView;
class SvgPainter;

/**
 * This format filter class can import and export SVG files.
 *
 * @author Andrew Mustun
 */
class RS_FilterSvg : public RS_FilterInterface {
public:
    RS_FilterSvg();
    ~RS_FilterSvg();

    virtual RS_String getDescription(RS2::FormatType t) {
        switch (t) {
        case RS2::FormatSvg:
            return "Scalable Vector Graphics (SVG)";
        default:
            return "";
        }
    }
    
    virtual RS_String getDefaultExtension(RS2::FormatType t) {
        switch (t) {
        case RS2::FormatSvg:
            return "svg";
        default:
            return "";
        }
    }

    virtual RS_String getFilterString(RS2::FormatType t, bool /*openMode*/=true) {
        switch (t) {
        case RS2::FormatSvg:
            return QObject::tr("Scalable Vector Graphics %1").arg("(*.svg *.SVG)");
        default:
            return "";
        }
    }
    
    // Import:
    virtual bool fileImport(RS_Graphic& /*g*/, const RS_String& /*file*/, RS2::FormatType /*formatType*/) {
        return false;
    }

    // Export:
    virtual bool fileExport(RS_Graphic& g, const RS_String& file, RS2::FormatType formatType);

    void exportContainer(RS_EntityContainer& c, SvgPainter& painter, RS_StaticGraphicView& gv);

    void setSize(int w, int h) {
        width = w;
        height = h;
    }

    void setLineFactor(double lf) {
        lineFactor = lf;
    }

    void setNoPoints(bool on) {
        noPoints = on;
    }
    void setTtfText(bool on) {
        ttfText = on;
    }

protected:
    int width;
    int height;
    double lineFactor;
    bool noPoints;
    bool ttfText;
#ifdef RS_DEMO
    bool exportingDemoText;
#endif
};



#include "rs_painteradapter.h"

/**
 * The SVG implementation of a painter. This simply streams entities
 * to a file rather than actually painting them. It's an abuse, but
 * a very effective one.
 */
class SvgPainter: public RS_PainterAdapter {
public:
    SvgPainter(const char* svg, int w, int h, double lf, bool np);
    virtual ~SvgPainter();
    virtual void moveTo(double, double);
    virtual void lineTo(double, double);
    virtual void drawPoint(const RS_Vector& p);
    virtual void drawLine(const RS_Vector& p1, const RS_Vector& p2);
    virtual void drawCircle(const RS_Vector& cp, double radius);
    virtual void drawArc(const RS_Vector& cp, double radius,
                         double a1, double a2,
                         const RS_Vector& p1, const RS_Vector& p2,
                         bool reversed);
    virtual void drawArc(const RS_Vector& cp, double radius,
                         double a1, double a2,
                         bool reversed);
    virtual void exportArcData(const RS_Arc& arc);
    RS_Arc getArc(const RS_Vector& cp, double radius,
                         double a1, double a2,
                         bool reversed);
    virtual void drawEllipse(const RS_Vector& cp,
                             double radius1, double radius2,
                             double angle,
                             double a1, double a2,
                             bool reversed);
    virtual void fillTriangle(const RS_Vector& p1,
                              const RS_Vector& p2,
                              const RS_Vector& p3);
    void drawText(RS_Text* text, RS_StaticGraphicView& gv);
    void drawPolyline(RS_Polyline* pl, RS_StaticGraphicView& gv);
    void drawSolidHatch(RS_Hatch* h, RS_StaticGraphicView& gv);
    virtual RS_Pen getPen();
    virtual void setPen(const RS_Pen& pen);
    virtual void setPen(const RS_Color& color);
    virtual void setPen(int r, int g, int b);
    virtual void setOpacity(double /*alpha*/) {};
    double getStrokeWidth() {
        if (pen.getWidth()==0) {
            return 0.1 * lineFactor;
        }
        else {
            return (double)((int)(pen.getWidth()))/100.0 * lineFactor;
        }
    }
    int getColR() {
        return pen.getColor().red();
    }
    int getColG() {
        return pen.getColor().green();
    }
    int getColB() {
        return pen.getColor().blue();
    }

    void rawOutput(const RS_String& s);

private:
    FILE* svg;
    RS_Pen pen;
    void startSvg();
    void endSvg();
    int width;
    int height;
    double lineFactor;
    bool noPoints;
	RS_Vector cursor;
};

#endif
