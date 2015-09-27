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
#include "QString.h"

class RB_Object;
class RB_ObjectBase;


#define RB_DEBUG RB_Debug::instance()

/**
 * Debugging facilities.
 */
class DB_EXPORT RB_Debug {

public:
    /**
     * Enum for debug levels. Only messages of the current
     * or a higher level are printed.
     * <ul>
     *  <li>D_ERROR:    errors
     *  <li>D_WARNING:  warnings
     *  <li>D_INFORMATIONAL: information for backtracing
     *  <li>D_DEBUGGING: very verbose for debugging purposes
     * </ul>
     */
    enum RB_DebugLevel {
        D_ERROR,
        D_WARNING,          // default
        D_INFORMATIONAL,    // for backtrace
        D_DEBUGGING
    };

public:
    static RB_Debug* instance();
    static void deleteInstance();

    void setLevel(RB_DebugLevel level);
    RB_DebugLevel getLevel();
    void print(RB_DebugLevel level, const QString& text);
    void print(const QString& text);
    void printUnicode(const QString& text);
    void timestamp();
//    void setStream(FILE* s) {
//        stream = s;
//    }

    // use for integer, string and double:
    // RB_DEBUG->print(QString(param %i: %s %f").arg(i).arg(argv[i]).arg(12.34));
    // use for pointer
    // RB_DEBUG->print(QString("RB_Object* : %p").arg(obj));
    // general use
    // RB_DEBUG->print(QString("%s line [%i] passed").arg(__PRETTY_FUNCTION__).arg(__LINE__));
    void error(const QString& text);
    void warning(const QString& text);
    void info(const QString& text);   // same as print
    void debug(const QString& text);

//    void append(QString& str, const char* format ...);
    void addMemberCreated();
    void addMemberDeleted();
    void addObjectCreated(RB_Object* obj);
    void addObjectDeleted(RB_Object* obj);

    void printMemberCreated();
    void printMemberDeleted();
    void printObject(RB_Object* obj);
    void printMessages();
    void printObjectCreated();
    void printObjectDeleted();
    void printObjectList();

    bool isValidId(const QString& id);

private:
    RB_Debug();
    void printObjectBase(RB_ObjectBase* obj,
                         RB2::ResolveLevel level = RB2::ResolveNone);
    QString pointerToString(void* ptr) const;

    static RB_Debug* uniqueInstance;

    RB_DebugLevel debugLevel;
//    FILE* stream;
    
    // to test memory leak
    int memberCreatedCount;
    int memberDeletedCount;
    int objectCreatedCount;
    int objectDeletedCount;

    // to test messages such as warnings, errors
    int msgErrorCount;
    int msgWarningCount;
    int msgInformationalCount;
    int msgDebuggingCount;

    std::list<RB_Object*> mObjectList;
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
