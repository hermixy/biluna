/****************************************************************************
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


#include "rs_filterdxf.h"

#include <stdio.h>

#include "dl_attributes.h"
#include "dl_codes.h"
#include "dl_writer_ascii.h"

#include "rs_dimaligned.h"
#include "rs_dimangular.h"
#include "rs_dimdiametric.h"
#include "rs_dimlinear.h"
#include "rs_dimradial.h"
#include "rs_dimordinate.h"
#include "rs_file.h"
#include "rs_fileinfo.h"
#include "rs_hatch.h"
#include "rs_image.h"
#include "rs_leader.h"
#include "rs_modification.h"
#include "rs_regexp.h"
#include "rs_system.h"

#include <qtextcodec.h>

/**
 * Default constructor.
 *
 */
RS_FilterDxf::RS_FilterDxf()
        :RS_FilterInterface() {

    RS_DEBUG->print("RS_FilterDxf::RS_FilterDxf()");

    addImportFormat(RS2::FormatDxf);
    addExportFormat(RS2::FormatDxf);
    addExportFormat(RS2::FormatDxf12);

    mtext = "";
    polyline = NULL;
    leader = NULL;
    hatch = NULL;
    hatchLoop = NULL;
    currentContainer = NULL;
    graphic = NULL;
    exportAllLayersVisible = false;
    RS_DEBUG->print("RS_FilterDxf::RS_FilterDxf(): OK");
}



/**
 * Destructor.
 */
RS_FilterDxf::~RS_FilterDxf() {
    RS_DEBUG->print("RS_FilterDxf::~RS_FilterDxf()");
    RS_DEBUG->print("RS_FilterDxf::~RS_FilterDxf(): OK");
}



/**
 * Implementation of the method used for RS_Import to communicate
 * with this filter.
 *
 * @param g The graphic in which the entities from the file
 * will be created or the graphics from which the entities are
 * taken to be stored in a file.
 */
bool RS_FilterDxf::fileImport(RS_Graphic& g, const RS_String& file, RS2::FormatType /*formatType*/) {
    RS_DEBUG->print("RS_FilterDxf::fileImport");

    RS_DEBUG->print("DXF Filter: importing file '%s'...", (const char*)QFile::encodeName(file));

    graphic = &g;
    currentContainer = graphic;
    this->file = file;

    RS_File f(file);
    if (!f.open(QIODevice::ReadOnly)) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
            "DXF Filter: cannot open file '%s'.", 
            (const char*)QFile::encodeName(file));
        return false;
    }
    QByteArray ba = f.readAll();
    f.close();

    QByteArray crlf(2, 10);
    crlf[0] = 13;
    ba.replace(crlf, QByteArray(1, 10));
    
    ba.replace(QByteArray(1, 13), QByteArray(1, 10));

    std::string s = ba.constData();
    std::stringstream stringStream(s);

    RS_DEBUG->print("graphic->countLayers(): %d", graphic->countLayers());

    RS_DEBUG->print("RS_FilterDxf::fileImport: reading file");
    //bool success = dxf.in((const char*)QFile::encodeName(file), this);
    bool success = dxf.in(stringStream, this);
    RS_DEBUG->print("RS_FilterDxf::fileImport: reading file: OK");

    if (success==false) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "Cannot open DXF file '%s'.", (const char*)QFile::encodeName(file));
        return false;
    }

    RS_DEBUG->print("RS_FilterDxf::fileImport: adding variables");

    // add some variables that need to be there for DXF drawings:
    if (graphic->getVariableString("$DIMSTYLE", "").isEmpty()) {
        RS_DEBUG->print("RS_FilterDxf::fileImport: adding DIMSTYLE");
        graphic->addVariable("$DIMSTYLE", "Standard", 2);
        RS_DEBUG->print("RS_FilterDxf::fileImport: adding DIMSTYLE: OK");
    }
    RS_DEBUG->print("RS_FilterDxf::fileImport: adding variables: OK");

    RS_DEBUG->print("RS_FilterDxf::fileImport: updating inserts");
    graphic->updateInserts();
    RS_DEBUG->print("RS_FilterDxf::fileImport: updating inserts: OK");

    if (!currentLayer.isEmpty()) {
        graphic->getLayerList()->activate(currentLayer/*, true*/);
        RS_DEBUG->print("RS_FilterDxf::fileImport: "
            "activating current layer: %s", 
                (const char*)currentLayer.toLatin1());
    }

    RS_DEBUG->print("RS_FilterDxf::fileImport OK");

    return true;
}



/**
 * Implementation of the method which handles layers.
 */
void RS_FilterDxf::addLayer(const DL_LayerData& data) {
    RS_DEBUG->print("RS_FilterDxf::addLayer: %s", data.name.c_str());

    RS_Layer* layer = new RS_Layer(data.name.c_str());
    bool frozen = (attributes.getColor()<0 || data.flags&0x01);
    attributes.setColor(abs(attributes.getColor()));
    layer->setPen(attributesToPen(true));
    
    if (frozen) {
        layer->freeze(true);
    }
    if (data.flags&0x04) {
        layer->lock(true);
    }
    
    graphic->addLayer(layer);
    RS_DEBUG->print("RS_FilterDxf::addLayer: OK");
}



/**
 * Implementation of the method which handles blocks.
 *
 * @todo Adding blocks to blocks (stack for currentContainer)
 */
void RS_FilterDxf::addBlock(const DL_BlockData& data) {

    RS_DEBUG->print("RS_FilterDxf::addBlock");

    RS_DEBUG->print("  adding block: %s", data.name.c_str());


    // Prevent special blocks (paper_space, model_space) from being added:
    if (RS_String(data.name.c_str()).toLower()!="*paper_space0" &&
            RS_String(data.name.c_str()).toLower()!="*paper_space" &&
            RS_String(data.name.c_str()).toLower()!="*model_space" &&
            RS_String(data.name.c_str()).toLower()!="$paper_space0" &&
            RS_String(data.name.c_str()).toLower()!="$paper_space" &&
            RS_String(data.name.c_str()).toLower()!="$model_space") {

// #ifndef RS_NO_COMPLEX_ENTITIES
        if (RS_String(data.name.c_str()).startsWith("__CE")) {
            RS_EntityContainer* ec = new RS_EntityContainer();
            ec->setLayer("0");
            currentContainer = ec;
            graphic->addEntity(ec);
        }
        else {
// #endif
            RS_Vector bp(data.bpx, data.bpy);
            RS_Block* block =
                new RS_Block(graphic,
                             RS_BlockData(data.name.c_str(), bp, false));

            if (graphic->addBlock(block)) {
                currentContainer = block;
            }
// #ifndef RS_NO_COMPLEX_ENTITIES

        }
// #endif

    }
}



/**
 * Implementation of the method which closes blocks.
 */
void RS_FilterDxf::endBlock() {
    currentContainer = graphic;
}



/**
 * Implementation of the method which handles point entities.
 */
void RS_FilterDxf::addPoint(const DL_PointData& data) {
    RS_Vector v(data.x, data.y);

    RS_Point* entity = new RS_Point(currentContainer,
                                    RS_PointData(v));
    setEntityAttributes(entity);

    currentContainer->addEntity(entity);
}



/**
 * Implementation of the method which handles line entities.
 */
void RS_FilterDxf::addLine(const DL_LineData& data) {
    RS_DEBUG->print("RS_FilterDxf::addLine");

    RS_Vector v1(data.x1, data.y1);
    RS_Vector v2(data.x2, data.y2);

    RS_DEBUG->print("RS_FilterDxf::addLine: create line");

    if (currentContainer==NULL) {
        RS_DEBUG->print("RS_FilterDxf::addLine: currentContainer is NULL");
    }

    RS_Line* entity = new RS_Line(currentContainer,
                                  RS_LineData(v1, v2));
    RS_DEBUG->print("RS_FilterDxf::addLine: set attributes");
    setEntityAttributes(entity);

    RS_DEBUG->print("RS_FilterDxf::addLine: add entity");

    currentContainer->addEntity(entity);

    RS_DEBUG->print("RS_FilterDxf::addLine: OK");
}



/**
 * Implementation of the method which handles arc entities.
 * 
 * @param angle1 Start angle in deg (!)
 * @param angle2 End angle in deg (!)
 */
void RS_FilterDxf::addArc(const DL_ArcData& data) {
    RS_DEBUG->print("RS_FilterDxf::addArc");
    RS_Vector v(data.cx, data.cy);
    RS_ArcData d(v, data.radius,
                 data.angle1/ARAD,
                 data.angle2/ARAD,
                 false);
    RS_Arc* entity = new RS_Arc(currentContainer, d);
    setEntityAttributes(entity);

    currentContainer->addEntity(entity);
}



/**
 * Implementation of the method which handles ellipse entities.
 *
 * @param angle1 Start angle in rad (!)
 * @param angle2 End angle in rad (!)
 */
void RS_FilterDxf::addEllipse(const DL_EllipseData& data) {
    RS_DEBUG->print("RS_FilterDxf::addEllipse");

    RS_Vector v1(data.cx, data.cy);
    RS_Vector v2(data.mx, data.my);

    RS_EllipseData ed(v1, v2,
                      data.ratio,
                      data.angle1,
                      data.angle2,
                      false);
    RS_Ellipse* entity = new RS_Ellipse(currentContainer, ed);
    setEntityAttributes(entity);

    currentContainer->addEntity(entity);
}



/**
 * Implementation of the method which handles circle entities.
 */
void RS_FilterDxf::addCircle(const DL_CircleData& data) {
    RS_DEBUG->print("RS_FilterDxf::addCircle");

    RS_Vector v(data.cx, data.cy);
    RS_CircleData d(v, data.radius);
    RS_Circle* entity = new RS_Circle(currentContainer, d);
    setEntityAttributes(entity);

    currentContainer->addEntity(entity);
}



/**
 * Implementation of the method which handles polyline entities.
 */
void RS_FilterDxf::addPolyline(const DL_PolylineData& data) {
    RS_DEBUG->print("RS_FilterDxf::addPolyline");
    RS_PolylineData d(RS_Vector(false),
                      RS_Vector(false),
                      data.flags&0x1);
    polyline = new RS_Polyline(currentContainer, d);
    setEntityAttributes(polyline);

    currentContainer->addEntity(polyline);
}



/**
 * Implementation of the method which handles polyline vertices.
 */
void RS_FilterDxf::addVertex(const DL_VertexData& data) {
    RS_DEBUG->print("RS_FilterDxf::addVertex(): %f/%f bulge: %f",
                    data.x, data.y, data.bulge);

    RS_Vector v(data.x, data.y);

    if (polyline!=NULL) {
        polyline->addVertex(v, data.bulge);
    }
}



/**
 * Implementation of the method which handles splines.
 */
void RS_FilterDxf::addSpline(const DL_SplineData& data) {
    RS_DEBUG->print("RS_FilterDxf::addSpline: degree: %d", data.degree);

    if (data.degree>=1 && data.degree<=3) {
        RS_SplineData d(data.degree, ((data.flags&0x1)==0x1));
        spline = new RS_Spline(currentContainer, d);
        setEntityAttributes(spline);

        currentContainer->addEntity(spline);
    } else {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "RS_FilterDxf::addSpline: Invalid degree for spline: %d. "
            "Accepted values are 1..3.", data.degree);
    }
}


/**
 * Implementation of the method which handles spline control points.
 */
void RS_FilterDxf::addControlPoint(const DL_ControlPointData& data) {
    RS_DEBUG->print("RS_FilterDxf::addControlPoint: %f/%f", data.x, data.y);

    RS_Vector v(data.x, data.y);

    if (spline!=NULL) {
        spline->addControlPoint(v);
        spline->update();
    }
}



/**
 * Implementation of the method which handles inserts.
 */
void RS_FilterDxf::addInsert(const DL_InsertData& data) {

    RS_DEBUG->print("RS_FilterDxf::addInsert");

    if (RS_String(data.name.c_str()).left(3)=="A$C") {
        return;
    }

    RS_Vector ip(data.ipx, data.ipy);
    RS_Vector sc(data.sx, data.sy);
    RS_Vector sp(data.colSp, data.rowSp);

    RS_InsertData d(data.name.c_str(),
                    ip, sc, data.angle/ARAD,
                    data.cols, data.rows,
                    sp,
                    NULL,
                    RS2::NoUpdate);
    RS_Insert* entity = new RS_Insert(currentContainer, d);
    setEntityAttributes(entity);
    RS_DEBUG->print("  id: %d", entity->getId());
    currentContainer->addEntity(entity);
}


void RS_FilterDxf::addSolid(const DL_SolidData& data) {
    RS_DEBUG->print("RS_FilterDxf::addSolid()");

    RS_Solid* s = new RS_Solid(currentContainer,
        RS_SolidData(
            RS_Vector(data.x[0], data.y[0], data.z[0]),
            RS_Vector(data.x[1], data.y[1], data.z[1]),
            RS_Vector(data.x[2], data.y[2], data.z[2]),
            RS_Vector(data.x[3], data.y[3], data.z[3])));
    
    setEntityAttributes(s);

    currentContainer->addEntity(s);

    /*
    RS_Hatch* h = new RS_Hatch(currentContainer,
                         RS_HatchData(true,
                                      1.0,
                                      0.0,
                                      "SOLID"));
    RS_EntityContainer* hl = new RS_EntityContainer(h);
    hl->setLayer(NULL);
    hl->addEntity(new RS_Line(hl,
                   RS_LineData(RS_Vector(data.x[0], data.y[0]),
                               RS_Vector(data.x[1], data.y[1]))));
    hl->addEntity(new RS_Line(hl,
                   RS_LineData(RS_Vector(data.x[1], data.y[1]),
                               RS_Vector(data.x[3], data.y[3]))));

    if (RS_Vector(data.x[2], data.y[2])
        .distanceTo(RS_Vector(data.x[3], data.y[3]))>1.0e-4) {
        hl->addEntity(new RS_Line(hl,
                       RS_LineData(RS_Vector(data.x[3], data.y[3]),
                                   RS_Vector(data.x[2], data.y[2]))));
        hl->addEntity(new RS_Line(hl,
                       RS_LineData(RS_Vector(data.x[2], data.y[2]),
                                   RS_Vector(data.x[0], data.y[0]))));
    }
    else {
        hl->addEntity(new RS_Line(hl,
                       RS_LineData(RS_Vector(data.x[3], data.y[3]),
                                   RS_Vector(data.x[0], data.y[0]))));
    }

    h->addEntity(hl);
    setEntityAttributes(h);

    currentContainer->addEntity(h);
    */
}


/**
 * Implementation of the method which handles text 
 * chunks for MText entities.
 */
void RS_FilterDxf::addMTextChunk(const char* text) {
    RS_DEBUG->print("RS_FilterDxf::addMTextChunk: %s", text);

    /*
    QCString locallyEncoded = text;
    RS_String enc = RS_System::getEncoding(variables.getString("$DWGCODEPAGE", "ANSI_1252"));
    QTextCodec *codec = QTextCodec::codecForName(enc); // get the codec for Japanese
    if (codec!=NULL) {
        mtext += codec->toUnicode(toNativeString(locallyEncoded));
    } else {
        mtext += toNativeString(text);
    }
    */
    
    mtext+=text;

}



/**
 * Implementation of the method which handles 
 * multi texts (MTEXT).
 */
void RS_FilterDxf::addMText(const DL_MTextData& data) {
    RS_DEBUG->print("RS_FilterDxf::addMText: %s", data.text.c_str());
    RS_DEBUG->print("RS_FilterDxf::addMText: style: %s", data.style.c_str());

    RS_Vector ip(data.ipx, data.ipy);
    RS2::VAlign valign;
    RS2::HAlign halign;
    RS2::TextDrawingDirection dir;
    RS2::TextLineSpacingStyle lss;
    RS_String sty = data.style.c_str();

    if (data.attachmentPoint<=3) {
        valign=RS2::VAlignTop;
    } else if (data.attachmentPoint<=6) {
        valign=RS2::VAlignMiddle;
    } else {
        valign=RS2::VAlignBottom;
    }

    if (data.attachmentPoint%3==1) {
        halign=RS2::HAlignLeft;
    } else if (data.attachmentPoint%3==2) {
        halign=RS2::HAlignCenter;
    } else {
        halign=RS2::HAlignRight;
    }

    if (data.drawingDirection==1) {
        dir = RS2::LeftToRight;
    } else if (data.drawingDirection==3) {
        dir = RS2::TopToBottom;
    } else {
        dir = RS2::ByStyle;
    }

    if (data.lineSpacingStyle==1) {
        lss = RS2::AtLeast;
    } else {
        lss = RS2::Exact;
    }

    mtext+=QString(data.text.c_str());
    
    RS_DEBUG->print("RS_FilterDxf::addMText: "
        "Text as unicode (before codec):");
    RS_DEBUG->printUnicode(mtext);

    RS_String locallyEncoded = mtext;
    RS_String dwgCodePage = variables.getString("$DWGCODEPAGE", "---");
    if (dwgCodePage!="---") {
        RS_DEBUG->print("RS_FilterDxf::addMText: code page is: %s", (const char*)dwgCodePage.toLatin1());
        RS_String enc = RS_System::getEncoding(dwgCodePage);
        RS_DEBUG->print("RS_FilterDxf::addMText: codec is: %s", (const char*)enc.toLatin1());
        // get the codec for Japanese
        QTextCodec* codec = QTextCodec::codecForName(enc.toLatin1());
        RS_DEBUG->print("RS_FilterDxf::addMText: 001"); 
        RS_DEBUG->print("RS_FilterDxf::addMText: 002a"); 
        RS_DEBUG->print("RS_FilterDxf::addMText: codec is: %s", 
            (const char*)enc.toLatin1());
        if (codec!=NULL) {
            mtext = codec->toUnicode(toNativeString(locallyEncoded).toLatin1());
        }
        else {
            mtext = toNativeString(locallyEncoded);
            RS_DEBUG->print(RS_Debug::D_WARNING, 
                "RS_FilterDxf::addMText: invalid codec: %s", (const char*)enc.toLatin1()); 
        }
    } else {
        RS_DEBUG->print("RS_FilterDxf::addMText: 002b"); 
        RS_DEBUG->print("RS_FilterDxf::addMText: no codec"); 
        mtext = toNativeString(mtext);
    }
    RS_DEBUG->print("RS_FilterDxf::addMText: 004"); 
    

    // use default style for the drawing:
    if (sty.isEmpty()) {
        // japanese, cyrillic:
        RS_String codepage = variables.getString("$DWGCODEPAGE", "ANSI_1252");
        if (codepage=="ANSI_932" || codepage=="ANSI_1251") {
            sty = "Unicode";
        } else {
            sty = variables.getString("$TEXTSTYLE", "Standard");
        }
    }

    RS_DEBUG->print("RS_FilterDxf::addMText: Text as unicode:");
    RS_DEBUG->printUnicode(mtext);

    RS_TextData d(ip, data.height, data.width,
                  valign, halign,
                  dir, lss,
                  data.lineSpacingFactor,
                  mtext, sty, data.angle,
                  RS2::NoUpdate);
    RS_Text* entity = new RS_Text(currentContainer, d);

    setEntityAttributes(entity);
    entity->update();
    currentContainer->addEntity(entity);

    mtext = "";
}



/**
 * Implementation of the method which handles 
 * texts (TEXT).
 */
void RS_FilterDxf::addText(const DL_TextData& data) {
    RS_DEBUG->print("RS_FilterDxf::addText");
    int attachmentPoint;
    RS_Vector refPoint;
    double angle = data.angle;

    // TODO: check, maybe implement a separate TEXT instead of using MTEXT

    // baseline has 5 vertical alignment modes:
    if (data.vJustification!=0 || data.hJustification!=0) {
        switch (data.hJustification) {
        default:
        case 0: // left aligned
            attachmentPoint = 1;
            refPoint = RS_Vector(data.apx, data.apy);
            break;
        case 1: // centered
            attachmentPoint = 2;
            refPoint = RS_Vector(data.apx, data.apy);
            break;
        case 2: // right aligned
            attachmentPoint = 3;
            refPoint = RS_Vector(data.apx, data.apy);
            break;
        case 3: // aligned (TODO)
            attachmentPoint = 2;
            refPoint = RS_Vector((data.ipx+data.apx)/2.0,
                                 (data.ipy+data.apy)/2.0);
            angle =
                RS_Vector(data.ipx, data.ipy).angleTo(
                    RS_Vector(data.apx, data.apy));
            break;
        case 4: // Middle (TODO)
            attachmentPoint = 2;
            refPoint = RS_Vector(data.apx, data.apy);
            break;
        case 5: // fit (TODO)
            attachmentPoint = 2;
            refPoint = RS_Vector((data.ipx+data.apx)/2.0,
                                 (data.ipy+data.apy)/2.0);
            angle =
                RS_Vector(data.ipx, data.ipy).angleTo(
                    RS_Vector(data.apx, data.apy));
            break;
        }

        switch (data.vJustification) {
        default:
        case 0: // baseline
        case 1: // bottom
            attachmentPoint += 6;
            break;

        case 2: // middle
            attachmentPoint += 3;
            break;

        case 3: // top
            break;
        }
    } else {
        //attachmentPoint = (data.hJustification+1)+(3-data.vJustification)*3;
        attachmentPoint = 7;
        refPoint = RS_Vector(data.ipx, data.ipy);
    }

    int drawingDirection = 5;
    double width = 100.0;

    mtext = "";
    addMText(DL_MTextData(
                 refPoint.x,
                 refPoint.y,
                 refPoint.z,
                 data.height, width,
                 attachmentPoint,
                 drawingDirection,
                 RS2::Exact,
                 1.0,
                 data.text.c_str(), data.style,
                 angle));
}



/**
 * Implementation of the method which handles 
 * dimensions (DIMENSION).
 */
RS_DimensionData RS_FilterDxf::convDimensionData(
    const DL_DimensionData& data) {

    RS_Vector defP(data.dpx, data.dpy);
    RS_Vector midP(data.mpx, data.mpy);
    RS2::VAlign valign;
    RS2::HAlign halign;
    RS2::TextLineSpacingStyle lss;
    RS_String sty = data.style.c_str();
    RS_String t; //= data.text;

    // middlepoint of text can be 0/0 which is considered to be invalid (!):
    //  0/0 because older QCad versions save the middle of the text as 0/0
    //  althought they didn't suport saving of the middle of the text.
    if (fabs(data.mpx)<1.0e-6 && fabs(data.mpy)<1.0e-6) {
        midP = RS_Vector(false);
    }

    if (data.attachmentPoint<=3) {
        valign=RS2::VAlignTop;
    } else if (data.attachmentPoint<=6) {
        valign=RS2::VAlignMiddle;
    } else {
        valign=RS2::VAlignBottom;
    }

    if (data.attachmentPoint%3==1) {
        halign=RS2::HAlignLeft;
    } else if (data.attachmentPoint%3==2) {
        halign=RS2::HAlignCenter;
    } else {
        halign=RS2::HAlignRight;
    }

    if (data.lineSpacingStyle==1) {
        lss = RS2::AtLeast;
    } else {
        lss = RS2::Exact;
    }

    t = toNativeString(data.text.c_str());

    if (sty.isEmpty()) {
        sty = variables.getString("$DIMSTYLE", "Standard");
    }

    RS_DEBUG->print("Text as unicode:");
    RS_DEBUG->printUnicode(t);

    // data needed to add the actual dimension entity
    return RS_DimensionData(defP, midP,
                            valign, halign,
                            lss,
                            data.lineSpacingFactor,
                            t, sty, data.angle);
}



/**
 * Implementation of the method which handles 
 * aligned dimensions (DIMENSION).
 */
void RS_FilterDxf::addDimAlign(const DL_DimensionData& data,
                               const DL_DimAlignedData& edata) {
    RS_DEBUG->print("RS_FilterDxf::addDimAligned");

    RS_DimensionData dimensionData = convDimensionData(data);

    RS_Vector ext1(edata.epx1, edata.epy1);
    RS_Vector ext2(edata.epx2, edata.epy2);

    RS_DimAlignedData d(ext1, ext2);

    RS_DimAligned* entity = new RS_DimAligned(currentContainer,
                            dimensionData, d);
    setEntityAttributes(entity);
    entity->update();
    currentContainer->addEntity(entity);
}



/**
 * Implementation of the method which handles 
 * linear dimensions (DIMENSION).
 */
void RS_FilterDxf::addDimLinear(const DL_DimensionData& data,
                                const DL_DimLinearData& edata) {
    RS_DEBUG->print("RS_FilterDxf::addDimLinear");

    RS_DimensionData dimensionData = convDimensionData(data);

    RS_Vector dxt1(edata.dpx1, edata.dpy1);
    RS_Vector dxt2(edata.dpx2, edata.dpy2);

    RS_DimLinearData d(dxt1, dxt2, RS_Math::deg2rad(edata.angle),
                       RS_Math::deg2rad(edata.oblique));

    RS_DimLinear* entity = new RS_DimLinear(currentContainer,
                                            dimensionData, d);
    setEntityAttributes(entity);
    entity->update();
    currentContainer->addEntity(entity);
}



/**
 * Implementation of the method which handles
 * radial dimensions (DIMENSION).
 */
void RS_FilterDxf::addDimRadial(const DL_DimensionData& data,
                                const DL_DimRadialData& edata) {
    RS_DEBUG->print("RS_FilterDxf::addDimRadial");

    RS_DimensionData dimensionData = convDimensionData(data);
    RS_Vector dp(edata.dpx, edata.dpy);

    RS_DimRadialData d(dp, edata.leader);

    RS_DimRadial* entity = new RS_DimRadial(currentContainer,
                                            dimensionData, d);

    setEntityAttributes(entity);
    entity->update();
    currentContainer->addEntity(entity);
}



/**
 * Implementation of the method which handles
 * diametric dimensions (DIMENSION).
 */
void RS_FilterDxf::addDimDiametric(const DL_DimensionData& data,
                                   const DL_DimDiametricData& edata) {
    RS_DEBUG->print("RS_FilterDxf::addDimDiametric");

    RS_DimensionData dimensionData = convDimensionData(data);
    RS_Vector dp(edata.dpx, edata.dpy);

    RS_DimDiametricData d(dp, edata.leader);

    RS_DimDiametric* entity = new RS_DimDiametric(currentContainer,
                              dimensionData, d);

    setEntityAttributes(entity);
    entity->update();
    currentContainer->addEntity(entity);
}



/**
 * Implementation of the method which handles
 * angular dimensions (DIMENSION).
 */
void RS_FilterDxf::addDimAngular(const DL_DimensionData& data,
                                 const DL_DimAngularData& edata) {
    RS_DEBUG->print("RS_FilterDxf::addDimAngular");

    RS_DimensionData dimensionData = convDimensionData(data);
    RS_Vector dp1(edata.dpx1, edata.dpy1);
    RS_Vector dp2(edata.dpx2, edata.dpy2);
    RS_Vector dp3(edata.dpx3, edata.dpy3);
    RS_Vector dp4(edata.dpx4, edata.dpy4);

    RS_DimAngularData d(dp1, dp2, dp3, dp4);

    RS_DimAngular* entity = new RS_DimAngular(currentContainer,
                            dimensionData, d);

    setEntityAttributes(entity);
    entity->update();
    currentContainer->addEntity(entity);
}



/**
 * Implementation of the method which handles
 * angular dimensions (DIMENSION).
 */
void RS_FilterDxf::addDimAngular3P(const DL_DimensionData& data,
                                   const DL_DimAngular3PData& edata) {
    RS_DEBUG->print("RS_FilterDxf::addDimAngular3P");

    RS_DimensionData dimensionData = convDimensionData(data);
    RS_Vector dp1(edata.dpx3, edata.dpy3);
    RS_Vector dp2(edata.dpx1, edata.dpy1);
    RS_Vector dp3(edata.dpx3, edata.dpy3);
    RS_Vector dp4 = dimensionData.definitionPoint;
    dimensionData.definitionPoint = RS_Vector(edata.dpx2, edata.dpy2);

    RS_DimAngularData d(dp1, dp2, dp3, dp4);

    RS_DimAngular* entity = new RS_DimAngular(currentContainer,
                            dimensionData, d);

    setEntityAttributes(entity);
    entity->update();
    currentContainer->addEntity(entity);
}



/**
 * Implementation of the method which handles 
 * ordinate dimensions (DIMENSION).
 */
void RS_FilterDxf::addDimOrdinate(const DL_DimensionData& data,
                               const DL_DimOrdinateData& edata) {
    RS_DEBUG->print("RS_FilterDxf::addDimOrdinate");

    RS_DimensionData dimensionData = convDimensionData(data);

    RS_Vector dp1(edata.dpx1, edata.dpy1);
    RS_Vector dp2(edata.dpx2, edata.dpy2);

    RS_DimOrdinateData d(dp1, dp2, edata.xtype);

    RS_DimOrdinate* entity = new RS_DimOrdinate(currentContainer,
                            dimensionData, d);
    setEntityAttributes(entity);
    entity->update();
    currentContainer->addEntity(entity);
}



/**
 * Implementation of the method which handles leader entities.
 */
void RS_FilterDxf::addLeader(const DL_LeaderData& data) {
    RS_DEBUG->print("RS_FilterDxf::addDimLeader");
    RS_LeaderData d(data.arrowHeadFlag==1);
    leader = new RS_Leader(currentContainer, d);
    setEntityAttributes(leader);

    currentContainer->addEntity(leader);
}



/**
 * Implementation of the method which handles leader vertices.
 */
void RS_FilterDxf::addLeaderVertex(const DL_LeaderVertexData& data) {
    RS_DEBUG->print("RS_FilterDxf::addLeaderVertex");
    //RS_DEBUG->print("RS_FilterDxf::addVertex() bulge: %f", bulge);

    RS_Vector v(data.x, data.y);

    if (leader!=NULL) {
        leader->addVertex(v);
    }
}



/**
 * Implementation of the method which handles hatch entities.
 */
void RS_FilterDxf::addHatch(const DL_HatchData& data) {
    RS_DEBUG->print("RS_FilterDxf::addHatch()");

    hatch = new RS_Hatch(currentContainer,
                         RS_HatchData(data.solid,
                                      data.scale,
                                      data.angle,
                                      RS_String(data.pattern.c_str())));
    hatch->setUpdateEnabled(false);
    setEntityAttributes(hatch);

    currentContainer->addEntity(hatch);
}



/**
 * Implementation of the method which handles hatch loops.
 */
void RS_FilterDxf::addHatchLoop(const DL_HatchLoopData& /*data*/) {
    RS_DEBUG->print("RS_FilterDxf::addHatchLoop()");
    if (hatch!=NULL) {
        hatchLoop = new RS_EntityContainer(hatch);
        hatchLoop->setLayer(NULL);
        hatch->addEntity(hatchLoop);
    }
}



/**
 * Implementation of the method which handles hatch edge entities.
 */
void RS_FilterDxf::addHatchEdge(const DL_HatchEdgeData& data) {
    RS_DEBUG->print("RS_FilterDxf::addHatchEdge()");

    if (hatchLoop!=NULL) {
        RS_Entity* e = NULL;
        switch (data.type) {
        case 1:
            RS_DEBUG->print("RS_FilterDxf::addHatchEdge(): "
                            "line: %f,%f %f,%f",
                            data.x1, data.y1, data.x2, data.y2);
            e = new RS_Line(hatchLoop,
                            RS_LineData(RS_Vector(data.x1, data.y1),
                                        RS_Vector(data.x2, data.y2)));
            break;
        case 2:
            if (data.ccw && data.angle1<1.0e-6 && data.angle2>2*M_PI-1.0e-6) {
                e = new RS_Circle(hatchLoop,
                                  RS_CircleData(RS_Vector(data.cx, data.cy),
                                                data.radius));
            } else {
                if (data.ccw) {
                    e = new RS_Arc(
                            hatchLoop,
                            RS_ArcData(RS_Vector(data.cx, data.cy),
                                       data.radius,
                                       RS_Math::correctAngle(data.angle1),
                                       RS_Math::correctAngle(data.angle2),
                                       false));
                } else {
                    e = new RS_Arc(
                            hatchLoop,
                            RS_ArcData(RS_Vector(data.cx, data.cy),
                                       data.radius,
                                       RS_Math::correctAngle(2*M_PI-data.angle1),
                                       RS_Math::correctAngle(2*M_PI-data.angle2),
                                       true));
                }
            }
            break;
        default:
            break;
        }

        if (e!=NULL) {
            e->setLayer(NULL);
            hatchLoop->addEntity(e);
        }
    }
}



/**
 * Implementation of the method which handles image entities.
 */
void RS_FilterDxf::addImage(const DL_ImageData& data) {
    RS_DEBUG->print("RS_FilterDxf::addImage");

    RS_Vector ip(data.ipx, data.ipy);
    RS_Vector uv(data.ux, data.uy);
    RS_Vector vv(data.vx, data.vy);
    RS_Vector size(data.width, data.height);

    RS_Image* image =
        new RS_Image(
            currentContainer,
            RS_ImageData(RS_String(data.ref.c_str()).toInt(NULL, 16),
                         ip, uv, vv,
                         size,
                         RS_String(""),
                         data.brightness,
                         data.contrast,
                         data.fade));

    setEntityAttributes(image);
    currentContainer->addEntity(image);
}



/**
 * Implementation of the method which links image entities to image files.
 */
void RS_FilterDxf::linkImage(const DL_ImageDefData& data) {
    RS_DEBUG->print("RS_FilterDxf::linkImage");

    int handle = RS_String(data.ref.c_str()).toInt(NULL, 16);
    RS_String sfile(data.file.c_str());
    RS_FileInfo fiDxf(file);
    RS_FileInfo fiBitmap(sfile);

    // try to find the image file:

    // first: absolute path:
    if (!fiBitmap.exists()) {
        RS_DEBUG->print("File %s doesn't exist.",
                        (const char*)QFile::encodeName(sfile));
        // try relative path:
        RS_String f1 = fiDxf.absolutePath() + "/" + sfile;
        if (RS_FileInfo(f1).exists()) {
            sfile = f1;
        } else {
            RS_DEBUG->print("File %s doesn't exist.", (const char*)QFile::encodeName(f1));
            // try drawing path:
            RS_String f2 = fiDxf.absolutePath() + "/" + fiBitmap.fileName();
            if (RS_FileInfo(f2).exists()) {
                sfile = f2;
            } else {
                RS_DEBUG->print("File %s doesn't exist.", (const char*)QFile::encodeName(f2));
            }
        }
    }

    // Also link images in subcontainers (e.g. inserts):
    for (RS_Entity* e=graphic->firstEntity(RS2::ResolveNone);
            e!=NULL; e=graphic->nextEntity(RS2::ResolveNone)) {
        if (e->rtti()==RS2::EntityImage) {
            RS_Image* img = dynamic_cast<RS_Image*>(e);
            if (img->getHandle()==handle) {
                img->setFile(sfile);
                RS_DEBUG->print("image found: %s", (const char*)QFile::encodeName(img->getFile()));
                img->update();
            }
        }
    }

    // update images in blocks:
    for (int i=0; i<graphic->countBlocks(); ++i) {
        RS_Block* b = graphic->blockAt(i);
        for (RS_Entity* e=b->firstEntity(RS2::ResolveNone);
                e!=NULL; e=b->nextEntity(RS2::ResolveNone)) {
            if (e->rtti()==RS2::EntityImage) {
                RS_Image* img = dynamic_cast<RS_Image*>(e);
                if (img->getHandle()==handle) {
                    img->setFile(sfile);
                    RS_DEBUG->print("image in block found: %s",
                                    (const char*)QFile::encodeName(img->getFile()));
                    img->update();
                }
            }
        }
    }
    RS_DEBUG->print("linking image: OK");
}



/**
 * Finishes a hatch entity.
 */
void RS_FilterDxf::endEntity() {
    RS_DEBUG->print("RS_FilterDxf::endEntity");

    if (hatch!=NULL) {
        hatch->setUpdateEnabled(true);
        if (hatch->validate()) {
            hatch->update();
        } else {
            graphic->removeEntity(hatch);
            RS_DEBUG->print(RS_Debug::D_ERROR,
                "RS_FilterDxf::endEntity(): "
                "updating hatch failed: invalid hatch area");
        }
        hatch=NULL;
    }
}



/**
 * Sets a comment. This might also be used as a way to read stored meta data 
 * from a DXF file.
 */
void RS_FilterDxf::addComment(const char* /*comment*/) {
}



/**
 * Sets a vector variable from the DXF file.
 */
void RS_FilterDxf::setVariableVector(const char* key,
                                     double v1, double v2, double v3, int code) {
    RS_DEBUG->print("RS_FilterDxf::setVariableVector");

    // update document's variable list:
    if (currentContainer->rtti()==RS2::EntityGraphic) {
        dynamic_cast<RS_Graphic*>(currentContainer)->addVariable(RS_String(key),
                RS_Vector(v1, v2, v3), code);
    }
}



/**
 * Sets a string variable from the DXF file.
 */
void RS_FilterDxf::setVariableString(const char* key,
                                     const char* value, int code) {
    RS_DEBUG->print("RS_FilterDxf::setVariableString: "
        "%s = %s (%d)", key, value, code);

    // update local DXF variable list:
    variables.add(RS_String(key), RS_String(value), code);

    // update document's variable list:
    if (currentContainer->rtti()==RS2::EntityGraphic) {
        dynamic_cast<RS_Graphic*>(currentContainer)->addVariable(RS_String(key),
                RS_String(value), code);
    }

    if (RS_String(key)=="$CLAYER") {
        currentLayer = value;
    }
}



/**
 * Sets an int variable from the DXF file.
 */
void RS_FilterDxf::setVariableInt(const char* key, int value, int code) {
    RS_DEBUG->print("RS_FilterDxf::setVariableInt: %s: %d", key, value);

    // update document's variable list:
    if (currentContainer->rtti()==RS2::EntityGraphic) {
        dynamic_cast<RS_Graphic*>(currentContainer)->addVariable(RS_String(key),
                value, code);
    }
}



/**
 * Sets a double variable from the DXF file.
 */
void RS_FilterDxf::setVariableDouble(const char* key, double value, int code) {
    RS_DEBUG->print("RS_FilterDxf::setVariableDouble");

    // update document's variable list:
    if (currentContainer->rtti()==RS2::EntityGraphic) {
        dynamic_cast<RS_Graphic*>(currentContainer)->addVariable(RS_String(key),
                value, code);
    }

}



/**
 * Implementation of the method used for RS_Export to communicate
 * with this filter.
 *
 * @param file Full path to the DXF file that will be written.
 */
bool RS_FilterDxf::fileExport(RS_Graphic& g, const RS_String& file, RS2::FormatType formatType) {

    RS_DEBUG->print("RS_FilterDxf::fileExport: exporting file '%s'...",
                    (const char*)QFile::encodeName(file));
    RS_DEBUG->print("RS_FilterDxf::fileExport: file type '%d'", (int)formatType);

    this->formatType = formatType;
    this->graphic = &g;

    // check if we can write to that directory:
#ifndef Q_OS_WIN

    RS_String path = RS_FileInfo(file).absolutePath();
    if (RS_FileInfo(path).isWritable()==false) {
        RS_DEBUG->print("RS_FilterDxf::fileExport: can't write file: "
                        "no permission");
        return false;
    }
    //
#endif

    // set version for DXF filter:
    DL_Codes::version exportVersion;
    if (formatType==RS2::FormatDxf12) {
        exportVersion = DL_Codes::AC1009;
    } else {
        exportVersion = DL_Codes::AC1015;
    }

    dw = dxf.out((const char*)QFile::encodeName(file), exportVersion);

    if (dw==NULL) {
        RS_DEBUG->print("RS_FilterDxf::fileExport: can't write file");
        return false;
    }

    // Header
    RS_DEBUG->print("writing headers...");
    dxf.writeHeader(*dw);

    // Variables
    RS_DEBUG->print("writing variables...");
    writeVariables();

    // Section TABLES
    RS_DEBUG->print("writing tables...");
    dw->sectionTables();

    // VPORT:
    dxf.writeVPort(*dw);

    // Line types:
    RS_DEBUG->print("writing line types...");
    int numLT = (int)RS2::BorderLineX2-(int)RS2::LineByBlock;
    if (formatType==RS2::FormatDxf12) {
        numLT-=2;
    }
    dw->tableLineTypes(numLT);
    for (int t=(int)RS2::LineByBlock; t<=(int)RS2::BorderLineX2; ++t) {
        if ((RS2::LineType)t!=RS2::NoPen) {
            writeLineType((RS2::LineType)t);
        }
    }
    dw->tableEnd();

    // Layers:
    RS_DEBUG->print("writing layers...");
    dw->tableLayers(graphic->countLayers());
    for (int i=0; i<graphic->countLayers(); ++i) {
        RS_Layer* l = graphic->layerAt(i);
        writeLayer(l);
    }
    dw->tableEnd();

    // STYLE:
    RS_DEBUG->print("writing styles...");
    dxf.writeStyle(*dw);

    // VIEW:
    RS_DEBUG->print("writing views...");
    dxf.writeView(*dw);

    // UCS:
    RS_DEBUG->print("writing ucs...");
    dxf.writeUcs(*dw);

    // Appid:
    RS_DEBUG->print("writing appid...");
    dw->tableAppid(1);
    writeAppid("ACAD");
    dw->tableEnd();

    // DIMSTYLE:
    RS_DEBUG->print("writing dim styles...");
    dxf.writeDimStyle(*dw, 
                      graphic->getVariableDouble("$DIMASZ", 2.5),
                      graphic->getVariableDouble("$DIMEXE", 1.25),
                      graphic->getVariableDouble("$DIMEXO", 0.625),
                      graphic->getVariableDouble("$DIMGAP", 0.625),
                      graphic->getVariableDouble("$DIMTXT", 2.5));

    // BLOCK_RECORD:
    if (formatType!=RS2::FormatDxf12) {
        RS_DEBUG->print("writing block records...");
        dxf.writeBlockRecord(*dw);

        for (int i=0; i<graphic->countBlocks(); ++i) {
            RS_Block* blk = graphic->blockAt(i);
            dxf.writeBlockRecord(*dw, 
                std::string((const char*)blk->getName().toLatin1()));
        }
        dw->tableEnd();
    }

    // end of tables:
    RS_DEBUG->print("writing end of section TABLES...");
    dw->sectionEnd();


    // Section BLOCKS:
    RS_DEBUG->print("writing blocks...");
    dw->sectionBlocks();

    if (formatType!=RS2::FormatDxf12) {
        RS_Block b1(graphic, RS_BlockData("*Model_Space",
                                          RS_Vector(0.0,0.0), false));
        writeBlock(&b1);
        RS_Block b2(graphic, RS_BlockData("*Paper_Space",
                                          RS_Vector(0.0,0.0), false));
        writeBlock(&b2);
        RS_Block b3(graphic, RS_BlockData("*Paper_Space0",
                                          RS_Vector(0.0,0.0), false));
        writeBlock(&b3);
    }

    for (int i=0; i<graphic->countBlocks(); ++i) {
        RS_Block* blk = graphic->blockAt(i);

        writeBlock(blk);
    }
    dw->sectionEnd();


    // Section ENTITIES:
    RS_DEBUG->print("writing section ENTITIES...");
    dw->sectionEntities();
    for (RS_Entity* e=graphic->firstEntity(RS2::ResolveNone);
            e!=NULL;
            e=graphic->nextEntity(RS2::ResolveNone)) {

        writeEntity(e);
    }
    RS_DEBUG->print("writing end of section ENTITIES...");
    dw->sectionEnd();

    if (formatType!=RS2::FormatDxf12) {
        RS_DEBUG->print("writing section OBJECTS...");
        dxf.writeObjects(*dw);

        // IMAGEDEF's from images in entities and images in blocks
        RS_StringList written;
        for (int i=0; i<graphic->countBlocks(); ++i) {
            RS_Block* block = graphic->blockAt(i);
            for (RS_Entity* e=block->firstEntity(RS2::ResolveAll);
                    e!=NULL;
                    e=block->nextEntity(RS2::ResolveAll)) {

                if (e->rtti()==RS2::EntityImage) {
                    RS_Image* img = dynamic_cast<RS_Image*>(e);
                    if (written.contains(file)==0 && img->getHandle()!=0) {
                        writeImageDef(img);
                        written.append(img->getFile());
                    }
                }
            }
        }
        for (RS_Entity* e=graphic->firstEntity(RS2::ResolveNone);
                e!=NULL;
                e=graphic->nextEntity(RS2::ResolveNone)) {

            if (e->rtti()==RS2::EntityImage) {
                RS_Image* img = dynamic_cast<RS_Image*>(e);
                if (written.contains(file)==0 && img->getHandle()!=0) {
                    writeImageDef(img);
                    written.append(img->getFile());
                }
            }
        }
        RS_DEBUG->print("writing end of section OBJECTS...");
        dxf.writeObjectsEnd(*dw);
    }

    RS_DEBUG->print("writing EOF...");
    dw->dxfEOF();


    RS_DEBUG->print("close..");
    dw->close();

    delete dw;
    dw = NULL;

    // check if file was actually written (strange world of windoze xp):
    if (RS_FileInfo(file).exists()==false) {
        RS_DEBUG->print("RS_FilterDxf::fileExport: file could not be written");
        return false;
    }

    return true;
}

    
 
/**
 * Writes the given comment to the DXF file (code 999).
 * Might be used for meta data.
 */
void RS_FilterDxf::writeComment(const char* comment) {
    dxf.writeComment(*dw, comment);
}


/**
 * Writes all known variable settings to the DXF file.
 */
void RS_FilterDxf::writeVariables() {
    RS_Hash<RS_String, RS_Variable>::iterator it;
    (graphic->getVariableDict());
    for (it=graphic->getVariableDict().begin(); it!=graphic->getVariableDict().end(); ++it) {
        // exclude variables that are not known to DXF 12:
        if (!DL_Dxf::checkVariable(it.key().toLatin1(), dxf.getVersion())) {
            continue;
        }

        if (it.key()!="$ACADVER" && it.key()!="$HANDSEED") {

            dw->dxfString(9, (const char*) it.key().toLatin1());
            switch (it.value().getType()) {
            case RS2::VariableVoid:
                break;
            case RS2::VariableInt:
                dw->dxfInt(it.value().getCode(), it.value().getInt());
                break;
            case RS2::VariableDouble:
                dw->dxfReal(it.value().getCode(), it.value().getDouble());
                break;
            case RS2::VariableString:
                dw->dxfString(it.value().getCode(),
                             (const char*) it.value().getString().toLatin1());
                break;
            case RS2::VariableVector:
                dw->dxfReal(it.value().getCode(),
                           it.value().getVector().x);
                dw->dxfReal(it.value().getCode()+10,
                           it.value().getVector().y);
                if (isVariableTwoDimensional(it.key())==false) {
                    dw->dxfReal(it.value().getCode()+20,
                               it.value().getVector().z);
                }
                break;
            }
        }
    }
    RS_Layer* current = graphic->getActiveLayer();
    if (current!=NULL) {
        dw->dxfString(9, "$CLAYER");
        dw->dxfString(8, (const char*)current->getName().toLatin1());
    }
    dw->sectionEnd();
}



/**
 * Writes one layer to the DXF file.
 *
 * @todo Add support for unicode layer names
 */
void RS_FilterDxf::writeLayer(RS_Layer* l) {
    if (l==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
            "RS_FilterDxf::writeLayer: layer is NULL");
        return;
    }

    RS_DEBUG->print("RS_FilterDxf::writeLayer %s", (const char*)l->getName().toLatin1());

    int colorSign = 1;
    if (!exportAllLayersVisible && l->isFrozen()) {
        colorSign = -1;
    }

    dxf.writeLayer(
        *dw,
        DL_LayerData((const char*)l->getName().toLatin1(),
                     l->isFrozen() + (l->isLocked()<<2)),
        DL_Attributes(std::string(""),
                      colorSign * colorToNumber(l->getPen().getColor()),
                      colorToNumber24(l->getPen().getColor()),
                      widthToNumber(l->getPen().getWidth()),
                      (const char*)lineTypeToName(
                          l->getPen().getLineType()).toLatin1()));

    RS_DEBUG->print("RS_FilterDxf::writeLayer end");
}



/**
 * Writes a line type to the DXF file.
 */
void RS_FilterDxf::writeLineType(RS2::LineType t) {
    dxf.writeLineType(
        *dw,
        DL_LineTypeData((const char*)lineTypeToName(t).toLatin1(), 0));
}



/**
 * Writes an application id to the DXF file.
 *
 * @param appid Application ID (e.g. "QCad").
 */
void RS_FilterDxf::writeAppid(const char* appid) {
    dxf.writeAppid(*dw, appid);
}



/**
 * Writes a block (just the definition, not the entities in it).
 */
void RS_FilterDxf::writeBlock(RS_Block* blk) {
    if (blk==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "RS_FilterDxf::writeBlock: Block is NULL");
        return;
    }

    RS_DEBUG->print("writing block: %s", (const char*)blk->getName().toLatin1());
    
    RS_String blockName = blk->getName();
    if (formatType==RS2::FormatDxf12) {
        if (blockName.at(0)=='*') {
            blockName[0] = '_';
        }
    }

    dxf.writeBlock(*dw,
                   DL_BlockData((const char*)blockName.toLatin1(), 0,
                                blk->getBasePoint().x,
                                blk->getBasePoint().y,
                                blk->getBasePoint().z));
    for (RS_Entity* e=blk->firstEntity(RS2::ResolveNone);
            e!=NULL;
            e=blk->nextEntity(RS2::ResolveNone)) {
        writeEntity(e);
    }
    dxf.writeEndBlock(*dw, (const char*)blk->getName().toLatin1());
}



/**
 * Writes the given entity to the DXF file.
 */
void RS_FilterDxf::writeEntity(RS_Entity* e) {

    if (e==NULL || e->getFlag(RS2::FlagUndone)) {
        return;
    }
    RS_DEBUG->print("writing Entity");

    attributes = getEntityAttributes(e);

    switch (e->rtti()) {
    case RS2::EntityPoint:
        writePoint(dynamic_cast<RS_Point*>(e));
        break;
    case RS2::EntityLine:
        writeLine(dynamic_cast<RS_Line*>(e));
        break;
    case RS2::EntityPolyline:
        writePolyline(dynamic_cast<RS_Polyline*>(e));
        break;
    case RS2::EntitySpline:
        writeSpline(dynamic_cast<RS_Spline*>(e));
        break;
    case RS2::EntityVertex:
        break;
    case RS2::EntityCircle:
        writeCircle(dynamic_cast<RS_Circle*>(e));
        break;
    case RS2::EntityArc:
        writeArc(dynamic_cast<RS_Arc*>(e));
        break;
    case RS2::EntityEllipse:
        writeEllipse(dynamic_cast<RS_Ellipse*>(e));
        break;
    case RS2::EntityInsert:
        writeInsert(dynamic_cast<RS_Insert*>(e));
        break;
    case RS2::EntityText:
        writeText(dynamic_cast<RS_Text*>(e));
        break;

    case RS2::EntityDimAligned:
    case RS2::EntityDimAngular:
    case RS2::EntityDimLinear:
    case RS2::EntityDimRadial:
    case RS2::EntityDimDiametric:
    case RS2::EntityDimOrdinate:
        writeDimension(dynamic_cast<RS_Dimension*>(e));
        break;
    case RS2::EntityDimLeader:
        writeLeader(dynamic_cast<RS_Leader*>(e));
        break;
    case RS2::EntityHatch:
        writeHatch(dynamic_cast<RS_Hatch*>(e));
        break;
    case RS2::EntityImage:
        writeImage(dynamic_cast<RS_Image*>(e));
        break;
    case RS2::EntitySolid:
        writeSolid(dynamic_cast<RS_Solid*>(e));
        break;
    case RS2::Entity3dFace:
        write3dFace(dynamic_cast<RS_3dFace*>(e));
        break;

#ifndef RS_NO_COMPLEX_ENTITIES
    case RS2::EntityContainer:
        writeEntityContainer(dynamic_cast<RS_EntityContainer*>(e));
        break;
#endif

    default:
        break;
    }
}



/**
 * Writes the given Point entity to the file.
 */
void RS_FilterDxf::writePoint(RS_Point* p) {
    dxf.writePoint(
        *dw,
        DL_PointData(p->getPos().x,
                     p->getPos().y,
                     0.0),
        attributes);
}


/**
 * Writes the given Line( entity to the file.
 */
void RS_FilterDxf::writeLine(RS_Line* l) {
    dxf.writeLine(
        *dw,
        DL_LineData(l->getStartpoint().x,
                    l->getStartpoint().y,
                    0.0,
                    l->getEndpoint().x,
                    l->getEndpoint().y,
                    0.0),
        attributes);
}



/**
 * Writes the given polyline entity to the file.
 */
void RS_FilterDxf::writePolyline(RS_Polyline* l) {

    int count = l->count();
    if (l->isClosed()==false) {
        count++;
    }

    dxf.writePolyline(
        *dw,
        DL_PolylineData(count,
                        0, 0,
                        l->isClosed()*0x1),
        attributes);
    bool first = true;
    RS_Entity* nextEntity = 0;
    RS_AtomicEntity* ae = NULL;
    RS_Entity* lastEntity = l->lastEntity(RS2::ResolveNone);
    for (RS_Entity* v=l->firstEntity(RS2::ResolveNone);
            v!=NULL;
            v=nextEntity) {

        nextEntity = l->nextEntity(RS2::ResolveNone);

        if (!v->isAtomic()) {
            continue;
        }

        ae = dynamic_cast<RS_AtomicEntity*>(v);
        double bulge=0.0;
        
        // Write vertex:
        if (first) {
            if (v->rtti()==RS2::EntityArc) {
                bulge = dynamic_cast<RS_Arc*>(v)->getBulge();
            }
            dxf.writeVertex(*dw,
                            DL_VertexData(ae->getStartpoint().x,
                                          ae->getStartpoint().y,
                                          0.0,
                                          bulge));
            first = false;
        }

        if (nextEntity!=NULL) {
            if (nextEntity->rtti()==RS2::EntityArc) {
                bulge = dynamic_cast<RS_Arc*>(nextEntity)->getBulge();
            }
            else {
                bulge = 0.0;
            }
        }

        if (l->isClosed()==false || v!=lastEntity) {
            dxf.writeVertex(*dw,
                        DL_VertexData(ae->getEndpoint().x,
                                      ae->getEndpoint().y,
                                      0.0,
                                      bulge));
        }
    }
    dxf.writePolylineEnd(*dw);
}



/**
 * Writes the given spline entity to the file.
 */
void RS_FilterDxf::writeSpline(RS_Spline* s) {

    // split spline into atomic entities for DXF R12:
    if (dxf.getVersion()==VER_R12) {
        writeAtomicEntities(s, RS2::ResolveNone);
        return;
    }

    if (s->getNumberOfControlPoints() < s->getDegree()+1) {
        RS_DEBUG->print(RS_Debug::D_ERROR, "RS_FilterDxf::writeSpline: "
                        "Discarding spline: not enough control points given.");
        return;
    }

    // Number of control points:
    int numCtrl = s->getNumberOfControlPoints();

    // Number of knots (= number of control points + spline degree + 1)
    int numKnots = numCtrl + s->getDegree() + 1;

    int flags;
    if (s->isClosed()) {
        flags = 11;
    } else {
        flags = 8;
    }

    // write spline header:
    dxf.writeSpline(
        *dw,
        DL_SplineData(s->getDegree(),
                      numKnots,
                      numCtrl,
                      flags),
        attributes);

    // write spline knots:
    RS_ValueList<RS_Vector> cp = s->getControlPoints();
    RS_ValueList<RS_Vector>::iterator it;

    int k = s->getDegree()+1;
    DL_KnotData kd;
    for (int i=1; i<=numKnots; i++) {
        if (i<=k) {
            kd = DL_KnotData(0.0);
        } else if (i<=numKnots-k) {
            kd = DL_KnotData(1.0/(numKnots-2*k+1) * (i-k));
        } else {
            kd = DL_KnotData(1.0);
        }
        dxf.writeKnot(*dw,
                      kd);
    }

    // write spline control points:
    for (it = cp.begin(); it!=cp.end(); ++it) {
        dxf.writeControlPoint(*dw,
                              DL_ControlPointData((*it).x,
                                                  (*it).y,
                                                  0.0));
    }
}



/**
 * Writes the given circle entity to the file.
 */
void RS_FilterDxf::writeCircle(RS_Circle* c) {
    dxf.writeCircle(
        *dw,
        DL_CircleData(c->getCenter().x,
                      c->getCenter().y,
                      0.0,
                      c->getRadius()),
        attributes);
}



void RS_FilterDxf::writeArc(RS_Arc* a) {
    double a1, a2;
    if (a->isReversed()) {
        a1 = a->getAngle2()*ARAD;
        a2 = a->getAngle1()*ARAD;
    } else {
        a1 = a->getAngle1()*ARAD;
        a2 = a->getAngle2()*ARAD;
    }
    dxf.writeArc(
        *dw,
        DL_ArcData(a->getCenter().x,
                   a->getCenter().y,
                   0.0,
                   a->getRadius(),
                   a1, a2),
        attributes);
}


void RS_FilterDxf::writeEllipse(RS_Ellipse* s) {
    if (s->isFullEllipse()) {
        dxf.writeEllipse(
            *dw,
            DL_EllipseData(s->getCenter().x,
                           s->getCenter().y,
                           0.0,
                           s->getMajorP().x,
                           s->getMajorP().y,
                           0.0,
                           s->getRatio(),
                           0.0,
                           2.0*M_PI),
            attributes);
    }
    else {
        if (s->isReversed()) {
            dxf.writeEllipse(
                *dw,
                DL_EllipseData(s->getCenter().x,
                               s->getCenter().y,
                               0.0,
                               s->getMajorP().x,
                               s->getMajorP().y,
                               0.0,
                               s->getRatio(),
                               s->getAngle2(),
                               s->getAngle1()),
                attributes);
        } else {
            dxf.writeEllipse(
                *dw,
                DL_EllipseData(s->getCenter().x,
                               s->getCenter().y,
                               0.0,
                               s->getMajorP().x,
                               s->getMajorP().y,
                               0.0,
                               s->getRatio(),
                               s->getAngle1(),
                               s->getAngle2()),
                attributes);
        }
    }
}



void RS_FilterDxf::writeInsert(RS_Insert* i) {

    RS_String blockName = i->getName();
    if (formatType==RS2::FormatDxf12) {
        if (blockName.at(0)=='*') {
            blockName[0] = '_';
        }
    }
    
    dxf.writeInsert(
        *dw,
        DL_InsertData((const char*)blockName.toLatin1(),
                      i->getInsertionPoint().x,
                      i->getInsertionPoint().y,
                      0.0,
                      i->getScale().x,
                      i->getScale().y,
                      0.0,
                      i->getAngle()*ARAD,
                      i->getCols(), i->getRows(),
                      i->getSpacing().x,
                      i->getSpacing().y),
        attributes);
}



void RS_FilterDxf::writeText(RS_Text* t) {

    if (dxf.getVersion()==VER_R12) {
        if (t->getNumberOfLines()>1) {
            // split up text into single lines:
            RS_PtrList<RS_Entity> lineList;
    
            RS_Modification modification(*currentContainer);
            modification.explodeTextIntoLines(t, lineList);

            for (int i=0; i<lineList.size(); ++i) {
                if (lineList.at(i)->rtti()==RS2::EntityText) {
                    writeText(dynamic_cast<RS_Text*>(lineList.at(i)));
                }
                else {
                    RS_DEBUG->print(RS_Debug::D_ERROR, 
                        "RS_FilterDxf::writeText: "
                        "non-text entity found after splitting "
                        "text up into lines.");
                }
            }
        }
        else {
            int hJust=0;
            int vJust=0;
            if (t->getHAlign()==RS2::HAlignLeft) {
                hJust=0;
            } else if (t->getHAlign()==RS2::HAlignCenter) {
                hJust=1;
            } else if (t->getHAlign()==RS2::HAlignRight) {
                hJust=2;
            }
            if (t->getVAlign()==RS2::VAlignTop) {
                vJust=3;
            } else if (t->getVAlign()==RS2::VAlignMiddle) {
                vJust=2;
            } else if (t->getVAlign()==RS2::VAlignBottom) {
                vJust=1;
            }
            dxf.writeText(
                *dw,
                DL_TextData(t->getInsertionPoint().x,
                            t->getInsertionPoint().y,
                            0.0,
                            t->getInsertionPoint().x,
                            t->getInsertionPoint().y,
                            0.0,
                            t->getHeight(),
                            0.8,
                            0,
                            hJust, vJust,
                            (const char*)toDxfString(
                                t->getText(), formatType).toLatin1(),
                            (const char*)t->getStyle().toLatin1(),
                            t->getAngle()),
                attributes);
        }

    } else {
        int attachmentPoint=1;
        if (t->getHAlign()==RS2::HAlignLeft) {
            attachmentPoint=1;
        } else if (t->getHAlign()==RS2::HAlignCenter) {
            attachmentPoint=2;
        } else if (t->getHAlign()==RS2::HAlignRight) {
            attachmentPoint=3;
        }
        if (t->getVAlign()==RS2::VAlignTop) {
            attachmentPoint+=0;
        } else if (t->getVAlign()==RS2::VAlignMiddle) {
            attachmentPoint+=3;
        } else if (t->getVAlign()==RS2::VAlignBottom) {
            attachmentPoint+=6;
        }

        dxf.writeMText(
            *dw,
            DL_MTextData(t->getInsertionPoint().x,
                         t->getInsertionPoint().y,
                         0.0,
                         t->getHeight(),
                         t->getWidth(),
                         attachmentPoint,
                         t->getDrawingDirection(),
                         t->getLineSpacingStyle(),
                         t->getLineSpacingFactor(),
                         (const char*)toDxfString(
                             t->getText(), formatType).toLatin1(),
                         (const char*)t->getStyle().toLatin1(),
                         t->getAngle()),
            attributes);
    }
}



void RS_FilterDxf::writeDimension(RS_Dimension* d) {

    // split hatch into atomic entities:
    if (dxf.getVersion()==VER_R12) {
        writeAtomicEntities(d, RS2::ResolveNone);
        return;
    }

    int dimType;
    int attachmentPoint=1;
    if (d->getHAlign()==RS2::HAlignLeft) {
        attachmentPoint=1;
    } else if (d->getHAlign()==RS2::HAlignCenter) {
        attachmentPoint=2;
    } else if (d->getHAlign()==RS2::HAlignRight) {
        attachmentPoint=3;
    }
    if (d->getVAlign()==RS2::VAlignTop) {
        attachmentPoint+=0;
    } else if (d->getVAlign()==RS2::VAlignMiddle) {
        attachmentPoint+=3;
    } else if (d->getVAlign()==RS2::VAlignBottom) {
        attachmentPoint+=6;
    }

    switch (d->rtti()) {
    case RS2::EntityDimAligned:
        dimType = 1;
        break;
    case RS2::EntityDimLinear:
        dimType = 0;
        break;
    case RS2::EntityDimRadial:
        dimType = 4;
        break;
    case RS2::EntityDimDiametric:
        dimType = 3;
        break;
    case RS2::EntityDimOrdinate:
        dimType = 6;
        break;
    default:
        dimType = 0;
        break;
    }

    DL_DimensionData dimData(d->getDefinitionPoint().x,
                             d->getDefinitionPoint().y,
                             0.0,
                             d->getMiddleOfText().x,
                             d->getMiddleOfText().y,
                             0.0,
                             dimType,
                             attachmentPoint,
                             d->getLineSpacingStyle(),
                             d->getLineSpacingFactor(),
                             (const char*)toDxfString(
                                 d->getText(), formatType).toLatin1(),
                             (const char*)d->getStyle().toLatin1(),
                             d->getAngle());

    if (d->rtti()==RS2::EntityDimAligned) {
        RS_DimAligned* da = dynamic_cast<RS_DimAligned*>(d);

        DL_DimAlignedData dimAlignedData(da->getExtensionPoint1().x,
                                         da->getExtensionPoint1().y,
                                         0.0,
                                         da->getExtensionPoint2().x,
                                         da->getExtensionPoint2().y,
                                         0.0);

        dxf.writeDimAligned(*dw, dimData, dimAlignedData, attributes);
    } else if (d->rtti()==RS2::EntityDimLinear) {
        RS_DimLinear* dl = dynamic_cast<RS_DimLinear*>(d);

        DL_DimLinearData dimLinearData(dl->getExtensionPoint1().x,
                                       dl->getExtensionPoint1().y,
                                       0.0,
                                       dl->getExtensionPoint2().x,
                                       dl->getExtensionPoint2().y,
                                       0.0,
                                       dl->getAngle(),
                                       dl->getOblique());

        dxf.writeDimLinear(*dw, dimData, dimLinearData, attributes);
    } else if (d->rtti()==RS2::EntityDimRadial) {
        RS_DimRadial* dr = dynamic_cast<RS_DimRadial*>(d);

        DL_DimRadialData dimRadialData(dr->getDefinitionPoint().x,
                                       dr->getDefinitionPoint().y,
                                       0.0,
                                       dr->getLeader());

        dxf.writeDimRadial(*dw, dimData, dimRadialData, attributes);
    } else if (d->rtti()==RS2::EntityDimDiametric) {
        RS_DimDiametric* dr = dynamic_cast<RS_DimDiametric*>(d);

        DL_DimDiametricData dimDiametricData(dr->getDefinitionPoint().x,
                                             dr->getDefinitionPoint().y,
                                             0.0,
                                             dr->getLeader());

        dxf.writeDimDiametric(*dw, dimData, dimDiametricData, attributes);
    } else if (d->rtti()==RS2::EntityDimAngular) {
        RS_DimAngular* da = dynamic_cast<RS_DimAngular*>(d);

        DL_DimAngularData dimAngularData(da->getDefinitionPoint1().x,
                                         da->getDefinitionPoint1().y,
                                         0.0,
                                         da->getDefinitionPoint2().x,
                                         da->getDefinitionPoint2().y,
                                         0.0,
                                         da->getDefinitionPoint3().x,
                                         da->getDefinitionPoint3().y,
                                         0.0,
                                         da->getDefinitionPoint4().x,
                                         da->getDefinitionPoint4().y,
                                         0.0);

        dxf.writeDimAngular(*dw, dimData, dimAngularData, attributes);
    } else if (d->rtti()==RS2::EntityDimOrdinate) {
        RS_DimOrdinate* dr = dynamic_cast<RS_DimOrdinate*>(d);

        DL_DimOrdinateData dimOrdinateData(dr->getDefinitionPoint1().x,
                                         dr->getDefinitionPoint1().y,
                                         0.0,
                                         dr->getDefinitionPoint2().x,
                                         dr->getDefinitionPoint2().y,
                                         0.0,
                                         dr->isXType());

        dxf.writeDimOrdinate(*dw, dimData, dimOrdinateData, attributes);
    }
}


void RS_FilterDxf::writeLeader(RS_Leader* l) {
    if (l->count()>0) {
        dxf.writeLeader(
            *dw,
            DL_LeaderData(l->hasArrowHead(),
                          0,
                          3,
                          0,
                          0,
                          1.0,
                          10.0,
                          l->count()),
            attributes);
        bool first = true;
        for (RS_Entity* v=l->firstEntity(RS2::ResolveNone);
                v!=NULL;
                v=l->nextEntity(RS2::ResolveNone)) {

            // Write line verties:
            if (v->rtti()==RS2::EntityLine) {
                RS_Line* l = dynamic_cast<RS_Line*>(v);
                if (first) {
                    dxf.writeLeaderVertex(
                        *dw,
                        DL_LeaderVertexData(l->getStartpoint().x,
                                            l->getStartpoint().y,
                                            0.0));
                    first = false;
                }
                dxf.writeLeaderVertex(
                    *dw,
                    DL_LeaderVertexData(l->getEndpoint().x,
                                        l->getEndpoint().y,
                                        0.0));
            }
        }
    } else {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "dropping leader with no vertices");
    }
}


void RS_FilterDxf::writeHatch(RS_Hatch* h) {

    // split hatch into atomic entities:
    if (dxf.getVersion()==VER_R12) {
        writeAtomicEntities(h, RS2::ResolveAll);
        return;
    }

    bool writeIt = true;
    if (h->countLoops()>0) {
        // check if all of the loops contain entities:
        for (RS_Entity* l=h->firstEntity(RS2::ResolveNone);
                l!=NULL;
                l=h->nextEntity(RS2::ResolveNone)) {

            if (l->isContainer() && !l->getFlag(RS2::FlagTemp)) {
                if (l->count()==0) {
                    writeIt = false;
                }
            }
        }
    } else {
        writeIt = false;
    }

    if (!writeIt) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "RS_FilterDxf::writeHatch: Dropping Hatch");
        return;
    }

    DL_HatchData data(h->countLoops(),
                      h->isSolid(),
                      h->getScale(),
                      h->getAngle(),
                      (const char*)h->getPattern().toLatin1());
    dxf.writeHatch1(*dw, data, attributes);

    for (RS_Entity* l=h->firstEntity(RS2::ResolveNone);
            l!=NULL;
            l=h->nextEntity(RS2::ResolveNone)) {

        // Write hatch loops:
        if (l->isContainer() && !l->getFlag(RS2::FlagTemp)) {
            RS_EntityContainer* loop = dynamic_cast<RS_EntityContainer*>(l);
            writeHatchLoop(loop);
        }
    }
    dxf.writeHatch2(*dw, data, attributes);
}
    
    
    
void RS_FilterDxf::writeHatchLoop(RS_EntityContainer* loop) {
    if (loop==NULL) {
        return;
    }

    DL_HatchLoopData lData(loop->count());
    dxf.writeHatchLoop1(*dw, lData);

    for (RS_Entity* ed=loop->firstEntity(RS2::ResolveNone);
            ed!=NULL;
            ed=loop->nextEntity(RS2::ResolveNone)) {

        // Write hatch loop edges:
        if (ed->rtti()==RS2::EntityLine) {
            RS_Line* ln = dynamic_cast<RS_Line*>(ed);
            dxf.writeHatchEdge(
                *dw,
                DL_HatchEdgeData(ln->getStartpoint().x,
                                 ln->getStartpoint().y,
                                 ln->getEndpoint().x,
                                 ln->getEndpoint().y));
        } else if (ed->rtti()==RS2::EntityArc) {
            RS_Arc* ar = dynamic_cast<RS_Arc*>(ed);
            if (!ar->isReversed()) {
                dxf.writeHatchEdge(
                    *dw,
                    DL_HatchEdgeData(ar->getCenter().x,
                                     ar->getCenter().y,
                                     ar->getRadius(),
                                     ar->getAngle1(),
                                     ar->getAngle2(),
                                     true));
            } else {
                dxf.writeHatchEdge(
                    *dw,
                    DL_HatchEdgeData(ar->getCenter().x,
                                     ar->getCenter().y,
                                     ar->getRadius(),
                                     2*M_PI-ar->getAngle1(),
                                     2*M_PI-ar->getAngle2(),
                                     false));
            }
        } else if (ed->rtti()==RS2::EntityCircle) {
            RS_Circle* ci = dynamic_cast<RS_Circle*>(ed);
            dxf.writeHatchEdge(
                *dw,
                DL_HatchEdgeData(ci->getCenter().x,
                                 ci->getCenter().y,
                                 ci->getRadius(),
                                 0.0,
                                 2*M_PI,
                                 true));
        }
    }
    dxf.writeHatchLoop2(*dw, lData);
}



void RS_FilterDxf::writeSolid(RS_Solid* s) {

    RS_Vector c1 = s->getCorner(0);
    RS_Vector c2 = s->getCorner(1);
    RS_Vector c3 = s->getCorner(2);
    RS_Vector c4 = s->getCorner(3);

    if (!c4.valid) {
        c4 = c3;
    }

    dxf.writeSolid(*dw,
        DL_SolidData(c1.x, c1.y, c1.z,
                     c2.x, c2.y, c2.z,
                     c3.x, c3.y, c3.z,
                     c4.x, c4.y, c4.z,
                     s->getThickness()),
        attributes);

    /*
    for (int i=0; i<4; ++i) {
        if (i<3 && s->getCorner(i).valid) {
            dxf.writeLine(
                *dw,
                DL_LineData(s->getCorner(i).x,
                            s->getCorner(i).y,
                            0.0,
                            s->getCorner((i+1)%4).x,
                            s->getCorner((i+1)%4).y,
                            0.0),
                attributes);
        }
    }
    */
}



void RS_FilterDxf::write3dFace(RS_3dFace* f) {

    RS_Vector c1 = f->getCorner(0);
    RS_Vector c2 = f->getCorner(1);
    RS_Vector c3 = f->getCorner(2);
    RS_Vector c4 = f->getCorner(3);
    if (!c4.valid) {
        c4 = c3;
    }

    dxf.write3dFace(*dw,
        DL_3dFaceData(c1.x, c1.y, c1.z,
                     c2.x, c2.y, c2.z,
                     c3.x, c3.y, c3.z,
                     c4.x, c4.y, c4.z),
        attributes);

    /*
    for (int i=0; i<4; ++i) {
        if (i<3 && s->getCorner(i).valid) {
            dxf.writeLine(
                *dw,
                DL_LineData(s->getCorner(i).x,
                            s->getCorner(i).y,
                            0.0,
                            s->getCorner((i+1)%4).x,
                            s->getCorner((i+1)%4).y,
                            0.0),
                attributes);
        }
    }
    */
}


void RS_FilterDxf::writeImage(RS_Image* i) {
    int handle = dxf.writeImage(
                     *dw,
                     DL_ImageData(std::string(""),
                                  i->getInsertionPoint().x,
                                  i->getInsertionPoint().y,
                                  0.0,
                                  i->getUVector().x,
                                  i->getUVector().y,
                                  0.0,
                                  i->getVVector().x,
                                  i->getVVector().y,
                                  0.0,
                                  i->getWidth(),
                                  i->getHeight(),
                                  i->getBrightness(),
                                  i->getContrast(),
                                  i->getFade()),
                     attributes);
    i->setHandle(handle);
}



void RS_FilterDxf::writeEntityContainer(RS_EntityContainer* con) {
    RS_String blkName;
    blkName = "__CE";

    // Creating an unique ID from the element ID
    int tmp, c=1; // tmp = temporary var c = counter var
    tmp = con->getId();

    while (true) {
        tmp = tmp/c;
        blkName.append((char) tmp %10 + 48);
        c *= 10;
        if (tmp < 10) {
            break;
        }
    }

    //Block definition
    dw->sectionTables();
    dxf.writeBlockRecord(*dw);
    dw->dxfString(  0, "BLOCK_RECORD");

    dw->handle();
    dw->dxfHex(330, 1);
    dw->dxfString(100, "AcDbSymbolTableRecord");
    dw->dxfString(100, "AcDbBlockTableRecord");
    dw->dxfString(  2, blkName.toLatin1());
    dw->dxfHex(340, 0);
    dw->dxfString(0, "ENDTAB");

    //Block creation
    RS_BlockData blkdata(blkName, RS_Vector(0,0), false);

    RS_Block* blk = new RS_Block(graphic, blkdata);

    for (RS_Entity* e1 = con->firstEntity(); e1 != NULL;
            e1 = con->nextEntity() ) {
        blk->addEntity(e1);
    }
    writeBlock(blk);
    //delete e1;

}



/**
 * Writes the atomic entities of the given container to the file.
 */
void RS_FilterDxf::writeAtomicEntities(RS_EntityContainer* c,
                                       RS2::ResolveLevel level) {

    for (RS_Entity* e=c->firstEntity(level);
            e!=NULL;
            e=c->nextEntity(level)) {

        writeEntity(e);
    }
}

/**
 * Writes an IMAGEDEF object into an OBJECT section.
 */
void RS_FilterDxf::writeImageDef(RS_Image* i) {
    if (i==NULL || i->getFlag(RS2::FlagUndone)) {
        return;
    }

    dxf.writeImageDef(
        *dw,
        i->getHandle(),
        DL_ImageData((const char*)i->getFile().toLatin1(),
                     i->getInsertionPoint().x,
                     i->getInsertionPoint().y,
                     0.0,
                     i->getUVector().x,
                     i->getUVector().y,
                     0.0,
                     i->getVVector().x,
                     i->getVVector().y,
                     0.0,
                     i->getWidth(),
                     i->getHeight(),
                     i->getBrightness(),
                     i->getContrast(),
                     i->getFade()));
}



/**
 * Sets the entities attributes according to the attributes
 * that come from a DXF file.
 */
void RS_FilterDxf::setEntityAttributes(RS_Entity* entity) {
    RS_DEBUG->print("RS_FilterDxf::setEntityAttributes");

    RS_Pen pen;
    pen.setColor(RS_Color(0,0,0));
    pen.setLineType(RS2::SolidLine);

    // Layer:
    if (attributes.getLayer().empty()) {
        RS_DEBUG->print("RS_FilterDxf::setEntityAttributes: resort to layer: '0'");
        entity->setLayer("0");
    } else {
        // add layer in case it doesn't exist:
        if (graphic->findLayer(attributes.getLayer().c_str())==NULL) {
            RS_DEBUG->print("RS_FilterDxf::setEntityAttributes: "
                "creating layer: '%s'", attributes.getLayer().c_str());
            addLayer(DL_LayerData(attributes.getLayer(), 0));
        }

        entity->setLayer(attributes.getLayer().c_str());

        RS_DEBUG->print("RS_FilterDxf::setEntityAttributes: "
            "set layer: '%s'", attributes.getLayer().c_str());
    }

    // Color:
    RS_Color col = numberToColor(attributes.getColor());
    RS_Color col24 = numberToColor24(attributes.getColor24());

    // bylayer / byblock overrules all colors:
    if (col.isByBlock() || col.isByLayer()) {
        pen.setColor(col);
    }
    else {
        // 24 bit colors overrule palette colors:
        if (attributes.getColor24()!=-1) {
            pen.setColor(col24);
        }
        else {
            pen.setColor(col);
        }
    }

    // Linetype:
    pen.setLineType(nameToLineType(attributes.getLineType().c_str()));

    // Width:
    pen.setWidth(numberToWidth(attributes.getWidth()));

    entity->setPen(pen);
    RS_DEBUG->print("RS_FilterDxf::setEntityAttributes: OK");
}



/**
 * Gets the entities attributes as a DL_Attributes object.
 */
DL_Attributes RS_FilterDxf::getEntityAttributes(RS_Entity* entity) {

    // Layer:
    RS_Layer* layer = entity->getLayer();
    RS_String layerName;
    if (layer!=NULL) {
        layerName = layer->getName();
    } else {
        layerName = "NULL";
    }

    RS_Pen pen = entity->getPen(false);

    // Color:
    int color = colorToNumber(pen.getColor());
    int color24 = colorToNumber24(pen.getColor());

    // Linetype:
    RS_String lineType = lineTypeToName(pen.getLineType());

    // Width:
    int width = widthToNumber(pen.getWidth());

    DL_Attributes attrib((const char*)layerName.toLatin1(),
                         color,
                         color24,
                         width,
                         (const char*)lineType.toLatin1());

    return attrib;
}



/**
 * @return Pen with the same attributes as 'attributes'.
 */
RS_Pen RS_FilterDxf::attributesToPen(bool forLayer) const {
    RS_Color color;
    if (attributes.getColor24()!=-1) {
        color = numberToColor24(attributes.getColor24());
    }
    else {
        color = numberToColor(attributes.getColor(), false, forLayer);
    }

    RS_Pen pen(
        color,
        numberToWidth(attributes.getWidth()),
        nameToLineType(attributes.getLineType().c_str())
    );
    return pen;
}



/**
 * Converts a color index (num) into a RS_Color object.
 * Please refer to the dxflib documentation for details.
 *
 * @param num Color number.
 * @param comp Compatibility with older QCad versions (1.5.3 and older)
 */
RS_Color RS_FilterDxf::numberToColor(int num, bool comp, bool forLayer) {
    if (forLayer) {
        num = abs(num);
    }
    
    // Compatibility with QCad 1.5.3 and older:
    if (comp) {
        switch(num) {
        case 0:
            return RS_Color(Qt::black);
            break;
        case 1:
            return RS_Color(Qt::darkBlue);
            break;
        case 2:
            return RS_Color(Qt::darkGreen);
            break;
        case 3:
            return RS_Color(Qt::darkCyan);
            break;
        case 4:
            return RS_Color(Qt::darkRed);
            break;
        case 5:
            return RS_Color(Qt::darkMagenta);
            break;
        case 6:
            return RS_Color(Qt::darkYellow);
            break;
        case 7:
            return RS_Color(Qt::lightGray);
            break;
        case 8:
            return RS_Color(Qt::darkGray);
            break;
        case 9:
            return RS_Color(Qt::blue);
            break;
        case 10:
            return RS_Color(Qt::green);
            break;
        case 11:
            return RS_Color(Qt::cyan);
            break;
        case 12:
            return RS_Color(Qt::red);
            break;
        case 13:
            return RS_Color(Qt::magenta);
            break;
        case 14:
            return RS_Color(Qt::yellow);
            break;
        case 15:
            return RS_Color(Qt::black);
            break;
        default:
            break;
        }
    } else {
        if (num==0) {
            return RS_Color(RS2::FlagByBlock);
        } else if (num==256) {
            return RS_Color(RS2::FlagByLayer);
        } else if (num<=255 && num>=0) {
            return RS_Color((int)(dxfColors[num][0]*255),
                            (int)(dxfColors[num][1]*255),
                            (int)(dxfColors[num][2]*255));
        } else {
            RS_DEBUG->print(RS_Debug::D_WARNING,
                "RS_FilterDxf::numberToColor: Invalid color number given.");
            return RS_Color(RS2::FlagByLayer);
        }
    }
    return RS_Color();
}



/**
 * @return color object from DXF coded integar 24 bit color number.
 */
RS_Color RS_FilterDxf::numberToColor24(int num) {
    return RS_Color(
              (num&0x00ff0000) >> 16,
              (num&0x0000ff00) >> 8,
              (num&0x000000ff) >> 0
           );
}



/**
 * Converts a color into a color number in the DXF palette.
 * The color that fits best is chosen.
 */
int RS_FilterDxf::colorToNumber(const RS_Color& col) {

    // Special color BYBLOCK:
    if (col.getFlag(RS2::FlagByBlock)) {
        return 0;
    }

    // Special color BYLAYER
    else if (col.getFlag(RS2::FlagByLayer)) {
        return 256;
    }

    // Special color black is not in the table but white represents both
    // black and white
    else if (col.red()==0 && col.green()==0 && col.blue()==0) {
        return 7;
    }

    // All other colors
    else {
        int num=0;
        int diff=255*3;  // smallest difference to a color in the table found so far

        // Run through the whole table and compare
        for (int i=1; i<=255; i++) {
            int d = abs(col.red()-(int)(dxfColors[i][0]*255))
                    + abs(col.green()-(int)(dxfColors[i][1]*255))
                    + abs(col.blue()-(int)(dxfColors[i][2]*255));

            if (d<diff) {
                diff = d;
                num = i;
                if (d==0) {
                    break;
                }
            }
        }
        return num;
    }
}



/**
 * @return 24 bit color as DXF prepared int.
 */
int RS_FilterDxf::colorToNumber24(const RS_Color& col) {
    RS_DEBUG->print("RS_FilterDxf::colorToNumber24: %d", 
        (col.red()<<16) | (col.green()<<8) | (col.blue()<<0));

    if (col.isByLayer() || col.isByBlock()) {
        return -1;
    }
    else {
        return (col.red()<<16) | (col.green()<<8) | (col.blue()<<0);
    }
}



/**
 * Converts a line type name (e.g. "CONTINUOUS") into a RS2::LineType
 * object.
 */
RS2::LineType RS_FilterDxf::nameToLineType(const RS_String& name) {

    RS_String uName = name.toUpper();

    // Standard linetypes for QCad II / AutoCAD
    if (uName.isEmpty() || uName=="BYLAYER") {
        return RS2::LineByLayer;

    } else if (uName=="BYBLOCK") {
        return RS2::LineByBlock;

    } else if (uName=="CONTINUOUS" || uName=="ACAD_ISO01W100") {
        return RS2::SolidLine;

    } else if (uName=="ACAD_ISO07W100" || uName=="DOT") {
        return RS2::DotLine;

    } else if (uName=="DOT2") {
        return RS2::DotLine2;

    } else if (uName=="DOTX2") {
        return RS2::DotLineX2;


    } else if (uName=="ACAD_ISO02W100" || uName=="ACAD_ISO03W100" ||
               uName=="DASHED" || uName=="HIDDEN") {
        return RS2::DashLine;

    } else if (uName=="DASHED2" || uName=="HIDDEN2") {
        return RS2::DashLine2;

    } else if (uName=="DASHEDX2" || uName=="HIDDENX2") {
        return RS2::DashLineX2;


    } else if (uName=="ACAD_ISO10W100" ||
               uName=="DASHDOT") {
        return RS2::DashDotLine;

    } else if (uName=="DASHDOT2") {
        return RS2::DashDotLine2;

    } else if (uName=="ACAD_ISO04W100" ||
               uName=="DASHDOTX2") {
        return RS2::DashDotLineX2;


    } else if (uName=="ACAD_ISO12W100" || uName=="DIVIDE") {
        return RS2::DivideLine;

    } else if (uName=="DIVIDE2") {
        return RS2::DivideLine2;

    } else if (uName=="ACAD_ISO05W100" || uName=="DIVIDEX2") {
        return RS2::DivideLineX2;


    } else if (uName=="CENTER") {
        return RS2::CenterLine;

    } else if (uName=="CENTER2") {
        return RS2::CenterLine2;

    } else if (uName=="CENTERX2") {
        return RS2::CenterLineX2;


    } else if (uName=="BORDER") {
        return RS2::BorderLine;

    } else if (uName=="BORDER2") {
        return RS2::BorderLine2;

    } else if (uName=="BORDERX2") {
        return RS2::BorderLineX2;
    }

    return RS2::SolidLine;
}



/**
 * Converts a RS_LineType into a name for a line type.
 */
RS_String RS_FilterDxf::lineTypeToName(RS2::LineType lineType) {

    // Standard linetypes for QCad II / AutoCAD
    switch (lineType) {

    case RS2::SolidLine:
        return "CONTINUOUS";
        break;

    case RS2::DotLine:
        return "DOT";
        break;
    case RS2::DotLine2:
        return "DOT2";
        break;
    case RS2::DotLineX2:
        return "DOTX2";
        break;

    case RS2::DashLine:
        return "DASHED";
        break;
    case RS2::DashLine2:
        return "DASHED2";
        break;
    case RS2::DashLineX2:
        return "DASHEDX2";
        break;

    case RS2::DashDotLine:
        return "DASHDOT";
        break;
    case RS2::DashDotLine2:
        return "DASHDOT2";
        break;
    case RS2::DashDotLineX2:
        return "DASHDOTX2";
        break;

    case RS2::DivideLine:
        return "DIVIDE";
        break;
    case RS2::DivideLine2:
        return "DIVIDE2";
        break;
    case RS2::DivideLineX2:
        return "DIVIDEX2";
        break;

    case RS2::CenterLine:
        return "CENTER";
        break;
    case RS2::CenterLine2:
        return "CENTER2";
        break;
    case RS2::CenterLineX2:
        return "CENTERX2";
        break;

    case RS2::BorderLine:
        return "BORDER";
        break;
    case RS2::BorderLine2:
        return "BORDER2";
        break;
    case RS2::BorderLineX2:
        return "BORDERX2";
        break;


    case RS2::LineByLayer:
        return "ByLayer";
        break;
    case RS2::LineByBlock:
        return "ByBlock";
        break;
    default:
        break;
    }

    return "CONTINUOUS";
}



/**
 * Converts a line width number (e.g. 1) into a RS2::LineWidth.
 */
RS2::LineWidth RS_FilterDxf::numberToWidth(int num) {
    switch (num) {
    case -1:
        return RS2::WidthByLayer;
        break;
    case -2:
        return RS2::WidthByBlock;
        break;
    case -3:
        return RS2::WidthDefault;
        break;
    default:
        if (num<3) {
            return RS2::Width00;
        } else if (num<7) {
            return RS2::Width01;
        } else if (num<11) {
            return RS2::Width02;
        } else if (num<14) {
            return RS2::Width03;
        } else if (num<16) {
            return RS2::Width04;
        } else if (num<19) {
            return RS2::Width05;
        } else if (num<22) {
            return RS2::Width06;
        } else if (num<27) {
            return RS2::Width07;
        } else if (num<32) {
            return RS2::Width08;
        } else if (num<37) {
            return RS2::Width09;
        } else if (num<45) {
            return RS2::Width10;
        } else if (num<52) {
            return RS2::Width11;
        } else if (num<57) {
            return RS2::Width12;
        } else if (num<65) {
            return RS2::Width13;
        } else if (num<75) {
            return RS2::Width14;
        } else if (num<85) {
            return RS2::Width15;
        } else if (num<95) {
            return RS2::Width16;
        } else if (num<103) {
            return RS2::Width17;
        } else if (num<112) {
            return RS2::Width18;
        } else if (num<130) {
            return RS2::Width19;
        } else if (num<149) {
            return RS2::Width20;
        } else if (num<180) {
            return RS2::Width21;
        } else if (num<205) {
            return RS2::Width22;
        } else {
            return RS2::Width23;
        }
        break;
    }
    return (RS2::LineWidth)num;
}



/**
 * Converts a RS2::LineWidth into an int width.
 */
int RS_FilterDxf::widthToNumber(RS2::LineWidth width) {
    switch (width) {
    case RS2::WidthByLayer:
        return -1;
        break;
    case RS2::WidthByBlock:
        return -2;
        break;
    case RS2::WidthDefault:
        return -3;
        break;
    default:
        return (int)width;
        break;
    }
    return (int)width;
}



/**
 * Converts a native unicode string into a DXF encoded string.
 *
 * DXF endoding includes the following special sequences:
 * - %%%c for a diameter sign
 * - %%%d for a degree sign
 * - %%%p for a plus/minus sign
 */
RS_String RS_FilterDxf::toDxfString(const RS_String& string, RS2::FormatType formatType) {

    RS_String res = "";

    for (int i=0; i<string.length(); ++i) {
        int c = string.at(i).unicode();
        switch (c) {
        case 0x0A:
            res+="\\P";
            break;
        case 0x20:
            if (formatType==RS2::FormatDxf12) {
                res+=" ";
            }
            else {
                res+="\\~";
            }
            break;
            // diameter:
        case 0x2205:
            res+="%%c";
            break;
            // degree:
        case 0x00B0:
            res+="%%d";
            break;
            // plus/minus
        case 0x00B1:
            res+="%%p";
            break;
        default:
            if (c>127) {
                RS_String hex;
                hex = RS_String("%1").arg(c, 4, 16);
                hex = hex.replace(' ', '0');

                res+=RS_String("\\U+%1").arg(hex);
            } else {
                res+=string.at(i);
            }
            break;
        }
    }
            
    return res;
}



/**
 * Converts a DXF encoded string into a native Unicode string.
 */
RS_String RS_FilterDxf::toNativeString(const RS_String& string) {
    RS_String res = string;
    // Line feed:
    res = res.replace(RS_RegExp("\\\\P"), "\n");
    // Space:
    res = res.replace(RS_RegExp("\\\\~"), " ");
    // diameter:
    res = res.replace(RS_RegExp("%%c"), QString("%1").arg(QChar(0x2205)));
    res = res.replace(RS_RegExp("%%C"), QString("%1").arg(QChar(0x2205)));
    // degree:
    res = res.replace(RS_RegExp("%%d"), QString("%1").arg(QChar(0x00B0)));
    res = res.replace(RS_RegExp("%%D"), QString("%1").arg(QChar(0x00B0)));
    // plus/minus
    res = res.replace(RS_RegExp("%%p"), QString("%1").arg(QChar(0x00B1)));
    res = res.replace(RS_RegExp("%%P"), QString("%1").arg(QChar(0x00B1)));

    // Unicode characters:
    RS_String cap = "";
    int uCode = 0;
    bool ok = false;
    do {
        RS_RegExp regexp("\\\\U\\+[0-9A-Fa-f]{4,4}");
        regexp.indexIn(res);
        cap = regexp.cap();
        if (!cap.isNull()) {
            uCode = cap.right(4).toInt(&ok, 16);
            RS_DEBUG->print(RS_String("dxf unicode: %1 is character %2").arg(cap.right(4)).arg(uCode));
            res.replace(cap, QChar(uCode));
        }
    } while (!cap.isNull());

    // ASCII code:
    cap = "";
    uCode = 0;
    ok = false;
    do {
        RS_RegExp regexp("%%[0-9]{3,3}");
        regexp.indexIn(res);
        cap = regexp.cap();
        if (!cap.isNull()) {
            uCode = cap.right(3).toInt(&ok, 10);
            res.replace(cap, QChar(uCode));
        }
    }
    while (!cap.isNull());

    // Ignore font tags:
    res = res.replace(RS_RegExp("\\\\f[0-9A-Za-z| ]{0,};"), "");

    // Ignore {}
    res = res.replace("\\{", "#curly#");
    res = res.replace("{", "");
    res = res.replace("#curly#", "{");

    res = res.replace("\\}", "#curly#");
    res = res.replace("}", "");
    res = res.replace("#curly#", "}");

    RS_DEBUG->print("RS_FilterDxf::toNativeString:");
    RS_DEBUG->printUnicode(res);
    return res;
}



/**
 * Converts the given number from a DXF file into an AngleFormat enum.
 *
 * @param num $DIMAUNIT from DXF (0: decimal deg, 1: deg/min/sec, 2: gradians,
 *                                3: radians, 4: surveyor's units)
 *
 * @ret Matching AngleFormat enum value.
 */
RS2::AngleFormat RS_FilterDxf::numberToAngleFormat(int num) {

    RS2::AngleFormat af;

    switch (num) {
    default:
    case 0:
        af = RS2::DegreesDecimal;
        break;
    case 1:
        af = RS2::DegreesMinutesSeconds;
        break;
    case 2:
        af = RS2::Gradians;
        break;
    case 3:
        af = RS2::Radians;
        break;
    case 4:
        af = RS2::Surveyors;
        break;
    }

    return af;
}


/**
 * Converts AngleFormat enum to DXF number.
 */
int RS_FilterDxf::angleFormatToNumber(RS2::AngleFormat af) {

    int num;

    switch (af) {
    default:
    case RS2::DegreesDecimal:
        num = 0;
        break;
    case RS2::DegreesMinutesSeconds:
        num = 1;
        break;
    case RS2::Gradians:
        num = 2;
        break;
    case RS2::Radians:
        num = 3;
        break;
    case RS2::Surveyors:
        num = 4;
        break;
    }

    return num;
}



/**
 * converts a DXF unit setting (e.g. INSUNITS) to a unit enum.
 */
RS2::Unit RS_FilterDxf::numberToUnit(int num) {
    switch (num) {
    default:
    case  0:
        return RS2::None;
        break;
    case  1:
        return RS2::Inch;
        break;
    case  2:
        return RS2::Foot;
        break;
    case  3:
        return RS2::Mile;
        break;
    case  4:
        return RS2::Millimeter;
        break;
    case  5:
        return RS2::Centimeter;
        break;
    case  6:
        return RS2::Meter;
        break;
    case  7:
        return RS2::Kilometer;
        break;
    case  8:
        return RS2::Microinch;
        break;
    case  9:
        return RS2::Mil;
        break;
    case 10:
        return RS2::Yard;
        break;
    case 11:
        return RS2::Angstrom;
        break;
    case 12:
        return RS2::Nanometer;
        break;
    case 13:
        return RS2::Micron;
        break;
    case 14:
        return RS2::Decimeter;
        break;
    case 15:
        return RS2::Decameter;
        break;
    case 16:
        return RS2::Hectometer;
        break;
    case 17:
        return RS2::Gigameter;
        break;
    case 18:
        return RS2::Astro;
        break;
    case 19:
        return RS2::Lightyear;
        break;
    case 20:
        return RS2::Parsec;
        break;
    }

    return RS2::None;
}



/**
 * Converst a unit enum into a DXF unit number e.g. for INSUNITS.
 */
int RS_FilterDxf::unitToNumber(RS2::Unit unit) {
    switch (unit) {
    default:
    case RS2::None:
    case RS2::Plu:
        return  0;
        break;
    case RS2::Inch:
        return  1;
        break;
    case RS2::Foot:
        return  2;
        break;
    case RS2::Mile:
        return  3;
        break;
    case RS2::Millimeter:
        return  4;
        break;
    case RS2::Centimeter:
        return  5;
        break;
    case RS2::Meter:
        return  6;
        break;
    case RS2::Kilometer:
        return  7;
        break;
    case RS2::Microinch:
        return  8;
        break;
    case RS2::Mil:
        return  9;
        break;
    case RS2::Yard:
        return 10;
        break;
    case RS2::Angstrom:
        return 11;
        break;
    case RS2::Nanometer:
        return 12;
        break;
    case RS2::Micron:
        return 13;
        break;
    case RS2::Decimeter:
        return 14;
        break;
    case RS2::Decameter:
        return 15;
        break;
    case RS2::Hectometer:
        return 16;
        break;
    case RS2::Gigameter:
        return 17;
        break;
    case RS2::Astro:
        return 18;
        break;
    case RS2::Lightyear:
        return 19;
        break;
    case RS2::Parsec:
        return 20;
        break;
    }

    return 0;
}



/**
 * Checks if the given variable is two-dimensional (e.g. $LIMMIN).
 */
bool RS_FilterDxf::isVariableTwoDimensional(const RS_String& var) {
    if (var=="$LIMMIN" ||
            var=="$LIMMAX" ||
            var=="$PLIMMIN" ||
            var=="$PLIMMAX" ||
            var=="$GRIDUNIT" ||
            var=="$VIEWCTR") {

        return true;
    } else {
        return false;
    }
}
