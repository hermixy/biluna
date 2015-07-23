/*****************************************************************
 * $Id: db_xmlreader.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 5, 2011 5:53:43 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_XMLREADER_H
#define DB_XMLREADER_H

#include <stack>
#include <QXmlStreamReader>
#include "rb_xmlreader.h"

/**
 * XML reader of hierachical XML exports from a perspective
 */
class DB_EXPORT DB_XmlReader : public RB_XmlReader{

public:
    DB_XmlReader(RB_ObjectBase* obj);
    virtual ~DB_XmlReader() { }

    virtual bool read(QIODevice *device);

protected:
    virtual void readXml();
    virtual void readListContainer();
    virtual void readObject();
    virtual void readMember();

};

#endif // DB_XMLREADER_H
