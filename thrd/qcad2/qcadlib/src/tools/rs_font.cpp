/****************************************************************************
** $Id: rs_font.cpp 4862 2007-03-07 15:32:00Z andrew $
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


#include "rs_font.h"

#include <iostream>
//#include <values.h>

#include "rs_color.h"
#include "rs_file.h"
#include "rs_fileinfo.h"
#include "rs_fontchar.h"
#include "rs_math.h"
#include "rs_regexp.h"
#include "rs_string.h"
#include "rs_system.h"
#include "rs_textstream.h"


/**
 * Constructor.
 *
 * @param owner true if the font owns the letters (blocks). Otherwise 
 *              the letters will be deleted when the font is deleted.
 */
RS_Font::RS_Font(const RS_String& fileName, bool owner)
        :    letterList(owner) {
    this->fileName = fileName;
    encoding = "";
    loaded = false;
    letterSpacing = 3.0;
    wordSpacing = 6.75;
    lineSpacingFactor = 1.0;
}

    
RS_Font::~RS_Font() {
    RS_DEBUG->print("RS_Font::~RS_Font");
}


/**
 * Loads the font into memory.
 *
 * @retval true font was already loaded or is loaded now.
 * @retval false font could not be loaded.
 */
bool RS_Font::loadFont() {
    RS_DEBUG->print("RS_Font::loadFont");

    if (loaded) {
        RS_DEBUG->print("RS_Font::loadFont: font already loaded");
        return true;
    }

    RS_String path;

    // Search for the appropriate font if we have only the name of the font:
    if (!fileName.toLower().contains(".cxf")) {
        RS_StringList fonts = RS_SYSTEM->getFontList();
        RS_FileInfo file;
        for (RS_StringList::Iterator it = fonts.begin();
                it!=fonts.end();
                it++) {

            if (RS_FileInfo(*it).baseName().toLower()==fileName.toLower()) {
                path = *it;
                break;
            }
        }
    }

    // We have the full path of the font:
    else {
        path = fileName;
    }

    // No font paths found:
    if (path.isEmpty()) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "RS_Font::loadFont: font not available.");
        return false;
    }

    // Open cxf file:
    RS_File f(path);
    if (!f.open(QIODevice::ReadOnly)) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "RS_Font::loadFont: Cannot open font file: %s", 
            (const char*)path.toLatin1());
        return false;
    } else {
        RS_DEBUG->print("RS_Font::loadFont: "
            "found font file: %s", 
            (const char*)path.toLatin1());
    }

    RS_TextStream ts(&f);
    ts.setCodec("UTF-8");
    RS_String line;
    int lineCount = 0;

    // Read line by line until we find a new letter:
    while (!ts.atEnd()) {
        line = ts.readLine();
        RS_DEBUG->print("RS_Font::loadFont: line: '%s'", (const char*)line.toLatin1());
        lineCount++;
        
        if (line.isEmpty()) {
            continue;
        }

        // Read font settings:
        if (line.at(0)=='#') {
            RS_StringList lst = line.right(line.length()-1).split(':');
                //RS_StringList::split(':', line.right(line.length()-1));
            RS_StringList::Iterator it3 = lst.begin();

            RS_String identifier;
            RS_String value;

            if (it3!=lst.end()) {
                identifier = (*it3).trimmed();
                it3++;
                if (it3!=lst.end()) {
                    value = (*it3).trimmed();
                }
            }

            RS_DEBUG->print("RS_Font::loadFont: meta info found: %s: %s", 
                (const char*)identifier.toLatin1(), (const char*)value.toLatin1());

            if (identifier.toLower()=="letterspacing") {
                letterSpacing = value.toDouble();
            } else if (identifier.toLower()=="wordspacing") {
                wordSpacing = value.toDouble();
            } else if (identifier.toLower()=="linespacingfactor") {
                lineSpacingFactor = value.toDouble();
            } else if (identifier.toLower()=="author") {
                authors.append(value);
            } else if (identifier.toLower()=="name") {
                names.append(value);
            } else if (identifier.toLower()=="encoding") {
                ts.setCodec(RS_TextCodec::codecForName(value.toLatin1()));
                encoding = value;
            }
        }

        // Add another letter to this font:
        else if (line.at(0)=='[') {
            RS_DEBUG->print("RS_Font::loadFont: character found");

            // uniode character:
            RS_Char ch;

            // read unicode:
            RS_RegExp regexp("[0-9A-Fa-f]{4,4}");
            regexp.indexIn(line);
            RS_String cap = regexp.cap();
            if (!cap.isNull()) {
                int uCode = cap.toInt(NULL, 16);
                ch = RS_Char(uCode);
                RS_DEBUG->print("RS_Font::loadFont: "
                    "unicode character found: %x", uCode);
            }

            // read UTF8 (qcad 1 compatibility)
            else if (line.indexOf(']')>=3) {
                int i = line.indexOf(']');
                RS_String mid = line.mid(1, i-1);
                ch = RS_String::fromUtf8(mid.toLatin1()).at(0);
                RS_DEBUG->print("RS_Font::loadFont: "
                    "old QCad 1 character found: %x", ch.unicode());
            }

            // read normal ascii character:
            else {
                ch = line.at(1);
                RS_DEBUG->print("RS_Font::loadFont: "
                    "latin1 character found: %x", ch.unicode());
            }

            // create new letter:
            RS_FontChar* letter =
                new RS_FontChar(NULL, 
                                RS_String("%1").arg(ch), 
                                RS_Vector(0.0, 0.0));
                
            // Read entities of this letter:
            RS_DEBUG->print("RS_Font::loadFont: "
                    "reading letter entities");
            RS_String coordsStr;
            RS_StringList coords;
            RS_StringList::Iterator it2;
            do {
                line = ts.readLine();
                
                if (!line.isEmpty()) {
                    RS_DEBUG->print("RS_Font::loadFont: "
                        "line: %s", (const char*)line.toLatin1());
                    
                    coordsStr = line.right(line.length()-2);
                    RS_DEBUG->print("RS_Font::loadFont: "
                        "coordsStr: %s", (const char*)coordsStr.toLatin1());
                    
                    coords = coordsStr.split(',');
                    it2 = coords.begin();

                    // Line:
                    if (line.at(0)=='L') {
                        double x1 = (*it2++).toDouble();
                        double y1 = (*it2++).toDouble();
                        double x2 = (*it2++).toDouble();
                        double y2 = (*it2).toDouble();

                        RS_LineData ld(RS_Vector(x1, y1), RS_Vector(x2, y2));
                        RS_Line* line = new RS_Line(letter, ld);
                        line->setPen(RS_Pen(RS2::FlagInvalid));
                        line->setLayer(NULL);
                        letter->addEntity(line);
                    }

                    // Arc:
                    else if (line.at(0)=='A') {
                        double cx = (*it2++).toDouble();
                        double cy = (*it2++).toDouble();
                        double r = (*it2++).toDouble();
                        double a1 = (*it2++).toDouble()/ARAD;
                        double a2 = (*it2).toDouble()/ARAD;
                        bool reversed = (line.at(1)=='R');

                        RS_ArcData ad(RS_Vector(cx,cy),
                                      r, a1, a2, reversed);
                        RS_Arc* arc = new RS_Arc(letter, ad);
                        arc->setPen(RS_Pen(RS2::FlagInvalid));
                        arc->setLayer(NULL);
                        letter->addEntity(arc);
                    }
                }
            } while (!line.isEmpty());
            
            RS_DEBUG->print("RS_Font::loadFont: "
                    "calculating letter borders");

            letter->calculateBorders();
            
            RS_DEBUG->print("RS_Font::loadFont: "
                    "adding letter to letter list");
            letterList.add(letter);
        }
    }

    f.close();
    loaded = true;
    
    RS_DEBUG->print("RS_Font::loadFont OK");

    return true;
}

