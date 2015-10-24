/*****************************************************************
 * $Id: rb_objectmember.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_OBJECTMEMBER_H
#define RB_OBJECTMEMBER_H

#include <iostream>

#include "rb_objectbase.h"
#include "rb.h"
#include "rb_debug.h"
#include "rb_string.h"
#include "rb_variant.h"

class RB_ObjectBase;


/**
 * Base class for members of objects. This class replaces the 'normal' members
 * of a class to provide more flexibility and the possibility to add attributes.
 * Contains the value and the attributes of object member variables
 * @author Rutger Botermans
 */
class DB_EXPORT RB_ObjectMember {

public:
    RB_ObjectMember(RB_ObjectBase* parent,
                    const RB_String& name,
                    const RB_String& unit,
                    const RB_Variant& value,
                    RB2::MemberType type = RB2::MemberNone,
                    const RB_Variant& prevValue = RB_Variant());
    virtual ~RB_ObjectMember();

    virtual RB_ObjectBase* getParent() const;
    virtual RB_String getName() const;
    virtual RB2::MemberType getType() const;
    virtual RB_String getUnit() const;

    virtual RB_ObjectMember* copy(RB2::ResolveLevel /*level*/);
    virtual RB_ObjectMember* clone(RB2::ResolveLevel /*level*/);
    virtual void revert();
    virtual void setParent(RB_ObjectBase* parent);
    virtual RB_ObjectMember& operator= (const RB_ObjectMember& mem);

    virtual void setValue(const RB_Variant& val);
    virtual RB_Variant getValue() const;
    virtual void setPreviousValue(const RB_Variant& val);
    virtual RB_Variant getPreviousValue() const;
    virtual RB_Variant getDisplayValue() const;
    virtual RB_Variant getUuidValue() const;

    virtual RB_String toString();

private:
    //! Pointer to parent RB_ObjectBase
    RB_ObjectBase* mParent;
    //! Name of member/variable
    RB_String mName;
    //! Unit of measurement of member variable
    RB_String mUnit;
    //! Type of variable
    RB2::MemberType mType;
    //! Current value of member/variable
    RB_Variant mValue;
    //! Previous value of member/variable
    RB_Variant mPreviousValue;

};

#endif // RB_OBJECTMEMBER_H
