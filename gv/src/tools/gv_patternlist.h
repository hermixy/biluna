/****************************************************************************
** $Id: gv_patternlist.h 4862 2007-03-07 15:32:00Z andrew $
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


#ifndef GV_PATTERNLIST_H
#define GV_PATTERNLIST_H


#include "gv_pattern.h"
#include "gv_entity.h"
#include "gv_ptrlist.h"

#define GV_PATTERNLIST GV_PatternList::instance()

/**
 * The global list of patterns. This is implemented as a singleton.
 * Use GV_PatternList::instance() to get a pointer to the object.
 *
 * @author Andrew Mustun
 */
class GV_PatternList {
protected:
    GV_PatternList();

public:
    /**
     * @return Instance to the unique pattern list.
     */
    static GV_PatternList* instance() {
        if (uniqueInstance==NULL) {
            uniqueInstance = new GV_PatternList();
        }
        return uniqueInstance;
    }

    virtual ~GV_PatternList() {}

    void init();

    void clearPatterns();
    int countPatterns() {
        return patterns.count();
    }
    virtual void removePattern(GV_Pattern* pattern);
    GV_Pattern* requestPattern(const QString& name);
    //! @return First pattern of the list.
    GV_Pattern* firstPattern() {
        iter.toFront();
        return nextPattern();
    }
    /** 
     * @return Next pattern from the list after
     * calling firstPattern() or nextPattern().
     */
    GV_Pattern* nextPattern() {
        if (iter.hasNext()) {
            return iter.next();
        }
        else {
            return NULL;
        }
    }

    bool contains(const QString& name);

    //void addPatternListListener(GV_PatternListListener* listener);

    //static bool test();

protected:
    static GV_PatternList* uniqueInstance;

private:
    //! patterns in the graphic
    GV_PtrList<GV_Pattern> patterns;
    GV_MutableListIterator<GV_Pattern*> iter;
    //! List of registered PatternListListeners
    //GV_List<GV_PatternListListener> patternListListeners;
}
;

#endif
