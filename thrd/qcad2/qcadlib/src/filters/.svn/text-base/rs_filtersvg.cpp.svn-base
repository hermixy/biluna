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


#include "rs_filtersvg.h"

#include "rs_hatch.h"
#include "rs_polyline.h"
#include "rs_staticgraphicview.h"
#include "rs_text.h"



/**
 * Default constructor.
 *
 */
RS_FilterSvg::RS_FilterSvg()
    : RS_FilterInterface() {

    RS_DEBUG->print("RS_FilterSvg::RS_FilterSvg()");

    addExportFormat(RS2::FormatSvg);
    width = 0;
    height = 0;
    lineFactor = 1.0;
    noPoints = false;
    ttfText = false;
#ifdef RS_DEMO
    exportingDemoText = false;
#endif

    RS_DEBUG->print("RS_FilterSvg::RS_FilterSvg(): OK");
}



/**
 * Destructor.
 */
RS_FilterSvg::~RS_FilterSvg() {
    RS_DEBUG->print("RS_FilterSvg::~RS_FilterSvg()");
    RS_DEBUG->print("RS_FilterSvg::~RS_FilterSvg(): OK");
}



/**
 * Implementation of the method used for RS_Export to communicate
 * with this filter.
 *
 * @param file Full path to the DXF file that will be written.
 */
bool RS_FilterSvg::fileExport(RS_Graphic& g, const RS_String& file, RS2::FormatType formatType) {

    //RS_DEBUG->print("RS_FilterSvg::fileExport: exporting file '%s'...",
    //                (const char*)RS_File::encodeName(file));
    RS_DEBUG->print("RS_FilterSvg::fileExport: file type '%d'", (int)formatType);

    g.calculateBorders(true);
    //int width = (int)ceil(g.getSize().x) + 2;
    //int height = (int)ceil(g.getSize().y) + 2;
    if (width==0) {
        width = (int)ceil(g.getSize().x) + 2;
    }
    if (height==0) {
        height = (int)ceil(g.getSize().y) + 2;
    }
    
    // the graphic view takes ownership of the painter:
    SvgPainter* painter = new SvgPainter((const char*)file.toLatin1(), width, height, lineFactor, noPoints);

    RS_StaticGraphicView gv(width, height, painter);
    gv.setPrinting(true);
    gv.setContainer(&g);
    gv.setBorders(1, 1, 1, 1);
    gv.zoomAuto(false);

    exportContainer(g, *painter, gv);

#ifdef RS_DEMO
    RS_String s = RS_String(
        "\nThis SVG export might be incomplete because \n"
        "it was produced with a Demo version that is not \n"
        "intended for production use. Full versions are\n"
        "available from http://www.ribbonsoft.com for\n"
        "a small license fee.\n"
        "With your purchase you are supporting the\n"
        "development of future versions of QCad.");
    RS_Text t(
        NULL, 
        RS_TextData(
            RS_Vector(0, height-height/25.0),
            height/50.0,
            100.0,
            RS2::VAlignTop,
            RS2::HAlignLeft,
            RS2::LeftToRight,
            RS2::Exact,
            1.0,
            s,
            "standard",
            0.0
        )
    );
    t.update();
    exportingDemoText = true;
    exportContainer(t, *painter, gv);
    exportingDemoText = false;
#endif

    return true;
}



/**
 * Helper function. Exports the given container by 'drawing' it with the
 * given painter object.
 */
void RS_FilterSvg::exportContainer(RS_EntityContainer& c, SvgPainter& painter, RS_StaticGraphicView& gv) {
#ifdef RS_DEMO
    int counter=0;
#endif

    for (RS_Entity* e=c.firstEntity(); e!=NULL; e=c.nextEntity()) {
        if (!e->isVisible() || (c.rtti()==RS2::EntityHatch && !e->isTemp())) {
            continue;
        }

#ifdef RS_DEMO
        if (!exportingDemoText && counter++>32) {
            return;
        }
#endif

        RS_Pen pen = e->getPen();
        if (pen.getColor()==RS_Color(255, 255, 255)) {
            pen.setColor(RS_Color(0, 0, 0));
        }
        painter.setPen(pen);

        if (ttfText && e->rtti()==RS2::EntityText) {
            RS_Text* text = dynamic_cast<RS_Text*>(e);
            painter.drawText(text, gv);
        }
        else if (e->rtti()==RS2::EntityPolyline) {
            painter.drawPolyline(dynamic_cast<RS_Polyline*>(e), gv);
        }
        else if (e->rtti()==RS2::EntityHatch && dynamic_cast<RS_Hatch*>(e)->isSolid()) {
            painter.drawSolidHatch(dynamic_cast<RS_Hatch*>(e), gv);
        }
        else if (e->isContainer()) {
            exportContainer(*dynamic_cast<RS_EntityContainer*>(e), painter, gv);
        }
        else {
            gv.drawEntity(e);
        }
    }
}













/**
 * Constructor.
 */
SvgPainter::SvgPainter(const char* svg, int w, int h, double lf, bool np) 
    : RS_PainterAdapter() {
    
    RS_DEBUG->print("SvgPainter::SvgPainter");
    
    this->svg = fopen(svg, "w");
    width=w;
    height=h;
    lineFactor=lf;
    noPoints = np;

    startSvg();
    
    if (this->svg==NULL) {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "Cannot open output file");
    }
    
    RS_DEBUG->print("SvgPainter::SvgPainter: OK");
}


/**
 * Destructor
 */
SvgPainter::~SvgPainter() {
    if (svg==NULL) {
        return;
    }
    endSvg();
    fclose(svg);
}


void SvgPainter::moveTo(double x, double y) {
	cursor = RS_Vector(x, y);
}


void SvgPainter::lineTo(double x, double y) {
	drawLine(cursor, RS_Vector(x, y));
}


/**
 * Draws a point at (x,y).
 */
void SvgPainter::drawPoint(const RS_Vector& p) {
    if (!noPoints) {
        drawCircle(p, 0.01);
    }
}


/**
 * Draws a line from (x1, y1) to (x2, y2).
 */
void SvgPainter::drawLine(const RS_Vector& p1, const RS_Vector& p2) {
    if (svg==NULL) {
        return;
    }
        
    rawOutput(
        RS_String("  <path d=\"M%1,%2 L%3,%4\" "
            "style=\"stroke:rgb(%5,%6,%7);stroke-width:%8;fill:none\" />\n")
            .arg(p1.x)
            .arg(p1.y)
            .arg(p2.x)
            .arg(p2.y)
            .arg(getColR())
            .arg(getColG())
            .arg(getColB())
            .arg(getStrokeWidth())
    );
}



void SvgPainter::drawCircle(const RS_Vector& cp, double radius) {
    if (svg==NULL) {
        return;
    }
    rawOutput(
        RS_String("  <ellipse cx=\"%1\" cy=\"%2\" rx=\"%3\" ry=\"%4\" "
            "style=\"stroke:rgb(%5,%6,%7);stroke-width:%8;fill:none;\"/>\n")
            .arg(cp.x)
            .arg(cp.y)
            .arg(radius)
            .arg(radius)
            .arg(getColR())
            .arg(getColG())
            .arg(getColB())
            .arg(getStrokeWidth())
    );
    
    /*
    drawArc(cp, radius, 0.0, M_PI, false);
    drawArc(cp, radius, M_PI, 2*M_PI, false);
    */
}


/**
 * Draws an arc.
 * @param cx center in x
 * @param cy center in y
 * @param radius Radius
 * @param a1 Angle 1 in rad
 * @param a2 Angle 2 in rad
 * @param x1 startpoint x
 * @param y1 startpoint y
 * @param x2 endpoint x
 * @param y2 endpoint y
 * @param reversed true: clockwise, false: counterclockwise
 */
void SvgPainter::drawArc(const RS_Vector& cp, double radius,
                          double a1, double a2,
                          const RS_Vector& /*p1*/,
                          const RS_Vector& /*p2*/,
                          bool reversed) {
    drawArc(cp, radius, a1, a2, reversed);
}

/**
 * Draws an arc.
 * @param cx center in x
 * @param cy center in y
 * @param radius Radius
 * @param a1 Angle 1 in rad
 * @param a2 Angle 2 in rad
 * @param reversed true: clockwise, false: counterclockwise
 */
void SvgPainter::drawArc(const RS_Vector& cp, double radius,
                          double a1, double a2,
                          bool reversed) {
    if (svg==NULL) {
        return;
    }

    if (RS_Math::getAngleDifference(a1, a2)<1.0e-6) {
        drawCircle(cp, radius);
    }
    else {
        RS_Arc arc = getArc(cp, radius, a1, a2, reversed);
        rawOutput(
            RS_String("  <path d=\"M%1,%2 ")
                .arg(arc.getStartpoint().x)
                .arg(arc.getStartpoint().y)
        );
        exportArcData(arc);
        rawOutput(
            RS_String("\" style=\"stroke:rgb(%1,%2,%3);stroke-width:%4;fill:none\" />\n")
                .arg(getColR())
                .arg(getColG())
                .arg(getColB())
                .arg(getStrokeWidth())
        );
    }
}



void SvgPainter::exportArcData(const RS_Arc& arc) {
    if (svg==NULL) {
        return;
    }
    
    // full circle:
    if (arc.getStartpoint().distanceTo(arc.getEndpoint())<1.0e-3) {
        rawOutput(
            RS_String("A%1,%2 0 0,0 %3,%4 ")
                .arg(arc.getRadius())
                .arg(arc.getRadius())
                .arg(arc.getMiddlepoint().x)
                .arg(arc.getMiddlepoint().y)
        );
        rawOutput(
            RS_String("A%1,%2 0 0,0 %3,%4 ")
                .arg(arc.getRadius())
                .arg(arc.getRadius())
                .arg(arc.getEndpoint().x)
                .arg(arc.getEndpoint().y)
        );
        return;
    }

    int largeArc = (int)(arc.getAngleLength() > M_PI);
    int sweep = !arc.isReversed();

    /*
    if (arc.getStartpoint().distanceTo(arc.getEndpoint())<1.0e-3) {
        drawCircle(arc.getCenter(), arc.getRadius());
    }
    else {
    */
        rawOutput(
            RS_String("A%1,%2 0 %3,%4 %5,%6 ")
                .arg(arc.getRadius())
                .arg(arc.getRadius())
                .arg(largeArc)
                .arg(sweep)
                .arg(arc.getEndpoint().x)
                .arg(arc.getEndpoint().y)
        );
    //}
}
    
    
    
// problem: getArc might change arc direction!!!
RS_Arc SvgPainter::getArc(const RS_Vector& cp, double radius,
                         double a1, double a2,
                         bool reversed) {
    RS_Arc arc(NULL, RS_ArcData());
    arc.setData(RS_ArcData(cp, radius, a1, a2, reversed));
    /*
    if (!reversed) {
        arc.setData(RS_ArcData(cp, radius, a1, a2, false));
    }
    else {
        arc.setData(RS_ArcData(cp, radius, a2, a1, false));
    }
    */
    arc.mirror(cp, cp + RS_Vector(1.0,0.0));
    arc.calculateEndpoints();
    return arc;
}



/**
 * Draws an ellipse.
 * @param cx center in x
 * @param cy center in y
 * @param radius1 Radius 1
 * @param radius2 Radius 2
 * @param angle Angle in rad
 * @param a1 Angle 1 in rad
 * @param a2 Angle 2 in rad
 * @param reversed true: clockwise, false: counterclockwise
 */
void SvgPainter::drawEllipse(const RS_Vector& cp,
                              double radius1, double radius2,
                              double angle,
                              double a1, double a2,
                              bool reversed) {

    // path data cannot handle full ellipses:
    if (RS_Math::getAngleDifference(a1, a2)<1.0e-2) {
        drawEllipse(cp, radius1, radius2, angle, 0.0, M_PI, false);
        drawEllipse(cp, radius1, radius2, angle, M_PI, 2.0*M_PI, false);
        return;
    }

    RS_Vector toMajor;
    toMajor.setPolar(radius1, angle);

    RS_Ellipse ellipse(NULL, 
        RS_EllipseData(
            cp, 
            toMajor,
            radius2/radius1,
            a1, a2,
            reversed
        )
    );
    ellipse.mirror(cp, cp + RS_Vector(1.0,0.0));

    /*
    float ang, ang1, ang2;
    ang = (float)( (M_PI * 2 - angle) * ARAD );
    ang1 = (float)( (M_PI * 2 - a1) * ARAD );
    ang2 = (float)( (M_PI * 2 - a2) * ARAD );
    */
    /*
    if (reversed) {
        float d = ang1;
        ang1 = ang2;
        ang2 = d;
    }

    if (ang2 > ang1) {
        ang1 += 360;
    }
    */

    /*
    float x1 = cp.x + radius1 * cos(a1);
    float y1 = cp.y + radius1 * sin(a1);
    */

    float x1 = ellipse.getStartpoint().x;
    float y1 = ellipse.getStartpoint().y;
    float x2 = ellipse.getEndpoint().x;
    float y2 = ellipse.getEndpoint().y;
    
    int largeArc = (int)(ellipse.getAngleLength() > M_PI);
    int sweep = !ellipse.isReversed();
    
    rawOutput(
        RS_String("  <path d=\"M%1,%2 A%3,%4 %5 %6 %7 %8,%9\" "
            "style=\"stroke:rgb(%10,%11,%12);stroke-width:%13;fill:none\" />\n")
            .arg(x1).arg(y1)
            .arg(radius1).arg(radius2)
            .arg(-angle * ARAD)
            .arg(largeArc)
            .arg(sweep)
            .arg(x2).arg(y2)
            .arg(getColR())
            .arg(getColG())
            .arg(getColB())
            .arg(getStrokeWidth())
    );

    /*
    rawOutput(
        RS_String("  <ellipse x=\"%1\" y=\"%2\" "
            "radius1=\"%3\" radius2=\"%4\" "
            "angle1=\"%5\" angle2=\"%6\" angle3=\"%7\" "
            "style=\"stroke:rgb(%8,%9,%10);stroke-width:%11\" />\n")
            .arg(cp.x)
            .arg(cp.y)
            .arg(radius1)
            .arg(radius2)
            .arg(ang)
            .arg(ang2)
            .arg(ang1)
            .arg(getColR()).arg(getColG()).arg(getColB())
            .arg(getStrokeWidth())
    );
    */
}
    
    
void SvgPainter::fillTriangle(const RS_Vector& p1,
                              const RS_Vector& p2,
                              const RS_Vector& p3) {
    if (svg==NULL) {
        return;
    }
    rawOutput(
        RS_String("  <path d=\"M%1,%2 L%3,%4 L%5,%6 Z\" "
            "style=\"stroke:rgb(%7,%8,%9);stroke-width:%10;fill:rgb(%11,%12,%13)\" />\n")
            .arg(p1.x) .arg(p1.y)
            .arg(p2.x) .arg(p2.y)
            .arg(p3.x) .arg(p3.y)
            .arg(getColR()).arg(getColG()).arg(getColB())
            .arg(getStrokeWidth())
            .arg(getColR()).arg(getColG()).arg(getColB())
    );
}
    
    

void SvgPainter::drawText(RS_Text* text, RS_StaticGraphicView& gv) {
    if (svg==NULL || text==NULL) {
        return;
    }
                    
    RS_Vector pos = gv.toGui(text->getInsertionPoint());
    double height = gv.toGuiDX(text->getHeight()) * 1.333;
    RS_String s = text->getText();

    RS_String xmlText = s;
    xmlText = xmlText.replace("&", "&amp;");
    xmlText = xmlText.replace("<", "&lt;");
    xmlText = xmlText.replace(">", "&gt;");

    RS_String halign;
    switch (text->getHAlign()) {
    default:
    case RS2::HAlignLeft:
        halign = "start";
        break;
    case RS2::HAlignCenter:
        halign = "middle";
        break;
    case RS2::HAlignRight:
        halign = "end";
        break;
    }
    
    double vOffset = 0.0;
    switch (text->getVAlign()) {
    default:
    case RS2::VAlignTop:
        vOffset = height - height*0.266;
        break;
    case RS2::VAlignMiddle:
        vOffset = height/2;
        break;
    case RS2::VAlignBottom:
        vOffset = 0.0;
        break;
    }

    rawOutput(
        RS_String("  <text x=\"%1\" y=\"%2\" font-family=\"Deja\" font-size=\"%3\" "
            "style=\"fill:rgb(%4,%5,%6)\" text-anchor=\"%7\">\n"
            "    %8\n"
            "  </text>")
            .arg(pos.x).arg(pos.y + vOffset)
            .arg(height)
            .arg(getColR()).arg(getColG()).arg(getColB())
            .arg(halign)
            .arg(xmlText)
    );
}



void SvgPainter::drawPolyline(RS_Polyline* pl, RS_StaticGraphicView& gv) {
    if (svg==NULL || pl==NULL) {
        return;
    }

    bool first = true;

    rawOutput("  <path d=\"");
    for (RS_Entity* e=pl->firstEntity(); e!=NULL; e=pl->nextEntity()) {
        if (first && e->isAtomic()) {
            RS_AtomicEntity* ae = dynamic_cast<RS_AtomicEntity*>(e);
            RS_Vector pos = gv.toGui(ae->getStartpoint());
            rawOutput(
                RS_String("M%1,%2 ")
                    .arg(pos.x)
                    .arg(pos.y)
            );
            first = false;
        }

        if (e->rtti()==RS2::EntityLine) {
            RS_Line* line = dynamic_cast<RS_Line*>(e);
            RS_Vector pos = gv.toGui(line->getEndpoint());
            rawOutput(
                RS_String("L%3,%4 ")
                    .arg(pos.x)
                    .arg(pos.y)
            );
        }
        else if (e->rtti()==RS2::EntityArc) {
            RS_Arc* arc = dynamic_cast<RS_Arc*>(e);
            exportArcData(
                getArc(
                    gv.toGui(arc->getCenter()), 
                    gv.toGuiDX(arc->getRadius()),
                    arc->getAngle1(),
                    arc->getAngle2(),
                    arc->isReversed()
                )
            );
            /*
            rawOutput(
                RS_String("A%1,%2 0 %3,0 %4,%5 ")
                    .arg(arc->getRadius()),
                    .arg(arc->getRadius()),
                    .arg(arc->getEndpoint().x),
                    .arg(arc->getEndpoint().y)
            );
            */
        }
    }
    
    rawOutput(
        RS_String("\" style=\"stroke:rgb(%1,%2,%3);stroke-width:%4;fill:none\" />\n")
            .arg(getColR())
            .arg(getColG())
            .arg(getColB())
            .arg(getStrokeWidth())
    );
}
    
    
void SvgPainter::drawSolidHatch(RS_Hatch* h, RS_StaticGraphicView& gv) {
    if (svg==NULL || h==NULL) {
        return;
    }

    rawOutput("  <path d=\"");
    for (RS_Entity* e=h->firstEntity(); e!=NULL; e=h->nextEntity()) {
        RS_EntityContainer* loop = dynamic_cast<RS_EntityContainer*>(e);

        if (loop==NULL) {
            continue;
        }
                
        bool first = true;
        RS_Vector cursor;
    
        for (RS_Entity* e2=loop->firstEntity(); e2!=NULL; e2=loop->nextEntity()) {
            RS_AtomicEntity* ae = dynamic_cast<RS_AtomicEntity*>(e2);

            if (ae==NULL) {
                continue;
            }


            RS_Vector start = gv.toGui(ae->getStartpoint());
    
            if (first || cursor.distanceTo(start)>1.0e-4) {
                rawOutput(
                    RS_String("M%1,%2 ")
                        .arg(start.x)
                        .arg(start.y)
                );
                first = false;
                cursor = start;
            }

            if (ae->rtti()==RS2::EntityLine) {
                RS_Line* line = dynamic_cast<RS_Line*>(ae);
                RS_Vector pos = gv.toGui(line->getEndpoint());
                rawOutput(
                    RS_String("L%3,%4 ")
                        .arg(pos.x)
                        .arg(pos.y)
                );
                cursor = pos;
            }
            else if (ae->rtti()==RS2::EntityArc) {
                RS_Arc* arc = dynamic_cast<RS_Arc*>(ae);
                exportArcData(
                    getArc(
                        gv.toGui(arc->getCenter()), 
                        gv.toGuiDX(arc->getRadius()),
                        arc->getAngle1(),
                        arc->getAngle2(),
                        arc->isReversed()
                    )
                );
                cursor = gv.toGui(arc->getEndpoint());
            }
            else if (ae->rtti()==RS2::EntityCircle) {
                RS_Circle* circle = dynamic_cast<RS_Circle*>(ae);
                exportArcData(
                    getArc(
                        gv.toGui(circle->getCenter()), 
                        gv.toGuiDX(circle->getRadius()),
                        0.0,
                        2*M_PI,
                        false
                    )
                );
                /*
                exportArcData(
                    getArc(
                        gv.toGui(circle->getCenter()), 
                        gv.toGuiDX(circle->getRadius()),
                        0.0,
                        2*M_PI,
                        false
                    )
                );
                */
                cursor = gv.toGui(circle->getCenter() + RS_Vector(circle->getRadius(), 0));
            }
        }
    }
    
    rawOutput(
        RS_String("\" style=\"stroke:none;fill:rgb(%1,%2,%3)\" />\n")
            .arg(getColR())
            .arg(getColG())
            .arg(getColB())
    );
}



RS_Pen SvgPainter::getPen() {
    return pen;
}


void SvgPainter::setPen(const RS_Pen& pen) {
    this->pen = pen;
}


void SvgPainter::setPen(const RS_Color& color) {
    pen.setColor(color);
}


void SvgPainter::setPen(int r, int g, int b) {
    pen.setColor(RS_Color(r, g, b));
}


void SvgPainter::startSvg() {
    if (svg==NULL) {
        return;
    }

    RS_DEBUG->print("SvgPainter::startSvg");
    
    rawOutput(
        RS_String(
            "<?xml version=\"1.0\" standalone=\"yes\"?>\n"
            "\n"
            "<svg width=\"%1mm\" height=\"%2mm\" viewBox=\"0 0 %3 %4\" version=\"1.1\"\n"
            "xmlns=\"http://www.w3.org/2000/svg\" style=\"stroke-linecap:round\">\n")
            .arg(width).arg(height).arg(width).arg(height)
    );

    /*
    fprintf(svg, "<?xml version=\"1.0\" standalone=\"no\"?>\n"
    "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \n"
    "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
    "\n"
    "<svg width=\"%d\" height=\"%d\" version=\"1.1\"\n"
    "xmlns=\"http://www.w3.org/2000/svg\">\n", width, height);
    */
    
    RS_DEBUG->print("SvgPainter::startSvg: OK");
}


void SvgPainter::endSvg() {
    rawOutput("</svg>\n");
}
    
    
void SvgPainter::rawOutput(const RS_String& s) {
    if (svg==NULL) {
        return;
    }
    
    fprintf(svg, "%s", (const char*)s.toUtf8());
}
