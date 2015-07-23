/*****************************************************************
 * $Id: rb_debug.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/



#ifndef RB_DEBUG_H
#define RB_DEBUG_H

#ifdef __hpux
#include <sys/_size_t.h>
#endif

#include <iostream>
#include <stdio.h>
#include <list>

#include "db_global.h"
#include "rb.h"
#include "rb_string.h"

class RB_Object;
class RB_ObjectBase;


#define RB_DEBUG RB_Debug::instance()

/**
 * Debugging facilities.
 * @author Andrew Mustun, updated Rutger Botermans
 */
class DB_EXPORT RB_Debug {

public:
    /**
     * Enum for debug levels. Only messages of the current
     * or a higher level are printed.
     * <ul>
     *  <li>D_NOTHING:  nothing
     *  <li>D_CRITICAL: critical messages
     *  <li>D_ERROR:    errors
     *  <li>D_WARNING:  warnings
     *  <li>D_NOTICE:   notes
     *  <li>D_INFORMATIONAL: infos
     *  <li>D_DEBUGGING: very verbose
     * </ul>
     */
    enum RB_DebugLevel { D_NOTHING,
                         D_CRITICAL,
                         D_ERROR,
                         D_WARNING,
                         D_NOTICE,
                         D_INFORMATIONAL,
                         D_DEBUGGING };

private:
    RB_Debug();

public:
    static RB_Debug* instance();
    static void deleteInstance();

    void setLevel(RB_DebugLevel level);
    RB_DebugLevel getLevel();
    void print(RB_DebugLevel level, const char* format ...);
    void print(const char* format ...);
//    void print(const std::string& text);
    void print(const RB_String& text);
    void printUnicode(const RB_String& text);
    void timestamp();
//    void setStream(FILE* s) {
//        stream = s;
//    }

    //
    // added 2005-07-08 by rutger: use for integer, string and double:
    // RB_DEBUG->print("param %i: %s %f", i, argv[i], 12.34);
    // use for pointer
    // RB_DEBUG->print("RB_Object* : %p", obj);
    // general use
    // RB_DEBUG->print("%s line [%i] passed", __PRETTY_FUNCTION__, __LINE__);
    void error(const RB_String& text);
    void warning(const RB_String& text);
    void debug(const RB_String& text); // same as print

    void append(RB_String& str, const char* format ...);
    void addObjectBaseCreated(RB_ObjectBase* obj);
    void addObjectBaseDeleted(RB_ObjectBase* obj);
    void addMemberCreated();
    void addMemberDeleted();
    void addObjectCreated(RB_Object* obj);
    void addObjectDeleted(RB_Object* obj);

    void printObjectBaseCreated();
    void printObjectBaseDeleted();
    void printMemberCreated();
    void printMemberDeleted();
    void printObjectBaseList();
    void printObject(RB_ObjectBase* obj,
                     RB2::ResolveLevel level = RB2::ResolveNone);
    void printMessages();
    void printObjectCreated();
    void printObjectDeleted();
    void printObjectList();

    bool isValidId(const RB_String& id);

private:
    static RB_Debug* uniqueInstance;

    RB_DebugLevel debugLevel;
//    FILE* stream;
    
    // to test memory leak
    int objectBaseCreatedCount;
    int objectBaseDeletedCount;
    int memberCreatedCount;
    int memberDeletedCount;
    int objectCreatedCount;
    int objectDeletedCount;

    // to test messages such as warnings, errors
    int msgNothingCount;
    int msgCriticalCount;
    int msgErrorCount;
    int msgWarningCount;
    int msgNoticeCount;
    int msgInformationalCount;
    int msgDebuggingCount;

    std::list<RB_Object*> mObjectList;
    std::list<RB_ObjectBase*> mObjectBaseList;
};

#endif

/*
#include <execinfo.h>
#include <stdlib.h>

static QString qBacktrace( int levels = -1 )
{
    QString s;
    void* trace[256];
    int n = backtrace(trace, 256);
    char** strings = backtrace_symbols (trace, n);

    if ( levels != -1 )
        n = qMin( n, levels );
    s = QString::fromLatin1("[\n");

    for (int i = 0; i < n; ++i)
        s += QString::number(i) +
             QString::fromLatin1(": ") +
             QString::fromLatin1(strings[i]) + QString::fromLatin1("\n");
    s += QString::fromLatin1("]\n");
    free (strings);
    return s;
}

Usage:
        qDebug("%s",qPrintable(qBacktrace()));
or
        qDebug() << qBacktrace();

*/
// EOF
