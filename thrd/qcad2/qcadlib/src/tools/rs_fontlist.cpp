/****************************************************************************
** $Id: rs_fontlist.cpp 4862 2007-03-07 15:32:00Z andrew $
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


#include "rs_fontlist.h"

#include "rs_fileinfo.h"
#include "rs_stringlist.h"
#include "rs_system.h"
#include "rs_hash.h"

RS_FontList* RS_FontList::uniqueInstance = NULL;



/**
 * Default constructor.
 */
RS_FontList::RS_FontList() {
    fonts.setAutoDelete(true);
    //fontListListeners.setAutoDelete(false);
}



/**
 * Deletes the one and only RS_FontList instance.
 */
void RS_FontList::deleteInstance() {
    if (uniqueInstance!=NULL) {
        delete uniqueInstance;
        uniqueInstance = NULL;
    }
}



/**
 * Initializes the font list by creating empty RS_Font 
 * objects, one for each font that could be found.
 */
void RS_FontList::init() {
    RS_DEBUG->print("RS_FontList::initFonts");

    RS_StringList list = RS_SYSTEM->getFontList();
    RS_Hash<QString, char> added; //used to remeber added fonts (avoid duplication)
    RS_Font* font;

    for ( RS_StringList::Iterator it = list.begin();
            it != list.end(); ++it ) {
        RS_DEBUG->print("font: %s:", (const char*)(*it).toLatin1());

        RS_FileInfo fi(*it);
        if (!added[fi.baseName()]) {
            font = new RS_Font(fi.baseName());
            fonts.append(font);
            added.insert(fi.baseName(), 1);
        }

        RS_DEBUG->print("base: %s", (const char*)fi.baseName().toLatin1());
    }
}



/**
 * Removes all fonts in the fontlist.
 */
void RS_FontList::clearFonts() {
    fonts.clear();
}



/**
 * Removes a font from the list.
 * Listeners are notified after the font was removed from 
 * the list but before it gets deleted.
 */
void RS_FontList::removeFont(RS_Font* font) {
    RS_DEBUG->print("RS_FontList::removeFont()");

    // here the font is removed from the list but not deleted
    fonts.removeAt(fonts.indexOf(font));

    //for (uint i=0; i<fontListListeners.count(); ++i) {
    //    RS_FontListListener* l = fontListListeners.at(i);
    //    l->fontRemoved(font);
    //}
}



/**
 * @return Pointer to the font with the given name or
 * \p NULL if no such font was found. The font will be loaded into
 * memory if it's not already.
 */
RS_Font* RS_FontList::requestFont(const RS_String& name) {
    RS_DEBUG->print("RS_FontList::requestFont %s",  (const char*)name.toLatin1());

    RS_String name2 = name.toLower();
    RS_Font* foundFont = NULL;

    // QCad 1 compatibility:
    if (name2.contains('#') && name2.contains('_')) {
        name2 = name2.left(name2.indexOf('_'));
    } else if (name2.contains('#')) {
        name2 = name2.left(name2.indexOf('#'));
    }

    RS_DEBUG->print("name2: %s", (const char*)name2.toLatin1());

    // Search our list of available fonts:
    for (int i = 0; i < fonts.size(); ++i) {
        if (fonts[i]->getFileName()==name2) {
            // Make sure this font is loaded into memory:
            fonts[i]->loadFont();
            foundFont = fonts[i];
            break;
        }
    }

    if (foundFont==NULL && name2!="standard") {
        foundFont = requestFont("standard");
    }

    return foundFont;
}

