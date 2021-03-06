/*****************************************************************
 * $Id: rb_object.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Created: 2015-05-01 rutger
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the data base (DB) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_OBJECT_H
#define RB_OBJECT_H

#include <vector>

#include "rb.h"
#include "rb_flags.h"
#include "rb_string.h"


/**
 * Template of light weight object that reports to RB_Debug
 */
class DB_EXPORT RB_Object : public RB_Flags {

public:
    RB_Object(const RB_String& name = "RB_Object");
    RB_Object(RB_Object* obj);
    virtual ~RB_Object();
    virtual RB_Object& operator= (const RB_Object& obj);

    virtual void init();

    virtual RB_String getName() const;
    virtual void setName(const RB_String& name);

protected:
    //! Name of object for identification or debugging usually class name
    RB_String mName;
};

#endif // RB_OBJECT_H
