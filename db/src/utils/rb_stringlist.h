/*****************************************************************
 * $Id: rb_stringlist.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_STRINGLIST_H
#define RB_STRINGLIST_H

#include <QStringList>
#include "db_global.h"
#include "rb_string.h"

class DB_EXPORT RB_StringList : public QStringList {

public:
    RB_StringList() { }
    RB_StringList(const QStringList& strL) : QStringList(strL) { }

    virtual RB_String toString(const RB_String& separator) {
        RB_String str;
        int size = this->size();

        for (int i = 0; i < size; ++i) {
            if (i > 0) {
                str += separator;
            }

            str += this->at(i);
        }

        return str;
    }

};

#endif
