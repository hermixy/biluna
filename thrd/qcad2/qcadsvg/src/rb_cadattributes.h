/****************************************************************************
** $Id: rb_cadattributes.h, 2004/09/18 15:51:48 rutger Exp $
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the PNID project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_CADATTRIBUTES_H
#define RB_CADATTRIBUTES_H

#include <QString>
#include "rs.h"
#include "rs_color.h"
#include "rb_svgcodes.h"

/**
 * Storing and passing around CAD attributes. Attributes
 * are the layer name, color, width and line type.
 */
class RB_CadAttributes {

public:
    RB_CadAttributes();
    RB_CadAttributes(const QString& layerName, const QString& blockName,
                  						const RS_Color& color, double width,
                  						RS2::LineType lineType, 
                  						bool validAttrib = true);
	
	virtual ~RB_CadAttributes() { }
	
	void reset();
	
	QString getSvgLineType() const;
	// int getIntLineType(const QString& lineType) const;
	
    void setLayer(const QString& lName);
    QString getLayer() const;
    
    void setBlock(const QString& bName);
    QString getBlock() const;

    void setColor(RS_Color c);
    RS_Color getColor() const;

    QString getSvgColor() const;

    void setWidth(double width);
    double getWidth() const;
    
    double getSvgWidth() const;
    
    void setLineType(RS2::LineType lineType);
    RS2::LineType getLineType() const;
    
    void setValid(bool validAttrib);
    bool isValid() const;

    RB_CadAttributes& operator= (const RB_CadAttributes& attrib);
    
	enum color {
        black,
        green,
        red,
        brown,
        yellow,
        cyan,
        magenta,
        gray,
        blue,
        l_blue,
        l_green,
        l_cyan,
        l_red,
        l_magenta,
        l_gray,
        white,
        bylayer,
        byblock
    };
    
private:
	/** Layer name */
    QString layerName;
    /** Block name */
    QString blockName;
    /** Color for most entities except text*/ 
    RS_Color color;
    /** Line width */
    double width;
    /** Line type */
    RS2::LineType lineType;
    /** attribute for text */
    bool validAttribute;
};

#endif

// EOF
