/*****************************************************************
 * $Id: rb_objectcontainer.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2004 Red-Bag. All rights reserved.
 * This file is part of the Database (DB) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/


#ifndef RB_OBJECTCONTAINER_H
#define RB_OBJECTCONTAINER_H

#include <algorithm>
#include <list>
#include "rb_objectbase.h"
#include "rb_objectiterator.h"
#include <QDateTime>


/**
 * Template of container in model (now list, could also be vector or deque)
 * has always one (1) parent object and one or many same children which are
 * (different) containers.
 */
class DB_EXPORT RB_ObjectContainer : public RB_ObjectBase {

public:
    RB_ObjectContainer(const RB_String& id = "", RB_ObjectBase* p = NULL,
                       const RB_String& n = "", RB_ObjectFactory* f = NULL,
                       bool woMembers = false);
    RB_ObjectContainer(RB_ObjectContainer* objc);
    virtual ~RB_ObjectContainer();
    virtual RB_ObjectContainer& operator= (const RB_ObjectContainer& objc);

    virtual void setId(const RB_String& id);

    virtual RB_ObjectBase* copy(RB2::ResolveLevel level = RB2::ResolveNone);
    virtual RB_ObjectBase* clone(RB2::ResolveLevel level = RB2::ResolveNone);
    virtual void revert();

    virtual bool isAtomic();
    virtual bool isContainer();
    virtual bool isList();

    // container functions
    virtual int countObject() const;
    virtual RB_ObjectBase* getObject(int row);
    virtual RB_ObjectBase* getObject(const RB_String& id = "");
    virtual RB_ObjectBase* getObject(const RB_String& memberName,
                                     const RB_Variant& value);
    virtual RB_ObjectBase* newObject(const RB_String& id = "");
    virtual bool newObject(int row, int count = 1);

    virtual RB_ObjectContainer* getContainer(int number);
    virtual RB_ObjectContainer* getContainer(const RB_String& name = "");
    virtual RB_ObjectContainer* newContainer(const RB_String& name);

    virtual bool addObject(RB_ObjectBase* obj);

    virtual bool insert(int index, RB_ObjectBase* obj);
    virtual bool remove(int row, bool deleteObj = true);
    virtual bool remove(RB_ObjectBase* obj, bool deleteObj = true);
    virtual RB_ObjectBase* getChildObject(const RB_String& objName);
    virtual RB_ObjectBase* getDescendantObject(const RB_String& id);
    virtual void swapObject(int fromPosition, int toPosition);
    virtual void swapObject(RB_ObjectBase* objFrom, RB_ObjectBase* objTo);

    virtual void sort(int column, RB2::SortOrderType order,
                      RB2::MemberType memberSortType = RB2::MemberDefault);
    virtual void sort(const RB_String& columnName, RB2::SortOrderType order,
                      RB2::MemberType memberSortType = RB2::MemberDefault);
    virtual int getColumnToSort() { return columnToSort; }

    virtual bool erase();
    virtual bool eraseChildren();
    virtual bool clear();

    virtual void setCloned(bool cloned);

    virtual RB_ObjectIterator* createIterator();

    virtual int getRow(RB_ObjectBase* obj);
    virtual int getRow(const RB_String& member, const RB_Variant& value);
    // TODO: implement get value from object where member with name is value
//    virtual RB_Variant getValueWhere(const RB_String& valName,
//                                     const RB_String& memName,
//                                     const RB_Variant& value);

    virtual bool dbReadList(QSqlDatabase db,
                            RB2::ResolveLevel level = RB2::ResolveNone,
                            const RB_String& fieldName = "parent");
    virtual bool dbUpdateList(QSqlDatabase db,
                              RB2::ResolveLevel level = RB2::ResolveNone);
    virtual bool dbDeleteList(QSqlDatabase db,
                              const RB_String& fieldName = "parent");
    virtual void acceptVisitor(RB_Visitor& visitor);

    virtual RB_String toString();

protected:
    virtual void createCopy(RB_ObjectBase* copy, RB2::ResolveLevel level);
    /**
     * List of containers, a container can contain same objects inherited from
     * RB_ObjectBase.
     * Note: The containerList of a class is not intended to contain
     * different objects but is merely a list of different containers. These
     * containers can hold one or more objects of the same class. The name of
     * these containers ends with 'List', for example LDT_LineList.
     * TODO: std::list because of sort possibilities reconsider with std::sort
     * on a vector, refer http://www.codeguru.com/forum/showthread.php?t=366064
     * RB_ObjectBase to implement operator >
     */
    std::list<RB_ObjectBase*> mObjectList;
    /**
     * Map container of containers, only for quick find by name
     */
//    std::map<std::string, RB_ObjectBase*> mObjectMap;
    /**
     * Column number or member number to sort. This number is the sequence
     * number of the objectmember that are part of objects in the containerList.
     * Needs to be set globaly otherwise the getColumnSort() will return the
     * previous set value for the column and the user has to click twice the header.
     * TODO: further investigate this working and bring to .cpp same as sortType
     */
    static int columnToSort;

};

#endif // RB_OBJECTCONTAINER_H

