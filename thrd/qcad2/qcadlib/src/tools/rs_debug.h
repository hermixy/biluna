/****************************************************************************
** $Id: rs_debug.h 8823 2008-02-02 17:32:04Z andrew $
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


#ifndef RS_DEBUG_H
#define RS_DEBUG_H

#ifdef __hpux
#include <sys/_size_t.h>
#endif

#include <iostream>
#include <stdio.h>

#include "rs_string.h"
#include "rs_stringlist.h"

#define RS_DEBUG RS_Debug::instance()

class RS_Entity;

/**
 * Debugging facilities.
 *
 * @author Andrew Mustun
 */
class RS_Debug {

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
     *  <li>D_TESTING:  testing messages
     *  <li>D_DEBUGGING: very verbose
     * </ul>
     */
    enum RS_DebugLevel { D_NOTHING,
                         D_CRITICAL,
                         D_ERROR,
                         D_WARNING,
                         D_NOTICE,
                         D_INFORMATIONAL,
                         D_TESTING,
                         D_DEBUGGING };

private:
    RS_Debug();

public:
    static RS_Debug* instance();

    static void deleteInstance();
    void setLevel(RS_DebugLevel level);
    RS_DebugLevel getLevel();
    /**
     * Prints the given message to stdout if the current debug level
     * is lower then the given level
     *
     * @param level Debug level.
     */
    inline void print(RS_DebugLevel level, const char* format ...) {
        if (debugLevel>=level) {
            va_list ap;
            va_start(ap, format);
            vfprintf(stream, format, ap);
            fprintf(stream, "\n");
            va_end(ap);
            fflush(stream);
        }
    }

    /**
     * Prints the given message to stdout.
     */
    inline void print(const char* format ...) {
        if (debugLevel==D_DEBUGGING) {
            va_list ap;
            va_start(ap, format);
            vfprintf(stream, format, ap);
            fprintf(stream, "\n");
            va_end(ap);
            fflush(stream);
        }
    }
    
    /**
     * Prints the given message to stdout.
     */
    inline void print(RS_DebugLevel level, const RS_String& msg) {
        if (debugLevel>=level) {
            fprintf(stream, "%s\n", (const char*)msg.toLatin1());
            fflush(stream);
        }
    }
    
    /**
     * Prints the given message to stdout.
     */
    inline void print(const RS_String& msg) {
        if (debugLevel==D_DEBUGGING) {
            fprintf(stream, "%s\n", (const char*)msg.toLatin1());
            fflush(stream);
        }
    }
    
    /**
     * Prints the given string list to stdout.
     */
    inline void print(const RS_StringList& l) {
        if (debugLevel==D_DEBUGGING) {
            for (int i=0; i<l.size(); ++i) {
                fprintf(stream, "%s\n", (const char*)l.at(i).toLatin1());
                fflush(stream);
            }
        }
    }

    void printUnicode(const RS_String& text);
    void printEntity(RS_Entity* e, const RS_String& prepend="") {
        printEntity(RS_Debug::D_DEBUGGING, e, prepend);
    }
    void printEntity(RS_DebugLevel level, RS_Entity* e, const RS_String& prepend="");
    void timestamp(const RS_String& text = RS_String());
    void setStream(FILE* s) {
        stream = s;
    }

private:
    static RS_Debug* uniqueInstance;

    RS_DebugLevel debugLevel;
    FILE* stream;
};

#endif

