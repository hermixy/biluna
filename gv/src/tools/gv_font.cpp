/****************************************************************************
** $Id: gv_font.cpp 4862 2007-03-07 15:32:00Z andrew $
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


#include "gv_font.h"

#include <iostream>
//#include <values.h>

#include "gv_color.h"
#include "gv_file.h"
#include "gv_fileinfo.h"
#include "gv_fontchar.h"
#include "gv_math.h"
#include <QregExp>
#include <QString>
#include "gv_system.h"
#include "gv_textstream.h"


/**
 * Constructor.
 *
 * @param owner true if the font owns the letters (blocks). Otherwise 
 *              the letters will be deleted when the font is deleted.
 */
GV_Font::GV_Font(const QString& fileName, bool owner)
        :    letterList(owner) {
    this->fileName = fileName;
    encoding = "";
    loaded = false;
    letterSpacing = 3.0;
    wordSpacing = 6.75;
    lineSpacingFactor = 1.0;
}

    
GV_Font::~GV_Font() {
    RB_DEBUG->print("GV_Font::~GV_Font");
}


/**
 * Loads the font into memory.
 *
 * @retval true font was already loaded or is loaded now.
 * @retval false font could not be loaded.
 */
bool GV_Font::loadFont() {
    RB_DEBUG->print("GV_Font::loadFont");

    if (loaded) {
        RB_DEBUG->print("GV_Font::loadFont: font already loaded");
        return true;
    }

    QString path;

    // Search for the appropriate font if we have only the name of the font:
    if (!fileName.toLower().contains(".cxf")) {
        QStringList fonts = GV_SYSTEM->getFontList();
        GV_FileInfo file;
        for (QStringList::Iterator it = fonts.begin();
                it!=fonts.end();
                it++) {

            if (GV_FileInfo(*it).baseName().toLower()==fileName.toLower()) {
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
        RB_DEBUG->print(RB_Debug::D_WARNING,
            "GV_Font::loadFont: font not available.");
        return false;
    }

    // Open cxf file:
    GV_File f(path);
    if (!f.open(QIODevice::ReadOnly)) {
        RB_DEBUG->print(RB_Debug::D_WARNING,
            "GV_Font::loadFont: Cannot open font file");
        return false;
    } else {
        RB_DEBUG->print("GV_Font::loadFont: found font file");
    }

    GV_TextStream ts(&f);
    ts.setCodec("UTF-8");
    QString line;
    int lineCount = 0;

    // Read line by line until we find a new letter:
    while (!ts.atEnd()) {
        line = ts.readLine();
        RB_DEBUG->print("GV_Font::loadFont: line: " + line);
        lineCount++;
        
        if (line.isEmpty()) {
            continue;
        }

        // Read font settings:
        if (line.at(0)=='#') {
            QStringList lst = line.right(line.length()-1).split(':');
                //QStringList::split(':', line.right(line.length()-1));
            QStringList::Iterator it3 = lst.begin();

            QString identifier;
            QString value;

            if (it3!=lst.end()) {
                identifier = (*it3).trimmed();
                it3++;
                if (it3!=lst.end()) {
                    value = (*it3).trimmed();
                }
            }

            RB_DEBUG->print("GV_Font::loadFont: meta info found:"
                + identifier + ": " + value);

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
                ts.setCodec(GV_TextCodec::codecForName(value.toLatin1()));
                encoding = value;
            }
        }

        // Add another letter to this font:
        else if (line.at(0)=='[') {
            RB_DEBUG->print("GV_Font::loadFont: character found");

            // uniode character:
            QChar ch;

            // read unicode:
            QRegExp regexp("[0-9A-Fa-f]{4,4}");
            regexp.indexIn(line);
            QString cap = regexp.cap();
            if (!cap.isNull()) {
                int uCode = cap.toInt(NULL, 16);
                ch = QChar(uCode);
//                RB_DEBUG->print("GV_Font::loadFont: "
//                    "unicode character found: %x", uCode);
            }

            // read UTF8 (qcad 1 compatibility)
            else if (line.indexOf(']')>=3) {
                int i = line.indexOf(']');
                QString mid = line.mid(1, i-1);
                ch = QString::fromUtf8(mid.toLatin1()).at(0);
//                RB_DEBUG->print("GV_Font::loadFont: "
//                    "old QCad 1 character found: %x", ch.unicode());
            }

            // read normal ascii character:
            else {
                ch = line.at(1);
//                RB_DEBUG->print("GV_Font::loadFont: "
//                    "latin1 character found: %x", ch.unicode());
            }

            // create new letter:
            GV_FontChar* letter =
                new GV_FontChar(NULL, 
                                QString("%1").arg(ch), 
                                GV_Vector(0.0, 0.0));
                
            // Read entities of this letter:
            RB_DEBUG->print("GV_Font::loadFont: "
                    "reading letter entities");
            QString coordsStr;
            QStringList coords;
            QStringList::Iterator it2;
            do {
                line = ts.readLine();
                
                if (!line.isEmpty()) {
//                    RB_DEBUG->print("GV_Font::loadFont: "
//                        "line: %s", (const char*)line.toLatin1());
                    
                    coordsStr = line.right(line.length()-2);
//                    RB_DEBUG->print("GV_Font::loadFont: "
//                        "coordsStr: %s", (const char*)coordsStr.toLatin1());
                    
                    coords = coordsStr.split(',');
                    it2 = coords.begin();

                    // Line:
                    if (line.at(0)=='L') {
                        double x1 = (*it2++).toDouble();
                        double y1 = (*it2++).toDouble();
                        double x2 = (*it2++).toDouble();
                        double y2 = (*it2).toDouble();

                        GV_LineData ld(GV_Vector(x1, y1), GV_Vector(x2, y2));
                        GV_Line* line = new GV_Line(letter, ld);
                        line->setPen(GV_Pen(GV2::FlagInvalid));
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

                        GV_ArcData ad(GV_Vector(cx,cy),
                                      r, a1, a2, reversed);
                        GV_Arc* arc = new GV_Arc(letter, ad);
                        arc->setPen(GV_Pen(GV2::FlagInvalid));
                        arc->setLayer(NULL);
                        letter->addEntity(arc);
                    }
                }
            } while (!line.isEmpty());
            
            RB_DEBUG->print("GV_Font::loadFont: "
                    "calculating letter borders");

            letter->calculateBorders();
            
            RB_DEBUG->print("GV_Font::loadFont: "
                    "adding letter to letter list");
            letterList.add(letter);
        }
    }

    f.close();
    loaded = true;
    
    RB_DEBUG->print("GV_Font::loadFont OK");

    return true;
}

