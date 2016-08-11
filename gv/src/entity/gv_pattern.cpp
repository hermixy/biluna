/****************************************************************************
** $Id: gv_pattern.cpp 8330 2008-01-20 12:38:01Z andrew $
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


#include "gv_pattern.h"

#include <QStringList>
#include "gv_system.h"
// #include "gv_fileio.h" TODO: Rutger


/**
 * Constructor.
 *
 * @param fileName File name of a DXF file defining the pattern
 */
GV_Pattern::GV_Pattern(const QString& fileName)
        : GV_Entity(NULL), GV_EntityContainer(NULL) {

    RB_DEBUG->print("GV_Pattern::GV_Pattern() ");

    this->fileName = fileName;
    loaded = false;
}



/**
 * Constructor.
 *
 * @param fileName File name of a PAT file which defines this 
 *         pattern among others.
 * @param name Pattern name.
 *
 */
/*GV_Pattern::GV_Pattern(const QString& fileName, const QString& name)
        : GV_EntityContainer(NULL) {
    this->fileName = fileName;
    this->name = name;
    loaded = false;
}*/



GV_Pattern::~GV_Pattern() {}



GV_Entity* GV_Pattern::clone() {
    GV_Pattern* ec = 
        new GV_Pattern(fileName);
    ec->loaded = loaded;
    ec->copyPropertiesFrom(this);
    ec->entities = entities;
    ec->detach();
    ec->initId();
    return ec;
}


/**
 * Loads the given pattern file into this pattern.
 * Entities other than lines are ignored.
 *
 * @param filename File name of the pattern file (without path and 
 * extension or full path.
 */
bool GV_Pattern::loadPattern() {
/*
    if (loaded) {
        return true;
    }

    RB_DEBUG->print("GV_Pattern::loadPattern");

    QString path;

    // Search for the appropriate pattern if we have only the name of the pattern:
    if (!fileName.toLower().contains(".dxf")) {
        QStringList patterns = GV_SYSTEM->getPatternList();
        GV_FileInfo file;
        for (QStringList::Iterator it = patterns.begin();
                it!=patterns.end();
                it++) {

            if (GV_FileInfo(*it).baseName().toLower()==fileName.toLower()) {
                path = *it;
                RB_DEBUG->print("Pattern found: %s", (const char*)path.toLatin1());
                break;
            }
        }
    }

    // We have the full path of the pattern:
    else {
        path = fileName;
    }

    // No pattern paths found:
    if (path.isEmpty()) {
        RB_DEBUG->print("No pattern \"%s\"available.", (const char*)fileName.toLatin1());
        return false;
    }

    GV_Graphic* gr = new GV_Graphic();

    // TODO: Find out why the new dxf filter doesn't work for patterns:
    GV_FILEIO->fileImport(*gr, path, GV2::FormatDxf1);

    for (GV_Entity* e=gr->firstEntity(); e!=NULL; e=gr->nextEntity()) {
        if (e->rtti()==GV2::EntityLine || e->rtti()==GV2::EntityArc) {
            GV_Layer* l = e->getLayer();
            GV_Entity* cl = e->clone();
            cl->reparent(this);
            if (l!=NULL) {
                cl->setLayer(l->getName());
            }
            addEntity(cl);
        }
    }
    delete gr;

    loaded = true;
    RB_DEBUG->print("GV_Pattern::loadPattern: OK");

    return true;
*/
    return false;
}

