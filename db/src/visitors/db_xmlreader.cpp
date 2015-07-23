/*****************************************************************
 * $Id: db_xmlreader.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Dec 5, 2011 5:53:43 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_xmlreader.h"

#include "db_objectfactory.h"
#include "rb_objectbase.h"


/**
 * Constructor
 */
DB_XmlReader::DB_XmlReader(RB_ObjectBase* obj)
        : RB_XmlReader(obj) {
    // nothing
}

/**
 * Read from device or file and check first element type/version
 * @param device device to read XML from
 * @returns true if XML has been read without error
 */
bool DB_XmlReader::read(QIODevice *device) {
    if (!mRoot) {
        mReader.raiseError("Root not set.");
        return false;
    }
    mReader.setDevice(device);

    // Make sure stack is empty
    while (mObjectStack.size() != 0) {
        RB_ObjectBase* obj = mObjectStack.top();
        delete obj;
        mObjectStack.pop();
    }

    mObjectFactory = mRoot->getFactory();

    if (mObjectFactory) {
        mPerspective = mObjectFactory->getPerspective();
        mVersion = mObjectFactory->getVersion();
    } else {
        mReader.raiseError("No perspective factory available.");
        return false;
    }

    if (mReader.readNextStartElement()) {
        if (mReader.name() == mPerspective
                && mReader.attributes().value("version") == mVersion) {
            readXml();
        } else {
            mReader.raiseError("The file is not an "
                               + mPerspective + " version "
                               + mVersion + " file.");
        }
    } else {
        mReader.raiseError("Could not read file.");
    }

    return !mReader.error();
}

/**
 * Read XML file
 */
void DB_XmlReader::readXml() {
    if (!mReader.isStartElement()) return;

    RB_ObjectContainer* obj = NULL;

    if (mIsRootRead) {
        obj = dynamic_cast<RB_ObjectContainer*>(mObjectStack.top());
    }

    // mReader.readNextStartElement() returns false when end element is reached,
    // has read name, attributes and element inner text, untill next start element.
    while (mReader.readNextStartElement()) {
        if (mReader.name().toString().endsWith("List")) {
            readListContainer();
            mObjectStack.pop();
        } else if (!mIsRootRead) {
            readObject();
            mObjectStack.pop();
        } else if (obj && obj->getName().endsWith("List")) {
            readObject();
            mObjectStack.pop();
        } else {
            readMember();
        }
    }
}

/**
 * Read list container
 */
void DB_XmlReader::readListContainer() {
    if (!mReader.isStartElement()) return;

    RB_ObjectContainer* parent = NULL;

    if (mObjectStack.size() != 0) {
        parent = dynamic_cast<RB_ObjectContainer*>(mObjectStack.top());
    } else {
        mReader.raiseError("Read xxxList element while stack is empty ERROR.");
        mReader.skipCurrentElement();
    }

    RB_ObjectContainer* objL = parent->getContainer(mReader.name().toString());

    if (objL) {
        mObjectStack.push(objL);
    } else {
        mReader.raiseError("Read xxxList element not found ERROR.");
        mReader.skipCurrentElement();
    }

    readXml();
}

/**
 * Read object
 */
void DB_XmlReader::readObject() {
    if (!mReader.isStartElement()) return;

    RB_ObjectContainer* parent = NULL;

    if (mIsRootRead) {
        parent = dynamic_cast<RB_ObjectContainer*>(mObjectStack.top());
    }

    RB_String strId, strParent, strCreated, strChanged, strUser;
    int intStatus = 0;

    QXmlStreamAttributes attrs =  mReader.attributes();
    int count = attrs.count();

    // Read attributes, parent and name have been set by object factory
    // except the parent of the XXX_Project object
    for (int i = 0; i < count; ++i) {
        if (attrs.at(i).name() == "id") {
            if (!mIsNewId) {
                strId = attrs.at(i).value().toString();
            } else {
                strId = RB_Uuid::createUuid().toString();
            }
        } else if (attrs.at(i).name() == "parent"
                   && mReader.name().toString().endsWith("_Project",
                                                         Qt::CaseInsensitive)) {
            strParent = attrs.at(i).value().toString();
        } else if (attrs.at(i).name() == "status") {
            intStatus = attrs.at(i).value().toString().toInt();
        } else if (attrs.at(i).name() == "created") {
            strCreated = attrs.at(i).value().toString();
        } else if (attrs.at(i).name() == "changed") {
            strChanged = attrs.at(i).value().toString();
        } else if (attrs.at(i).name() == "muser") {
            strUser = attrs.at(i).value().toString();
            if (strUser.trimmed().isEmpty()) {
                strUser = "file";
            }
        }
    }

    RB_ObjectBase* obj = NULL;

    if (mIsRootRead) {
        obj = mObjectFactory->newObject(strId, parent, "", true);
    } else {
        obj = mRoot;
        obj->setValue("parent", strParent); // only for XXX_Project
        mIsRootRead = true;
    }

    if (obj) {
        obj->setStatus(intStatus);
        obj->setCreated(strCreated);
        obj->setChanged(strChanged);
        obj->setUser(strUser);

        obj->delFlag(RB2::FlagFromDatabase);
        obj->setFlag(RB2::FlagIsDirty);

        mObjectStack.push(obj);
    } else {
        mReader.raiseError("New object ERROR.");
        mReader.skipCurrentElement();
    }

    readXml();
}

/**
 * Read member
 * TODO: handle unit differences
 */
void DB_XmlReader::readMember() {
    if (!mReader.isStartElement()) return;

    RB_ObjectBase* obj = NULL;

    if (mObjectStack.size() != 0) {
        obj = mObjectStack.top();

        if (!obj || obj->getName().endsWith("List")) {
            mReader.raiseError("Object is NULL or List ERROR.");
            return;
        }
    } else {
        mReader.raiseError("Read xxxList element while stack is empty ERROR.");
        return;
    }

    RB_ObjectMember* mem = obj->getMember(mReader.name().toString());

    if (mem) {
        QXmlStreamAttributes attrs = mReader.attributes();
        int count = attrs.count();

        for (int i = 0; i < count; ++i) {
            if (attrs.at(i).name() == "uom") {
                // TODO: handle unit differences
            } else if (attrs.at(i).name() == "pvalue") {
                mem->setPreviousValue(attrs.at(i).value().toString());
            } else if (attrs.at(i).name() == "dvalue") {
                mem->setDisplayValue(attrs.at(i).value().toString());
            }
        }

        RB_String str = mReader.readElementText();
        mem->setValue(str);
    } else {
        mReader.raiseError("Member " + mReader.name().toString()
                           + " does not exist.");
        mReader.skipCurrentElement();
    }

    readXml();
}

