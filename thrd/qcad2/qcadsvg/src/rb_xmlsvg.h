/*****************************************************************
 * $Id: rb_xmlsvg.h,v 1.22 2004/09/10 21:41:29 - rutger $
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the PNID project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/


#ifndef RB_XMLSVG_H
#define RB_XMLSVG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef __GCC2x__
#include <sstream>
#endif

#include <QSize>
#include <QString>

#include "dl_entities.h"
#include "rs_vector.h"

#include "rb_cadattributes.h"
#include "rb_xmlcodes.h"
#include "rb_xmlwriter.h"


/**
 * Helper class for writing of SVG files with XML writer (RB_XmlWriter)
 *
 * This class writes an SVG file and calls methods from the 
 * interface RS_EntityContainer to read the entities and to them
 * to the file.
 *
 * When saving entities, special values for line thickness are used because
 * SVG does not know 'engineering' lines with zero thickness. The default
 * value for line thickness is 0.2. Refer to writeLayerStyles() method. 
 */
class RB_XmlSvg {
public:
    RB_XmlSvg();
    virtual ~RB_XmlSvg();

    RB_XmlWriter* out(const char* file, RB_XmlCodes::TypeML typeML);

    void writeHeader(RB_XmlWriter& xw);
	void writeRoot(RB_XmlWriter& xw);
	
	void writeLayer(RB_XmlWriter& xw, const DL_LayerData& data);
    void writeDefinition(RB_XmlWriter& xw);
    void writeGroup(RB_XmlWriter& xw, const DL_BlockData& data, 
    					const RB_CadAttributes& attrib = RB_CadAttributes());
    void writeGroupLayerId(RB_XmlWriter& xw, const DL_LayerData& data);
        
//	void writeTest(RB_XmlWriter& xw);
        
    void closeElement(RB_XmlWriter& xw);


    void writePoint(RB_XmlWriter& xw, const DL_PointData& data,
						const RB_CadAttributes& attrib);
    void writeLine(RB_XmlWriter& xw, const DL_LineData& data,
                   		const RB_CadAttributes& attrib);

    double convertToSvgX(double x);
    double convertToSvgY(double y);
  
	void writePolyline(RB_XmlWriter& xw, const DL_PolylineData& data,
						const RB_CadAttributes& attrib);
    void writeVertex(const DL_VertexData& data);

    void writeCircle(RB_XmlWriter& xw, const DL_CircleData& data,
                     	const RB_CadAttributes& attrib);
    void writeArc(RB_XmlWriter& xw, const DL_ArcData& data,
                  		const RB_CadAttributes& attrib, 
                  		bool isDimensionLine = false);

    void writeEllipse(RB_XmlWriter& xw, const DL_EllipseData& data,
                      	const RB_CadAttributes& attrib);
    void writeUse(RB_XmlWriter& xw, const DL_InsertData& data,
                     	const DL_BlockData& blData,
                     	const RB_CadAttributes& attrib);
    void writeMText(RB_XmlWriter& xw, const DL_MTextData& data,
                    	const RB_CadAttributes& attrib);
/*  // TODO  
	void writeText(RB_XmlWriter& xw,
                    const DL_TextData& data,
                    const RB_CadAttributes& attrib); 
*/
    void writeDimAligned(RB_XmlWriter& xw, const DL_DimensionData& data,
               			const DL_DimAlignedData& edata,
                        const RB_CadAttributes& attrib);
    void writeDimLinear(RB_XmlWriter& xw, const DL_DimensionData& data,
                        const DL_DimLinearData& edata,
                        const RB_CadAttributes& attrib);
    void writeDimRadial(RB_XmlWriter& xw, const DL_DimensionData& data,
                        const DL_DimRadialData& edata,
                        const RB_CadAttributes& attrib);
    void writeDimDiametric(RB_XmlWriter& xw, const DL_DimensionData& data,
                        const DL_DimDiametricData& edata,
                        const RB_CadAttributes& attrib);
    void writeDimAngular(RB_XmlWriter& xw, const DL_DimensionData& data,
                        const DL_DimAngularData& edata,
                        const RB_CadAttributes& attrib);
/* 
	// TODO in rb_filtersvg.cpp
	writeDimAngular3P not in rb_filtersvg.cpp 
*/    
    void writeDimAngular3P(RB_XmlWriter& xw, const DL_DimensionData& data,
                        const DL_DimAngular3PData& edata,
						const RB_CadAttributes& attrib);
    void writeLeader(RB_XmlWriter& xw, const DL_LeaderData& data,
                     	const RB_CadAttributes& attrib);
    void writeLeaderVertex(RB_XmlWriter& xw, const DL_LeaderVertexData& data);
	
	/* for leader arrowHead */
    void writeMarker(RB_XmlWriter& xw); 
  
    void writeHatch1(RB_XmlWriter& xw, const DL_HatchData& data,
                     	const RB_CadAttributes& attrib);
    void writeHatch2(RB_XmlWriter& xw, const DL_HatchData& data,
                     	const RB_CadAttributes& attrib);
    void writeHatchLoop1(RB_XmlWriter& xw, const DL_HatchLoopData& data);
    void writeHatchLoop2(RB_XmlWriter& xw, const DL_HatchLoopData& data);
    void writeHatchEdge(RB_XmlWriter& xw, const DL_HatchEdgeData& data);

    int writeImage(RB_XmlWriter& xw, const DL_ImageData& data,
                   		const RB_CadAttributes& attrib);

    void writeImageDef(RB_XmlWriter& xw, int handle, const DL_ImageData& data);

	void setStyles(const RB_SvgCodes& xc);

	/**
	 * writes the <style> element and all the layer styles within <![CDATA[ .. ]]>
	 */
    void writeStyle(RB_XmlWriter& xw);




/*
    void writeLineType(RB_XmlWriter& xw,
                       const DL_LineTypeData& data);
*/
    void writeAppid(RB_XmlWriter& xw, const RS_String& name);

	void insertEntityAttrib(RB_XmlWriter& xw, const RB_CadAttributes& attrib,
							bool textEntity = false);


/*
	int getLibVersion(const char* str);
*/
//	static void test();

	/**
	 * returns the file name selected by user
	 */
	const QString getFile() const {
		return file;
	}

	/**
	 * returns the graphic size selection by user
	 */
	QSize getSize() const {
		return size;
	}

	/**
	 * sets the graphic size selection by user
	 */
	void setSize(const QSize size) {
		this->size = size;
	}
	
	/**
	 * returns the background color (black or white) selection by user
	 */
	bool isBackgroundBlack() const {
		return black;
	}

	/**
	 * sets the background color (black or white) selection by user
	 */
	void setBackgroundBlack(const bool black) {
		this->black = black;
	}

	/**
	 * returns color or black and white selection by user
	 */
	bool isBlackWhite() const {
		return bw;
	}

	/**
	 * sets color or black and white selection by user
	 */
	void setBlackWhite(const bool bw) {
		this->bw = bw;
	}
	
	/**
	 * Returns whether layers should not be used in SVG file
	 */
	bool isRemoveLayers() {
		return this->removeLayers;
	}
	
	/**
	 * Remove layers
	 */
	void setRemoveLayers(bool remLayers) {
		this->removeLayers = remLayers;
	}


	/**
	 * Returns whether blocks should be exploded to individual entities
	 */
	bool isExplodeBlocks() {
		return this->explodeBlocks;
	}
	
	/**
	 * Explode blocks
	 */
	void setExplodeBlocks(bool expBlocks) {
		this->explodeBlocks = expBlocks;
	}

	/**
	 * returns the original paper size in the units set by user
	 */
	RS_Vector getPaperSize() const {
		return paperSize;
	}

	/**
	 * sets the original paper size in the units set by user
	 */
	void setPaperSize(const RS_Vector paperSize) {
		this->paperSize = paperSize;
	}

	/**
	 * set to true to not adjust the origine in the coordinates
     *  of the entities to SVG elements
	 */
	void setIsPartOfBlock(bool bIsPartOfBlock) {
		this->isPartOfBlock = bIsPartOfBlock;
	}

	/**
	 * returns the shift of the origine in units set by the user
	 */
	RS_Vector getPaperInsertionBase() const {
		return paperInsertionBase;
	}

	/**
	 * sets the shift of the origine in units set by the user
	 */
	void setPaperInsertionBase(const RS_Vector paperInsertionBase) {
		this->paperInsertionBase = paperInsertionBase;
	}

private:
	void writeLayerStyles(RB_XmlWriter& xw);
	
    unsigned long styleHandleStd;

    double* vertices;
    int maxVertices;
    int vertexIndex;

    double* leaderVertices;
    int maxLeaderVertices;
    int leaderVertexIndex;

    // array of hatch loops
    DL_HatchLoopData* hatchLoops;
    int maxHatchLoops;
    int hatchLoopIndex;
    // array in format [loop#][edge#]
    DL_HatchEdgeData** hatchEdges;
    int* maxHatchEdges;
    int* hatchEdgeIndex;
    bool dropEdges;
/*
    // Bulge for the next vertex.
    double bulge;

    // Only the useful part of the group code
    char groupCodeTmp[DL_SVG_MAXLINE+1];
    // ...same as integer
    unsigned int groupCode;
    // Only the useful part of the group value
    char groupValue[DL_SVG_MAXLINE+1];
    // Current entity type
    int currentEntity;
    // Value of the current setting
    char settingValue[DL_SVG_MAXLINE+1];
    // Key of the current setting (e.g. "$ACADVER")
    char settingKey[DL_SVG_MAXLINE+1];
    // Stores the group codes
    char values[DL_SVG_MAXGROUPCODE][DL_SVG_MAXLINE+1];
    // First call of this method. We initialize all group values in
    //  the first call.
    bool firstCall;
*/    

    // Attributes of the current entity (layer, color, width, line type)
    RB_CadAttributes attrib;
	// library version. hex: 0x20003001 = 2.0.3.1
	int libVersion;
    

    // Variables for Qcad to SVG conversions
	QString file;                   // the selected file name and path
    QSize size;                     // size of overall SVG graphic in pixels 
    bool black;                     // background in black if true
    bool bw;                        // entities in black and white
    bool removeLayers;
    bool explodeBlocks;
    bool isPartOfBlock;             // is set to true if entity is part of block

	RS_Vector paperSize;            // overall paper size from Qcad
	RS_Vector paperInsertionBase; 	// the shift of the origine on the graphic
    
    // todo: check Qt documentation which says the class (RB_XmlCodes) requires a
    //   a copy constructor,
    //   an assignment operator and
    //   a default constructor without arguments to qualify as a value.
    QVector<RB_SvgCodes> vectorLayerStyles;   // contains the styles per layer
    QVector<RS_Vector> vectorVectors;         // contains vector coordinates
    
 
};

#endif
