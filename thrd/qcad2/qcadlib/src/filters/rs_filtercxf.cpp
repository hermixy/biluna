/****************************************************************************
** $Id: rs_filtercxf.cpp 7841 2008-01-11 19:34:12Z andrew $
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


#include "rs_filtercxf.h"

#include "rs_file.h"
#include "rs_font.h"
#include "rs_insert.h"
#include "rs_regexp.h"
#include "rs_system.h"
#include "rs_textstream.h"
#include "rs_utility.h"
#include "rs_text.h"


/**
 * Default constructor.
 */
RS_FilterCxf::RS_FilterCxf() : RS_FilterInterface() {

    RS_DEBUG->print("Setting up CXF filter...");

    addImportFormat(RS2::FormatCxf);
    addExportFormat(RS2::FormatCxf);
}



/**
 * Implementation of the method used for RS_Import to communicate
 * with this filter.
 *
 * @param g The graphic in which the entities from the file
 * will be created or the graphics from which the entities are
 * taken to be stored in a file.
 */
bool RS_FilterCxf::fileImport(RS_Graphic& g, const RS_String& file, RS2::FormatType /*type*/) {
    RS_DEBUG->print("RS_FilterCxf::fileImport: "
        "importing file '%s'...", (const char*)file.toLatin1());

    //this->graphic = &g;
    bool success = false;

    // Load font file as we normally do, but the font doesn't own the
    //  letters (we'll add them to the graphic instead. Hence 'false').
    RS_Font font(file, false);
    success = font.loadFont();

    if (success==false) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "Cannot open CXF file '%s'.", (const char*)file.toLatin1());
        return false;
    }

    g.addVariable("Names", font.getNames().join(","), 0);
    g.addVariable("LetterSpacing", font.getLetterSpacing(), 0);
    g.addVariable("WordSpacing", font.getWordSpacing(), 0);
    g.addVariable("LineSpacingFactor", font.getLineSpacingFactor(), 0);
    g.addVariable("Authors", font.getAuthors().join(","), 0);
    if (!font.getEncoding().isEmpty()) {
        g.addVariable("Encoding", font.getEncoding(), 0);
    }
    
    RS_BlockList* letterList = font.getLetterList();

    int maxUnicode = 0;
    for (int i=0; i<font.countLetters(); ++i) {
        RS_Block* ch = font.letterAt(i);
        if (ch->getName().at(0).unicode() > maxUnicode) {
            maxUnicode = ch->getName().at(0).unicode();
        }
    }
    
    maxUnicode = (maxUnicode / 0x80 + 1) * 0x80;

    RS_DEBUG->print("RS_FilterCxf::fileImport: maxUnicode: %x", maxUnicode);

    // create tables for glyphs:
    for (int unicode=0; unicode<=maxUnicode; unicode+=0x80) {
        RS_Vector pos = getTablePos(unicode);

        RS_Text* t = 
            new RS_Text(&g, 
                RS_TextData(
                    pos + RS_Vector(76, 40),
                    10.0,
                    100.0,
                    RS2::VAlignBottom,
                    RS2::HAlignCenter,
                    RS2::LeftToRight,
                    RS2::Exact,
                    1.0,
                    RS_String("%1 - %2")
                        .arg(unicode, 4, 16, RS_Char('0'))
                        .arg(unicode+0x7F, 4, 16, RS_Char('0')),
                    "Standard",
                    0.0));
        g.addEntity(t);
        
        for (int c=0; c<8; ++c) {
            RS_Line* l = 
                new RS_Line(&g,
                    RS_LineData(
                        RS_Vector(pos + RS_Vector(c*20-4, 16)),
                        RS_Vector(pos + RS_Vector(c*20-4, -304))));
            g.addEntity(l);
        }
        
        for (int r=0; r<16; ++r) {
            RS_Line* l = new RS_Line(&g,
                    RS_LineData(
                        RS_Vector(pos + RS_Vector( -4, -r*20-4)),
                        RS_Vector(pos + RS_Vector(156, -r*20-4))));
            g.addEntity(l);
        }
    }

    // fill tables with available glyphs and placeholders (points):
    //for (int i=0; i<font.countLetters(); ++i) {
    RS_String character;
    RS_String blockName;
    for (int unicode=0; unicode<=maxUnicode; unicode++) {
        //RS_DEBUG->print("RS_FilterCxf::fileImport: unicode: %x", unicode);
        /*
        if (!RS_Char(unicode).isPrint()) {
            continue;
        }
        */

        character = RS_String(RS_Char(unicode));
        RS_Block* ch = font.findLetter(character);
        if (ch!=NULL || unicode<=0x17f) {
            RS_DEBUG->print("RS_FilterCxf::fileImport: unicode: %x", unicode);
            blockName = 
                RS_String("[%1] ")
                    .arg(unicode, 4, 16, RS_Char('0'));
            if (RS_Char(unicode).isPrint()) {
                blockName += character;
            }
        }

        if (ch!=NULL) {
            letterList->rename(ch, blockName);
            g.addBlock(ch, false);
            ch->reparent(&g);
        }
        else if (unicode<=0x17f) {
            ch = new RS_Block(&g, 
                RS_BlockData(blockName, RS_Vector(0,0), false));
            ch->addEntity(
                new RS_Point(ch, RS_PointData(RS_Vector(0,0))));
            g.addBlock(ch, false);
        }
        else {
            ch = NULL;
        }

        if (ch!=NULL) {
            RS_Vector pos = getTablePos(unicode);
    
            RS_Insert* ins = 
                new RS_Insert(&g,
                RS_InsertData(blockName, 
                    pos,
                    RS_Vector(1,1),
                    0.0, 
                    1, 1, 
                    RS_Vector(0,0)));
            g.addEntity(ins);
        }
    }

    g.addBlockNotification();

    return true;
}



/**
 * Implementation of the method used for RS_Export to communicate
 * with this filter.
 *
 * @param file Full path to the CXF file that will be written.
 */
bool RS_FilterCxf::fileExport(RS_Graphic& g, const RS_String& file, RS2::FormatType /*type*/) {

    RS_DEBUG->print("CXF Filter: exporting file '%s'...", (const char*)file.toLatin1());

    RS_DEBUG->print("RS_FilterCxf::fileExport: open");
    //FILE* fp;
    RS_File f(file);

    //if ((fp = fopen((const char*)file.toLatin1(), "wt")) != NULL) {
    if (f.open(RS_File::WriteOnly | RS_File::Truncate)) {
        RS_TextStream ts(&f);
        ts.setRealNumberPrecision(4);
        ts.setRealNumberNotation(QTextStream::FixedNotation);
        ts.setCodec("UTF-8");

        RS_DEBUG->print("RS_FilterCxf::fileExport: open: OK");

        RS_DEBUG->print("RS_FilterCxf::fileExport: header");

        // header:
        ts << "# Format:            QCad II Font" << endl;
        ts << "# Creator:           " << RS_SYSTEM->getAppName() << endl;
        ts << "# Version:           " << RS_SYSTEM->getAppVersion() << endl;

        RS_String ns = g.getVariableString("Names", "");
        if (!ns.isEmpty()) {
            RS_StringList names = ns.split(',');
            for (RS_StringList::Iterator it = names.begin(); it!=names.end(); ++it) {
                ts << "# Name:              " << *it << endl;
            }
        }

        RS_String es = g.getVariableString("Encoding", "");
        if (!es.isEmpty()) {
            ts << "# Encoding:          " << es << endl;
        }

        ts << "# LetterSpacing:     " << 
            g.getVariableDouble("LetterSpacing", 3.0) << endl;
        ts << "# WordSpacing:       " << 
            g.getVariableDouble("WordSpacing", 6.75) << endl;
        ts << "# LineSpacingFactor: " << 
            g.getVariableDouble("LineSpacingFactor", 1.0) << endl;

        RS_String sa = g.getVariableString("Authors", "");
        RS_DEBUG->print("authors: %s", (const char*)sa.toLatin1());
        if (!sa.isEmpty()) {
            RS_StringList authors = sa.split(',');
            RS_DEBUG->print("count: %d", authors.count());

            RS_String a;
            for (RS_StringList::Iterator it2 = authors.begin();
                    it2!=authors.end(); ++it2) {

                a = RS_String(*it2);
                ts << "# Author:            " << a << endl;
            }
        }

        RS_DEBUG->print("RS_FilterCxf::fileExport: header: OK");

        // iterate through blocks (=letters of font)
        for (int i=0; i<g.countBlocks(); ++i) {
            RS_Block* blk = g.blockAt(i);

            RS_DEBUG->print("block: %d", i);

            if (blk!=NULL) {
                if (blk->count()!=0 &&
                    (blk->count()!=1 ||
                     blk->firstEntity(RS2::ResolveAll)->rtti()!=RS2::EntityPoint)) {
                
                    ts << endl << blk->getName() << endl;

                    // iterate through entities of this letter:
                    for (RS_Entity* e=blk->firstEntity(RS2::ResolveAll);
                            e!=NULL;
                            e=blk->nextEntity(RS2::ResolveAll)) {

                        if (!e->isUndone()) {

                            // lines:
                            if (e->rtti()==RS2::EntityLine) {
                                RS_Line* l = dynamic_cast<RS_Line*>(e);

                                ts << "L " 
                                   << RS_Math::doubleToString(l->getStartpoint().x, 4) << ","
                                   << RS_Math::doubleToString(l->getStartpoint().y, 4) << ","
                                   << RS_Math::doubleToString(l->getEndpoint().x, 4) << ","
                                   << RS_Math::doubleToString(l->getEndpoint().y, 4) 
                                   << endl;
                            }

                            // arcs:
                            else if (e->rtti()==RS2::EntityArc) {
                                RS_Arc* a = dynamic_cast<RS_Arc*>(e);

                                if (!a->isReversed()) {
                                    ts << "A ";
                                } else {
                                    ts << "AR ";
                                }

                                ts << RS_Math::doubleToString(a->getCenter().x, 4) << ","
                                   << RS_Math::doubleToString(a->getCenter().y, 4) << ","
                                   << RS_Math::doubleToString(a->getRadius(), 4) << ","
                                   << RS_Math::doubleToString(a->getAngle1()*ARAD, 4) << ","
                                   << RS_Math::doubleToString(a->getAngle2()*ARAD, 4) 
                                   << endl;
                            }

                            // Ignore entities other than arcs / lines
                            else {}
                        }

                    }
                }
            }
        }

        f.close();
        RS_DEBUG->print("CXF Filter: exporting file: OK");
        return true;
    }
    else {
        RS_DEBUG->print("CXF Filter: exporting file failed");
    }

    return false;
}
    
    
    
/**
 * Streams a double value to the given stream cutting away trailing 0's.
 *
 * @param value A double value. e.g. 2.700000
 */
/*
void RS_FilterCxf::stream(std::ofstream& fs, double value) {
    fs << (const char*)RS_Utility::doubleToString(value).toLatin1();
}
*/

    
RS_Vector RS_FilterCxf::getTablePos(int unicode) {
    int codeTable = unicode / 0x80;
    int column = unicode / 0x10;
    int row = unicode % 0x10;
    
    return RS_Vector(codeTable*40 + column*20, row*-20);
}

