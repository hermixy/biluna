/*****************************************************************
 * $Id: rb_xmlwriter.h - Jul 9, 2005 4:00:49 PM - rutger $
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/


/*
 * Important: do not replace QString, QMap, QVector etc. because this class
 * is also used in QCad
 */ 


#ifndef RB_XMLWRITER_H
#define RB_XMLWRITER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fstream>
#include <string>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QVector>

#include "rb_xmlcodes.h"


/**
 * Base class for writing XML to a file.
 */
class CAD_EXPORT RB_XmlWriter {

public:
    RB_XmlWriter(char* fname, RB_XmlCodes::TypeML typeML = RB_XmlCodes::TypeXML);
    virtual ~RB_XmlWriter() {}
    
    virtual bool openFailed() const;
	virtual void close() const;
	
    virtual void headerElement();
    virtual void style(const QString& styleSheet);
    virtual void docTypeElement(const QString& element);
    virtual void commentElement(const QString& element);
    virtual void cdataElement(const QString& element);
    virtual void addIndent();
    
	virtual void insertAttribute(const QString& name, const QVariant& value);
	virtual void insertAttribute(const QString& name, const std::string& value);
    virtual void insertAttribute(const QString& name, const char* value);
    virtual void insertAttribute(const QString& name, double value);
    virtual void insertAttribute(const QString& name, int value);
    virtual void insertAttribute(const QString& name, bool value);
    
    virtual void plainContent(const QString& content);

    virtual void push(const QString& element);
	virtual void element(const QString& element, const QVariant& content);
	virtual void element(const QString& element, const std::string& content);
    virtual void element(const QString& element, const char* content);
    virtual void element(const QString& element, double content);
    virtual void element(const QString& element, int content);
    virtual void element(const QString& element, bool content);    
    virtual void emptyElement(const QString& element);
    virtual void pop();
    
    /** Get indent (number of spaces) */
	virtual QString getIndent() const {
        return indent;
    }
    
    /** Get mark-up language type XML, HTML or SVG  */
	virtual RB_XmlCodes::TypeML getTypeML() const {
        return typeML;
    }
    
private:	
	void baseElement(const QString& element, const QString& content);
	QString replaceHtmlEntities(const QString& str);

	/** (XML) file to be created */    
	mutable std::ofstream m_ofile;
	/** Indent in spaces between elements */
    QString indent;
	/** Type of mark-up language */
    RB_XmlCodes::TypeML typeML;
	/** Element names in sequence returns last in first out */
    QVector<QString> elementStack;
	/** Attributes per each element, returns/writes in alphabetical order  */
    QMap<QString, QString> attributes;
};

#endif // RB_XMLWRITER_H
