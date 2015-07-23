/*****************************************************************
 * $Id: rb_objectiterator.cpp 1915 2013-03-11 22:19:45Z rutger $
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the Line Designation Table (LDT) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_objectiterator.h"

/**
 * Constructor
 * @param objc object container
 */
RB_ObjectIterator::RB_ObjectIterator(std::list<RB_ObjectBase*>& objc)
                : mContainer(objc) {
    mDone = true;
    mIndex = -1;
}


RB_ObjectIterator::~RB_ObjectIterator() {

}

/**
 * Go to first object of container, sets isDone() to false
 */
void RB_ObjectIterator::first() {
    if (mContainer.size() == 0) {
        mDone = true;
        mIndex = -1;
        return;
    }
    mIter = mContainer.begin();
    mDone = false;
    mIndex = 0;
}

/**
 * Go to last object of container, sets isDone() to false
 */
void RB_ObjectIterator::last() {
    if (mContainer.size() == 0) {
        mDone = true;
        mIndex = -1;
        return;
    }
    mIter = mContainer.end();
	previous();
    mDone = false;
    mIndex = mContainer.size() - 1;
}

/**
 * Go to next object in container
 */
void RB_ObjectIterator::next(){
    if (mIter == mContainer.end()) {
        mDone = true;
        mIndex = -1;
		return;			
	}
    ++mIter;
    ++mIndex;
}

/**
 * Go to previous object in container
 */
void RB_ObjectIterator::previous() {
    if (mIter == mContainer.begin()) {
        mDone = true;
        mIndex = -1;
		return;			
	}
    --mIter;
    --mIndex;
}

/**
 * @return true if iterator is at end of container
 */
bool RB_ObjectIterator::isDone() const {
    if (mDone) {
		return true;	
	}
    return mIter == mContainer.end();
}

/**
 * Set iterator done to true which sets isDone() to true
 */
void RB_ObjectIterator::setDone() {
    mDone = true;
    mIndex = -1;
}

/**
 * Set current object based on index, sets isDone() to false
 * @param row index in container
 */
void RB_ObjectIterator::setCurrent(int row) {
	int counter = 0;
    first();
	
	while (!isDone() && counter < row) {
        next();
		counter++;
	}
}

/**
 * @return current object as RB_ObjectBase*
 */
RB_ObjectBase* RB_ObjectIterator::currentObject() {
	if (isDone()) {
		// throw iteratorOutOfBounds;
		return NULL;
	}
    return dynamic_cast<RB_ObjectBase*>(*mIter);
}

/**
 * @returns index (position) of iterator in container
 */
int RB_ObjectIterator::index() {
    return mIndex;
}

// EOF
