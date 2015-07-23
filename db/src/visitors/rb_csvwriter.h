/*****************************************************************
 * $Id: rb_csvwriter.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 7, 2011 3:49:10 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_CSVWRITER_H
#define RB_CSVWRITER_H

#include <QIODevice>
#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_string.h"
#include "rb_visitor.h"

/**
 * Base class for writing of Comma Separate Value (CSV) files. This writer
 * is for general use and is not related to a specific object model
 */
class DB_EXPORT RB_CsvWriter : public RB_Visitor {
	
public:
    RB_CsvWriter(QIODevice* device);
    RB_CsvWriter(RB_String* str);
    virtual ~RB_CsvWriter() {}

    virtual void visitObject(RB_ObjectBase* obj);
    virtual void leaveObject(RB_ObjectBase* obj);

    virtual void setSeparator(RB2::CsvSeparator sep) { mSeparator = sep; }

protected:
    virtual void writeCharacters(const RB_String& str);
    virtual void writeMemberName(RB_ObjectBase* obj);
    virtual void writeMemberValue(RB_ObjectBase* obj);
    
private:
    //! Flag indicating that root element is set
    QString mRootName;
    //! Context/perspective name such as DB, ACC, etc.
    QString mContextName;

    QIODevice* mDevice;
    RB_String* mString;

    /** Flag for column names written */
    bool mIsFirstRow;
    /** CSV file separator */
    RB2::CsvSeparator mSeparator;
    /** Separator character*/
    RB_String mSeparatorCharacter;

};

#endif /*RB_CSVWRITER_H*/
