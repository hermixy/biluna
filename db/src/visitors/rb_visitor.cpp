/*****************************************************************
 * $Id: rb_visitor.h 1491 2011-12-07 21:51:55Z rutger $
 * Created: Nov 21, 2011 7:29 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_visitor.h"

#include "rb_debug.h"


/**
 * Constructor
 */
RB_Visitor::RB_Visitor() {
    mResolveLevel = RB2::ResolveAll;
}

/**
 * Destructor
 */
RB_Visitor::~RB_Visitor() {
    // nothing
}

/**
 * Get resolve level
 * @returns resolve level
 */
RB2::ResolveLevel RB_Visitor::getResolveLevel() {
    return mResolveLevel;
}

/**
 * Set resolve level
 * @param level resolve level
 */
void RB_Visitor::setResolveLevel(RB2::ResolveLevel level) {
    mResolveLevel = level;
}

/**
 * Resolve next level e.g. RB2::ResolveTwo becomes RB2::ResolveOne
 * @return next resolve level
 */
RB2::ResolveLevel RB_Visitor::nextResolveLevel() {
    switch (mResolveLevel) {
        case RB2::ResolveNone:
            mResolveLevel = RB2::ResolveNone;
            break;
        case RB2::ResolveOne:
            mResolveLevel = RB2::ResolveNone;
            break;
        case RB2::ResolveTwo:
            mResolveLevel = RB2::ResolveOne;
            break;
        case RB2::ResolveThree:
            mResolveLevel = RB2::ResolveTwo;
            break;
        case RB2::ResolveFour:
            mResolveLevel = RB2::ResolveThree;
            break;
        case RB2::ResolveFive:
            mResolveLevel = RB2::ResolveFour;
            break;
        case RB2::ResolveSix:
            mResolveLevel = RB2::ResolveFive;
            break;
        case RB2::ResolveSeven:
            mResolveLevel = RB2::ResolveSix;
            break;
        case RB2::ResolveEight:
            mResolveLevel = RB2::ResolveSeven;
            break;
        case RB2::ResolveNine:
            mResolveLevel = RB2::ResolveEight;
            break;
        case RB2::ResolveTen:
            mResolveLevel = RB2::ResolveNine;
            break;
        case RB2::ResolveAll:
            mResolveLevel = RB2::ResolveAll;
            break;
        default:
            mResolveLevel = RB2::ResolveNone;
            break;
    }

    return mResolveLevel;
}

/**
 * Resolve previous level e.g. RB2::ResolveOne becomes RB2::ResolveTwo
 * @return previous resolve level
 */
RB2::ResolveLevel RB_Visitor::previousResolveLevel() {
    switch (mResolveLevel) {
        case RB2::ResolveNone:
            mResolveLevel = RB2::ResolveOne;
            break;
        case RB2::ResolveOne:
            mResolveLevel = RB2::ResolveTwo;
            break;
        case RB2::ResolveTwo:
            mResolveLevel = RB2::ResolveThree;
            break;
        case RB2::ResolveThree:
            mResolveLevel = RB2::ResolveFour;
            break;
        case RB2::ResolveFour:
            mResolveLevel = RB2::ResolveFive;
            break;
        case RB2::ResolveFive:
            mResolveLevel = RB2::ResolveSix;
            break;
        case RB2::ResolveSix:
            mResolveLevel = RB2::ResolveSeven;
            break;
        case RB2::ResolveSeven:
            mResolveLevel = RB2::ResolveEight;
            break;
        case RB2::ResolveEight:
            mResolveLevel = RB2::ResolveNine;
            break;
        case RB2::ResolveNine:
            mResolveLevel = RB2::ResolveTen;
            break;
        case RB2::ResolveTen:
            mResolveLevel = RB2::ResolveTen;
            RB_DEBUG->warning("RB_Visitor::previousResolveLevel() level Ten WARNING");
            break;
        case RB2::ResolveAll:
            mResolveLevel = RB2::ResolveAll;
            break;
        default:
            mResolveLevel = RB2::ResolveNone;
            break;
    }

    return mResolveLevel;
}

