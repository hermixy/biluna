/*****************************************************************
 * $Id: rs_filtersvg.h,v 1.15 2004/09/18 23:04:18 - rutger $
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the PNID project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_FILTERSVG_H
#define RB_FILTERSVG_H

#include <QXmlSimpleReader>
#include "rs_block.h"
#include "rs_color.h"
// #include "rs_dict.h"
#include "rs_dimension.h"
#include "rs_hatch.h"
#include "rs_insert.h"
#include "rs_layer.h"
#include "rs_layer.h"
#include "rs_leader.h"
#include "rs_polyline.h"
#include "rs_text.h"
#include "rs_image.h"
#include "rs_filterinterface.h"

#include "rb_xmlreader.h"
#include "rb_xmlsvg.h"



/**
 * This filter class exports QCad RS_Graphic to an SVG file.
 */
class CAD_EXPORT RB_FilterSvg : public RS_FilterInterface, public QXmlDefaultHandler {

public:
    RB_FilterSvg();
    virtual ~RB_FilterSvg();

    virtual RS_String getDescription(RS2::FormatType t) {
		if (t == RS2::FormatSvg) {
			return "SVG";
		} 
		else {
			return "";
		}
	}
        
    virtual RS_String getDefaultExtension(RS2::FormatType t) {
		if (t == RS2::FormatSvg) {
			return "svg";
		} 
		else {
			return "";
		}
	}

	virtual RS_String getFilterString(RS2::FormatType t, bool /*openMode*/) {
		if (t == RS2::FormatSvg) {
			return QObject::tr("Scalable Vector Graphic %1").arg("(*.svg *.SVG)");
		} 
		else {
			return "";
		}
	}

    // Import:
    virtual bool fileImport(RS_Graphic& g, const RS_String& file, 
    									RS2::FormatType type);
    virtual bool startCDATA();
    virtual bool endCDATA();
    // virtual bool comment(const QString& ch);
    
    virtual bool startElement(const QString& namespaceURI, const QString& localName,
                    const QString& qName, const QXmlAttributes& attribs);
    virtual bool endElement(const QString& namespaceURI, const QString& localName,
                    const QString& qName);
    virtual bool characters(const QString& str);
    virtual bool fatalError(const QXmlParseException& exception);
	virtual QString getParseMessage();

    // Export:
    virtual bool fileExport(RS_Graphic& g, const RS_String& file, 
    									RS2::FormatType type);
    virtual bool fileExport(RS_Graphic& g, const QString& file, 
    									const QSize& size, 
    									bool black, bool bw,
										bool removeLayers,
										bool explodeBlocks);
	
//    void writeVariables(RB_XmlWriter& xw);
    void writeStyles(RB_XmlWriter& xw);   			// to write css styles per layer and block 
    void setStyle(RS_Pen pen);
    
    void writeLayer(RB_XmlWriter& xw, RS_Layer* l);   	// to write entities and blocks per layer
//    void writeLineType(RB_XmlWriter& xw, RS2::LineType t);
    void writeAppid(RB_XmlWriter& xw, const RS_String& appid);
    void writeBlock(RB_XmlWriter& xw, RS_Block* blk);
    void writeEntity(RB_XmlWriter& xw, RS_Entity* e);
    void writeEntitySvg(RB_XmlWriter& xw, RS_Entity* e, RB_CadAttributes attrib);
	void writePoint(RB_XmlWriter& xw, RS_Point* p, const RB_CadAttributes& attrib);
	void writeLine(RB_XmlWriter& xw, RS_Line* l, const RB_CadAttributes& attrib);
	void writePolyline(RB_XmlWriter& xw, 
						RS_Polyline* l, const RB_CadAttributes& attrib);
	void writeCircle(RB_XmlWriter& xw, RS_Circle* c, const RB_CadAttributes& attrib);
	void writeArc(RB_XmlWriter& xw, RS_Arc* a, const RB_CadAttributes& attrib);
	void writeEllipse(RB_XmlWriter& xw, RS_Ellipse* s, const RB_CadAttributes& attrib);
	void writeInsert(RB_XmlWriter& xw, RS_Insert* i, const RB_CadAttributes& attrib);
	void writeText(RB_XmlWriter& xw, RS_Text* t, RB_CadAttributes& attrib);
	void writeDimension(RB_XmlWriter& xw, RS_Dimension* d, 
						const RB_CadAttributes& attrib);
	void writeLeader(RB_XmlWriter& xw, RS_Leader* l, const RB_CadAttributes& attrib);
//	void writeHatch(RB_XmlWriter& xw, RS_Hatch* h, const RB_CadAttributes& attrib);
	
	void writeImage(RB_XmlWriter& xw, RS_Image* i, const RB_CadAttributes& attrib);
	void writeEntityContainer(RB_XmlWriter& xw, RS_EntityContainer* con, 
						const RB_CadAttributes& attrib);
	void writeAtomicEntities(RB_XmlWriter& xw, RS_EntityContainer* c, 
						const RB_CadAttributes& attrib, RS2::ResolveLevel level);
	
    void writeImageDef(RB_XmlWriter& xw, RS_Image* i);
	
    RS_Vector PaperSize();
	RS_Vector PaperInsertionBase();
	
	void setEntityAttributes(RS_Entity* entity, const RB_CadAttributes& attrib);
    RB_CadAttributes getEntityAttributes(RS_Entity* entity);
	
	static RS_String toSvgString(const RS_String& string);
    static RS_String toNativeString(const RS_String& string);

public:
    RS_Pen attributesToPen(const RB_CadAttributes& attrib) const;

    static RS_Color numberToColor(int num, bool comp=false);
    static int colorToNumber(const RS_Color& col);

//    static RS2::LineType nameToLineType(const RS_String& name);
//    static RS_String lineTypeToName(RS2::LineType lineType);
    //static RS_String lineTypeToDescription(RS2::LineType lineType);

    static RS2::LineWidth numberToWidth(int num);
    static int widthToNumber(RS2::LineWidth width);

	static RS2::AngleFormat numberToAngleFormat(int num);
	static int angleFormatToNumber(RS2::AngleFormat af);

	static RS2::Unit numberToUnit(int num);
	static int unitToNumber(RS2::Unit unit);
	
	static bool isVariableTwoDimensional(const RS_String& var);

private:
    /** Pointer to the graphic we currently operate on. */
    RS_Graphic* graphic;
	/** File name. Used to find out the full path of images. */
	RS_String file;
    /** string for concatinating text parts of MTEXT entities. */
    RS_String mtext;
    /** Pointer to current polyline entity we're adding vertices to. */
    RS_Polyline* polyline;
    /** Pointer to current leader entity we're adding vertices to. */
    RS_Leader* leader;
    /** Pointer to current entity container (either block or graphic) */
    RS_EntityContainer* currentContainer;

    /** Pointer to current hatch or NULL. */
    RS_Hatch* hatch;
    /** Pointer to current hatch loop or NULL. */
    RS_EntityContainer* hatchLoop;
    
    /** Translate document entity data to SVG(XML) format */
    RB_XmlSvg svg;
	RB_SvgCodes svgStyle;
	
	/** CDATA section valid */
	bool CDATAsection;
	/** content of current element or CDATA section */
	RS_String currentContent;
	/** Error message when parsing SVG file */
	RS_String parseMessage;
	/** Xml file reader */
	RB_XmlReader reader;
};

#endif
