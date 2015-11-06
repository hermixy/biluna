/*****************************************************************
 * $Id: acc_syssetxmlwriter.cpp 1979 2013-08-22 19:56:38Z rutger $
 * Created: Jan 16, 2012 9:53:43 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_syssetxmlwriter.h"

#include "acc_objectfactory.h"
#include "rb_system.h"


/**
 * Constructor
 * @param device device to be written
 */
ACC_SysSetXmlWriter::ACC_SysSetXmlWriter(QIODevice* device)
    : RB_XmlWriter(device) {
    // nothing
}

/**
 * Constructor
 * @param txt string to be written
 */
ACC_SysSetXmlWriter::ACC_SysSetXmlWriter(RB_String* str)
    : RB_XmlWriter(str) {
    // nothing
}

/**
 * Visit object and write opening element, attributes
 * < .... >
 * @param obj object
 */
void ACC_SysSetXmlWriter::visitObject(RB_ObjectBase* obj) {
    if (mRootName.isEmpty()) {
        mRootName = obj->getName();
        QStringList strL = mRootName.split("_");
        mContextName = strL.at(0).toUpper();

        // XML header
        mWriter.setAutoFormatting(true);
        // mWriter.setAutoFormattingIndent(2); // positive is space, negative is tabs
        mWriter.writeStartDocument();

        // Application name and version
        mWriter.writeComment("********************************************");
        QString str = " Generated with " + mContextName + " ";
        mWriter.writeComment(str);
        mWriter.writeComment(" For more information visit www.biluna.com  ");
        mWriter.writeComment("********************************************");

        // Root element
        mWriter.writeStartElement(mContextName);
        mWriter.writeAttribute("version", RB_SYSTEM->getAppVersion());
    }

    RB_String objName = obj->getName();

    if (objName == "ACC_Project" || objName == "DB_Project"
            || objName == "DB_ProjectList"
            || ACC_OBJECTFACTORY->isSystemSetting(objName)
            || ACC_OBJECTFACTORY->isProjectSetting(objName)) {

        // write object name and attributes
        mWriter.writeStartElement(obj->getName());

        // write attributes of object
        if (!objName.endsWith("List")) {
            writeObject(obj);
        }
    } else {
        // no more writing of this object
        setResolveLevel(RB2::ResolveNone);
    }
}

/**
 * Leave object and write closing element
 * </ .... >
 * @param obj object
 */
void ACC_SysSetXmlWriter::leaveObject(RB_ObjectBase* obj) {
    RB_String objName = obj->getName();

    if (objName == "ACC_Project" || objName == "DB_Project"
            || objName == "DB_ProjectList"
            || ACC_OBJECTFACTORY->isSystemSetting(objName)
            || ACC_OBJECTFACTORY->isProjectSetting(objName)) {

        RB_XmlWriter::leaveObject(obj);
    } else {
        // reset writing of objects
        setResolveLevel(RB2::ResolveAll);
    }
}
