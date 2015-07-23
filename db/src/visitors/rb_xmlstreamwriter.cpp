/*****************************************************************
 * $Id: rb_xmlstreamwriter.cpp 1489 2011-12-07 18:21:38Z rutger $
 * Created: Dec 5, 2011 7:29 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_xmlwriter.h"

#include <qtextcodec.h>
#include <qvariant.h>

#include "rb_debug.h"
#include "rb_objectbase.h"
#include "rb_system.h"



/**
 * Constructor
 * @param device device to be written
 */
RB_XmlWriter::RB_XmlWriter(QIODevice* device) : mWriter(device) {
    mRootName = "";
}

/**
 * Constructor
 * @param txt string to be written
 */
RB_XmlWriter::RB_XmlWriter(RB_String* str) : mWriter(str) {
    mRootName = "";
}

/**
 * Visit object and write opening element, attributes
 * < .... >
 * @param obj object
 */
void RB_XmlWriter::visitObject(RB_ObjectBase* obj) {
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
        QString str = " Generated with " + mContextName + "           ";
        mWriter.writeComment(str);
        mWriter.writeComment(" For more information visit www.red-bag.com ");
        mWriter.writeComment("********************************************");

        // Root element
        mWriter.writeStartElement(mContextName);
        mWriter.writeAttribute("version", RB_SYSTEM->getAppVersion());
    }

    // write object name and attributes
    mWriter.writeStartElement(obj->getName());
    RB_String str;

    // write attributes of object
    if (!obj->getName().endsWith("List")) {
        str = obj->getId();

        if (str != "") {
            mWriter.writeAttribute("id", str);
        }

        str = obj->getValue("parent").toString();

        if (str != "") {
            mWriter.writeAttribute("parent", str);
        }

        mWriter.writeAttribute("status", RB_String::number(obj->getStatus()));
        mWriter.writeAttribute("created", obj->getCreated());
        mWriter.writeAttribute("changed", obj->getChanged());
        mWriter.writeAttribute("muser", obj->getUser());
    }

    // write members with their attributes
    if (!obj->getName().endsWith("List")) {
        for (int i = RB2::HIDDENCOLUMNS; i < obj->countMember(); i++) {
            RB_ObjectMember* mem = obj->getMember(i);
            mWriter.writeStartElement(mem->getName());

            str = mem->getId();
            if (str.trimmed() != "") {
                mWriter.writeAttribute("id", str);
            }

            str = mem->getUnit();
            if (str.trimmed() != "-" && str.trimmed() != "") {
                mWriter.writeAttribute("uom", str);
            }

            RB_Variant var = mem->getPreviousValue();
            if (var.isValid() && var != mem->getValue()) {
                mWriter.writeAttribute("pvalue", var.toString().trimmed());
            }

            var = mem->getDisplayValue();
            if (var.isValid() && var != mem->getValue()) {
                mWriter.writeAttribute("dvalue", var.toString().trimmed());
            }

            mWriter.writeCharacters(mem->getValue().toString().trimmed());
            mWriter.writeEndElement();
        }
    }
}

/**
 * Leave object and write closing element
 * </ .... >
 * @param obj object
 */
void RB_XmlWriter::leaveObject(RB_ObjectBase* obj) {
    mWriter.writeEndElement();

    // handle (last) perspective element
    if (obj->getName() == mRootName) {
        mWriter.writeEndElement();
        mWriter.writeEndDocument();
    }
}

/**
 * Replace symbols and other characters to HTLM entities
 */
//QString RB_XmlWriter::replaceHtmlEntities(const QString& str) {
//    QString s = str;
//    // check if ampersand is already used in SVG style
//    QRegExp rx("&(?!#x)");
//    s.replace( rx, "&amp;" );

//    s.replace( ">", "&gt;" );
//    s.replace( "<", "&lt;" );
//    s.replace( "\"", "&quot;" );
//    s.replace( "\'", "&apos;" );
//    return s;
//}

