/*****************************************************************
 * $Id: rb_objectiterator.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the Line Designation Table (LDT) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/


#ifndef RB_OBJECTITERATOR_H
#define RB_OBJECTITERATOR_H

#include <list>
// #include <vector> alternative? decide at RB_ObjectBase and -Container

#include "rb.h"
#include "rb_objectbase.h"


/**
 * Iterator for iterating through std::list containers. Based on the
 * iterator design pattern by the Gang of Four.
 */
class DB_EXPORT RB_ObjectIterator {
	
public:
    RB_ObjectIterator(std::list<RB_ObjectBase*>& objc);
	virtual ~RB_ObjectIterator();

	virtual void first();
	virtual void last();
	virtual void next();
	virtual void previous();
	virtual bool isDone() const;
	virtual void setDone();
	virtual void setCurrent(int row);
	virtual RB_ObjectBase* currentObject();
    virtual int index();
	
	
private:
    //! Container of children RB_ObjectBase
    std::list<RB_ObjectBase*>& mContainer;
    //! The (internal) iterator of the container
    std::list<RB_ObjectBase*>::iterator mIter;
    //! Is-done flag
    bool mDone;
    //! Index, first()==0, last()==mContainer.size()-1.
    int mIndex;

};

#endif // RB_OBJECTITERATOR_H
