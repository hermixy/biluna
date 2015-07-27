/*****************************************************************
 * $Id: gv_filterxml.h 1765 2012-09-04 19:15:27Z rutger $
 * Created: Apr 24, 2008 4:53:43 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_FILTERXML_H
#define GV_FILTERXML_H

#include <stack>
#include <QString>
#include <QXmlSimpleReader>

#include "rb_objectbase.h"
#include "rb_xmlreader.h"
#include "rb_xmlwriter.h"
#include "rb.h"

class RB_ModelFactory;
class RB_ObjectFactory;
class RB_XmlWriter;

/**
 * This format filter class can import and export XML (GV) files.
 * @author Rutger Botermans
 */
class GV_FilterXml : public QXmlDefaultHandler {

public:
    GV_FilterXml();
    virtual ~GV_FilterXml();
#if 0
    // Import:
    virtual bool fileImport(RB_ModelFactory* mf,
    				const QString& file);
	
	//sax handler functions
    bool startElement(const QString& namespaceURI,
                    const QString& localName,
                    const QString& qName,
                    const QXmlAttributes& attribs);
    bool endElement(const QString& namespaceURI,
                    const QString& localName,
                    const QString& qName);
    bool characters(const QString& str);
    bool fatalError(const QXmlParseException& exception);
	virtual QString getParseMessage() {
		return parseMessage;	
	}
    
    // Export:
    virtual bool fileExport(RB_ModelFactory* mf, const QString& file);

    void writeAppid(RB_XmlWriter& xw, const char* appid);
    void writeRoot(RB_XmlWriter& xw, const char* rootElement);
    void closeElement(RB_XmlWriter& xw);
    void writeModel(RB_XmlWriter& xw, RB_ObjectBase* obj);
    
private:

    // Model View Controler
    RB_ModelFactory* mModelFactory;
    RB_ObjectFactory* mObjectFactory;
	QString parseMessage;
	
	// store attributes for use in endElement(), ID in startElement()
	QXmlAttributes currentAttributes;
	QString currentContent;
	
	// stack (memory) container of parent objects
	std::stack<RB_ObjectBase*> parentStack;
#endif
};

#endif /*GV_FILTERXML_H*/
