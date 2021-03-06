/****************************************************************************
** $Id: rs_filtercxf.h 5107 2007-03-30 09:22:29Z andrew $
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


#ifndef RS_FILTERCXF_H
#define RS_FILTERCXF_H

//#include <fstream>

#include "rs_filterinterface.h"

/**
 * This format filter class can import and export CXF (CAM Expert Font) files.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_FilterCxf : public RS_FilterInterface {
public:
    RS_FilterCxf();
    ~RS_FilterCxf() {}
    
    virtual RS_String getDescription(RS2::FormatType t) {
        if (t==RS2::FormatCxf) {
            return "Font";
        }
        else {
            return "";
        }
    }
    
    virtual RS_String getDefaultExtension(RS2::FormatType t) {
        if (t==RS2::FormatCxf) {
            return "cxf";
        }
        else {
            return "";
        }
    }

    virtual RS_String getFilterString(RS2::FormatType t, bool /*openMode*/) {
        if (t==RS2::FormatCxf) {
            return QObject::tr("Font %1").arg("(*.cxf *.CXF)");
        }
        else {
            return "";
        }
    }
    
    virtual bool fileImport(RS_Graphic& g, const RS_String& file, RS2::FormatType /*type*/);

    virtual bool fileExport(RS_Graphic& g, const RS_String& file, RS2::FormatType /*type*/);

    RS_Vector getTablePos(int unicode);

};



class CAD_EXPORT RS_FilterCxfSilent : public RS_FilterCxf {
    virtual RS_String getFilterString(RS2::FormatType /*t*/, bool /*openMode*/) {
        return "";
    }
};

#endif
