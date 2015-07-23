/*****************************************************************
 * $Id: rb_xmlwriter.h 2248 2015-06-21 09:13:00Z rutger $
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
class DB_EXPORT RB_XmlWriter : public RB_Visitor {

public:
    RB_XmlWriter(QIODevice* device);
    RB_XmlWriter(RB_String* str);
    virtual ~RB_XmlWriter() {}

    virtual void visitObject(RB_ObjectBase* obj);
    virtual void leaveObject(RB_ObjectBase* obj);

    virtual void setUomIsColumnDefinition(bool isColDef);
    
    
protected:
    virtual void writeObject(RB_ObjectBase* obj);
//    QString replaceHtmlEntities(const QString& str);

    //! Flag indicating that root element is set
    QString mRootName;
    //! Context/perspective name such as DB, ACC, etc.
    QString mContextName;
    //! Flag indicating that 'uom' attribute should written as 'columndef'
    bool mIsUomColumnDefinition;
    //! To compare current object name with previous name
    QString mObjectName;

    //! XML writer
    QXmlStreamWriter mWriter;
};

#endif // RB_XMLWRITER_H
