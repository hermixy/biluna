/****************************************************************************
** $Id: gv_fontlist.h 4861 2007-03-07 15:29:43Z andrew $
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


#ifndef GV_FONTLIST_H
#define GV_FONTLIST_H


#include "gv_font.h"
#include "gv_entity.h"
#include "gv_ptrlist.h"

#define GV_FONTLIST GV_FontList::instance()

/**
 * The global list of fonts. This is implemented as a singleton.
 * Use GV_FontList::instance() to get a pointer to the object.
 *
 * @author Andrew Mustun
 */
class GV_FontList {
protected:
    GV_FontList();

public:
    /**
     * @return Instance to the unique font list.
     */
    static GV_FontList* instance() {
        if (uniqueInstance==NULL) {
            uniqueInstance = new GV_FontList();
        }
        return uniqueInstance;
    }
    static void deleteInstance();

    virtual ~GV_FontList() {}

    void init();

    void clearFonts();
    int countFonts() {
        return fonts.count();
    }
    virtual void removeFont(GV_Font* font);
    GV_Font* requestFont(const QString& name);
    //! @return First font of the list.
    GV_Font* firstFont() {
        return fonts.first();
    }
    /** 
     * @return Next font from the list after
     * calling firstFont() or nextFont().
     */
    GV_Font* nextFont() {
        return fonts.next();
    }

protected:
    static GV_FontList* uniqueInstance;

private:
    //! list of available fonts
    GV_PtrList<GV_Font> fonts;
};

#endif
