/*****************************************************************
 * $Id: rb_xmlcodes.h - Jul 9, 2005 4:01:43 PM - rutger $
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the enclosing_project project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_XMLCODES_H
#define RB_XMLCODES_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <QString>
#include "cad_global.h"


/**
 * Defines common XML codes and constants.
 */
class CAD_EXPORT RB_XmlCodes {
	
public:
    RB_XmlCodes();
    virtual ~RB_XmlCodes();

    /**
     * Type of mark-up language
     */
    enum TypeML {
    	TypeHTML,
    	TypeSVG,
        TypeXML
    };

    QString replaceHtmlEntities(const QString& str);
};

#endif // RB_XMLCODES_H
