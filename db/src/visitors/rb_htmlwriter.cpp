/*****************************************************************
 * $Id: rb_htmlwriter.cpp 1983 2013-09-02 18:10:02Z rutger $
 *
 * Copyright (C) 2007 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_htmlwriter.h"

#include "db_objectfactory.h"
#include "rb_objectbase.h"

/**
 * Constructor
 * @param device to be written
 */
RB_HtmlWriter::RB_HtmlWriter(QIODevice* device)
                : RB_XmlWriter(device) {
    mHtml = NULL;
}

/**
 * Constructor
 * @param txt string to be written
 */
RB_HtmlWriter::RB_HtmlWriter(RB_String* str)
                : RB_XmlWriter(str), mHtml(str) {
    // nothing
}

/**
 * Visit object and write opening element, attributes
 * < .... >
 * @param obj object
 */
void RB_HtmlWriter::visitObject(RB_ObjectBase* obj) {
    if (mRootName.isEmpty()) {
        mRootName = obj->getName();
        QStringList strL = mRootName.split("_");
        mContextName = strL.at(0).toUpper();

        mWriter.setAutoFormatting(true);
        // DOCTYPE and html element
        writeStartDocument();

        // head element
        writeStartHead();
        writeTitle(mContextName);
        writeEndHead();

        // body element
        writeStartBody();
    }


    // write object name and attributes
    mWriter.writeStartElement("div");
    mWriter.writeAttribute("style", "padding-left:10px;");

    mWriter.writeStartElement("p");
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

    mWriter.writeCharacters(obj->getName());
    mWriter.writeEndElement(); // </p>

    unsigned int counter = 0;

    // write members with their attributes
    if (!obj->getName().endsWith("List")) {
        mWriter.writeStartElement("table");
        mWriter.writeAttribute("border", "1");

        for (int i = RB2::HIDDENCOLUMNS; i < obj->memberCount(); i++) {
            counter = i - RB2::HIDDENCOLUMNS;
            if (counter % 2 == 0) { // even, (counter & 0) does not work?
                mWriter.writeStartElement("tr");
            }

            RB_ObjectMember* mem = obj->getMember(i);
            mWriter.writeStartElement("td");

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

            mWriter.writeCharacters(mem->getName());
            mWriter.writeEndElement();
            mWriter.writeStartElement("td");
            mWriter.writeCharacters(mem->getValue().toString().trimmed());
            mWriter.writeEndElement();

            if (counter % 2 != 0) { // uneven
                mWriter.writeEndElement(); // </tr>
            }
        }

        counter = obj->memberCount() - RB2::HIDDENCOLUMNS;

        if (counter % 2 != 0) { // uneven
            mWriter.writeStartElement("td");
            mWriter.writeEndElement();
            mWriter.writeStartElement("td");
            mWriter.writeEndElement();
            mWriter.writeEndElement(); // </tr>
        }

        mWriter.writeEndElement(); // </table>
    }
}

/**
 * Leave object and write closing element
 * </ .... >
 * @param obj object
 */
void RB_HtmlWriter::leaveObject(RB_ObjectBase* obj) {
    mWriter.writeEndElement();

    // handle (last) perspective element
    if (obj->getName() == mRootName) {
        writeEndBody();
        writeEndDocument();
    }
}

/**
 * Write plaint text, characters such as < are not escaped
 */
void RB_HtmlWriter::writePlainText(const RB_String& str) {
    QIODevice* device = mWriter.device();
    if (device) {
        device->write(str.toStdString().c_str());
    } else {
        mHtml->append(str);
    }
}

/**
 * Write start head with basic head content without title element
 *   <head>
 *       <meta http-equiv="Content-type" content="text/html; charset=utf-8" />
 *       <meta http-equiv="Content-Language" content="en-us" />
 *       <meta generator="biluna.com />
 */
void RB_HtmlWriter::writeStartHead() {
    mWriter.writeStartElement("head");
    mWriter.writeEmptyElement("meta");
    mWriter.writeAttribute("http-equiv", "Content-type");
    mWriter.writeAttribute("content", "text/html; charset=utf-8");
    mWriter.writeEmptyElement("meta");
    mWriter.writeAttribute("http-equiv", "Content-Language");
    mWriter.writeAttribute("content", "en-us");
    mWriter.writeEmptyElement("meta");
    mWriter.writeAttribute("generator", "biluna.com");
}

/**
 * Write end head element, same as writeEndElement()
 */
void RB_HtmlWriter::writeEndHead() {
    mWriter.writeEndElement();
}

/**
 * Write end head element, same as writeEndElement()
 * @param str title text
 */
void RB_HtmlWriter::writeTitle(const RB_String& str) {
    mWriter.writeStartElement("title");
    mWriter.writeCharacters(str);
    mWriter.writeEndElement();
}

/**
 * Write start body element, same as writeStartElement("body")
 *   <body>
 */
void RB_HtmlWriter::writeStartBody() {
    mWriter.writeStartElement("body");
}

/**
 * Write end body element, same as writeEndElement()
 */
void RB_HtmlWriter::writeEndBody() {
    mWriter.writeEndElement();
}

/**
 * Write DOCTYPE and HTML element, strict XHTML
 *   <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
 *       "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
 *   <html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
 */
void RB_HtmlWriter::writeStartDocument() {
    //    <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    //        "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
    //    <html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
    //    <head>
    //        <meta http-equiv="Content-type" content="text/html; charset=utf-8" />
    //        <meta http-equiv="Content-Language" content="en-us" />
    //        <title>XHTML Template</title>
    //    </head>
    //    <body>
    //    </body>
    //    </html>

    RB_String str = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" "
                    "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"
                    "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">";
    writePlainText(str);
}

/**
 * Write closing HTML element with \n
 */
void RB_HtmlWriter::writeEndDocument() {
    RB_String str = "\n</html>\n";
    writePlainText(str);
}

