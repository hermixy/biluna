/****************************************************************************
** $Id: gv_patternlist.cpp 4862 2007-03-07 15:32:00Z andrew $
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


#include "gv_patternlist.h"

//#include "gv_fileinfo.h"
#include <QStringList>
//#include "gv_system.h"

GV_PatternList* GV_PatternList::uniqueInstance = NULL;



/**
 * Default constructor.
 */
GV_PatternList::GV_PatternList() : iter(patterns) {
    patterns.setAutoDelete(true);
    //patternListListeners.setAutoDelete(false);
}



/**
 * Initializes the pattern list by creating empty GV_Pattern 
 * objects, one for each pattern that could be found.
 */
void GV_PatternList::init() {
    RB_DEBUG->print("GV_PatternList::initPatterns");

//    QStringList list = GV_SYSTEM->getPatternList();
//    GV_Pattern* pattern;

//    patterns.clear();

//    for (QStringList::Iterator it = list.begin();
//            it != list.end(); ++it) {
//        RB_DEBUG->print("pattern: %s:", (const char*)(*it).toLatin1());

//        GV_FileInfo fi(*it);
//        pattern = new GV_Pattern(fi.baseName().toLower());
//        patterns.append(pattern);

//        RB_DEBUG->print("base: %s", (const char*)pattern->getFileName().toLatin1());
//    }
}



/**
 * Removes all patterns in the patternlist.
 */
void GV_PatternList::clearPatterns() {
    patterns.clear();
}



/**
 * Removes a pattern from the list.
 * Listeners are notified after the pattern was removed from 
 * the list but before it gets deleted.
 */
void GV_PatternList::removePattern(GV_Pattern* pattern) {
    RB_DEBUG->print("GV_PatternList::removePattern()");

    // here the pattern is removed from the list but not deleted
    patterns.removeAt(patterns.indexOf(pattern));

    //for (uint i=0; i<patternListListeners.count(); ++i) {
    //    GV_PatternListListener* l = patternListListeners.at(i);
    //    l->patternRemoved(pattern);
    //}
}



/**
 * @return Pointer to the pattern with the given name or
 * \p NULL if no such pattern was found. The pattern will be loaded into
 * memory if it's not already.
 */
GV_Pattern* GV_PatternList::requestPattern(const QString& name) {
//    RB_DEBUG->print("GV_PatternList::requestPattern %s", (const char*)name.toLatin1());

    QString name2 = name.toLower();
    GV_Pattern* foundPattern = NULL;

//    RB_DEBUG->print("name2: %s", (const char*)name2.toLatin1());

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

    
bool GV_PatternList::contains(const QString& name) {
    QString name2 = name.toLower();

    // Search our list of available patterns:
    for (int i=0; i<patterns.size(); ++i) {
        if (patterns[i]->getFileName()==name2) {
            return true;
        }
    }

    return false;
}

