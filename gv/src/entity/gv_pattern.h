/****************************************************************************
** $Id: gv_pattern.h 8330 2008-01-20 12:38:01Z andrew $
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
** not clear to you. Updated for Biluna 2016-08-11 Updated for Biluna 2016-08-11
**
**********************************************************************/


#ifndef GV_PATTERN_H
#define GV_PATTERN_H

#include "gv_entitycontainer.h"
#include "gv_fileinfo.h"

class GV_PatternList;

/**
 * Patterns are used for hatches. They are stored in a GV_PatternList.
 * Use GV_PatternList to access a pattern.
 *
 * @author Andrew Mustun
 */
class GV_Pattern : public GV_EntityContainer {
public:
    GV_Pattern(const QString& fileName);
    virtual ~GV_Pattern();
    
    virtual GV_Entity* clone();

    virtual bool loadPattern();
    
    /** @return the fileName of this pattern. */
    QString getFileName() const {
        return fileName;
    }

    QString getName() const {
        return GV_FileInfo(fileName).baseName().toLower();
    }

protected:
    //! Pattern file name
    QString fileName;

    //! Is this pattern currently loaded into memory?
    bool loaded;

    
};


#endif
