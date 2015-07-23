/*****************************************************************
 * $Id: rb_flags.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 3, 2009 6:07:28 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_FLAGS_H
#define RB_FLAGS_H

#include "db_global.h"

/**
 * Base class for objects which have flags.
  */
class DB_EXPORT RB_Flags {
public:
    /** Default constructor. Resets all flags to 0. */
    RB_Flags() {
        mFlags = 0;
    }

    /** Constructor with initialisation to the given flags. */
    RB_Flags(unsigned int f) {
        mFlags = f;
    }

    virtual ~RB_Flags() {}

    inline unsigned int getFlags() const {
        return mFlags;
    }

    inline void resetFlags() {
        mFlags=0;
    }

    inline void setFlags(unsigned int f) {
        mFlags=f;
    }

    inline void setFlag(unsigned int f) {
        mFlags=mFlags|f;
    }

    inline void delFlag(unsigned int f) {
        mFlags=mFlags&(~f);
    }

    inline void toggleFlag(unsigned int f) {
        mFlags=mFlags^f;
    }

    inline bool getFlag(unsigned int f) const {
        if(mFlags&f) {
            return true;
        } else {
            return false;
        }
    }

private:
    unsigned int mFlags;
};

#endif
