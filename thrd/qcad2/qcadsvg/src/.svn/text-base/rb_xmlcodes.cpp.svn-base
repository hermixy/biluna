/*****************************************************************
 * $Id: rb_xmlcodes.cpp - Jul 9, 2005 3:59:06 PM - rutger $
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the enclosing_project project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_xmlcodes.h"
#include <QRegExp>



/**
 * Default constructor.
 */
RB_XmlCodes::RB_XmlCodes() {

}


/**
 * Destructor.
 */
RB_XmlCodes::~RB_XmlCodes() {}


QString RB_XmlCodes::replaceHtmlEntities(const QString& str) {
    QString s = str;
    // check if ampersand is already used in SVG style
    QRegExp rx("&(?!#x)");
    s.replace( rx, "&amp;" );
    
    s.replace( ">", "&gt;" );
    s.replace( "<", "&lt;" );
    s.replace( "\"", "&quot;" );
    s.replace( "\'", "&apos;" );
    return s;
}
