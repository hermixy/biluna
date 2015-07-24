/*****************************************************************
 * $Id: acc_syssetxmlwriter.h 1517 2012-01-16 16:04:02Z rutger $
 * Created: Jan 16, 2012 9:53:43 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SYSSETXMLWRITER_H
#define ACC_SYSSETXMLWRITER_H

#include <QIODevice>
#include <QXmlStreamWriter>
#include "rb_string.h"
#include "rb_xmlwriter.h"


/**
 * Writer for XML system settings.
 * Writes the system settings including the current project settings.
 */
class ACC_SysSetXmlWriter : public RB_XmlWriter {

public:
    ACC_SysSetXmlWriter(QIODevice* device);
    ACC_SysSetXmlWriter(RB_String* str);
    virtual ~ACC_SysSetXmlWriter() {}

    virtual void visitObject(RB_ObjectBase* obj);
    virtual void leaveObject(RB_ObjectBase* obj);

protected:

};

#endif // ACC_SYSSETXMLWRITER_H
