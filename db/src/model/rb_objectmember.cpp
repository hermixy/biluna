/*****************************************************************
 * $Id: rb_objectmember.cpp 2034 2013-11-14 15:34:56Z rutger $
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the Biluna LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_objectmember.h"


/**
 * Constructor
 * TODO: after extensive testing remove RB_DEBUG->addMemberCreated();
 * @param id unique identification of the member, mainly for database interfacing
 * @param parent parent object
 * @param name name of member (variable)
 * @param unit unit of measurement for this member
 * @param value current value of the member
 * @param prevValue the previous (official) value of the member
 */
RB_ObjectMember::RB_ObjectMember(RB_ObjectBase* parent,
                                 const RB_String& name, const RB_String& unit,
                                 const RB_Variant& value, RB2::MemberType type,
                                 const RB_Variant& prevValue) {
    RB_DEBUG->addMemberCreated();

    mParent = parent;
    mName = name;
    mUnit = unit;
    mType = type;
    mValue = value;
    // if prevValue = RB_Variant() (default) than prevValue.isValid() == false
    mPreviousValue = prevValue;
}

/**
 * Destructor
 * TODO: after extensive testing remove RB_DEBUG->addMemberDeleted();
 */
RB_ObjectMember::~RB_ObjectMember() {
    RB_DEBUG->addMemberDeleted();
}

/**
 * @return parent object of this member
 */
RB_ObjectBase* RB_ObjectMember::getParent() const {
    return mParent;
}

/**
 * @return name of the member (variable)
 */
RB_String RB_ObjectMember::getName() const {
    return mName;
}

/**
 * @return type of variable e.g. integer, double, bool, RB_String
 */
RB2::MemberType RB_ObjectMember::getType() const {
    return mType;
}

/**
 * @return unit of measurement
 */
RB_String RB_ObjectMember::getUnit() const {
    return mUnit;
}

/**
 * Create copy of this member (parent NULL, ID is not used)
 * @param level (not applicable, not used)
 * @return objMem clone of this (object) member
 */
RB_ObjectMember* RB_ObjectMember::copy(RB2::ResolveLevel /*level*/) {
    RB_ObjectMember* objMem = new RB_ObjectMember(NULL, mName, mUnit, mValue,
                                                  mType, mPreviousValue);
    return objMem;
}

/**
 * Create clone (exact copy with parent, ID is not used) of this member
 * @param level (not applicable, not used)
 * @return objMem clone of this (object) member
 */
RB_ObjectMember* RB_ObjectMember::clone(RB2::ResolveLevel /*level*/) {
    RB_ObjectMember* objMem = new RB_ObjectMember(mParent, mName, mUnit, mValue,
                                                  mType, mPreviousValue);
    return objMem;
}

/**
 * Reverts the changes to the member. The (original) previous value will
 * be set as the value and the previous value will be set to 'invalid'
 * @brief Reverts the changes
 */
void RB_ObjectMember::revert() {
    if (!mPreviousValue.isValid()) {
        return;
    }

    mValue = mPreviousValue;
}

/**
 * Change parent of member
 * @param parent new parent object
 */
void RB_ObjectMember::setParent(RB_ObjectBase* parent) {
    mParent = parent;
}

/**
 * Overloaded operator= , does not set ID and parent
 * @param mem member
 * @return *this
 */
RB_ObjectMember& RB_ObjectMember::operator= (const RB_ObjectMember& mem) {
    if (&mem != this) {
        // mId = mem.getId();
        // mParent = mem.getParent();
        mName = mem.getName();
        mUnit = mem.getUnit();
        mValue = mem.getValue();
        mType = mem.getType();
        mPreviousValue = mem.getPreviousValue();
    }

    return *this;
}

/**
 * Set member value, sets previous value if previous value does not exist.
 * Previous value could have been set when reading from a database or
 * by setPreviousValue()
 * @param val set value to val
 */
void RB_ObjectMember::setValue(const RB_Variant& val) {
    RB_ObjectBase* p = getParent();

    if (!p) {
        RB_DEBUG->error("RB_ObjectMember::setValue() parent ERROR");
    }

    if (!mPreviousValue.isValid()) {
        // first time loading of member value
        mPreviousValue = val;
    } else if (mPreviousValue.isValid() &&
               !p->getFlag(RB2::FlagIsDirty)) {
        // second time and first time changing of original value
        p->setFlag(RB2::FlagIsDirty);
    }

    mValue = val;
}

/**
 * @return value (current) value
 */
RB_Variant RB_ObjectMember::getValue() const {
    return mValue;
}

/**
 * @param val set previous (official) value to val
 */
void RB_ObjectMember::setPreviousValue(const RB_Variant& val) {
    mPreviousValue = val;
}

/**
 * @return previousValue previous (official) value
 */
RB_Variant RB_ObjectMember::getPreviousValue() const {
    return mPreviousValue;
}

/**
 * @return display value
 */
RB_Variant RB_ObjectMember::getDisplayValue() const {
    if (mName.endsWith("_idx")) {
        QString strValue = mValue.toString();
        // remove the Uuid part including the curly braces
        return strValue.remove(0, 38);
    }

    return mValue;
}

/**
 * @brief RB_ObjectMember::getUuidValue
 * @return return Uuid value
 */
RB_Variant RB_ObjectMember::getUuidValue() const {
    if (mName.endsWith("_idx")) {
        QString strValue = mValue.toString();
        // remove the part after the Uuid, 0 is the first character
        return strValue.remove(38, strValue.length());
    }

    return mValue;
}

/**
 * Debug only: print data
 * @return string with all member data
 */
RB_String RB_ObjectMember::toString() {
    RB_String str = "";
    str += getName() + ", ";
    str += getUnit() + ", ";
    str += getValue().toString() + ", ";
    str += RB2::memberTypeToString(getType()) + ", ";
    str += getPreviousValue().toString() + ", ";
    return str;
}

