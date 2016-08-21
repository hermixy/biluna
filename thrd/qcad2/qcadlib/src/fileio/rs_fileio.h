/****************************************************************************
** $Id: rs_fileio.h 4325 2007-01-30 22:33:54Z andrew $
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

#ifndef RS_FILEIO_H
#define RS_FILEIO_H

#include "rs_entitycontainer.h"
#include "rs_filterinterface.h"

#define RS_FILEIO RS_FileIO::instance()

/**
 * API Class for importing / exporting files. 
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_FileIO {
protected:
    RS_FileIO();
    
public:
    /**
     * @return Instance to the unique import object.
     */
    static RS_FileIO* instance() {
        if (uniqueInstance==NULL) {
            uniqueInstance = new RS_FileIO();
        }
        return uniqueInstance;
    }
    
    static void deleteInstance() {
        if (uniqueInstance!=NULL) {
            delete uniqueInstance;
            uniqueInstance = NULL;
        }
    }
    
    void registerFilter(RS_FilterInterface* f);

    RS_PtrList<RS_FilterInterface> getFilterList();
    
    RS_String getFilterString(RS2::FormatType t);
    RS_StringList getAllImportFilterStrings();
    RS_StringList getAllExportFilterStrings();
    RS2::FormatType getType(const RS_String& filterString);
    RS_FilterInterface* getImportFilter(RS2::FormatType t);
    RS_FilterInterface* getExportFilter(RS2::FormatType t);

    bool fileImport(RS_Graphic& graphic, const RS_String& fileName, 
        RS2::FormatType type = RS2::FormatUnknown);
        
    bool fileExport(RS_Graphic& graphic, const RS_String& fileName,
        RS2::FormatType type = RS2::FormatUnknown);

    RS2::FormatType detectFormat(const RS_String& fileName, bool exists=true);


protected:
    static RS_FileIO* uniqueInstance;
    RS_PtrList<RS_FilterInterface> filterList;
}
;

#endif
