/*****************************************************************
 * $Id: gv_filterxml.cpp 1765 2012-09-04 19:15:27Z rutger $
 * Created: Apr 24, 2008 4:53:57 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_filterxml.h"

#include <exception>
//#include <iostream>
#include <list>
#include <stdio.h>
#include <QFileInfo>
#include <QTextCodec>

#include "rb_modelfactory.h"
#include "rb_objectfactory.h"
#include "rb_system.h"


#if defined(__OS2__)||defined(__EMX__)||defined(_WIN32)
#define strcasecmp(s,t) stricmp(s,t)
#endif




/**
 * Default constructor.
 * TODO: remove EQL specifics releated to XX_Equipment
 */
GV_FilterXml::GV_FilterXml() {
    RB_DEBUG->print("GV_FilterXml::GV_FilterXml()");

//    mModelFactory = NULL;
//    mObjectFactory = NULL;
//    parseMessage = "";
//	currentContent = "";
}


/**
 * Destructor.
 */
GV_FilterXml::~GV_FilterXml() {
	RB_DEBUG->print("GV_FilterXml::~GV_FilterXml()");
	RB_DEBUG->print("GV_FilterXml::~GV_FilterXml() OK");
}


#if 0
bool GV_FilterXml::fileImport(RB_ModelFactory* mf, const QString& file) {
	RB_DEBUG->print("GV_FilterXml::fileImport()");
    mModelFactory = mf;
    mObjectFactory = mf->getObjectFactory();

	QFile fn(file);
	RB_XmlReader reader;
	
	// empty stack
	while (!parentStack.empty()) {
		parentStack.pop();	
	}

	reader.setContentHandler(this);
	reader.setErrorHandler(this);
	if (!reader.parse(&fn)) {
		return false;	
	}

	return true;
}


bool GV_FilterXml::startElement(const QString&, const QString&,
                              const QString& qName,
                              const QXmlAttributes& attribs) {
//   	RB_DEBUG->print("GV_FilterXml::startElement() startElement = %s", 
//   										qName.toStdString().c_str());

   	
	QString mId = "";
	int i = 0;
	
	// store attibutes for use in endElement()
	currentAttributes = attribs;

    /**
	The start elements of lists (containers) are handled differently from 
    non-container objects, containers can contain many of the same object, 
    an object can only contain one of a certain type of list.
	
    For example: a GV_Project can only contain one lineList, a lineList can contain
	zero, one or many GV_Line's.
	*/
	
    if (qName == "GV") {
   		// RB_DEBUG->print("startElement = ");
   		// RB_DEBUG->print(qName.toStdString().c_str());


    	// todo: at this moment action, but should generate a parse error
		//   in case this is not the root element or incorrect version number,
		//	 for example use a boolean to set the first check to true
    	return true;
	} else if (qName == "GV_Project" || qName == "GV_Ini") {
   		// RB_DEBUG->print("startElement = %s", qName.toStdString().c_str());
    	
		// process attributes, at this moment only the id
    	while (i < attribs.count() && i < 100) {
			if (attribs.qName(i) == "id") {
	    		mId = attribs.value(i);
			}
			i += 1;
    	}
    	
    	// remove current project if exists and create new GV_Project with mId
//    	if (model->getRoot() != NULL) {
			// model is deleted and new is created
            //model->setRoot(mId);
//    	}
    	
        parentStack.push(mModelFactory->getRoot());
	
	} else {
   		// RB_DEBUG->print("startElement = %s", qName.toStdString().c_str());
	
		QString mType = "";	// for GV_Equipment 'type' creation
	
		// process attributes, at this moment only the id and (eql)type 
		//  id's are for lists not necessary
    	while (i < attribs.count() && i < 100) {
			if (attribs.qName(i) == "id") {
	    		mId = attribs.value(i);
			} else if (attribs.qName(i) == "eqlType") {
				mType = attribs.value(i);
			}
			i += 1;
    	}

		// if statement to prevent crash if parentStack is still empty
		// when reading only a deaper part of the XML file. For native
		// LDT files this is not necessary
		RB_ObjectContainer* parent;
		if (parentStack.size() != 0) {
   			parent = (RB_ObjectContainer*)parentStack.top();
		} else {
			return false;
		}
   		
   		// Create new object with mId, parent:
   		RB_ObjectBase* tmpObj = NULL;
   		
        if (mObjectFactory->isObject(qName)) {
	   		if (qName.endsWith("List")) {
	   			// RB_DEBUG->print("startElementList = %s", qName.toStdString().c_str());
				tmpObj = parent->getContainer(qName); 
			} else {
				// RB_DEBUG->print("startElement = %s", qName.toStdString().c_str());
                tmpObj = mObjectFactory->newObject(mId, parent, mType, true);
				parent->addObject(tmpObj);
				// RB_DEBUG->print("parent = %s", parent->getName().toStdString().c_str());

				// set attributes as member values of GV_Equipment object
				if ((qName == "GV_Equipment" || qName == "GV_Section" 
										|| qName == "GV_Subsection") 
										&& currentAttributes.count() > 0) {
					for (int i = 0; i < currentAttributes.count(); ++i) {
						if (currentAttributes.qName(i) != "id") {
							tmpObj->setValue(currentAttributes.qName(i), 
										currentAttributes.value(i));
						}
					}
				}

	   		}
   		}
   		
   		if (tmpObj != NULL) {
   			parentStack.push(tmpObj);
   		}
    }   		
	
    // currentContent = "";
    return true;
}


bool GV_FilterXml::characters(const QString& str) {
    // RB_DEBUG->print(str.toStdString().c_str());
    currentContent = str;
    return true;
}


bool GV_FilterXml::endElement(const QString&, const QString&,
                            			const QString& qName) {
	// RB_DEBUG->print("GV_FilterXml::endElement()");
	
	if (parentStack.size() > 0) {
		RB_ObjectBase* obj = parentStack.top();
		
		if (qName == "EQL") {
			return true;
		} else {
            if (mObjectFactory->isObject(qName)) {
				// remove (pop) top object from stack if equal type
				RB_ObjectBase* objTop = parentStack.top();
				
				if (objTop->getName() == qName) {
					parentStack.pop();
				}
			} else {
				// Members of objects do not have children therefore the attributes
				// can be set here at end element. Object attributes are set
				// at startElement()
				
			    // replace text 'true' and 'false' to boolean
			    bool strToBool = false;
			    if (currentContent == "true") {
			    	strToBool = true;
				}
				
				// get member and set value (current) and attributes
				// other than id, parent and name
				RB_ObjectMember* mem = obj->getMember(qName);
				if (mem) {
					if (mem->getType() == RB2::MemberInteger) {
						mem->setValue(currentContent.toInt());
					} else if (mem->getType() == RB2::MemberDouble) {
						mem->setValue(currentContent.toDouble());
					} else {
						mem->setValue(currentContent);
					} 
					
					if (currentAttributes.count() > 0) {
						for (int i = 0; i < currentAttributes.count(); ++i) {
							if (currentAttributes.qName(i) == "pValue") {
								mem->setPreviousValue(currentAttributes.value(i));
							}
						}
					}
				}
			}
		}
	}
	
	currentContent = "";
    return true;
}


bool GV_FilterXml::fatalError(const QXmlParseException &exception) {
    // to std::out
    qWarning("Item %d, column %d: %s", exception.lineNumber(),
             exception.columnNumber(), exception.message().toStdString().c_str());
    
    // to QMessageBox
    QString str;
    this->parseMessage = "Item " + str.setNum(exception.lineNumber()) +
    					" column " + str.setNum(exception.columnNumber()) + 
    					": " + exception.message();
    return false;
}




/****************************************************************************
 *
 *  File Export
 * 
 ****************************************************************************/


/**
 * Implementation of the method used for RB_Export to communicate
 * with this filter.
 *
 * @param file Full path to the SVG file that will be written.
 */

bool GV_FilterXml::fileExport(RB_ModelFactory* mf, const QString& file) {
	
	RB_DEBUG->print("GV_FilterXml::fileExport()");
    this->mModelFactory = mf;
    this->mObjectFactory = mf->getObjectFactory();
	
    // check if we can write to that directory:
#ifndef Q_OS_WIN

    QString path = QFileInfo(file).absolutePath();
    if (QFileInfo(path).isWritable()==false) {
        RB_DEBUG->print("GV_FilterXml::fileExport: can't write file: "
                        "no permission");
        return false;
    }

#endif

	// set type for XML filter
//    RB_XmlCodes::TypeML type = RB_XmlCodes::TypeXML;

	// create RB_XmlWriter with the file (=name)
	// note: compared QCad the dxf is taken out as in between translation 
    char* f = new char[strlen(file.toStdString().c_str())+1];
    strcpy(f, file.toStdString().c_str());

    // check whether correct writer is available
//    RB_XmlWriter* xw = new RB_XmlWriter(f, type);
    RB_XmlWriter* xw = new RB_XmlWriter(f);
    if (xw->openFailed()) {
        delete xw;
        RB_DEBUG->print("GV_FilterXml::fileExport: can't write file");
        return false;
    }
    
    // write header <?xml ... ?> and <!DOCTYPE ... >
    // RB_DEBUG->print("writing headers...");
    xw->headerElement();
    
    // write Appid in comment
    // RB_DEBUG->print("writing appid...");
    // writeAppid(*xw, RB_SYSTEM->getAppName().toStdString().c_str());
	writeAppid(*xw, "*********** GV made by Red-Bag ***********");
	writeAppid(*xw, "For more information visit www.red-bag.com");
	writeAppid(*xw, "******************************************");

    // write root element <ldt>
    // RB_DEBUG->print("writing root element...");
    writeRoot(*xw, "GV");

	// write model data to xml file
    // RB_DEBUG->print("writing model...");
    writeModel(*xw, mModelFactory->getRoot());
	
	// write close ement </ldt>
    // RB_DEBUG->print("writing close root element...");
	closeElement(*xw);

	// close writer to close writing to the file, is required 
    // RB_DEBUG->print("close..");
    xw->close();

    delete xw;

    // check if file was actually written (strange world of windoze xp):
    if (QFileInfo(file).exists()==false) {
        RB_DEBUG->print("GV_FilterXml::fileExport: file could not be written");
        return false;
    }
	return true;
}


/**
 * Writes an application id to the SVG file.
 *
 * @param appid Application ID (e.g. "QCad").
 */
void GV_FilterXml::writeAppid(RB_XmlWriter& xw, const char* appid) {
    // RB_DEBUG->print("GV_FilterXml::writeAppid...");
    xw.commentElement(appid);
}


/**
 * Writes the root element to the file.
 *
 * @param appid Application ID (e.g. "QCad").
 */

void GV_FilterXml::writeRoot(RB_XmlWriter& xw, const char* rootElement) {
    // RB_DEBUG->print("GV_FilterXml::writeRoot...");

	xw.insertAttribute("version", RB_SYSTEM->getAppVersion());
	xw.push(rootElement);
}



/**
 *	Closing tag </...> for example </svg> and </g>
 */
void GV_FilterXml::closeElement(RB_XmlWriter& xw) {
    // RB_DEBUG->print("GV_FilterXml::closeElement...");
	xw.pop();
}


/**
 * Writes the model to the file.
 *
 * @param xw is XML writer
 * @param obj is an object from the model
 */
void GV_FilterXml::writeModel(RB_XmlWriter& xw, RB_ObjectBase* obj) {
    // RB_DEBUG->print("GV_FilterXml::writeModel...");
        
	// set id attribute of object
	if (!obj->getName().endsWith("List")) {
		xw.insertAttribute("id", obj->getId());
	}

	// set (eql)type and staticRotating attribute of GV_Equipment object
	if (obj->getName() == "GV_Equipment") {
		xw.insertAttribute("type", obj->getValue("type").toString());
		xw.insertAttribute("eqlType", obj->getValue("eqlType").toString());
		xw.insertAttribute("eqlStatRot", obj->getValue("eqlStatRot").toString());
	} else if (obj->getName() == "GV_Section" 
				|| obj->getName() == "GV_Subsection") {
		xw.insertAttribute("type", obj->getValue("type").toString());
		xw.insertAttribute("eqlType", obj->getValue("eqlType").toString());
		xw.insertAttribute("description", obj->getValue("description").toString());
	}

	// write object- or list start element name
	xw.push(obj->getName());

	// write members with their attributes
	for (int i = 0; i < obj->countMember(); i++) {
		// do not write an element for type, eqlType and eqlStatRot members, 
		// they are attributes of GV_Equipment
		if ( !((obj->getMember(i)->getName() == "type" 
					|| obj->getMember(i)->getName() == "eqlType" 
					|| obj->getMember(i)->getName() == "eqlStatRot")
					&& obj->getName() == "GV_Equipment") 
				&& !((obj->getMember(i)->getName() == "type" 
					|| obj->getMember(i)->getName() == "eqlType" 
					|| obj->getMember(i)->getName() == "description") 
					&& (obj->getName() == "GV_Section" 
					|| obj->getName() == "GV_Subsection"))) { 

			QString str = obj->getMember(i)->getId();
			if (str != "") {
				xw.insertAttribute("id", obj->getId());
			}
			
			str = obj->getMember(i)->getUnit();
			if (str != "-") {
				xw.insertAttribute("UoM", str);
			}
			
			RB_Variant var = obj->getMember(i)->getPreviousValue();
			if (var.isValid()) {
				xw.insertAttribute("pValue", var);
			}
			xw.element(obj->getMember(i)->getName(), obj->getMember(i)->getValue());
		}
	}	
	
    // write children if children are atomic or container
	if (obj->isContainer()) {    
		RB_ObjectContainer* tmpObjc = (RB_ObjectContainer*)obj;
		RB_ObjectIterator* iter = tmpObjc->createIterator();
		for (iter->first(); !iter->isDone(); iter->next()) {
			writeModel(xw, iter->currentObject());
		}
		delete iter;
	}
	xw.pop();
}
#endif
// EOF
