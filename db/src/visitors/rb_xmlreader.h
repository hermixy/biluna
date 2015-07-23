/*****************************************************************
 * $Id: rb_xmlreader.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 23, 2011 5:53:43 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_XMLREADER_H
#define RB_XMLREADER_H

#include <stack>
#include <QXmlStreamReader>
#include "db_global.h"

class RB_ObjectBase;
class RB_ObjectFactory;

/**
 * General XML reader of XML exports from a Biluna database
 * Handles hierachical and non-hierachical files
 */
class DB_EXPORT RB_XmlReader {

public:
    RB_XmlReader(RB_ObjectBase* obj);
    virtual ~RB_XmlReader() { }

    virtual bool read(QIODevice *device);
    virtual QString errorString() const;
    virtual void setNewId(bool isNewId);

protected:
    virtual void readXml();
    virtual void readListContainer();
    virtual void readObject();
    virtual void readMember();

    QXmlStreamReader mReader;
    QString mPerspective;
    QString mVersion;
    bool mIsNewId;
    bool mIsRootRead;

    RB_ObjectBase* mRoot;
    RB_ObjectFactory* mObjectFactory;

    //! Stack (memory) container of parent objects
    std::stack<RB_ObjectBase*> mObjectStack;

};

#endif // RB_XMLREADER_H
