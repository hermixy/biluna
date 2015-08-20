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
#include "rb_string.h"
#include "rb_system.h"


RB_Debug* RB_Debug::uniqueInstance = NULL;


/**
 *  Gets the one and only RB_Debug instance
 *  (creates a new one on first call only)
 *  @return Pointer to the single instance of this singleton class
 */
RB_Debug* RB_Debug::instance() {
    if(uniqueInstance==NULL) {
        QDateTime now = QDateTime::currentDateTime();
        RB_String nowStr;
#if QT_VERSION>=0x030000
    nowStr = now.toString("yyyyMMdd_hhmmss");
#else
    nowStr = now.toString();
#endif
        //RB_String fName = RB_String("%1/debug_%2.log")
        //	.arg(RB_SYSTEM->getHomeDir())
        //	.arg(nowStr);
        // RB_String fName = RB_String("debug_%1.log")
        //        .arg(nowStr);

        uniqueInstance = new RB_Debug;
        //uniqueInstance->stream = fopen(fName.latin1(), "wt");
//        uniqueInstance->stream = stderr;

        uniqueInstance->memberCreatedCount = 0;
        uniqueInstance->memberDeletedCount = 0;
        uniqueInstance->objectCreatedCount = 0;
        uniqueInstance->objectDeletedCount = 0;

        uniqueInstance->msgNothingCount = 0;
        uniqueInstance->msgCriticalCount = 0;
        uniqueInstance->msgErrorCount = 0;
        uniqueInstance->msgWarningCount = 0;
        uniqueInstance->msgNoticeCount = 0;
        uniqueInstance->msgInformationalCount = 0;
        uniqueInstance->msgDebuggingCount = 0;
    }
    return uniqueInstance;
}


/**
 * Deletes the one and only RB_Debug instance.
 */
void RB_Debug::deleteInstance() {
    // first delete objects
    std::list<RB_Object*>::iterator iter;
    iter = uniqueInstance->mObjectList.begin();
    while (iter != uniqueInstance->mObjectList.end()) {
        if (*iter != NULL) {
            delete *iter;
            *iter = NULL;
        }
        ++iter;
    }

    // second clear/remove pointers
    uniqueInstance->mObjectList.clear();

    // last delete unique instance
    if (uniqueInstance!=NULL) {
//        fclose(uniqueInstance->stream);
        delete uniqueInstance;
    }
}


/**
 * Constructor for a point with default coordinates.
 */
RB_Debug::RB_Debug() {
    debugLevel = D_DEBUGGING;
}

/**
 * Sets the debugging level.
 */
void RB_Debug::setLevel(RB_DebugLevel level) {
    debugLevel = level;
    print("RB_DEBUG: Warnings", D_WARNING);
    print("RB_DEBUG: Errors", D_ERROR);
    print("RB_DEBUG: Notice", D_NOTICE);
    print("RB_DEBUG: Informational", D_INFORMATIONAL);
    print("RB_DEBUG: Debugging", D_DEBUGGING);
}


/**
 * Gets the current debugging level.
 */
RB_Debug::RB_DebugLevel RB_Debug::getLevel() {
    return debugLevel;
}


/**
 * Prints the given message to stdout. Usefull example:
 * RB_DEBUG->print("%s line [%i] passed", __PRETTY_FUNCTION__, __LINE__);
 */
void RB_Debug::print(const char* format ...) {
    if(debugLevel==D_DEBUGGING) {
//        char buffer[256];
//        va_list args;
//        va_start (args, format);
//        vsprintf (buffer,format, args);
//        RB_String strBuffer(buffer);
//        va_end (args);

        RB_String strBuffer;
        append(strBuffer, format);
        qDebug() << strBuffer << "\n" ;
    }

}

/**
 * Print RB_String to output device with fprintf and fflush. Example use could be:
 * @param text text for output
 */
void RB_Debug::print(const RB_String& text) {
    qDebug() << text << "\n";
}


/**
 * Prints the given message to stdout if the current debug level
 * is lower then the given level. Usefull example:
 * RB_DEBUG->print(RB_Debug::D_ERROR, "%s line [%i] passed", 
 *                  __PRETTY_FUNCTION__, __LINE__);
 * @param level Debug level.
 */
void RB_Debug::print(RB_DebugLevel level, const char* format ...) {
    if (level == D_NOTHING) {
        ++msgNothingCount;
    } else if (level == D_CRITICAL) {
        ++msgCriticalCount;
    } else if (level == D_ERROR) {
        ++msgErrorCount;
    } else if (level == D_WARNING) {
        ++msgWarningCount;
    } else if (level == D_NOTICE) {
        ++msgNoticeCount;
    } else if (level == D_INFORMATIONAL) {
        ++msgInformationalCount;
    } else if (level == D_DEBUGGING) {
        ++msgDebuggingCount;
    }

    if(debugLevel>=level) {
//        char buffer[256];
//        va_list args;
//        va_start (args, format);
//        vsprintf (buffer,format, args);
//        RB_String strBuffer(buffer);
//        va_end (args);

        RB_String strBuffer;
        append(strBuffer, format);
        qDebug() << strBuffer << "\n" ;
    }
}


/**
 * Prints a time stamp in the format yyyyMMdd_hhmmss.
 */
void RB_Debug::timestamp() {
    QDateTime now = QDateTime::currentDateTime();
    RB_String nowStr;

#if QT_VERSION>=0x030000
    nowStr = now.toString("yyyyMMdd_hh:mm:ss:zzz ");
#else
    nowStr = now.toString();
#endif
    qDebug() << nowStr << "\n";
}

/**
 * Prints the unicode for every character in the given string.
 */
void RB_Debug::printUnicode(const RB_String& text) {
    for (int i=0; i<(int)text.length(); i++) {
        print("[%X] %c", text.at(i).unicode(), text.at(i).toLatin1());
    }
}

/**
 * Same as print(RB_Debug::D_ERROR, "message")
 */
void RB_Debug::error(const RB_String& text) {
    print(RB_Debug::D_ERROR, text.toStdString().c_str());
}

/**
 * Same as print(RB_Debug::D_WARNING, "message")
 */
void RB_Debug::warning(const RB_String& text) {
    print(RB_Debug::D_WARNING, text.toStdString().c_str());
}

/**
 * Same as print(RB_Debug::D_DEBUGGING, "message")
 */
void RB_Debug::debug(const RB_String& text) {
    print(RB_Debug::D_DEBUGGING, text.toStdString().c_str());
}

/**
 * Appends format to string, TODO: handle buffer overflow
 * @param str string to append format to
 * @param format to be added
 */
void RB_Debug::append(RB_String& str, const char* format ...) {
    char buffer[256];
    va_list args;
    va_start (args, format);
#ifdef Q_OS_MAC
    vsprintf (buffer,format, args);
#else
    vsprintf_s (buffer,format, args);
#endif
    RB_String strBuffer(buffer);
    str.append(strBuffer);
    va_end (args);
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
 * Print the number of created objects
 */
//void RB_Debug::printObjectBaseCreated() {
//    print("Created base objects: %i", this->objectBaseCreatedCount);
//}


/**
 * Print the number of deleted objects
 */
//void RB_Debug::printObjectBaseDeleted() {
//    print("Deleted base objects: %i", this->objectBaseDeletedCount);
//}


/**
 * Print the number of created members
 */
void RB_Debug::printMemberCreated() {
   	print("Created members: %i", this->memberCreatedCount); 
}


/**
 * Print the number of deleted members
 */
void RB_Debug::printMemberDeleted() { 
    print("Deleted members: %i", this->memberDeletedCount);
}


void RB_Debug::printObject(RB_Object* obj) {
    RB_ObjectBase* objB = dynamic_cast<RB_ObjectBase*>(obj);

    if (objB) {
        printObjectBase(objB);
        return;
    }

    RB_String str = "";
    str = "  RB_Debug::printObject() START";

    if (!obj) {
        str += "\n    object pointer is NULL";
    } else {
        append(str, "\n    object pointer = %p", obj);
        str += "\n    object name = " + obj->getName();
    }
}

/**
 * To print id, parent, name and members of an RB_ObjectBase object
 */
void RB_Debug::printObjectBase(RB_ObjectBase* obj, RB2::ResolveLevel level) {
    RB_String str = "";
    str = "  RB_Debug::printObjectBase() START";
	
	if (!obj) {
        str += "\n    object pointer is NULL";
	} else {
        append(str, "\n    object pointer = %p", obj);
        str += "\n    object id = " + obj->getId();
		str += "\n    object name = " + obj->getName();
		
		RB_ObjectBase* parent = (RB_ObjectBase*)obj->getParent();
		
		if (parent) {
            append(str, "\n    object parent pointer = %p", parent);
			str += "\n    object parent id = " + parent->getId();
			str += "\n    object parent name = " + parent->getName();
		} else {
            str += "\n    object parent pointer = NULL";
            str += "\n    object parent id = NULL";
            str += "\n    object parent name = NULL";
		}
		
		int noMember = obj->countMember();
	
		for (int i = 0; i < noMember; ++i) {
			str += "\n    ";
			str += obj->getMember(i)->getName();
			str += " = ";
			str += obj->getMember(i)->getValue().toString();
            str += "; display = ";
            str += obj->getMember(i)->getDisplayValue().toString();
            str += "; previous = ";
            str += obj->getMember(i)->getPreviousValue().toString();
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

void RB_Debug::printMessages() {
    print("Nothing messages: %i", msgNothingCount);
    print("Critical messages: %i", msgCriticalCount);
    print("Error messages: %i", msgErrorCount);
    print("Warning messages: %i", msgWarningCount);
    print("Notice messages: %i", msgNoticeCount);
    print("Informational messages: %i", msgInformationalCount);
    print("Debugging messages: %i", msgDebuggingCount);
}

void RB_Debug::printObjectCreated() {
    print("Created objects: %i", this->objectCreatedCount);
}

void RB_Debug::printObjectDeleted() {
    print("Deleted objects: %i", this->objectDeletedCount);
}

void RB_Debug::printObjectList() {
    print("Objects in list:");

    std::list<RB_Object*>::iterator iter;
    iter = mObjectList.begin();
    while (iter != mObjectList.end()) {
        if (*iter != NULL) {
            print("  %p %s", (*iter), (*iter)->getName().toStdString().c_str());
        }
        ++iter;
    }
}

bool RB_Debug::isValidId(const RB_String& id) {
    if (id.isEmpty() || id == "0" || id.size() < 38) {
        warning("Not a valid ID = " + id + " WARNING");
        return false;
    }

    return true;
}

// EOF
