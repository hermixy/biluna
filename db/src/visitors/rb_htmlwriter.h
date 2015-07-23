/*****************************************************************
 * $Id: rb_htmlwriter.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2007 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/


#ifndef RB_HTMLWRITER_H
#define RB_HTMLWRITER_H

#include "rb_string.h"
#include "rb_xmlwriter.h"


/**
 * Hyper Text Mark-up Language (HTML) writer
 * :TODO Add error checking for string/entry length.
 */
class DB_EXPORT RB_HtmlWriter : public RB_XmlWriter {

public:
    RB_HtmlWriter(QIODevice* device);
    RB_HtmlWriter(RB_String* str);
    virtual ~RB_HtmlWriter() {}

    virtual void visitObject(RB_ObjectBase* obj);
    virtual void leaveObject(RB_ObjectBase* obj);

    virtual void writePlainText(const RB_String& str);

    virtual void writeStartHead();
    virtual void writeEndHead();
    virtual void writeTitle(const RB_String& str);
    
    virtual void writeStartBody();
    virtual void writeEndBody();

protected:
    virtual void writeStartDocument();
    virtual void writeEndDocument();
    
private:	
    /** Element HTML content */
    QString* mHtml;

};

#endif // RB_XMLWRITER_H
