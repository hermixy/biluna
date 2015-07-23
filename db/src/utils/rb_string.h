/*****************************************************************
 * $Id: rb_string.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_STRING_H
#define RB_STRING_H

#include <assert.h>

#include <QString>
#include "db_global.h"


#define RB_String QString
#define RB_Char QChar


class DB_EXPORT RB_StringCompat {

public:
    static RB_String replace(const RB_String& str,
                             const RB_String& s1, const RB_String& s2);
    // static void test();
};

#endif
