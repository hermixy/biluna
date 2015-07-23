/*****************************************************************
 * $Id: rb_xmlstreamwriter.h 1489 2011-12-07 18:21:38Z rutger $
 * Created: Dec 5, 2011 7:29 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_XMLWRITER_H
#define RB_XMLWRITER_H

#include <QIODevice>
#include <QXmlStreamWriter>
#include "rb_string.h"
#include "rb_visitor.h"


/**
 * Base class for writing XML text to a file.
 */
class RB_XmlWriter : public RB_Visitor {

public:
    RB_XmlWriter(QIODevice* device);
    RB_XmlWriter(RB_String* str);
    virtual ~RB_XmlWriter() {}

    virtual void visitObject(RB_ObjectBase* obj);
    virtual void leaveObject(RB_ObjectBase* obj);
    
    
protected:
//    QString replaceHtmlEntities(const QString& str);

    //! Flag indicating that root element is set
    QString mRootName;
    //! Context/perspective name such as DB, ACC, etc.
    QString mContextName;

    //! XML writer
    QXmlStreamWriter mWriter;
};

#endif // RB_XMLWRITER_H
