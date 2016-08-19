/*****************************************************************
 * $Id: rb_xmlsvg.cpp,v 1.49 2004/07/13 23:25:28 - rutger $
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the PNID project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_xmlsvg.h"

#include <algorithm>
#include <string>
#include <cstdio>
#include <cassert>
#include <cmath>

#include <QFile>
#include <QTextCodec>

#include "rs_debug.h"
#include "rs_color.h"
#include "rs_graphic.h"
#include "rs_math.h"
#include "rs_units.h"

#if defined(__OS2__)||defined(__EMX__)||defined(_WIN32)
#define strcasecmp(s,t) stricmp(s,t)
#endif

#ifdef _WIN32
#undef M_PI
#define M_PI   3.14159265358979323846
#pragma warning(disable : 4800)
#endif

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

/**
 * Default constructor.
 */
RB_XmlSvg::RB_XmlSvg() {
    styleHandleStd = 0;

    vertices = NULL;
    maxVertices = 0;
    vertexIndex = 0;

    leaderVertices = NULL;
    maxLeaderVertices = 0;
    leaderVertexIndex = 0;

    hatchLoops = NULL;
    maxHatchLoops = 0;
    hatchLoopIndex = -1;
    hatchEdges = NULL;
    maxHatchEdges = NULL;
    hatchEdgeIndex = NULL;
    dropEdges = false;

    //bulge = 0.0;
    
    // vectorLayerStyles = NULL;
}



/**
 * Destructor.
 */
RB_XmlSvg::~RB_XmlSvg() {
    if (vertices!=NULL) {
        delete[] vertices;
    }
    if (leaderVertices!=NULL) {
        delete[] leaderVertices;
    }
    if (hatchLoops!=NULL) {
        delete[] hatchLoops;
    }
    if (hatchEdges!=NULL) {
        for (int i=0; i<maxHatchLoops; ++i) {
            if (hatchEdges[i]!=NULL) {
                delete[] hatchEdges[i];
            }
        }
        delete[] hatchEdges;
    }
    if (maxHatchEdges!=NULL) {
        delete[] maxHatchEdges;
    }
    if (hatchEdgeIndex!=NULL) {
        delete[] hatchEdgeIndex;
    }
    
//    if (mapLayerStyles!=NULL) {
//        delete mapLayerStyles;
//    }

}


/**
 * Opens the given file for writing and returns a pointer
 * to the XML writer. This pointer needs to be passed on to other
 * writing functions.
 *
 * @param file Full path of the file to open.
 *
 * @return Pointer to an XML writer object.
 */
RB_XmlWriter* RB_XmlSvg::out(const char* file, RB_XmlCodes::TypeML typeML) {
    RS_DEBUG->print("RB_XmlSvg::out...");
	char* f = new char[strlen(file)+1];
    strcpy(f, file);

	this->file = file;

	RB_XmlWriter* xw = new RB_XmlWriter(f, typeML);
	if (xw->openFailed()) {
		delete xw;
		delete[] f;
		return NULL;
	} else {
		delete[] f;
		return xw;
	}
}


/**
 * @brief Writes a SVG header to the file currently opened 
 * by the given SVG writer object.
 */
void RB_XmlSvg::writeHeader(RB_XmlWriter& xw) {
    RS_DEBUG->print("RB_XmlSvg::writeHeader...");
	xw.headerElement();
	xw.docTypeElement("svg PUBLIC \"-//W3C//DTD SVG 20001102//EN\" \"http://www.w3.org/TR/2000/CR-SVG-20001102/DTD/svg-20001102.dtd\"");
//    xw.comment("svglib ", XML_VERSION);

}

void RB_XmlSvg::writeRoot(RB_XmlWriter& xw) {
    RS_DEBUG->print("RB_XmlSvg::writeRoot...");
	xw.insertAttribute("xmlns", "http://www.w3.org/2000/svg");
	xw.insertAttribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
	xw.insertAttribute("version", "1.2");
	xw.insertAttribute("baseProfile", "tiny");
	xw.insertAttribute("width", QString::number(getSize().width()) + "mm");
	xw.insertAttribute("height", QString::number(getSize().height()) + "mm");
	xw.insertAttribute("x", 0);
	xw.insertAttribute("y", 0);
	xw.insertAttribute("id", getFile());
	
	if (this->black) {
		xw.insertAttribute("style", "background:#000000");
	}
    
    // create SVG viewBox attribute
    QString viewBox = "";
    QString vbString;
    vbString = vbString.setNum(getPaperInsertionBase().x);
    viewBox += vbString;
    vbString = vbString.setNum(getPaperInsertionBase().y);
    viewBox += " " + vbString;
    vbString = vbString.setNum(getPaperSize().x);
    viewBox += " " + vbString;
    vbString = vbString.setNum(getPaperSize().y);
    viewBox += " " + vbString;
	xw.insertAttribute("viewBox", viewBox);

	xw.push("svg");
}


/**
 *	testEmptyElement
 * /
void RB_XmlSvg::writeTest(RB_XmlWriter& xw) {
    RS_DEBUG->print("RB_XmlSvg::test...");
/ *	xw.insertAttribute("file", this->file);
    xw.emptyElement("file");
	xw.insertAttribute("format", this->format);
    xw.emptyElement("format");
	xw.insertAttribute("sizeWidth", this->size.width());
	xw.insertAttribute("sizeHeight", this->size.height());
    xw.emptyElement("size");
	xw.insertAttribute("black", this->black);
    xw.emptyElement("black");
	xw.insertAttribute("bw", this->bw);
    xw.emptyElement("bw");
	xw.insertAttribute("papersizeX", this->paperSize.x);
	xw.insertAttribute("papersizeY", this->paperSize.y);
    xw.emptyElement("papersize");
	xw.insertAttribute("paperInsertionBaseX", this->graphicInsertionBase.x);
	xw.insertAttribute("paperInsertionBaseY", this->graphicInsertionBase.y);
    xw.emptyElement("graphicInsertionBase");
	xw.insertAttribute("paperScale", this->graphicScale);
    xw.emptyElement("graphicScale"); */
//	xw.insertAttribute("unit", this->unit);
//    xw.emptyElement("unit");

/*	For reference only:
    QString file;                   // is full path with file name
    QString format;                 // format is SVG or svg1_1
    QSize size;                     // size of overall SVG graphic in pixels 
    bool black;                     // background in black if true
    bool bw;                        // entities in black and white
    
   	RS_Vector paperSize;            // overall paper size
	RS_Vector paperInsertionBase;   // the shift of the origine on the paper   
	double paperScale;              // the scaling compared to the paper unit
    RS2::Unit unit;                 // unit of measurement
* /
}
*/



/**
 *	closing tag </...> for example </svg> and </g>
 */
void RB_XmlSvg::closeElement(RB_XmlWriter& xw) {
    RS_DEBUG->print("RB_XmlSvg::closeElement...");
	xw.pop();
}


/**
 * Set individual layer and block styles in 
 * vector template RB_XmlSvg.vectorLayerStyles
 */
void RB_XmlSvg::setStyles(const RB_SvgCodes& xc) {
    RS_DEBUG->print("RB_XmlSvg::setLayerStyle...");
    
    if (xc.name == "") {
        std::cerr << "RB_XmlSvg::writeLayer: "
        << "Layer name must not be empty\n";
    } else {
        vectorLayerStyles.push_back(xc);
    }
}


/**
 *	write all the layer styles with <![CDATA[....]]> element
 */
void RB_XmlSvg::writeLayerStyles(RB_XmlWriter& xw) {
    RS_DEBUG->print("RB_XmlSvg::writeLayerStyles...");
    QString str;
    QString indent = xw.getIndent() + "  ";
    QString sWidth = "0.2";
    
    // Run through all layer styles and format content
    QVector<RB_SvgCodes>::iterator it = vectorLayerStyles.begin();

    while (it != vectorLayerStyles.end()) {
        if (atof(it->strokeWidth.toAscii()) > 0.0) {
            sWidth = it->strokeWidth;
        } else {
            sWidth = "0.2";
        }

        str += "\n";
        str += indent + "g." + it->name + " "; 
        str += "{opacity:" + it->opacity + ";";
        str += "\n" + indent + "fill:" + it->fill + ";";
        str += "\n" + indent + "font-family:" + it->fontFamily + ";";
        str += "\n" + indent + "font-stretch:" + it->fontStretch + ";";
        str += "\n" + indent + "stroke-linecap:" + it->strokeLineCap + ";";
        str += "\n" + indent + "stroke-linejoin:" + it->strokeLineJoin + ";";
        str += "\n" + indent + "stroke:" + it->stroke + ";";
        str += "\n" + indent + "stroke-opacity:" + it->strokeOpacity + ";";
        str += "\n" + indent + "stroke-width:" + sWidth + ";";
        if (it->dashArray != "") {
            str += "\n" + indent + "stroke-dasharray:" + it->dashArray + ";";
        }
        if (it->display != "") {
            str += "\n" + indent + "display:" + it->display + ";";
        }
        str +=  + "}";
        
        // can be expanded as RB_SvgCodes contains more SVG styles
        
        ++it;
    }
    
    xw.cdataElement(str);
    vectorLayerStyles.clear();
}


/**
 * Writes style element and all layer styles to the file. Use setLayerStyles
 *   to set style per layer.
 * Layers are stored in the style section at the start of a SVG file in 
 *	<style type="text/css">
 *		<![CDATA[....]]>
 *	</style>
 *
 * @param xw SVG writer
 */
void RB_XmlSvg::writeStyle(RB_XmlWriter& xw) {
    // write style element <style>
    xw.insertAttribute("type","text/css");
    xw.push("style");
    // write style content in <![CDATA{ .. ]]>>
    writeLayerStyles(xw);
    // write ending style element </style>
    xw.pop();
}


/**
 * Writes defs element for symbols (blocks)
 */
void RB_XmlSvg::writeDefinition(RB_XmlWriter& xw) {
    // write style element <defs>
    xw.push("defs");
}


/**
 * Writes <g> group element in <defs>. These group elements are blocks in QCad.
 * The first intention was to use the SVG <symbol> for this purpose but that 
 * did not work properly.  
 */
void RB_XmlSvg::writeGroup(RB_XmlWriter& xw, const DL_BlockData& data, 
											const RB_CadAttributes& attrib) {
    if (data.name.empty()) {
        std::cerr << "RB_XmlSvg::writeGroup: Group name should not be empty\n";
        return;
    }
    
    // Blocks can also be paperspace etc.
    if (!strcasecmp(data.name.c_str(), "*paper_space")) {
        return;
    } else if (!strcasecmp(data.name.c_str(), "*model_space")) {
        return;
    } else if (!strcasecmp(data.name.c_str(), "*paper_space0")) {
        return;
    }
    
	insertEntityAttrib(xw, attrib);
    
    // insert id name for symbol
    xw.insertAttribute("id", data.name.c_str());
    
    if (data.flags == RS2::FlagVisible) { 
        xw.insertAttribute("display", "none");
    }
    xw.push("g");
}




/**
 * Writes entity part of block per layer id in <g> element in <defs> 
 */
void RB_XmlSvg::writeGroupLayerId(RB_XmlWriter& xw,
                    const DL_LayerData& data) {
    if (data.name.empty()) {
        std::cerr << "RB_XmlSvg::writeGroupLayerId: "
        << "Group name must not be empty\n";
        return;
    }
    
    RS_String mStr = "LAYER-"; 
    mStr += data.name.c_str();
    
    // insert id (layer) name for block, 
    // the block style is written separately in <![CDATA ... 
    xw.insertAttribute("id", mStr);
    
    xw.push("g");
}




/**
 * Writes <defs> element for marker as used for rs_leader
 */
void RB_XmlSvg::writeMarker(RB_XmlWriter& xw) {

    QString mStr = "";   // for local storage
    // QString str = "";    // for conversion
    
    // triangle type marker pointing at start
    xw.insertAttribute("id", "TriangleStart");
    mStr = "0 0 33 27";
    xw.insertAttribute("viewBox", mStr);
    xw.insertAttribute("refX", 12.5);
    xw.insertAttribute("refY", 13.5);
    xw.insertAttribute("markerUnits", "strokeWidth");
    xw.insertAttribute("markerWidth", 33);
    xw.insertAttribute("markerHeight", 27);
    xw.insertAttribute("orient", "auto");
    xw.insertAttribute("stroke", "rgb(0,0,0)");
    xw.insertAttribute("stroke-width", "inherit");
    xw.insertAttribute("stroke-linecap", "round");
    xw.insertAttribute("stroke-linejoin", "round");
    xw.push("marker");
    
    // path painting the marker or arrowHead 
    mStr = "M 12.5 13.5 l 8 -1 l 0 2 z";
    xw.insertAttribute("d", mStr);
    xw.emptyElement("path");
    
    xw.pop(); // pop marker element

    // triangle type marker pointing at end
    xw.insertAttribute("id", "TriangleEnd");
    mStr = "0 0 33 27";
    xw.insertAttribute("viewBox", mStr);
    xw.insertAttribute("refX", 20.5);
    xw.insertAttribute("refY", 13.5);
    xw.insertAttribute("markerUnits", "strokeWidth");
    xw.insertAttribute("markerWidth", 33);
    xw.insertAttribute("markerHeight", 27);
    xw.insertAttribute("orient", "auto");
    xw.insertAttribute("stroke", "rgb(0,0,0)");
    xw.insertAttribute("stroke-width", "inherit");
    xw.insertAttribute("stroke-linecap", "round");
    xw.insertAttribute("stroke-linejoin", "round");
    xw.push("marker");
    
    // path painting the marker or arrowHead 
    mStr = "M 20.5 13.5 l -8 -1 l 0 2 z";
    xw.insertAttribute("d", mStr);
    xw.emptyElement("path");
    
    xw.pop(); // pop marker element
}



/**
 * Writes group element for blocks (symbols)
 */
void RB_XmlSvg::writeLayer(RB_XmlWriter& xw, 
                        const DL_LayerData& data) {
    QString str = "LAYER-" + QString(data.name.c_str());
    xw.insertAttribute("class", str);
    xw.push("g");
}





/**
 * Writes a point entity to the file.
 *
 * @param xw SVG writer
 * @param data Entity data from the file
 * @param attrib Attributes
 */
void RB_XmlSvg::writePoint(RB_XmlWriter& xw,
                        const DL_PointData& data,
                        const RB_CadAttributes& attrib) {
    RS_DEBUG->print("RB_XmlSvg::writePoint...");
    
	xw.insertAttribute("x1", convertToSvgX(data.x));
	xw.insertAttribute("y1", convertToSvgY(data.y));
	// xw.insertAttribute("z1", data.z); not used. 2004-09-24 Rutger
	xw.insertAttribute("x2", convertToSvgX(data.x));
	xw.insertAttribute("y2", convertToSvgY(data.y));
	// xw.insertAttribute("z2", data.z); not used. 2004-09-24 Rutger

    insertEntityAttrib(xw, attrib);
	
    xw.emptyElement("line");
}



/**
 * Writes a line entity to the file.
 *
 * @param xw SVG writer
 * @param data Entity data from the file
 * @param attrib Attributes
 */
void RB_XmlSvg::writeLine(RB_XmlWriter& xw,
                       const DL_LineData& data,
                       const RB_CadAttributes& attrib) {
    RS_DEBUG->print("RB_XmlSvg::writeLine...");

	xw.insertAttribute("x1", convertToSvgX(data.x1));
	xw.insertAttribute("y1", convertToSvgY(data.y1));
	// xw.insertAttribute("z1", data.z1); not used. 2004-09-24 Rutger
	xw.insertAttribute("x2", convertToSvgX(data.x2));
	xw.insertAttribute("y2", convertToSvgY(data.y2));
	// xw.insertAttribute("z2", data.z2); not used. 2004-09-24 Rutger

    insertEntityAttrib(xw, attrib);
	
    xw.emptyElement("line");
}





/**
 * Writes a polyline entity to the file.
 *
 * @param xw SVG writer
 * @param data Entity data from the file
 * @param attrib Attributes
 * @see writeVertex
 */
void RB_XmlSvg::writePolyline(RB_XmlWriter& xw,
                           const DL_PolylineData& /*data*/,
                           const RB_CadAttributes& attrib) {
    RS_DEBUG->print("RB_XmlSvg::writePolyLine...");
    QString mStr;               // memory string
    QString str;                // used to convert numbers to string
    double endVertexX = 0.0;    // memory of previous vertex x
    double endVertexY = 0.0;    // memory of previous vertex y
        
    // Run through all the vectors
    QVector<RS_Vector>::iterator it = vectorVectors.begin();
	bool first = true;
	
    while (it != vectorVectors.end()) {
        if (first) {
            mStr += "M " + str.number(convertToSvgX(it->x) - endVertexX);
            mStr += "," + str.number(convertToSvgY(it->y) - endVertexY);
        } else {
            mStr += " l " + str.number(convertToSvgX(it->x) - endVertexX);
            mStr += "," + str.number(convertToSvgY(it->y) - endVertexY);
        }
        
        first = false;
        
        // remember last vertex
        endVertexX = convertToSvgX(it->x);
        endVertexY = convertToSvgY(it->y);
        
        /*
         * TODO: else if ("LINE") ("BEZIER")
         *       and if closed in polylinedata use z
         */
        
        ++it;
    }
        
    /*
     * close series of vertices with 'z'
     * 
     * TODO: it does not work like this see also rb_filtersvg.cpp
     * 		 check flags, some have been deleted
     * 
     * if (data.flags == RS2::FlagClosed) {
     *     mStr += " z";
     * }
     * 
     */
    
    xw.insertAttribute("d", mStr);    
    insertEntityAttrib(xw, attrib);    
    
    xw.emptyElement("path");
    vectorVectors.clear();
}



/**
 * Writes a single vertex of a polyline to the file.
 *
 * @param data Entity data from the file
 */
void RB_XmlSvg::writeVertex(const DL_VertexData& data) {

    RS_Vector* vertex = new RS_Vector;

    vertex->x = data.x;
    vertex->y = data.y;
    /*
    vertex->bulge = data.bulge;
    if (data.first) {
        vertex->flag = "FIRST";
    } else {
        vertex->flag = "LAST";
    }
	*/
    vectorVectors.push_back(*vertex);
    // TODO: check 'deletion' of vectors
}




/**
 * Writes a circle entity to the file.
 *
 * @param xw SVG writer
 * @param data Entity data from the file
 * @param attrib Attributes
 */
void RB_XmlSvg::writeCircle(RB_XmlWriter& xw,
                         const DL_CircleData& data,
                         const RB_CadAttributes& attrib) {
    RS_DEBUG->print("RB_XmlSvg::writeCircle...");

	xw.insertAttribute("cx", convertToSvgX(data.cx));
	xw.insertAttribute("cy", convertToSvgY(data.cy));
	// xw.insertAttribute("cz", data.cz); not used. 2004-09-24 Rutger
	xw.insertAttribute("r", convertToSvgX(data.radius));

    insertEntityAttrib(xw, attrib);
	
    xw.emptyElement("circle");
}



/**
 * Writes an arc entity to the file.
 *
 * @param xw SVG writer
 * @param data Entity data
 * @param attrib Attributes
 * @param isDimensionLine is arc part of a dimension line
 */
void RB_XmlSvg::writeArc(RB_XmlWriter& xw,
                      const DL_ArcData& data,
                      const RB_CadAttributes& attrib,
                      bool isDimensionLine) {
    RS_DEBUG->print("RB_XmlSvg::writeArc...");
    
    double startPointX = 0.0;   // starting x coordinate
    double startPointY = 0.0;   // starting y coordinate
    int largeArcFlag = 0;       // 0 is small part, 1 is large part
    int sweepFlag = 0;          // 0 is counter clockwise, 1 is clockwise
    double endPointX = 0.0;     // end x coordinate
    double endPointY = 0.0;     // end y coordinate
    QString mStr = "";          // to build string for d attribute of path
    QString str = "";           // to convert numbers in strings
    
    // calculate start and end point    
    startPointX = data.cx + cos(data.angle1) * data.radius;
    startPointY = data.cy + sin(data.angle1) * data.radius;
    endPointX = (data.cx + cos(data.angle2) * data.radius) - startPointX;
    endPointY = (data.cy + sin(data.angle2) * data.radius) - startPointY;
    
    double mAngle1 = data.angle1;
    double mAngle2 = data.angle2;
    
    // first select clockwise=1 or counter clockwise=0
    /*
    if (data.reversed) {
        sweepFlag = 1;
        
        // second select large arc or the small arc of circle
        if (mAngle2 < mAngle1) mAngle2 += 2*M_PI;
        
        if (M_PI > mAngle2 - mAngle1) {
            largeArcFlag = 1;
        } else {
            largeArcFlag = 0;
        }
        
    } else {
 	*/ 
        sweepFlag = 0;
        
        if (mAngle2 < mAngle1) mAngle2 += 2*M_PI;
        
        if (M_PI > mAngle2 - mAngle1) {
            largeArcFlag = 0;
        } else {
            largeArcFlag = 1;
        }
//    }
    
    mStr += "M " + str.number(convertToSvgX(startPointX));
    mStr += "," + str.number(convertToSvgY(startPointY));
    mStr += " a " + str.number(data.radius) + "," + str.number(data.radius);
    mStr += " 0 " + str.number(largeArcFlag) + "," + str.number(sweepFlag);
    mStr += " " + str.number(endPointX);        // no conversion only relative
    mStr += "," + str.number(-1.0*endPointY);   // no conversion only relative
    
    xw.insertAttribute("d", mStr);
    insertEntityAttrib(xw, attrib);
    if (isDimensionLine) {
        xw.insertAttribute("marker-start", "url(#TriangleStart)");
        xw.insertAttribute("marker-end", "url(#TriangleEnd)");
    }
    xw.emptyElement("path");
}




/**
 * Writes an ellipse entity to the file.
 *
 * @param xw SVG writer
 * @param data Entity data from the file
 * @param attrib Attributes
 */
void RB_XmlSvg::writeEllipse(RB_XmlWriter& xw,
                          const DL_EllipseData& data,
                          const RB_CadAttributes& attrib) {
    
    QString mStr = "";          // to build string for attributes
    QString str = "";           // to convert numbers in strings

    if ( ((data.angle1 == 0) && (data.angle2 == 2*M_PI)) ||
         ((data.angle1 == 2*M_PI) && (data.angle2 == 0)) ) {
        
        // full ellipse
        double mainAxisLength = pow( pow(data.mx,2) 
                                   + pow(data.my,2), 0.5);
        double mainAxisAngle = -1*(atan(data.my/data.mx) * 180/M_PI);
        // first translate than rotate because rotation has otherwise influence
        //  on the translation, we only want to rotate around the origine
        mStr += "translate(" + str.number(convertToSvgX(data.cx)) 
              + "," + str.number(convertToSvgY(data.cy)) + ")";
        mStr += " rotate(" + str.number(mainAxisAngle) + ")";
                
        xw.insertAttribute("cx",0);
        xw.insertAttribute("cy",0);
        xw.insertAttribute("rx",str.number(mainAxisLength));
        xw.insertAttribute("ry",str.number(mainAxisLength*data.ratio));
        xw.insertAttribute("transform", mStr);

        insertEntityAttrib(xw, attrib);

        xw.emptyElement("ellipse");
    } else {

        // part of ellipse
        double startPointX = 0.0;   // starting x coordinate
        double startPointY = 0.0;   // starting y coordinate
        int largeArcFlag = 0;       // 0 is small part, 1 is large part
        int sweepFlag = 0;          // 0 is counter clockwise, 1 is clockwise
        double endPointX = 0.0;     // end x coordinate
        double endPointY = 0.0;     // end y coordinate
        double mAngle1 = data.angle1;
        double mAngle2 = data.angle2;
        
        // half major axis
        double halfMaj = pow( (pow(data.mx, 2) + pow(data.my, 2)), 0.5);
        
        // half minor axis
        double halfMin = halfMaj * data.ratio;
                                      
        // determine current radius for angle1
        double radiusFromCenter1 = pow( (pow(halfMin,2)*pow(halfMaj,2)) /
            ( pow(halfMin,2)*pow(cos(mAngle1),2) + pow(halfMaj,2)*pow(sin(mAngle1),2) ), 0.5); 
        startPointX = radiusFromCenter1 * cos(mAngle1);
        startPointY = radiusFromCenter1 * sin(mAngle1);
        
        // determine current radius for angle2        
        double radiusFromCenter2 = pow( (pow(halfMin,2)*pow(halfMaj,2)) /
            ( pow(halfMin,2)*pow(cos(mAngle2),2) + pow(halfMaj,2)*pow(sin(mAngle2),2) ), 0.5); 
        endPointX = radiusFromCenter2 * cos(mAngle2) - startPointX;
        endPointY = radiusFromCenter2 * sin(mAngle2) - startPointY;
        
        // first select clockwise=1 or counter clockwise=0
        /*
        if (data.reversed) {
            sweepFlag = 1;
            
            // second select large arc or the small arc of circle
            if (mAngle2 < mAngle1) mAngle2 += 2*M_PI;
            
            if (M_PI > mAngle2 - mAngle1) {
                largeArcFlag = 1;
            } else {
                largeArcFlag = 0;
            }
            
        } else {
		*/
            sweepFlag = 0;
            
            if (mAngle2 < mAngle1) mAngle2 += 2*M_PI;
            
            if (M_PI > mAngle2 - mAngle1) {
                largeArcFlag = 0;
            } else {
                largeArcFlag = 1;
            }
//        }
    
        // determine the main axis angle with x-axis
        double mainAxisAngle = -1*(atan(data.my/data.mx) * 180/M_PI);
        // first translate than rotate because rotation has otherwise influence
        //  on the translation, we only want to rotate around the origine
        mStr = "translate(" + str.number(convertToSvgX(data.cx)) 
              + "," + str.number(convertToSvgY(data.cy)) + ")";
        mStr += " rotate(" + str.number(mainAxisAngle) + ")";
    
        xw.insertAttribute("transform", mStr);    
        
        mStr = "M " + str.number(startPointX);  // no converstion because all relative to origine
        mStr += "," + str.number(-startPointY); // no converstion because all relative to origine
        mStr += " a " + str.number(halfMaj) + "," + str.number(halfMin);
        mStr += " 0 " + str.number(largeArcFlag) + "," + str.number(sweepFlag);
        mStr += " " + str.number(endPointX);        // no conversion only relative
        mStr += "," + str.number(-1.0*endPointY);   // no conversion only relative
        
        xw.insertAttribute("d", mStr);
        insertEntityAttrib(xw, attrib);

        xw.emptyElement("path");
    }
}


/**
 * Writes an use (insert in QCad) to the file.
 * @param xw SVG writer
 * @param data Entity data from the file
 * @param attrib Attributes
 */
void RB_XmlSvg::writeUse(RB_XmlWriter& xw,
                         const DL_InsertData& data,
                         const DL_BlockData& /*blData*/,
                         const RB_CadAttributes& attrib) {
    
    // complete modified since symbol did not work with scaling
    // this code therefore does not work with the <symbol> element
    
    RS_DEBUG->print("RB_FilterSVG::writeUse()");
    
    if (data.name.empty()) {
        std::cerr << "RB_XmlSvg::writeInsert: "
        << "Block name must not be empty\n";
        return;
    }
    
    QString mStr = "";  // for storage only
    QString str = "";   // for conversion only
    
    // determine the (svg) rotation angle with x-axis
    double rotAngle = 0.0;
    if (data.angle != 0.0) {
        rotAngle = -1.0*data.angle;
    }
    
    mStr = "translate(" + str.number(convertToSvgX(data.ipx)) 
                + "," + str.number(convertToSvgY(data.ipy)) + ")";
    mStr += " scale(" + str.number(data.sx) 
                + "," + str.number(data.sy)+ ")";
    mStr += " rotate(" + str.number(rotAngle) + ")";
   
    xw.insertAttribute("transform", mStr);
    mStr = "#" + QString(data.name.c_str());
    xw.insertAttribute("xlink:href", mStr);
    insertEntityAttrib(xw, attrib);

    xw.emptyElement("use");
}



/**
 * Writes a multi text entity to the file.
 *
 * @param xw SVG writer
 * @param data Entity data from the file
 * @param attrib Attributes
 */
void RB_XmlSvg::writeMText(RB_XmlWriter& xw,
                        const DL_MTextData& data,
                        const RB_CadAttributes& attrib) {
    
    /* TODO: clean-up code
    drawingDirection
    lineSpacingStyle
    lineSpacingFactor
    Style
    */
        
    /* translation of text base point according attachment point height */
    double trHeight;
    /* TODO: translation of text base point according attachment point width. */
    double trWidth;
    /* correction to SVG font size. */
    double fontSize = data.height * 1.5;
        
    QString mStr = "";  // for storage only
    QString str = "";   // for conversion only
    
    // temporary container for SVG delta y coordinate first text line
    double tempY = 0.0; 
    
    str = QString(data.text.c_str());
    int lineCount = str.count("\\P", Qt::CaseInsensitive) + 1;
        
    // line distance
    trHeight = data.height * 1.6;    
    
    /**
     * Attachment point.
     *
     * 1 = Top left, 2 = Top center, 3 = Top right,
     * 4 = Middle left, 5 = Middle center, 6 = Middle right,
     * 7 = Bottom left, 8 = Bottom center, 9 = Bottom right
     */
    switch (data.attachmentPoint) {
        case 1: // top left aligned
            tempY = data.height; 
            trWidth = 0.0;
            break;
        case 2: // top center aligned
            tempY = data.height; 
            trWidth = 0.5;
            break;
        case 3: // top right aligned
            tempY = data.height; 
            trWidth = 1.0;
            break;
        case 4: // middle left aligned
            // note: this code uses the integer/2 = integer
            if (lineCount % 2 == 0) {
                tempY = -((lineCount/2 - 1) * trHeight)
                    - ((11.0/24.0) * data.height);
            } else { 
                // note: lineCount/2 gives an integer
                //  times trHeight gives a double
                tempY = -(lineCount/2 * trHeight)
                    + data.height/2;
            }
            trWidth = 0.0;
            break;
        case 5: // middle center aligned
            if (lineCount % 2 == 0) {
                tempY = -((lineCount/2 - 1) * trHeight) 
                    - (11.0/24.0 * data.height);
                xw.insertAttribute("dataHeight", data.height);            
            } else {
                tempY = -(lineCount/2 * trHeight)
                    + data.height/2;
            }
            trWidth = 0.5;
            break;
        case 6: // middle right aligned
            if (lineCount % 2 == 0) {
                tempY = -((lineCount/2 - 1) * trHeight)
                    - ((11.0/24.0) * data.height);
            } else {
                tempY = -(lineCount/2  * trHeight)
                    + data.height/2;
            }
            trWidth = 1.0;
            break;
        case 7: // bottom left aligned
            tempY = -trHeight * (lineCount - 1);
            trWidth = 0.0;
            break;
        case 8: // bottom center aligned
            tempY = -trHeight * (lineCount - 1);
            trWidth = 0.5;
            break;
        case 9: // bottom right aligned
            tempY = -trHeight * (lineCount - 1);
            trWidth = 1.0;
            break;
        default:
            tempY = -trHeight * (lineCount - 1);
            trWidth = 0.0;
            break;
    }
    
    // determine the (svg) rotation angle in degrees with x-axis
    double rotAngle = 0.0;
    if (data.angle != 0.0) {
        rotAngle = -1.0*data.angle*180/M_PI;
    }
    
    // translate text to correct position than rotate
    mStr = "translate(" + str.number(convertToSvgX(data.ipx)) 
                + "," + str.number(convertToSvgY(data.ipy)) + ")";
    mStr += " rotate(" + str.number(rotAngle) + ")";
    xw.insertAttribute("transform", mStr);
    
    // TODO: selector for font-family font-type etc. to RB_CadAttributes
    xw.insertAttribute("font-family", "helvetica");
    
    // TODO: check font-size with QCad height intention to RB_CadAttributes
    xw.insertAttribute("font-size", fontSize);
    
    // fill color to be set per text entity, layer in does not provide
    // xw.insertAttribute("fill", attrib.getSvgColor());	// string rgb(r,g,b)
    if (attrib.getWidth() <= 0.0) {
    	xw.insertAttribute("stroke", "none");
    }
    
	// set flag for text entity
	bool textEntity = true;
    // attribute only used for 'fill' of text, default is for stroke 'stroke=none' 
    insertEntityAttrib(xw, attrib, textEntity); 
    
    // text element
    xw.push("text");
    
    // split by "\P" multi line text for tspan elements
    QStringList textList = str.split("\\P", QString::KeepEmptyParts, Qt::CaseInsensitive);
    
    for ( QStringList::Iterator it = textList.begin(); it != textList.end(); ++it) {
        // bring text lines to correct y position
        xw.insertAttribute("x", 0);
        xw.insertAttribute("y", tempY);
        // bring text lines to correct insertion point
        if (trWidth  == 1.0) {
            xw.insertAttribute("text-anchor", "end");
        } else if (trWidth == 0.5)  {
            xw.insertAttribute("text-anchor", "middle");
        } else if (trWidth == 0.0)   {
            xw.insertAttribute("text-anchor", "start");
        }
        
        /* for testing only * /
        xw.insertAttribute("trHeight", trHeight);
        xw.insertAttribute("lineCount", lineCount);
        xw.insertAttribute("lineCountD2", lineCount/2);
        xw.insertAttribute("lineCountP2", lineCount%2);
        */
        
        str = (QString) *it;
        // replace tilde by space
        str.replace("\\~", " ");
        str.replace("%%d", "&#xb0;");
        str.replace("%%p", "&#xb1;");
        
        // TODO; improve the code for the QRegExp
        // replace symbols \U+00XX with &#xXX; in SVG
        // str.replace(QRegExp("<i>([^<]*)</i>"), "\\emph(\\1)" ); is example
        // str.replace(QRegExp("\\U+00(\\w{2})"), "&#x\\1;" ); does not work \U , + , ampersand
        // because all the other do not work 
        str.replace(QRegExp("\\WU\\W00(\\w{2})"), "&#x\\1;" );
                        
        xw.element("tspan", str);
        tempY += trHeight;
    }

    xw.pop();
}


/**
 * Writes a text entity to the file.
 *
 * @param xw SVG writer
 * @param data Entity data from the file
 * @param attrib Attributes
 * /
void RB_XmlSvg::writeText(RB_XmlWriter& xw,
                       const DL_TextData& data,
                       const RB_CadAttributes& attrib) {
	// TODO
}
*/


/**
 * Writes an aligned dimension entity to the file.
 *
 * @param xw SVG writer
 * @param data Generic dimension data for from the file
 * @param data Specific aligned dimension data from the file
 * @param attrib Attributes
 */
void RB_XmlSvg::writeDimAligned(RB_XmlWriter& xw,
                             const DL_DimensionData& data,
                             const DL_DimAlignedData& edata,
                             const RB_CadAttributes& attrib) {

    // write <g> to group the dimension entities
    // insert attributes in this <g> element
    xw.insertAttribute("id", "dimAligned");
    insertEntityAttrib(xw, attrib);
    xw.push("g");

    // calculate vector and distance (is vector length) to dimension line
    RS_Vector v((data.dpx-edata.epx2), (data.dpy-edata.epy2), 0.0);
    double lengthV = v.magnitude();
    
    double pb1x = 0.0;
    double pb1y = 0.0;
        
    double pb2x = 0.0;
    double pb2y = 0.0;
        
    double pb3x = 0.0;
    double pb3y = 0.0;

    double pa1x = 0.0;
    double pa1y = 0.0;
        
    double pa2x = 0.0;
    double pa2y = 0.0;
        
    double pa3x = 0.0;
    double pa3y = 0.0;
    
    /*
    Set relevant points for drawing the dimension lines
    TODO: collect the settings from the graphic settings
    
    Refer to QG_DlgOptionsDrawing for example set as 'global' variable
    dimension arrow size:
    	double dimasz = graphic->getVariableDouble("$DIMASZ",
                        RS_Units::convert(2.5, RS2::Millimeter, unit));
        cbDimAsz->setCurrentText(QString("%1").arg(dimasz)); 
    */
    
    if (0!=lengthV) {
        pb1x = edata.epx2 + ((v.x / lengthV) * 0.625);
        pb1y = edata.epy2 + ((v.y / lengthV) * 0.625);
        
        pb2x = data.dpx;
        pb2y = data.dpy;
        
        pb3x = pb2x + ((v.x / lengthV) * 1.25);
        pb3y = pb2y + ((v.y / lengthV) * 1.25);

        pa1x = edata.epx1 + ((v.x / lengthV) * 0.625);
        pa1y = edata.epy1 + ((v.y / lengthV) * 0.625);
        
        pa2x = edata.epx1 + v.x;
        pa2y = edata.epy1 + v.y;
        
        pa3x = pa2x + ((v.x / lengthV) * 1.25);
        pa3y = pa2y + ((v.y / lengthV) * 1.25);
    } else {
        xw.pop();
        return;
    }
    
    // write dimension lines
   	xw.insertAttribute("x1", convertToSvgX(pb1x));
	xw.insertAttribute("y1", convertToSvgY(pb1y));
	xw.insertAttribute("x2", convertToSvgX(pb3x));
	xw.insertAttribute("y2", convertToSvgY(pb3y));
    xw.emptyElement("line");

   	xw.insertAttribute("x1", convertToSvgX(pa1x));
	xw.insertAttribute("y1", convertToSvgY(pa1y));
	xw.insertAttribute("x2", convertToSvgX(pa3x));
	xw.insertAttribute("y2", convertToSvgY(pa3y));
    xw.emptyElement("line");

   	xw.insertAttribute("x1", convertToSvgX(pb2x));
	xw.insertAttribute("y1", convertToSvgY(pb2y));
	xw.insertAttribute("x2", convertToSvgX(pa2x));
	xw.insertAttribute("y2", convertToSvgY(pa2y));
    xw.insertAttribute("marker-start", "url(#TriangleStart)");
    xw.insertAttribute("marker-end", "url(#TriangleEnd)");
    xw.emptyElement("line");

    // data.text and data.angle can be empty, if not edited by user,
    // for SVG these values need to be inserted and therefore calculated
    QString dataText = "";           // memory string
    double dataAngle = 0.0;          // memory double
    QString str = "";                // used to convert numbers to string
    
    RS_Vector vDim((pb2x-pa2x), (pb2y-pa2y), 0.0);
    double lengthVdim = vDim.magnitude();

    if ("" == data.text) {
        dataText = str.number(lengthVdim); 
    } else {
        dataText = QString(data.text.c_str());
    }
    
    dataAngle = vDim.angle(); // radians
    
    // correction for SVG clockwise rotation
    if (1e-6 > cos(dataAngle) && -1e-6 < cos(dataAngle)) {
        dataAngle *= -1.0;
    }
    
    // write dimension text
    writeMText(
            xw,
            DL_MTextData(data.mpx,
                         data.mpy,
                         0.0,
                         2.5,
                         3.5,
                         data.attachmentPoint,
                         1,
                         data.lineSpacingStyle,
                         data.lineSpacingFactor,
                         string(dataText.toAscii()),
                         "Standard",
                         dataAngle),
             attrib);

    // close element </g>
    xw.pop(); 
}



/**
 * Writes a linear dimension entity to the file.
 *
 * @param xw SVG writer
 * @param data Generic dimension data for from the file
 * @param data Specific linear dimension data from the file
 * @param attrib Attributes
 */
void RB_XmlSvg::writeDimLinear(RB_XmlWriter& xw,
                            const DL_DimensionData& data,
                            const DL_DimLinearData& edata,
                            const RB_CadAttributes& attrib) {

    /*
     * copied mostly from xml_svg::writeDimAligned
     */

    // write <g> to group the dimension entities
    // insert attributes in this <g> element
    xw.insertAttribute("id", "dimLinear");
    insertEntityAttrib(xw, attrib);
    xw.push("g");

    // calculate vector and distance (is vector length) to dimension line
    RS_Vector v((data.dpx-edata.dpx2), (data.dpy-edata.dpy2), 0.0);
    double lengthV = v.magnitude();
    
    double pb1x = 0.0;
    double pb1y = 0.0;
        
    double pb2x = 0.0;
    double pb2y = 0.0;
        
    double pb3x = 0.0;
    double pb3y = 0.0;

    double pa1x = 0.0;
    double pa1y = 0.0;
        
    double pa2x = 0.0;
    double pa2y = 0.0;
        
    double pa3x = 0.0;
    double pa3y = 0.0;
    
    double tempFactor = 0.0;
    
    /*
    Set relevant points for drawing the dimension lines
    TODO: collect the settings from the graphic settings
    
    Refer to QG_DlgOptionsDrawing for example set as 'global' variable
    dimension arrow size:
    	double dimasz = graphic->getVariableDouble("$DIMASZ",
                        RS_Units::convert(2.5, RS2::Millimeter, unit));
        cbDimAsz->setCurrentText(QString("%1").arg(dimasz)); 
    */
    
    if (0!=lengthV) {
        pb1x = edata.dpx2 + ((v.x / lengthV) * 0.625);
        pb1y = edata.dpy2 + ((v.y / lengthV) * 0.625);
        
        pb2x = data.dpx;
        pb2y = data.dpy;
        
        pb3x = pb2x + ((v.x / lengthV) * 1.25);
        pb3y = pb2y + ((v.y / lengthV) * 1.25);

        pa1x = edata.dpx1 + ((v.x / lengthV) * 0.625);
        pa1y = edata.dpy1 + ((v.y / lengthV) * 0.625);
        
        // pa2x = edata.dpx1 + v->x;
        // pa2y = edata.dpy1 + v->y;
        
        tempFactor = (v.y * (data.dpy - edata.dpy1)
                    - v.x * (edata.dpx1 - data.dpx))
                    / (pow(v.x,2) + pow(v.y,2));
        pa2x = edata.dpx1 + tempFactor * v.x;
        pa2y = edata.dpy1 + tempFactor * v.y;
        
        pa3x = pa2x + ((v.x / lengthV) * 1.25);
        pa3y = pa2y + ((v.y / lengthV) * 1.25);
    } else {
        xw.pop(); // close element </g>    
        return;
    }
    
    // write dimension lines
   	xw.insertAttribute("x1", convertToSvgX(pb1x));
	xw.insertAttribute("y1", convertToSvgY(pb1y));
	xw.insertAttribute("x2", convertToSvgX(pb3x));
	xw.insertAttribute("y2", convertToSvgY(pb3y));
    xw.emptyElement("line");

   	xw.insertAttribute("x1", convertToSvgX(pa1x));
	xw.insertAttribute("y1", convertToSvgY(pa1y));
	xw.insertAttribute("x2", convertToSvgX(pa3x));
	xw.insertAttribute("y2", convertToSvgY(pa3y));
    xw.emptyElement("line");

   	xw.insertAttribute("x1", convertToSvgX(pb2x));
	xw.insertAttribute("y1", convertToSvgY(pb2y));
	xw.insertAttribute("x2", convertToSvgX(pa2x));
	xw.insertAttribute("y2", convertToSvgY(pa2y));
    xw.insertAttribute("marker-start", "url(#TriangleStart)");
    xw.insertAttribute("marker-end", "url(#TriangleEnd)");
    xw.emptyElement("line");

    
    // data.text and data.angle can be empty, if not edited by user,
    // for SVG these values need to be inserted and therefore calculated
    QString dataText = "";           // memory string
    double dataAngle = 0.0;          // memory double
    QString str = "";                // used to convert numbers to string
    
    RS_Vector vDim((pb2x-pa2x), (pb2y-pa2y), 0.0);
    double lengthVdim = vDim.magnitude();

    if ("" == data.text) {
        dataText = str.number(lengthVdim); 
    } else {
        dataText = data.text.c_str();
    }
    
    //dataAngle = vDim.angle(); // radians
    dataAngle = edata.angle; // radians
    
    /*
    correction for SVG clockwise rotation. Here not required, why?
    if (1e-6 > cos(dataAngle) && -1e-6 < cos(dataAngle)) {
        dataAngle *= -1.0;
    }
    */
    
    // TODO: oblique not used
    
    // write dimension text
    writeMText(
            xw,
            DL_MTextData(data.mpx,
                         data.mpy,
                         0.0,
                         2.5,
                         3.5,
                         data.attachmentPoint,
                         1,
                         data.lineSpacingStyle,
                         data.lineSpacingFactor,
                         string(dataText.toAscii()),
                         "Standard",
                         dataAngle),
             attrib);
    
    // close element </g>
    xw.pop(); 
}



/**
 * Writes a radial dimension entity to the file.
 *
 * @param xw SVG writer
 * @param data Generic dimension data for from the file
 * @param data Specific radial dimension data from the file
 * @param attrib Attributes
 */
void RB_XmlSvg::writeDimRadial(RB_XmlWriter& xw,
                            const DL_DimensionData& data,
                            const DL_DimRadialData& edata,
                            const RB_CadAttributes& attrib) {
    
    // write <g> to group the dimension entities
    // insert attributes in this <g> element
    xw.insertAttribute("id", "dimRadial");
    insertEntityAttrib(xw, attrib);
    xw.push("g");

    // calculate vector and distance (is vector length) of dimension line
    RS_Vector v((data.dpx-edata.dpx), (data.dpy-edata.dpy), 0.0);
    double lengthV = v.magnitude();
    
    double pb2x = 0.0;
    double pb2y = 0.0;
    
    double pa2x = 0.0;
    double pa2y = 0.0;
    
    /*
    Set relevant points for drawing the dimension lines
    TODO: collect the settings from the graphic settings
    
    Refer to QG_DlgOptionsDrawing for example set as 'global' variable
    dimension arrow size:
    	double dimasz = graphic->getVariableDouble("$DIMASZ",
                        RS_Units::convert(2.5, RS2::Millimeter, unit));
        cbDimAsz->setCurrentText(QString("%1").arg(dimasz)); 
    */
    
    if (0!=lengthV) {
        pb2x = data.dpx;
        pb2y = data.dpy;
        
        pa2x = edata.dpx;
        pa2y = edata.dpy;
    } else {
        // close element </g>    
        xw.pop();
        // finish function     
        return; 
    }
    
    // write dimension lines
   	xw.insertAttribute("x1", convertToSvgX(pb2x));
	xw.insertAttribute("y1", convertToSvgY(pb2y));
	xw.insertAttribute("x2", convertToSvgX(pa2x));
	xw.insertAttribute("y2", convertToSvgY(pa2y));

    xw.insertAttribute("marker-end", "url(#TriangleEnd)");
    xw.emptyElement("line");

    
    // data.text and data.angle can be empty, if not edited by user,
    // for SVG these values need to be inserted and therefore calculated
    QString dataText = "";           // memory string
    double dataAngle = 0.0;          // memory double
    QString str = "";                // used to convert numbers to string
    
    RS_Vector vDim((pb2x-pa2x), (pb2y-pa2y), 0.0);
    double lengthVdim = vDim.magnitude();

    if ("" == data.text) {
        dataText = str.number(lengthVdim); 
    } else {
        dataText = data.text.c_str();
    }
    
    dataAngle = vDim.angle(); // radians
    // dataAngle = data.angle; // radians
    
    /*
    correction for SVG clockwise rotation. Not required here, why not?
    if (1e-6 > cos(dataAngle) && -1e-6 < cos(dataAngle)) {
        dataAngle *= -1.0;
    }
    */ 
    
    if ( 0 > cos(dataAngle) ) {
         dataAngle += M_PI;
    }
    
    // write dimension text
    writeMText(
            xw,
            DL_MTextData(data.mpx,
                         data.mpy,
                         0.0,
                         2.5,
                         3.5,
                         data.attachmentPoint,
                         1,
                         data.lineSpacingStyle,
                         data.lineSpacingFactor,
                         string(dataText.toAscii()),
                         "Standard",
                         dataAngle),
             attrib);
    
    // close element </g>
    xw.pop(); 
}



/**
 * Writes a diametric dimension entity to the file.
 *
 * @param xw SVG writer
 * @param data Generic dimension data for from the file
 * @param data Specific diametric dimension data from the file
 * @param attrib Attributes
 */
void RB_XmlSvg::writeDimDiametric(RB_XmlWriter& xw,
                               const DL_DimensionData& data,
                               const DL_DimDiametricData& edata,
                               const RB_CadAttributes& attrib) {
    
    // write <g> to group the dimension entities
    // insert attributes in this <g> element
    xw.insertAttribute("id", "dimRadial");
    insertEntityAttrib(xw, attrib);
    xw.push("g");

    // calculate vector and distance (is vector length) of dimension line
    RS_Vector v((data.dpx-edata.dpx), (data.dpy-edata.dpy), 0.0);
    double lengthV = v.magnitude();

    double pb2x = 0.0;
    double pb2y = 0.0;
    
    double pa2x = 0.0;
    double pa2y = 0.0;
    
    // not used double lengthV = edata.leader;
    
    /*
    Set relevant points for drawing the dimension lines
    TODO: collect the settings from the graphic settings
    
    Refer to QG_DlgOptionsDrawing for example set as 'global' variable
    dimension arrow size:
    	double dimasz = graphic->getVariableDouble("$DIMASZ",
                        RS_Units::convert(2.5, RS2::Millimeter, unit));
        cbDimAsz->setCurrentText(QString("%1").arg(dimasz)); 
    */
    
    if (0!=lengthV) {
        pb2x = data.dpx;
        pb2y = data.dpy;
        
        pa2x = edata.dpx;
        pa2y = edata.dpy;
    } else {
        // close element </g>    
        xw.pop();
        return; 
    }
    
    // write dimension lines
   	xw.insertAttribute("x1", convertToSvgX(pb2x));
	xw.insertAttribute("y1", convertToSvgY(pb2y));
	xw.insertAttribute("x2", convertToSvgX(pa2x));
	xw.insertAttribute("y2", convertToSvgY(pa2y));
    xw.insertAttribute("marker-start", "url(#TriangleStart)");
    xw.insertAttribute("marker-end", "url(#TriangleEnd)");
    xw.emptyElement("line");

    
    // data.text and data.angle can be empty, if not edited by user,
    // for SVG these values need to be inserted and therefore calculated
    QString dataText = "";           // memory string
    double dataAngle = 0.0;          // memory double
    QString str = "";                // used to convert numbers to string
    
    RS_Vector vDim((pb2x-pa2x), (pb2y-pa2y), 0.0);
    double lengthVdim = vDim.magnitude();

    if ("" == data.text) {
        dataText = str.number(lengthVdim); 
    } else {
        dataText = data.text.c_str();
    }
    
    dataAngle = vDim.angle(); // radians
    // dataAngle = data.angle; // radians
    
    /*
    correction for SVG clockwise rotation. Here not required, why not?
    if (1e-6 > cos(dataAngle) && -1e-6 < cos(dataAngle)) {
        dataAngle *= -1.0;
    }
    */
    
    if ( 0 > cos(dataAngle) ) {
         dataAngle += M_PI;
    }
    
    // write dimension text
    writeMText(
            xw,
            DL_MTextData(data.mpx,
                         data.mpy,
                         0.0,
                         2.5,
                         3.5,
                         data.attachmentPoint,
                         1,
                         data.lineSpacingStyle,
                         data.lineSpacingFactor,
                         string(dataText.toAscii()),
                         "Standard",
                         dataAngle),
             attrib);
    
    // close element </g>
    xw.pop(); 
}



/**
 * Writes an angular dimension entity to the file.
 *
 * @param xw SVG writer
 * @param data Generic dimension data for from the file
 * @param data Specific angular dimension data from the file
 * @param attrib Attributes
 */
void RB_XmlSvg::writeDimAngular(RB_XmlWriter& xw,
                             const DL_DimensionData& data,
                             const DL_DimAngularData& edata,
                             const RB_CadAttributes& attrib) {

    /*
     * TODO: 2004-12-22: middle of text is not part of data.mpx and data.mpy
     * used a work-around to solve the problem by calculating the text position
     * here. That should not be necessary in the future
     *   => clean up this code, it is terrible to read
     */
    
    /* 
     * write <g> to group the dimension entities
     * insert attributes in this <g> element
     */
    xw.insertAttribute("id", "dimAngular");
    insertEntityAttrib(xw, attrib);
    xw.push("g");

    /* 
     * calculate center of dimension 'arc', is same as intersection of lines
     * create two vectors v1 and v2 giving the direction of the lines
     * create a third vector v3 that is based on the 
     * two points that each of the two lines pass
     */
    double lambda = 0.0;
        
    RS_Vector v1(edata.dpx2 - edata.dpx1, edata.dpy2 - edata.dpy1);    
    RS_Vector v2(data.dpx - edata.dpx3, data.dpy - edata.dpy3);
    RS_Vector v3(edata.dpx2 - data.dpx, edata.dpy2 - data.dpy); // points to 'arc'
    
    double denominator = v1.x * v2.y - v1.y * v2.x;
    
    if (0 != denominator) {
        lambda = (v3.y * v2.x - v3.x * v2.y) / denominator;
    }
        
    double centerX = lambda * v1.x + edata.dpx2;
    double centerY = lambda * v1.y + edata.dpy2;
        
    /* 
     * calculate vector and distance (is vector length) of dimension line
     * from intersection of lines center to dimenion 'arc' edata.dpx(y)4
     * edata.dpx(y)4 is where the user clicked for the dim line location?
     */
    RS_Vector v((edata.dpx4 - centerX), (edata.dpy4 - centerY), 0.0);
    double lengthV = v.magnitude();

    if (0 == lengthV) {
        // close element </g>    
        xw.pop(); 
        // finish function    
        return; 
    }
    
    /*
     * determine and set position of the dimension vectors with inverted matrix
     * conversion used to determine the quadrant of the arc position
     */
    double cosV1 = cos(v1.angle());
    double sinV1 = sin(v1.angle());
    
    double cosV2 = cos(v2.angle());
    double sinV2 = sin(v2.angle());
    
    double cosV = cos(v.angle());
    double sinV = sin(v.angle());
    
    double tempX = 0.0;
    double tempY = 0.0;
    double dataAngle = 0.0; 		// radians
    
    double e_dpx2 = edata.dpx2;
    double e_dpy2 = edata.dpy2;    
    
    double d_dpx = data.dpx;
    double d_dpy = data.dpy;
    
    QString dataText = "";           // memory string
    QString str = "";                // used to convert numbers to string

    // correct angle to get sharper corner < 180 degree
    double v1angle = v1.angle();
    double v2angle = v2.angle();

    if (M_PI < (v1angle - v2angle)) {
        v1angle = v1.angle() - 2 * M_PI;
    }
    
    if (-M_PI > (v1angle - v2angle)) {
        v2angle = v2.angle() - 2 * M_PI;
    }
    
    // set denominator used in inverse matrix
    denominator = sinV2 * cosV1 - cosV2 * sinV1;
    
    // calculated new projected coordinates of vector to 'arc'
    if (0 != denominator) {
        if (v1angle < v2angle) {
            // normal counter clockwise rotation
            // the tempX and tempY (+ or -) indicate the relevant quadrant of arc
            tempX = cosV * ( sinV2 / denominator) + sinV * (-cosV2 / denominator);
            tempY = cosV * (-sinV1 / denominator) + sinV * ( cosV1 / denominator);
            
            // determine the angle for the dimension text
            dataAngle = (v2angle + v1angle) / 2;
    
            // data.text can be empty, if not edited by user,
            // for SVG these values need to be inserted and therefore calculated
            if ("" == data.text) {
                if (0 < tempX * tempY) {
                    dataText = str.number((v2angle - v1angle) * 180 / M_PI);
                } else {
                    dataText = str.number((M_PI - (v2angle - v1angle)) 
                        * 180 / M_PI);
                }
            } else {
                dataText = data.text.c_str();
            }
            
            // move dimension lines to other side if required
            if (0 > tempX) {
                e_dpx2 = edata.dpx2 - 2 * (edata.dpx2 - centerX);
                e_dpy2 = edata.dpy2 - 2 * (edata.dpy2 - centerY);
                
                // adjust the location of the text with tempY still positive
                dataAngle += M_PI/2;
            }
            
            if (0 > tempY) {
                d_dpx = data.dpx - 2 * (data.dpx - centerX);
                d_dpy = data.dpy - 2 * (data.dpy - centerY);
                
                // adjust the location of the text with tempY negative
                //   and possibly tempX negative
                if (0 > tempX) {
                    dataAngle += M_PI/2;
                } else {
                    dataAngle -= M_PI/2;
                }
            }
            
        } else {
            // clockwise rotation
            tempX = cosV * (-sinV1 / denominator) + sinV * ( cosV1 / denominator);
            tempY = cosV * ( sinV2 / denominator) + sinV * (-cosV2 / denominator);
            
            // determine the angle for the dimension text
            dataAngle = (v1angle + v2angle) / 2;
            
            // data.text can be empty, if not edited by user,
            // for SVG these values need to be inserted and therefore calculated
            if ("" == data.text) {
                if (0 < tempX * tempY) {
                    dataText = str.number((v1angle - v2angle) * 180 / M_PI);
                } else {
                    dataText = str.number((M_PI - (v1angle - v2angle)) 
                        * 180 / M_PI);
                }
            } else {
                dataText = data.text.c_str();
            }
            
            // move dimension lines to other side if required (x and y swap)
            if (0 > tempY) {
                e_dpx2 = edata.dpx2 - 2 * (edata.dpx2 - centerX);
                e_dpy2 = edata.dpy2 - 2 * (edata.dpy2 - centerY);
                
                // adjust the location of the text with tempX still positive
                dataAngle -= M_PI/2;
            }
            
            if (0 > tempX) {
                d_dpx = data.dpx - 2 * (data.dpx - centerX);
                d_dpy = data.dpy - 2 * (data.dpy - centerY);
                
                // adjust the location of the text with tempX negative
                //   and possibly tempY negative
                if (0 > tempY) {
                    dataAngle -= M_PI/2;
                } else {
                    dataAngle += M_PI/2;
                }
            }
            
        }
    }
    

    /* 
     * TODO: collect the settings from the graphic settings
     * refer to QG_DlgOptionsDrawing for example set as 'global' variable
     * change direction of dim lines if arc is close to the center and
     * the location of the reference points pb1xy and pb3xy 
     */
    
    // calculate the angles of the dimension lines
    RS_Vector v11((d_dpx - centerX), (d_dpy - centerY), 0.0);
    RS_Vector v12((e_dpx2 - centerX), (e_dpy2 - centerY), 0.0);

    double cosV11 = cos(v11.angle());
    double sinV11 = sin(v11.angle());
    
    double cosV12 = cos(v12.angle());
    double sinV12 = sin(v12.angle());
    
    // calculate the reference points for dimension 'arc'
    double pb1x = d_dpx + cosV11 * 0.625;
    double pb1y = d_dpy + sinV11 * 0.625;
    
    double pb2x = centerX + lengthV * cosV11; // not used for 'arc'
    double pb2y = centerY + lengthV * sinV11; // not used for 'arc'
    
    double pb3x = pb2x + cosV11 * 1.25;
    double pb3y = pb2y + sinV11 * 1.25;

    double pa1x = e_dpx2 + cosV12 * 0.625;
    double pa1y = e_dpy2 + sinV12 * 0.625;
 
    double pa2x = centerX + lengthV * cosV12; // not used for 'arc'
    double pa2y = centerY + lengthV * sinV12; // not used for 'arc'
    
    double pa3x = pa2x + cosV12 * 1.25;
    double pa3y = pa2y + sinV12 * 1.25;

    // write dimension lines
   	xw.insertAttribute("x1", convertToSvgX(pb1x));
	xw.insertAttribute("y1", convertToSvgY(pb1y));
	xw.insertAttribute("x2", convertToSvgX(pb3x));
	xw.insertAttribute("y2", convertToSvgY(pb3y));
    xw.emptyElement("line");

   	xw.insertAttribute("x1", convertToSvgX(pa1x));
	xw.insertAttribute("y1", convertToSvgY(pa1y));
	xw.insertAttribute("x2", convertToSvgX(pa3x));
	xw.insertAttribute("y2", convertToSvgY(pa3y));
    xw.emptyElement("line");

    // set 'reverse' based on selection (angles) of the lines
    bool isReversed = false;
    if (((v1angle > v2angle) && (0 > (tempX * tempY)))  
        || ((v1angle < v2angle) && (0 < (tempX * tempY)))) {
        isReversed = true;
    }
   
    // write arc with arrows
    writeArc(
        xw,
        DL_ArcData(centerX,     // center x-coordinate 
                   centerY,      // center y-coordinate 
                   0.0,          // (center z-coordinate) 
                   lengthV,      // radius of dimension 'arc'
                   v11.angle(),       // angle11
                   v12.angle()),      // angle12
        attrib,
        true);                    // dimension line with arrows (true)

    // write dimension text on outside of the arc
    double dataMpx = centerX + cos(dataAngle) * (lengthV + 1.5);
    double dataMpy = centerY + sin(dataAngle) * (lengthV + 1.5);
    
    // correction rotation and location, watch out for SVG rotation direction
    if ( 0 >= sin(dataAngle) ) {
        // 3.0 = 2 * 1.5
        dataMpx -= 3.0 * cos(dataAngle);
        dataMpy -= 3.0 * sin(dataAngle); 
    }

    dataAngle += M_PI / 2;
    if ( 0 > cos(dataAngle) ) {
        dataAngle += M_PI;
    }
    
    // add degree symbol
    dataText += "&#xb0;";
    
    writeMText(
            xw,
            DL_MTextData(dataMpx,
                         dataMpy,
                         0.0,
                         2.5,
                         3.5,
                         data.attachmentPoint,
                         1,
                         data.lineSpacingStyle,
                         data.lineSpacingFactor,
                         string(dataText.toAscii()),
                         "Standard",
                         dataAngle),
             attrib);
    
    // write close element </g> of dimension entities
    xw.pop();
}


/**
 * Writes an angular dimension entity (3 points version) to the file.
 *
 * @param xw SVG writer
 * @param data Generic dimension data for from the file
 * @param data Specific angular dimension data from the file
 * @param attrib Attributes
 * /
void RB_XmlSvg::writeDimAngular3P(RB_XmlWriter& xw,
                               const DL_DimensionData& data,
                               const DL_DimAngular3PData& edata,
                               const RB_CadAttributes& attrib) {

    // TODO
}
*/


/**
 * Writes a leader entity to the file.
 *
 * @param xw SVG writer
 * @param data Entity data from the file
 * @param attrib Attributes
 * @see writeVertex
 */
void RB_XmlSvg::writeLeader(RB_XmlWriter& xw,
                         const DL_LeaderData& data,
                         const RB_CadAttributes& attrib) {
    
    QString mStr;               // memory string
    QString str;                // used to convert numbers to string
    double endVertexX = 0.0;    // memory of previous vertex x
    double endVertexY = 0.0;    // memory of previous vertex y
        
    // Run through all the vectors
    QVector<RS_Vector>::iterator it = vectorVectors.begin();
	
	bool first = true;
    while (it != vectorVectors.end()) {
        if (first) {
            mStr += "M " + str.number(convertToSvgX(it->x) - endVertexX);
            mStr += "," + str.number(convertToSvgY(it->y) - endVertexY);
        } else {
            mStr += " l " + str.number(convertToSvgX(it->x) - endVertexX);
            mStr += "," + str.number(convertToSvgY(it->y) - endVertexY);
        }
        
        first = false;
        
        // remember last vertex
        endVertexX = convertToSvgX(it->x);
        endVertexY = convertToSvgY(it->y);
        
        ++it;
    }
        
    /* 
     * close series of vertices with 'z'
     * 
     * TODO: it does not work like this see also rb_filterSvg.cpp
     * if (data.flags == RS2::FlagClosed) {
     *    mStr += " z";
     * }
     */
    
    // add arrow head (marker) if any
    if (data.arrowHeadFlag) {
        xw.insertAttribute("marker-start", "url(#TriangleStart)");
    };
    
    xw.insertAttribute("d", mStr);    
    insertEntityAttrib(xw, attrib);    
    
    xw.emptyElement("path");
    vectorVectors.clear();
}



/**
 * Writes a single vertex of a leader to the file.
 *
 * @param xw SVG writer
 * @param data Entity data
 */
void RB_XmlSvg::writeLeaderVertex(RB_XmlWriter& /*xw*/,
                               const DL_LeaderVertexData& data) {
    RS_Vector* vertex = new RS_Vector;

    vertex->x = data.x;
    vertex->y = data.y;
    /*
    vertex->bulge = data.bulge; // only with polyline
    if (data.first) {
        vertex->flag = "FIRST";
    } else {
        vertex->flag = "LAST";
    }
	*/
    vectorVectors.push_back(*vertex);
}


/**
 * Writes the application ID and application version to the SVG file.
 * @param name application name
 */
void RB_XmlSvg::writeAppid(RB_XmlWriter& xw, const RS_String& name) {
    if (name.isEmpty()) {
        std::cerr << "RB_XmlSvg::writeAppid: "
       			 	<< "Application  name must not be empty\n";
        return;
    }

    RS_String str = "  SVG file generated with "; 
    str += name + "                      ";
    
    xw.commentElement("************************************************************");
    xw.commentElement(str);
    xw.commentElement("************************************************************");
}


void RB_XmlSvg::insertEntityAttrib(RB_XmlWriter& xw, const RB_CadAttributes& attrib, 
														bool textEntity) {
    RS_DEBUG->print("RB_XmlSvg::insertEntityAttrib...");

    // note: rb_cadattributes is at QCad side and rb_xmlcodes is at the xml/svg 
    // side, only write the attributes if not by layer or by block
    
    if ((!attrib.getColor().isByLayer()) && (!attrib.getColor().isByBlock()) 
    													&& (!textEntity)) {
    	if ("rgb(255,255,255)" != attrib.getSvgColor()) {
            xw.insertAttribute("stroke", attrib.getSvgColor());	// string rgb(r,g,b)
        } else {
        	xw.insertAttribute("stroke", "rgb(0,0,0)");
        }
    } else if (textEntity) {
        if ("rgb(255,255,255)" != attrib.getSvgColor()) {
            xw.insertAttribute("fill", attrib.getSvgColor());
        } else {
        	xw.insertAttribute("fill", "rgb(0,0,0)");
        }
    }
    
    if (attrib.getWidth() != 0.0) {
        double sWidth = attrib.getSvgWidth();
        if (sWidth > 0) {
            xw.insertAttribute("stroke-width", sWidth); 	// double width in mm
        }
    }
    
    if ((attrib.getLineType() != RS2::LineByLayer) && (attrib.getLineType() != RS2::LineByBlock)) {
        QString sLineType = attrib.getSvgLineType();
        if (!(sLineType == "")) {
            xw.insertAttribute("stroke-dasharray", sLineType); // string lineType
        } 
    }
}


/**
 * Converts the Qcad x-coordinate to SVG coordinate
 */
double RB_XmlSvg::convertToSvgX(double x){
	/*
    double svgX;
    svgX = x * this->graphicScale + this->graphicInsertionBase.x;
    return svgX;
    */
	return x;
}


/**
 * Converts the Qcad y-coordinate to SVG coordinate
 */
double RB_XmlSvg::convertToSvgY(double y){
    double svgY;
    // y coordinate is upside down
    if (isPartOfBlock) {
        // keep origine the same for entities in blocks
        svgY = - y; 
    } else {
        //svgY =  this->size.height() - (y * this->graphicScale) 
        //			- this->graphicInsertionBase.y;
        svgY = getPaperSize().y - y;
    }
	return svgY;
}
