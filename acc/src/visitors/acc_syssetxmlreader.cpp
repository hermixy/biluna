/*****************************************************************
 * $Id: acc_syssetxmlreader.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jan 16, 2012 9:53:43 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_syssetxmlreader.h"

#include "db_objectfactory.h"
#include "rb_objectbase.h"


/**
 * Constructor
 */
ACC_SysSetXmlReader::ACC_SysSetXmlReader(RB_ObjectBase* obj) {
    mRoot = obj;
    mObjectFactory = NULL;
    mPerspective = "";
    mVersion = "";
    mIsNewId = false;
    mIsRootRead = false;
}

/**
 * Read from device or file and check first element type/version
 * @param device device to read XML from
 * @returns true if XML has been read without error
 */
bool ACC_SysSetXmlReader::read(QIODevice *device) {
    if (!mRoot) {
        return false;
    }
    mReader.setDevice(device);

    // Make sure stack is empty
    while (mObjectStack.size() != 0) {
        RB_ObjectBase* obj = mObjectStack.top();
        delete obj;
        mObjectStack.pop();
    }

    if (mReader.readNextStartElement()) {
        readXml();
    } else {
        mReader.raiseError("Could not read file.");
    }

    return !mReader.error();
}

/**
 * @returns error string
 */
QString ACC_SysSetXmlReader::errorString() const {
    return QObject::tr("%1\nLine %2, column %3")
            .arg(mReader.errorString())
            .arg(mReader.lineNumber())
            .arg(mReader.columnNumber());
}

/**
 * Set new ID for objects
 * @param isNewID true if new ID, default is false
 */
void ACC_SysSetXmlReader::setNewId(bool isNewId = true) {
    mIsNewId = isNewId;
}

/**
 * Read XML file
 */
void ACC_SysSetXmlReader::readXml() {
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
void ACC_SysSetXmlReader::readListContainer() {
    if (!mReader.isStartElement()) return;

    RB_ObjectContainer* parent = NULL;

    if (mObjectStack.size() != 0) {
        parent = dynamic_cast<RB_ObjectContainer*>(mObjectStack.top());
    } else {
        mReader.raiseError("Read xxxList element while stack is empty ERROR.");
        mReader.skipCurrentElement();
        return;
    }

    RB_String name = mReader.name().toString();
    RB_ObjectContainer* objL = parent->newContainer(name);
    mObjectStack.push(objL);

    readXml();
}

/**
 * Read object
 */
void ACC_SysSetXmlReader::readObject() {
    if (!mReader.isStartElement()) return;

    RB_ObjectContainer* parent = NULL;

    if (mIsRootRead) {
        parent = dynamic_cast<RB_ObjectContainer*>(mObjectStack.top());
    }

    RB_String strId, strCreated, strChanged, strUser;
    int intStatus = 0;

    QXmlStreamAttributes attrs =  mReader.attributes();
    int count = attrs.count();

    // Read attributes, parent and name are set by object factory
    for (int i = 0; i < count; ++i) {
        if (attrs.at(i).name() == "id") {
            if (!mIsNewId) {
                strId = attrs.at(i).value().toString();
            } else {
                strId = RB_Uuid::createUuid().toString();
            }
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
        obj = parent->newObject(strId);
    } else {
        obj = mRoot;
        mRoot->setName(mReader.name().toString());
        mIsRootRead = true;
    }

    if (obj) {
        obj->setStatus(intStatus);
        obj->setCreated(strCreated);
        obj->setChanged(strChanged);
        obj->setUser(strUser);

        obj->deleteFlag(RB2::FlagFromDatabase);
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
void ACC_SysSetXmlReader::readMember() {
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

    RB_ObjectMember* mem = obj->addMember(mReader.name().toString(),
                                          "-", RB_Variant());

    QXmlStreamAttributes attrs = mReader.attributes();
    int count = attrs.count();

    for (int i = 0; i < count; ++i) {
        if (attrs.at(i).name() == "uom") {
            // TODO: handle unit differences
        } else if (attrs.at(i).name() == "pvalue") {
            mem->setPreviousValue(attrs.at(i).value().toString());
        }
    }

    RB_String str = mReader.readElementText();
    mem->setValue(str);

    readXml();
}

