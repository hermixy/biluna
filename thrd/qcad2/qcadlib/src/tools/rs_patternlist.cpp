/****************************************************************************
** $Id: rs_patternlist.cpp 4862 2007-03-07 15:32:00Z andrew $
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


#include "rs_patternlist.h"

#include "rs_fileinfo.h"
#include "rs_stringlist.h"
#include "rs_system.h"

RS_PatternList* RS_PatternList::uniqueInstance = NULL;



/**
 * Default constructor.
 */
RS_PatternList::RS_PatternList() : iter(patterns) {
    patterns.setAutoDelete(true);
    //patternListListeners.setAutoDelete(false);
}



/**
 * Initializes the pattern list by creating empty RS_Pattern 
 * objects, one for each pattern that could be found.
 */
void RS_PatternList::init() {
    RS_DEBUG->print("RS_PatternList::initPatterns");

    RS_StringList list = RS_SYSTEM->getPatternList();
    RS_Pattern* pattern;

    patterns.clear();

    for (RS_StringList::Iterator it = list.begin();
            it != list.end(); ++it) {
        RS_DEBUG->print("pattern: %s:", (const char*)(*it).toLatin1());

        RS_FileInfo fi(*it);
        pattern = new RS_Pattern(fi.baseName().toLower());
        patterns.append(pattern);

        RS_DEBUG->print("base: %s", (const char*)pattern->getFileName().toLatin1());
    }
}



/**
 * Removes all patterns in the patternlist.
 */
void RS_PatternList::clearPatterns() {
    patterns.clear();
}



/**
 * Removes a pattern from the list.
 * Listeners are notified after the pattern was removed from 
 * the list but before it gets deleted.
 */
void RS_PatternList::removePattern(RS_Pattern* pattern) {
    RS_DEBUG->print("RS_PatternList::removePattern()");

    // here the pattern is removed from the list but not deleted
    patterns.removeAt(patterns.indexOf(pattern));

    //for (uint i=0; i<patternListListeners.count(); ++i) {
    //    RS_PatternListListener* l = patternListListeners.at(i);
    //    l->patternRemoved(pattern);
    //}
}



/**
 * @return Pointer to the pattern with the given name or
 * \p NULL if no such pattern was found. The pattern will be loaded into
 * memory if it's not already.
 */
RS_Pattern* RS_PatternList::requestPattern(const RS_String& name) {
    RS_DEBUG->print("RS_PatternList::requestPattern %s", (const char*)name.toLatin1());

    RS_String name2 = name.toLower();
    RS_Pattern* foundPattern = NULL;

    RS_DEBUG->print("name2: %s", (const char*)name2.toLatin1());

    // Search our list of available patterns:
    for (int i=0; i<patterns.size(); ++i) {
        if (patterns[i]->getFileName()==name2) {
            // Make sure this pattern is loaded into memory:
            patterns[i]->loadPattern();
            foundPattern = patterns[i];
            break;
        }
    }

    //if (foundPattern==NULL && name!="standard") {
    //    foundPattern = requestPattern("standard");
    //}

    return foundPattern;
}

    
bool RS_PatternList::contains(const RS_String& name) {
    RS_String name2 = name.toLower();

    // Search our list of available patterns:
    for (int i=0; i<patterns.size(); ++i) {
        if (patterns[i]->getFileName()==name2) {
            return true;
        }
    }

    return false;
}

