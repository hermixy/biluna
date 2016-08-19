/*****************************************************************
 * $Id: rb_xmlwriter.cpp - Jul 9, 2005 4:00:08 PM - rutger $
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <iostream>
#include <stdio.h>
#include <qtextcodec.h>

#include "rs_debug.h"
#include "rb_xmlwriter.h"


/**
 * Constructor
 * @param fname name of XML file 
 * @param type type of Mark-up Language (XML, HTML or SVG
 */
RB_XmlWriter::RB_XmlWriter(char* fname, RB_XmlCodes::TypeML type) 
													: m_ofile(fname) {
	RS_DEBUG->print("RB_XmlWriter::RB_XmlWriter()");
	
    indent = "  ";
	typeML = type;
}


/**
 * Closes the output file, required to write the output file.
 */
void RB_XmlWriter::close() const {
    m_ofile.close();
}


/**
 * @retval true Opening file failed.
 * @retval false Otherwise.
 */
bool RB_XmlWriter::openFailed() const {
	return m_ofile.fail();
}


/**
 * Write the header element 
 * <?xml .... ?>
 */
void RB_XmlWriter::headerElement() {
    // RS_DEBUG->print("RB_XmlWriter::headerElement...");
    m_ofile << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"; 
}


/**
 * Write a corresponding style sheet (XSL/XSLT) reference
 * @param styleSheet style sheet full path and name
 */
void RB_XmlWriter::style(const QString& styleSheet) {
    // RS_DEBUG->print("RB_XmlWriter::style...");
    m_ofile << "<?xml-stylesheet type=\"text/xsl\" href=\"" 
    		<< styleSheet.toStdString() << "\"?>\n";
}


/**
 * Write the document type header element with no closing "/" mark at the end
 * <!DOCTYPE .... >
 * @param element text to be inserted
 */
void RB_XmlWriter::docTypeElement(const QString& element) {
    // RS_DEBUG->print("RB_XmlWriter::docTypeElement...");
	QString str = "<!DOCTYPE " + element + ">\n";
    m_ofile << str.toStdString();
}

/**
 * Write a comment element
 * <!-- ... comment ... -->
 * @param element text to be inserted
 */
void RB_XmlWriter::commentElement(const QString& element) {
    // RS_DEBUG->print("RB_XmlWriter::commentElement...");
	QString str = "<!-- " + element + " -->\n";
    m_ofile << str.toStdString();
}


/**
 * Write insert a CDATA element, which is not to be parsed
 * <[CDATA[ ... ]]>
 * @param element text to be inserted
 */
void RB_XmlWriter::cdataElement(const QString& element) {
    // RS_DEBUG->print("RB_XmlWriter::cdataElement...");
	QString str = "<![CDATA[" + element + "]]>\n";
    addIndent();
    m_ofile << str.toStdString();
}


/**
 * Write indent spaces depending the position of the element in the stack
 */
void RB_XmlWriter::addIndent() {
    // RS_DEBUG->print("RB_XmlWriter::addIndent...");
    int i, j;
	for (i = 0, j = (int)elementStack.count(); i < j; i++) {
        m_ofile << indent.toStdString();
    }
}


/**
 * Insert an attributes for the next (starting) element
 * @param name name of the attribute
 * @param value variant value
 */
void RB_XmlWriter::insertAttribute(const QString& name, const QVariant& value) {
    // RS_DEBUG->print("RB_XmlWriter::insertAttribute(string,variant)...");
	attributes.insert(name, value.toString());
}


/**
 * Insert an attributes for the next (starting) element
 * @param name name of the attribute
 * @param value string value
 */
void RB_XmlWriter::insertAttribute(const QString& name, const std::string& value) {
    // RS_DEBUG->print("RB_XmlWriter::insertAttribute(string,string)...");
    QVariant var(value.c_str());
	attributes.insert(name, var.toString());
}


/**
 * Insert an attributes for the next (starting) element
 * @param name name of the attribute
 * @param value const char* value
 */
void RB_XmlWriter::insertAttribute(const QString& name, const char* value) {
    // RS_DEBUG->print("RB_XmlWriter::insertAttribute(string,char)...");
    QVariant var(value);
	attributes.insert(name, var.toString());
}


/**
 * Insert an attributes for the next (starting) element
 * @param name name of the attribute
 * @param value double value
 */
void RB_XmlWriter::insertAttribute( const QString& name, double value ) {
    // RS_DEBUG->print("RB_XmlWriter::insertAttribute(string,double)...");
    QVariant var(value);
    attributes.insert(name, var.toString());
}


/**
 * Insert an attributes for the next (starting) element
 * @param name name of the attribute
 * @param value integer value
 */
void RB_XmlWriter::insertAttribute(const QString& name, int value) {
    // RS_DEBUG->print("RB_XmlWriter::insertAttribute(string,int)...");
    QVariant var(value);    
    attributes.insert(name, var.toString());
}


/**
 * Write plain content, for example between opening element and closing element
 * with HTML mark-up
 */
void RB_XmlWriter::plainContent(const QString& content) {
	// RS_DEBUG->print("RB_XmlWriter::plainContent...");
	
	addIndent();
	m_ofile << "  " << content.toStdString();
}


/**
 * Insert an attributes for the next (starting) element
 * @param name name of the attribute
 * @param value bool value
 */
void RB_XmlWriter::insertAttribute(const QString& name, bool value) {
    // RS_DEBUG->print("RB_XmlWriter::insertAttribute(string,bool)...");
	QString str;
	
    if (value) {
    	str = "TRUE";
    } else {
    	str = "FALSE";
    }
    
    attributes.insert(name, str);
}


/**
 * Write only opening element with attributes for possible child element(s)
 * @param element element name
 */
void RB_XmlWriter::push(const QString& element) {
    // RS_DEBUG->print("RB_XmlWriter::push...%s", element.toStdString().c_str());
    QString str = "<" + element;
    QMap<QString, QString>::const_iterator attrIt = attributes.begin();

    while (attrIt != attributes.end()) {
        str += " " + attrIt.key() + "=\"" + replaceHtmlEntities(*attrIt) + "\"";
        ++attrIt;
    }
    attributes.clear();
    
    str += ">\n";
    addIndent();
    m_ofile << str.toStdString();
    elementStack.push_back(element);
}


/**
 * Write an element with content and no child elements
 * @param element element name
 * @param content element variant content
 */
void RB_XmlWriter::element(const QString& element, const QVariant& content) {
    // RS_DEBUG->print("RB_XmlWriter::element...");
    baseElement(element, content.toString());
}


/**
 * Write an element with content and no child elements
 * @param element element name
 * @param content element string content
 */
void RB_XmlWriter::element(const QString& element, const std::string& content) {
    // RS_DEBUG->print("RB_XmlWriter::element...");    
    baseElement(element, content.c_str());
}


/**
 * Write an element with content and no child elements
 * @param element element name
 * @param content element const char* content
 */
void RB_XmlWriter::element(const QString& element, const char* content) {
    // RS_DEBUG->print("RB_XmlWriter::element...");    
    baseElement(element, content);
}


/**
 * Write an element with content and no child elements
 * @param element element name
 * @param content element double content
 */
void RB_XmlWriter::element(const QString& element, double content) {
    // RS_DEBUG->print("RB_XmlWriter::element...");    
    QVariant var(content);
    baseElement(element, var.toString());
}


/**
 * Write an element with content and no child elements
 * @param element element name
 * @param content element integer content
 */
void RB_XmlWriter::element(const QString& element, int content) {
    // RS_DEBUG->print("RB_XmlWriter::element...");    
    QVariant var(content);
    baseElement(element, var.toString());
}


/**
 * Write an element with content and no child elements
 * @param element element name
 * @param content element boolean content
 */
void RB_XmlWriter::element(const QString& element, bool content) {
    // RS_DEBUG->print("RB_XmlWriter::element...");    
	QString str;
	
    if (content) {
    	str = "TRUE";
    } else {
    	str = "FALSE";
    }
    
    baseElement(element, str);
}


/**
 * Write an element with no content and no child elements
 * @param element element name
 */
void RB_XmlWriter::baseElement(const QString& element, const QString& content) {
    // RS_DEBUG->print("RB_XmlWriter::baseElement...");  
    QString str = "<" + element;
    QMap<QString, QString>::const_iterator attrIt = attributes.begin();

    while (attrIt != attributes.end()) {
        str += " " + attrIt.key() + "=\"" + replaceHtmlEntities(*attrIt) + "\"";
        ++attrIt;
    }
    attributes.clear();

	str += ">" + replaceHtmlEntities(content);
	str += "</" + element + ">" + "\n";

	addIndent();        
	m_ofile << str.toStdString();
}


/**
 * Write an element with no content and no child elements
 * @param element element name
 */
void RB_XmlWriter::emptyElement(const QString& element) {
    // RS_DEBUG->print("RB_XmlWriter::emptyElement...");
    QString str = "<" + element;
    QMap<QString, QString>::const_iterator attrIt = attributes.begin();

    while (attrIt != attributes.end()) {
        str += " " + attrIt.key() + "=\"" + replaceHtmlEntities(*attrIt) + "\"";
        ++attrIt;
    }
    attributes.clear();
    
    str += " />\n";

    addIndent();
    m_ofile << str.toStdString();
}


/**
 * Write the closing element
 */
void RB_XmlWriter::pop() {
    // RS_DEBUG->print("RB_XmlWriter::pop...");
    if (!elementStack.empty() && elementStack.count() > 0) {
        QString element = elementStack[elementStack.count() - 1];
        QString str = "</" + element + ">\n";
        elementStack.pop_back();

        addIndent();
        m_ofile << str.toStdString();
        // synchronize the output with the output (file) device
    	m_ofile.flush();	
    }
}


/**
 * Replace symbols and other characters to HTLM entities
 */
QString RB_XmlWriter::replaceHtmlEntities(const QString& str) {
    // RS_DEBUG->print("RB_XmlWriter::replaceHtmlEntities...");
    RB_XmlCodes codes;
    return codes.replaceHtmlEntities(str);
}


