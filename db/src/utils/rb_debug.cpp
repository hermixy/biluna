/****************************************************************************
** $Id: rb_debug.cpp 2235 2015-05-05 19:53:22Z rutger $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/


#include "rb_debug.h"

#include <cstdio>
#include <cstdarg>

#include <QDateTime>
#include <QDebug>
// #include "rb_datetime.h"
#include "rb_object.h"
#include "rb_objectcontainer.h"
#include "QString.h"
#include "rb_system.h"


RB_Debug* RB_Debug::uniqueInstance = NULL;


/**
 *  Gets the one and only RB_Debug instance
 *  (creates a new one on first call only)
 *  @return Pointer to the single instance of this singleton class
 */
RB_Debug* RB_Debug::instance() {
    if(uniqueInstance==NULL) {
        QString nowStr = QDateTime::currentDateTime().toString(Qt::ISODate);

        //QString fName = QString("%1/debug_%2.log")
        //	.arg(RB_SYSTEM->getHomeDir())
        //	.arg(nowStr);
        // QString fName = QString("debug_%1.log")
        //        .arg(nowStr);

        uniqueInstance = new RB_Debug();
        // uniqueInstance->stream = fopen(fName.latin1(), "wt");
        // uniqueInstance->stream = stderr;
        uniqueInstance->print(nowStr);

        uniqueInstance->memberCreatedCount = 0;
        uniqueInstance->memberDeletedCount = 0;
        uniqueInstance->objectCreatedCount = 0;
        uniqueInstance->objectDeletedCount = 0;

        uniqueInstance->msgErrorCount = 0;
        uniqueInstance->msgWarningCount = 0;
        uniqueInstance->msgInformationalCount = 0;
        uniqueInstance->msgDebuggingCount = 0;
    }
    return uniqueInstance;
}


/**
 * Deletes the one and only RB_Debug instance.
 */
void RB_Debug::deleteInstance() {
    std::list<RB_Object*>::iterator iter;
    iter = uniqueInstance->mObjectList.begin();

    while (iter != uniqueInstance->mObjectList.end()) {
        if (*iter != NULL) {
            delete *iter;
            *iter = NULL;
        }
        ++iter;
    }

    uniqueInstance->mObjectList.clear();

    if (uniqueInstance != NULL) {
        // fclose(uniqueInstance->stream);
        delete uniqueInstance;
    }
}


/**
 * Constructor with default D_DEBBUGING level
 */
RB_Debug::RB_Debug() {
    debugLevel = D_WARNING;
}

/**
 * Sets the debugging level.
 */
void RB_Debug::setLevel(RB_DebugLevel level) {
    debugLevel = level;
    print(QString("RB_DEBUG: Errors %1").arg(D_ERROR));
    print(QString("RB_DEBUG: Warnings %1").arg(D_WARNING));
    print(QString("RB_DEBUG: Informational %1").arg(D_INFORMATIONAL));
    print(QString("RB_DEBUG: Debugging %1").arg(D_DEBUGGING));
}

/**
 * Gets the current debugging level.
 */
RB_Debug::RB_DebugLevel RB_Debug::getLevel() {
    return debugLevel;
}

/**
 * Print QString to output device with fprintf and fflush. Example use could be:
 * @param text text for output
 */
void RB_Debug::print(const QString& text) {
    qDebug() << text;
}


/**
 * Prints the given message to stdout if the current debug level
 * is lower then the given level. Usefull example:
 * RB_DEBUG->print(RB_Debug::D_ERROR, "%s line [%i] passed", 
 *                  __PRETTY_FUNCTION__, __LINE__);
 * @param level Debug level.
 */
void RB_Debug::print(RB_DebugLevel level, const QString& text) {
    if (level == D_ERROR) {
        ++msgErrorCount;
    } else if (level == D_WARNING) {
        ++msgWarningCount;
    } else if (level == D_INFORMATIONAL) {
        ++msgInformationalCount;
    } else /*if (level == D_DEBUGGING)*/ {
        ++msgDebuggingCount;
    }

    if(debugLevel >= level) {
        qDebug() << text;
    }
}


/**
 * Prints a time stamp in the format yyyyMMddThh:mm:ss
 */
void RB_Debug::timestamp() {
    qDebug() << QDateTime::currentDateTime().toString(Qt::ISODate);
}

/**
 * Prints the unicode for every character in the given string.
 */
void RB_Debug::printUnicode(const QString& text) {
    for (int i=0; i<(int)text.length(); i++) {
        print(QString("[%1] %1").arg(text.at(i).unicode())
              .arg(text.at(i).toLatin1()));
    }
}

/**
 * Same as print(RB_Debug::D_ERROR, "message")
 */
void RB_Debug::error(const QString& text) {
    print(RB_Debug::D_ERROR, text.toStdString().c_str());
}

/**
 * Same as print(RB_Debug::D_WARNING, "message")
 */
void RB_Debug::warning(const QString& text) {
    print(RB_Debug::D_WARNING, text.toStdString().c_str());
}

/**
 * Same as print(RB_Debug::D_INFORMATIONAL, "message")
 */
void RB_Debug::info(const QString& text) {
    print(RB_Debug::D_INFORMATIONAL, text.toStdString().c_str());
}

/**
 * Same as print(RB_Debug::D_DEBUGGING, "message")
 */
void RB_Debug::debug(const QString& text) {
    print(RB_Debug::D_DEBUGGING, text.toStdString().c_str());
}

/**
 * Add one (1) to the count of created members
 */
void RB_Debug::addMemberCreated() { 
	++memberCreatedCount; 
}

/**
 * Add one (1) to the count of deleted members
 */
void RB_Debug::addMemberDeleted() { 
    ++memberDeletedCount;
}

void RB_Debug::addObjectCreated(RB_Object *obj) {
    ++objectCreatedCount;
    mObjectList.push_back(obj);
}

void RB_Debug::addObjectDeleted(RB_Object *obj) {
    ++objectDeletedCount;
    mObjectList.remove(obj);
}

/**
 * Print the number of created members
 */
void RB_Debug::printMemberCreated() {
    print(QString("Created members: %1").arg(this->memberCreatedCount));
}


/**
 * Print the number of deleted members
 */
void RB_Debug::printMemberDeleted() { 
    print(QString("Deleted members: %1").arg(this->memberDeletedCount));
}


void RB_Debug::printObject(RB_Object* obj, RB2::ResolveLevel level) {
    RB_ObjectBase* objB = dynamic_cast<RB_ObjectBase*>(obj);

    if (objB) {
        printObjectBase(objB, level);
        return;
    }

    QString str = "";
    str = "  RB_Debug::printObject() START\n";

    if (!obj) {
        str += "    object pointer is NULL\n";
    } else {
        str.append(QString("    object pointer = %1\n")
                   .arg(pointerToString(obj)));
        str += "    object name = " + obj->getName();
    }
}

/**
 * To print id, parent, name and members of an RB_ObjectBase object
 */
void RB_Debug::printObjectBase(RB_ObjectBase* obj, RB2::ResolveLevel level) {
    QString str = "";
    str = "  RB_Debug::printObjectBase() START\n";
	
	if (!obj) {
        str += "    object pointer is NULL\n";
	} else {
        str.append(QString("    object pointer = %1\n")
                   .arg(pointerToString(obj)));
        str += "    object id = " + obj->getId() + "\n";
        str += "    object name = " + obj->getName() + "\n";
		
		RB_ObjectBase* parent = (RB_ObjectBase*)obj->getParent();
		
		if (parent) {
            str.append(QString("    object parent pointer = %1\n")
                       .arg(pointerToString(parent)));
            str += "    object parent id = " + parent->getId() + "\n";
            str += "    object parent name = " + parent->getName() + "\n";
		} else {
            str += "    object parent pointer = NULL\n";
            str += "    object parent id = NULL\n";
            str += "    object parent name = NULL\n";
		}
		
        int noMember = obj->memberCount();
	
		for (int i = 0; i < noMember; ++i) {
            str += "    ";
			str += obj->getMember(i)->getName();
			str += " = ";
			str += obj->getMember(i)->getValue().toString();
            str += "; display = ";
            str += obj->getMember(i)->getDisplayValue().toString();
            str += "; previous = ";
            str += obj->getMember(i)->getPreviousValue().toString() + "\n";
        }

		print(str);
	}
	
	if (obj && obj->isContainer() && level != RB2::ResolveNone) {
		// print children if resolve level > resolve none
		RB2::ResolveLevel childLevel = RB2::ResolveNone;
		
		switch (level) {
			case RB2::ResolveNone:
				childLevel = RB2::ResolveNone;
				break;
			case RB2::ResolveOne:
				childLevel = RB2::ResolveNone;
				break;
			case RB2::ResolveTwo:
				childLevel = RB2::ResolveOne;
				break;
	        case RB2::ResolveThree:
				childLevel = RB2::ResolveTwo;
				break;
	        case RB2::ResolveFour:
				childLevel = RB2::ResolveThree;
				break;
	        case RB2::ResolveFive:
				childLevel = RB2::ResolveFour;
				break;
	        case RB2::ResolveSix:
				childLevel = RB2::ResolveFive;
				break;
	        case RB2::ResolveSeven:
				childLevel = RB2::ResolveSix;
				break;
	        case RB2::ResolveEight:
				childLevel = RB2::ResolveSeven;
				break;
	        case RB2::ResolveNine:
				childLevel = RB2::ResolveEight;
				break;
	        case RB2::ResolveTen:
				childLevel = RB2::ResolveNine;
				break;
	        case RB2::ResolveAll:
				childLevel = RB2::ResolveAll;
				break;
			default:
				childLevel = RB2::ResolveNone;
				break;
		}
		
		RB_ObjectContainer* objC = (RB_ObjectContainer*)obj; 
		RB_ObjectIterator* iter = objC->createIterator();
		
		for (iter->first(); !iter->isDone(); iter->next()) {
            printObjectBase(iter->currentObject(), childLevel);
		}
		delete iter;
	}
	
    print("  RB_Debug::printObjectBase() END");
}

QString RB_Debug::pointerToString(void* ptr) const {
//    return QString("0x%1").arg((quintptr)ptr, QT_POINTER_SIZE * 2, 16, QChar('0'));
    // alternative 1:
//    QString str;
//    str.sprintf("%08p", ptr);
//    return str;
    // alternative 2:
//    const void* address = static_cast<const void*>(ptr);
    QString addressString;
    QTextStream addressStream(&addressString);
    addressStream << ptr; // address;
    return addressString;
}

void RB_Debug::printMessages() {
    print(QString("Error messages: %1").arg(msgErrorCount));
    print(QString("Warning messages: %1").arg(msgWarningCount));
    print(QString("Informational messages: %1").arg(msgInformationalCount));
    print(QString("Debugging messages: %1").arg(msgDebuggingCount));
}

void RB_Debug::printObjectCreated() {
    print(QString("Created objects: %1").arg(this->objectCreatedCount));
}

void RB_Debug::printObjectDeleted() {
    print(QString("Deleted objects: %1").arg(this->objectDeletedCount));
}

void RB_Debug::printObjectList() {
    print("Objects in list:");
    std::list<RB_Object*>::iterator iter;
    iter = mObjectList.begin();

    while (iter != mObjectList.end()) {
        if (*iter != NULL) {
            print(QString("  %1 %2").arg(pointerToString(*iter))
                  .arg((*iter)->getName().toStdString().c_str()));
        }
        ++iter;
    }
}

bool RB_Debug::isValidId(const QString& id) {
    if (id.isEmpty() || id == "0" || id.size() < 38) {
        warning("Not a valid ID = " + id + " WARNING");
        return false;
    }

    return true;
}

// EOF
