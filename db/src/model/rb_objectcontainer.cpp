/****************************************************************************
 * $Id: rb_objectcontainer.cpp 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the Line Designation Table (LDT) project.
 *
 * See http://www.red-bag.com for further details.
 ***************************************************************************/

#include "rb_objectcontainer.h"

#include "rb_database.h"
#include "rb_dbvisitor.h"

int RB_ObjectContainer::columnToSort = -1;
// int columnToSort = -1;

/** Type of member sort, based on integer, double, date/time or string, outside class scope */
RB2::MemberType sortType = RB2::MemberDefault;

/**
 * Constructor
 * @param id identification
 * @param p parent container
 * @param n name of object such as LDT_Line
 * @param f object factory
 * @param woMembers without members id parent name
 */

RB_ObjectContainer::RB_ObjectContainer(const RB_String& id,
                    RB_ObjectBase* p, const RB_String& n,
                    RB_ObjectFactory* f, bool woMembers)
                        : RB_ObjectBase(id, p, n, f, woMembers) {
    // nothing
}

/**
 * Copy constructor
 * @param obj object to be copied
 */
RB_ObjectContainer::RB_ObjectContainer(RB_ObjectContainer* objc)
                        : RB_ObjectBase(objc)  {
    // containerList is not a pointer thus no dereference necessary for members
    mObjectList = objc->mObjectList;
}

/**
 * Destructor
 */
RB_ObjectContainer::~RB_ObjectContainer() {
    // clear pointers in container AND first delete children object(s)
    erase();
}

/**
 * operator= implementation, for members of object only. Pointers in
 * the containerList are not copied
 * @param objc object container
 */
RB_ObjectContainer& RB_ObjectContainer::operator= (const RB_ObjectContainer& objc) {
    if (&objc != this) {
        RB_ObjectBase::operator=(objc);
    }
    // Return also the mObjectList in case of RB_ObjectContainer
    return *this;
}

/**
 * Set the 'id' of this container and the 'parent' of the direct children
 * @param id identification
 */
void RB_ObjectContainer::setId(const RB_String& id) {
    RB_ObjectBase::setId(id);

    if (isContainer() && !isList()) {
        RB_ObjectIterator* iter = createIterator();
        for (iter->first(); !iter->isDone(); iter->next()) {
            RB_ObjectBase* obj = iter->currentObject();
            obj->setId(id); // indirectly sets children parent see below else.
        }
        delete iter;
    } else if (isList()) {
        RB_ObjectIterator* iter = createIterator();
        for (iter->first(); !iter->isDone(); iter->next()) {
            RB_ObjectBase* obj = iter->currentObject();
            obj->setValue("parent", id);
        }
        delete iter;
    }
}

/**
 * Copies this object, uses deep and shallow copy functions in object factories,
 * does not copy the ID, creates new ID.
 * @param level resolve level, currently two levels deep (with children, resolve
 * all) and shallow (without children, resolve none)
 * @return copy, if there is no object factory returns NULL
 */
RB_ObjectBase* RB_ObjectContainer::copy(RB2::ResolveLevel level) {
    RB_ObjectBase* obj = NULL;

    if (!mFactory) {
        RB_DEBUG->warning("RB_ObjectContainer::copy() factory NULL WARNING");
        obj = new RB_ObjectContainer(RB_Uuid::createUuid().toString(), NULL,
                                     getName(), NULL, false);
        createCopy(obj, level);
        return obj;
    }

    // use object factory
    mFactory->setCreateNewId(true);
    obj = clone(level);
    mFactory->setCreateNewId(false);
    return obj;
}

/**
 * Clones this object, uses deep and shallow copy functions in object factories
 * @param level resolve level, currently two levels deep (with children, resolve
 * all ) and shallow (without children, resolve none)
 * @return clone, if there is no object factory returns NULL
 */
RB_ObjectBase* RB_ObjectContainer::clone(RB2::ResolveLevel level) {
    RB_ObjectBase* obj = NULL;

    if (!mFactory) {
        RB_DEBUG->warning("RB_ObjectContainer::clone() factory NULL WARNING");
        obj = new RB_ObjectContainer(getId(), getParent(), getName(), NULL, false);
        obj->setCloned(true);
        obj->setOriginal(this);
        createCopy(obj, level);
        return obj;
    }

    if (level == RB2::ResolveNone) {
        obj = mFactory->shallowCopy(this); // including containers
    } else {
        obj = mFactory->deepCopy(this, level); // including containers
    }

    return obj;
}

/**
 * Revert all changes in child objects
 */
void RB_ObjectContainer::revert() {
    RB_ObjectBase* curObj = NULL;
    RB_ObjectIterator* iter = this->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        curObj = iter->currentObject();
        curObj->revert();
    }

    delete iter;
}

/**
 * Helper function to create members of object, including containers(lists),
 * in case of no object factory.
 * @param copy copy object to be created (is a container)
 * @param level resolve/depth level. Container(s) or lists are not created
 * in case of resolve none
 */
void RB_ObjectContainer::createCopy(RB_ObjectBase* copy, RB2::ResolveLevel level) {
    // Create members
    RB_ObjectBase::createCopy(copy, level);

    // Add containers
    RB_ObjectBase* curCopy = NULL;
    RB_ObjectBase* curObj = NULL;
    RB_ObjectIterator* iter = copy->createIterator();

    if (level == RB2::ResolveNone) {
        return;
    }

    for (iter->first(); !iter->isDone(); iter->next()) {
        curObj = iter->currentObject();

        if (!copy->isClone()) {
            curCopy = curObj->copy(resolveNext(level));
        } else {
            curCopy = curObj->clone(resolveNext(level));
        }

        // add to list and set parent
        copy->addObject(curCopy);
    }

    delete iter;
}

/**
 * Object is atomic: has members but no children (objects or lists)
 * @return false
 */
bool RB_ObjectContainer::isAtomic() {
    return false;
}

/**
 * Object is a container: has members and can have a container of lists or
 * is a list
 * @return true
 */
bool RB_ObjectContainer::isContainer() {
    return true;
}

/**
 * Object is a list: has no members and is a list of same type of objects
 * @return true if name does ends on 'List' and object is not atomic
 */
bool RB_ObjectContainer::isList() {
    if (!isAtomic() && getName().endsWith("List", Qt::CaseInsensitive)) {
        return true;
    }
    return false;
}

/**
 * Count object in container (same as size() which returns a unsigned int)
 * @return number of objects in container
 */
int RB_ObjectContainer::objectCount() const {
    return (int)mObjectList.size();
}

/**
 * Get object at index, function necessary for Qt4 MVC, refer LDT_ModelManager
 * @param row row number
 * @return object if not in list returns NULL
 */
RB_ObjectBase* RB_ObjectContainer::getObject(int row) {
    if (row < 0 || row >= objectCount()) {
        return NULL;
    }

    int i = 0;

    std::list<RB_ObjectBase*>::iterator iter;
    iter = mObjectList.begin();
    while (iter != mObjectList.end()) {

        if (row == i) {
            return *iter;
        }

        ++i;
        ++iter;
    }
    return NULL;
}

/**
 * Get the relevant object with the given id
 * @param id identification
 * @return RB_ObjectBase*
 */
RB_ObjectBase* RB_ObjectContainer::getObject(const RB_String& id) {
    if (id == "") {
        return NULL;
    }

    std::list<RB_ObjectBase*>::iterator iter;
    iter = mObjectList.begin();
    while (iter != mObjectList.end()) {

        if ((*iter)->getId() == id) {
            return *iter;
        }

        ++iter;
    }
//    return NULL;

//    std::map<std::string, RB_ObjectBase*>::const_iterator iter;
//    iter = mObjectMap.find(id.toStdString());

//    if (iter != mObjectMap.end()) {
//        return iter->second;
//    }

    return NULL;

}

/**
 * Get the first relevant object with the member equal to value
 * @param memberName member name
 * @param value member value
 * @return RB_ObjectBase*
 */
RB_ObjectBase* RB_ObjectContainer::getObject(const RB_String& memberName,
                                             const RB_Variant& value) {
    if (memberName == "") {
        return NULL;
    } else if (memberName == "id") {
        return getObject(value.toString());
    }

    std::list<RB_ObjectBase*>::iterator iter;
    iter = mObjectList.begin();
    while (iter != mObjectList.end()) {
        RB_ObjectBase* obj = *iter;
        // !obj->isList() because of tree search only
        if (!obj->isList() && obj->getValue(memberName) == value) {
            return *iter;
        }
        // tree search
        if (obj->getName().remove("List", Qt::CaseInsensitive)
                == this->getName().remove("List", Qt::CaseInsensitive)) {
            RB_ObjectBase* foundObj = obj->getObject(memberName, value);

            if (foundObj) {
                return foundObj;
            }
        }

        ++iter;
    }
    return NULL;
}

/**
 * Create new object (container) in this container with new id
 * @return object, if no success returns NULL
 */
RB_ObjectBase* RB_ObjectContainer::newObject(const RB_String& id) {
    RB_ObjectBase* obj = NULL;
    if (!isList()) {
        RB_DEBUG->error("RB_ObjectContainer::newObject() is not list ERROR");
    } else if (mFactory) {
        obj = mFactory->newObject(id, this, "", true);
    } else {
        obj = new RB_ObjectContainer(id, this, this->getName().remove("List"));
        addObject(obj);
    }
    return obj;
}

/**
 * Create new object in this container, insert at 'row' 'count' number of
 * objects. This action comes only from Gui
 * @param row row number
 * @param count (number of new objects not used)
 * @return bool
 */
bool RB_ObjectContainer::newObject(int row, int /*count*/) {
    // RB_DEBUG->print("RB_ObjectContainer::newObject(int, int)");

    RB_ObjectBase* obj = mFactory->newObject("", this, "", true);
    if (obj != NULL) {
        return insert(row, obj);
    }
    return false;
}

/**
 * Get objectContainer at number.
 * @param number sequence number
 * @return RB_ObjectContainer
 */
RB_ObjectContainer* RB_ObjectContainer::getContainer(int number) {
    if (number < 0 || number >= objectCount()) {
        return NULL;
    }

    int i = 0;

    std::list<RB_ObjectBase*>::iterator iter;
    iter = mObjectList.begin();
    while (iter != mObjectList.end()) {
        if (number == i) {
            return (RB_ObjectContainer*)*iter;
        }

        ++i;
        ++iter;
    }
    return NULL;
}


/**
 * Get the relevant objectContainer with name. The first container is returned
 * in case no list name is given. Only applicable for lists
 * @param name name of container
 * @return RB_ObjectContainer
 */
RB_ObjectContainer* RB_ObjectContainer::getContainer(const RB_String& name) {
    std::list<RB_ObjectBase*>::iterator iter;
    iter = mObjectList.begin();
    while (iter != mObjectList.end()) {
        if (name == "") {
            return (RB_ObjectContainer*)*iter; // default first container
        }

        if ((*iter)->getName().toLower() == name.toLower()) {
            return (RB_ObjectContainer*)*iter;
        }

        ++iter;
    }

//    std::map<std::string, RB_ObjectBase*>::const_iterator iter;
//    iter = mObjectMap.find(name.toStdString());

//    if (iter != mObjectMap.end()) {
//        return dynamic_cast<RB_ObjectContainer*>(iter->second);
//    }

    return NULL;
}

/**
 * Create new list (container) in this container
 * @return container, if no success returns NULL
 */
RB_ObjectContainer* RB_ObjectContainer::newContainer(const RB_String& name) {
    RB_ObjectContainer* list = NULL;
    if (isList()) {
        RB_DEBUG->error("RB_ObjectContainer::newContainer() is list ERROR");
    } else if (!name.endsWith("List")) {
        RB_DEBUG->error("RB_ObjectContainer::newContainer() name not xxxList ERROR");
    } else if (mFactory) {
        list = getContainer(name);
    } else {
        list = new RB_ObjectContainer("", this, name);
        addObject(list);
    }
    return list;
}

/**
 * Add new RB_ObjectBase or RB_ObjectContainer to the containerList and
 * set 'this' as parent of object
 * @param obj object
 * @return true on success or false if obj == NULL
 */
bool RB_ObjectContainer::addObject(RB_ObjectBase* obj) {
    return insert(-1, obj);
}

/**
 * Insert object at row number
 * @param row row number
 * @param obj object
 * @return true at success
 */
bool RB_ObjectContainer::insert(int row, RB_ObjectBase* obj) {
    if (obj == NULL) {
        return false;
    }

    // Catch already-parent-error, this should never be necessary
    RB_ObjectBase* p = obj->getParent();
    if (p && this != p) {
        RB_DEBUG->error("RB_ObjectContainer::addObject() "
                        "already another parent ERROR");
        RB_DEBUG->printObject(obj);
        // p->remove(obj, false); done by setParent()
    }

    // Set parent object
    obj->setParent(this);

    // Set object parent ID, TODO: already done in line above obj->setParent(this)
//    if (getParent()) {
//        obj->setValue("parent", getParent()->getId());
//    } else {
//        obj->setValue("parent", this->getValue("parent"));
//    }

    // Add to container
    if (row < 0 || row >= (int)mObjectList.size()) {
        mObjectList.push_back(obj);
    } else {
        int i = 0;
        std::list<RB_ObjectBase*>::iterator iter;
        iter = mObjectList.begin();

        while (iter != mObjectList.end() && i < row) {
            ++i;
            ++iter;
        }

        mObjectList.insert(iter, obj);
    }

//    if (isList()) {
//        mObjectMap[obj->getId().toStdString()] = obj;
//    } else {
//        mObjectMap[obj->getName().toStdString()] = obj;
//    }
    return true;
}

/**
 * Remove object at row number
 * \todo: does this function work correctly?
 * @param row row sequence number
 * @param deleteObj delete object yes or no
 * @return true at success
 */
bool RB_ObjectContainer::remove(int row, bool deleteObj) {
    // get iterator at position index and
    // remove/delete object (= *iter) from mObjectList
    RB_ObjectBase* obj = getObject(row);
    return remove(obj, deleteObj);
}

/**
 * Remove object from container
 * @param obj object
 * @param deleteObj delete object yes or no
 * @return true at success
 */
bool RB_ObjectContainer::remove(RB_ObjectBase* obj, bool deleteObj) {
    // RB_DEBUG->print("RB_ObjectContainer::remove(RB_ObjectBase*, bool) %p", obj);
    if (!obj) return false;
    mObjectList.remove(obj);

//    if (isList()) {
//        mObjectMap.erase(mObjectMap.find(obj->getId().toStdString()));
//    } else {
//        mObjectMap.erase(mObjectMap.find(obj->getName().toStdString()));
//    }

    if (deleteObj) {
        delete obj;
        obj = NULL;
    }

    return true;
}

/**
 * Get first child object with object name
 * TODO: use std::map
 * @param objName object name
 */
RB_ObjectBase* RB_ObjectContainer::getChildObject(const RB_String& objName) {
    RB_ObjectBase* obj = NULL;
    RB_ObjectIterator* iter = createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* tmpObj = iter->currentObject();
        if (objName.toLower() == tmpObj->getName().toLower()) {
            obj = tmpObj;
            iter->last();
        } else {
            obj = tmpObj->getChildObject(objName);
            if (obj) iter->last();
        }
    }

    delete iter;
    return obj;
}

/**
 * Get descendant object (including containers) with id, used in tree models
 * @param id ID
 * @return object if found otherwise NULL
 */
RB_ObjectBase* RB_ObjectContainer::getDescendantObject(const RB_String& id) {
    RB_ObjectBase* obj = NULL;
    RB_ObjectIterator* iter = createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* tmpObj = iter->currentObject();
        if (id == tmpObj->getId()) {
            obj = tmpObj;
            iter->last();
        } else {
            obj = tmpObj->getDescendantObject(id);
            if (obj) iter->last();
        }
    }

    delete iter;
    return obj;
}

/**
 * Exchange object position from a position to a new position in the list.
 * @param fromPosition the current position (number) of the current object
 * @param toPosition the new location of the current object in the list
 */
void RB_ObjectContainer::swapObject(int fromPosition, int toPosition) {
    // RB_DEBUG->print("RB_ObjectContainer::swapObject(int, int)");

    RB_ObjectBase* objFrom = getObject(fromPosition);
    RB_ObjectBase* objTo = getObject(toPosition);

    if (objFrom != NULL) {
        mObjectList.remove(objFrom);
        insert(toPosition, objFrom);
    }

    // if the two swap objects are more than one position apart
    if (((fromPosition - toPosition) > 1 || (fromPosition - toPosition) < 1)
            && objTo != NULL) {
        mObjectList.remove(objTo);
        insert(fromPosition, objTo);
    }
}

/**
 * Exchange two objects (row) in a the list
 * @param objFrom the current object to be replaced
 * @param objTo the object at the new location of the current object
 */
void RB_ObjectContainer::swapObject(RB_ObjectBase* objFrom, RB_ObjectBase* objTo) {
    // RB_DEBUG->print("RB_ObjectContainer::swapObject(obj, obj)");

    if (objFrom == NULL || objTo == NULL) {
            return;
    }

    int i = 0;
    int fromPosition = 0;
    int toPosition = 0;

    std::list<RB_ObjectBase*>::iterator iter;
    iter = mObjectList.begin();

    while (iter != mObjectList.end()) {
        if (*iter == objFrom) {
            fromPosition = i;
        }
        if (*iter == objTo) {
            toPosition = i;
        }
        ++i;
        ++iter;
    }

    if (fromPosition != 0 || toPosition != 0)
        swapObject(fromPosition, toPosition);
}

/**
 * Delete objects and clear container
 * @return true on success
 */
bool RB_ObjectContainer::erase() {
    // delete objects in mObjectList
    std::list<RB_ObjectBase*>::iterator iter;
    iter = mObjectList.begin();

    while (iter != mObjectList.end()) {
        if (*iter != NULL) {
            delete *iter;
            *iter = NULL;
        }
        ++iter;
    }

    // clear/remove pointers in mObjectList
    return clear();
}

/**
 * If this is a list container, delete objects and clear container.
 * Otherwise run through container list and call eraseChildren()
 * @return true on success
 */
bool RB_ObjectContainer::eraseChildren() {
    if (isList()) {
        std::list<RB_ObjectBase*>::iterator iter;
        iter = mObjectList.begin();

        while (iter != mObjectList.end()) {
            if (*iter != NULL) {
                delete *iter;
                *iter = NULL;
            }
            ++iter;
        }

        // clear/remove pointers in mObjectList
        return clear();
    } else {
        std::list<RB_ObjectBase*>::iterator iter;
        iter = mObjectList.begin();

        while (iter != mObjectList.end()) {
            if (*iter != NULL) {
                (*iter)->eraseChildren();
            }
            ++iter;
        }
    }

    return true;
}

/**
 * Clear container, do not delete objects
 * @return true on success
 */
bool RB_ObjectContainer::clear() {
    mObjectList.clear();
//    mObjectMap.clear();
    return true;
}

/**
 * Detaches shallow copies and creates deep copies of all subobjects.
 * This is called after cloning object containers.
 * \todo: old? we now have copies of all objects and new parents already
 */
//void RB_ObjectContainer::detach() {
//    // RB_DEBUG->print("RB_ObjectContainer::detach() %s", name.toStdString().c_str());

//    std::list<RB_ObjectBase*> tmpList;
//    std::list<RB_ObjectBase*>::iterator iter1;

//    iter1 = mObjectList.begin();
//    while (iter1 != mObjectList.end()) {
//        if (mFactory->createNewId()) {
//            tmpList.push_back((*iter1)->copy(RB2::ResolveAll));
//        } else {
//            tmpList.push_back((*iter1)->clone(RB2::ResolveAll));
//        }
//        ++iter1;
//    }

//    // clear shared pointers of this new objectContainer created in clone()
//    clear();

//    // push back the new objects from tmpList to mObjectList and reparent
//    std::list<RB_ObjectBase*>::iterator iter2;
//    iter2 = tmpList.begin();
//    while (iter2 != tmpList.end()) {
//        mObjectList.push_back((*iter2));
//        // mObjectMap[] = ; ?
//        (*iter2)->setParent(this);
//        ++iter2;
//    }
//}

/**
 * Reparent this object and all children objects
 * \todo: revise, because also all children are reparented,
 * only required if all ID's are changed, or a clone is made?
 * @param parent parent object
 */
//void RB_ObjectContainer::setParent(RB_ObjectBase* parent) {
//    // This (current) object:
//    RB_ObjectBase::setParent(parent);

//    // All sub-objects:
//    RB_ObjectIterator* iter = createIterator();
//    for (iter->first(); !iter->isDone(); iter->next()) {
//        iter->currentObject()->setParent(this);
//    }
//    delete iter;
//}

/**
 * Set the cloned flag for this container and its children
 * @param cloned true or false
 */
void RB_ObjectContainer::setCloned(bool cloned) {
    RB_ObjectBase::setCloned(cloned);

    RB_ObjectIterator* iter = createIterator();
    for (iter->first(); !iter->isDone(); iter->next()) {
            iter->currentObject()->setCloned(cloned);
    }
    delete iter;
}

/**
 * Create iterator for this container, do not forget to delete after use
 */
RB_ObjectIterator* RB_ObjectContainer::createIterator() {
    return new RB_ObjectIterator(mObjectList);
}

/**
 * Ascending sort criteria or predicate,
 * needs to be before RB_ObjectContainer::sort()
 * @param lhs left hand side of comparison
 * @param rhs right hand side of comparison
 * @return true on success
 */
bool sortAscending(RB_ObjectBase* lhs, RB_ObjectBase* rhs) {
    int mColumnToSort = dynamic_cast<RB_ObjectContainer*>(lhs->getParent())->getColumnToSort();
//    int mColumnToSort = columnToSort;
    RB2::MemberType mType = sortType;

    if (sortType == RB2::MemberDefault) {
        mType = lhs->getMember(mColumnToSort)->getType();
    }

    if (mType == RB2::MemberInteger) {
        return lhs->getValue(mColumnToSort).toInt()
                < rhs->getValue(mColumnToSort).toInt();
    } else if (mType == RB2::MemberDouble) {
        return lhs->getValue(mColumnToSort).toDouble()
                < rhs->getValue(mColumnToSort).toDouble();
    } else if (mType == RB2::MemberDateTime) {
        return lhs->getValue(mColumnToSort).toDateTime()
                < rhs->getValue(mColumnToSort).toDateTime();
    } else /* if (mType == RB2::MemberChar40 || mType == RB2::MemberChar125
                                            || mType == RB2::MemberChar255
                                            || mType == RB2::MemberString) */ {
        return lhs->getValue(mColumnToSort).toString()
                < rhs->getValue(mColumnToSort).toString();
    }

    return false;
}

/**
 * Descending sort criteria or predicate,
 * needs to be before RB_ObjectContainer::sort()
 * @param lhs left hand side of comparison
 * @param rhs right hand side of comparison
 * @return true on success
 */
bool sortDescending(RB_ObjectBase* lhs, RB_ObjectBase* rhs) {
    int mColumnToSort = dynamic_cast<RB_ObjectContainer*>(lhs->getParent())->getColumnToSort();
//    int mColumnToSort = columnToSort;
    RB2::MemberType mType = sortType;

    if (sortType == RB2::MemberDefault) {
        mType = lhs->getMember(mColumnToSort)->getType();
    }

    if (mType == RB2::MemberInteger) {
        return lhs->getValue(mColumnToSort).toInt()
                > rhs->getValue(mColumnToSort).toInt();
    } else if (mType == RB2::MemberDouble) {
        return lhs->getValue(mColumnToSort).toDouble()
                > rhs->getValue(mColumnToSort).toDouble();
    } else if (mType == RB2::MemberDateTime) {
        return lhs->getValue(mColumnToSort).toDateTime()
                > rhs->getValue(mColumnToSort).toDateTime();
    } else /* if (mType == RB2::MemberChar40 || mType == RB2::MemberChar125
                                        || mType == RB2::MemberChar255
                                        || mType == RB2::MemberString) */ {
        return lhs->getValue(mColumnToSort).toString()
                > rhs->getValue(mColumnToSort).toString();
    }

    return false;
}


/**
 * Sort by column number
 * The predicates need to be defined before this function
 * @param column column number to be sorted
 * @param order sort order
 * @param memberSortType member sort type (ascending or descending)
 */
void RB_ObjectContainer::sort(int column, RB2::SortOrderType order,
                                        RB2::MemberType memberSortType) {
//	if (column < 3)
//		return;

    columnToSort = column /* - 3 */;
    sortType = memberSortType;

    // for predicates sortAscending, sortDescending refer to above
    if (order == RB2::SortOrderAscending) {
        mObjectList.sort(sortAscending);
    } else {
        mObjectList.sort(sortDescending);
    }
}

/**
 * Sort by column name
 * The predicates need to be defined before this function
 * @param columnName column name to be sorted
 * @param order sort order
 * @param memberSortType member sort type (ascending or descending)
 */
void RB_ObjectContainer::sort(const RB_String& columnName, RB2::SortOrderType order,
                              RB2::MemberType memberSortType) {
    // RB_DEBUG->print("RB_ObjectContainer::sort(column, order, type) %s",
    //	columnName.toStdString().c_str());
    RB_ObjectBase* obj = NULL;

    if (objectCount()) {
            obj = getObject(0);
    } else {
            return;
    }

    int columnNumber = obj->getMemberNo(columnName);

    if (columnNumber > -1) {
        sort(columnNumber /* + 3 */, order, memberSortType);
    }
}

/**
 * Get row sequence number
 * @param obj object
 * @return row sequence number, -1 if object can not be found
 */
int RB_ObjectContainer::getRow(RB_ObjectBase* obj) {
    if (!obj || obj->getParent() != this) {
        RB_DEBUG->print("RB_ObjectContainer::getRow() error");
        return -1;
    }

    int row = 0;
    RB_ObjectIterator* iter = createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
            if (obj == iter->currentObject()) {
                    delete iter;
                    return row;
            }

            row++;
    }

    delete iter;
    return -1;
}

/**
 * Get row sequence number
 * @param id id of object (Uuid)
 * @return row sequence number, -1 if object can not be found
 */
int RB_ObjectContainer::getRow(const RB_String& member,
                               const RB_Variant& value) {
    if (member == "" || !value.isValid()) return -1;
    int row = -1;

    RB_ObjectBase* obj;
    RB_ObjectIterator* iter = createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        obj = iter->currentObject();
        row++;
        if (obj->getValue(member) == value) {
            delete iter;
            return row;
        }
    }

    delete iter;
    return -1; // no success
}


/**
 * Read objects from database in list
 * @param db database connection
 * @param level resolve level
 * @param fieldName field name default is 'parent'. This results in reading
 * the list of objects with parent ID equal to the grandparent object ID if
 * the grandparent object exists. In case fieldName is set to 'parent_id'
 * the ID of the container 'parent_id' field is used, to be used
 * if parent object is NULL.
 * @return true on success
 */
bool RB_ObjectContainer::dbReadList(QSqlDatabase db,
                               RB2::ResolveLevel level,
                               const RB_String& fieldName) {
    RB_DbVisitor* vis = new RB_DbVisitor();
    vis->setDatabase(db);
    vis->setResolveLevel(level);
    vis->setIdFieldName(fieldName);
    vis->setDbRead();
    this->acceptVisitor(*vis);
    delete vis;
    return true;

/*
    // Stop at this list container
    if (level == RB2::ResolveNone) return true;

    if (!this->isList()) {
        return dbRead(db, level);
    }

    bool success = true;

    RB_String tableName = getName().remove("List");
    RB_String sqlStr = "SELECT * FROM " + tableName.toLower();
    if (this->getParent()) {
        sqlStr += " WHERE parent='" + getParent()->getId() + "';";
    } else {
        // RB_ObjectContainer list was send without parent to read from a table
        sqlStr += " WHERE " + fieldName + "='" + getValue("parent").toString() + "';";
    }

    QSqlQuery query(db);
    query.setForwardOnly(true);
    success = query.exec(sqlStr);

    if (!success) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                "RB_ObjectContainer::dbReadList()1 ERROR");
        return success;
    }

    while (query.next()) {
        QSqlRecord rec = query.record();
        RB_ObjectBase* childObj = getFactory()->newObject("", this, "", true);

        for (int i = 0; i < rec.count(); ++i) {
            if (rec.fieldName(i).compare("id", Qt::CaseInsensitive) == 0) {
                childObj->setId(rec.value(i).toString());
            } else if (rec.fieldName(i).compare("parent", Qt::CaseInsensitive) != 0
                       && rec.fieldName(i).compare("name", Qt::CaseInsensitive) != 0) {
                RB_ObjectMember* mem = childObj->getMember(rec.fieldName(i));
                if (mem) {
                    mem->setValue(rec.value(i));
                }
            }
        }

        childObj->setFlag(RB2::FlagFromDatabase);
        childObj->deleteFlag(RB2::FlagIsDirty);
        success = childObj->dbRead(db, resolveNext(level), true);
    }

    return success;
*/
}

/**
 * Update or insert objects from list into database
 * @param db database connection
 * @param level resolve level
 * @return true on success
 */
bool RB_ObjectContainer::dbUpdateList(QSqlDatabase db,
                                      RB2::ResolveLevel level) {
    RB_DbVisitor* vis = new RB_DbVisitor();
    vis->setDatabase(db);
    vis->setResolveLevel(level);
    vis->setDbUpdate();
    this->acceptVisitor(*vis);
    delete vis;
    return true;

/*
    // Stop at this list container
    if (level == RB2::ResolveNone) return true;

    bool success = true;
    if (!isList()) {
        return dbUpdate(db, level);
    }

    RB_ObjectIterator* iter = createIterator();

    for (iter->first(); !iter->isDone() && success; iter->next()) {
        // This is a list container otherwise no objects
        RB_ObjectBase* obj = iter->currentObject();
        success = obj->dbUpdate(db, resolveNext(level));
    }

    delete iter;
    return success;
*/
}

/**
 * Delete in database objects from list and their children. The list is
 * coming from the objectfactory modelstructure and has the corresponding
 * parent id.
 * TODO: no flags are set, because maybe not necessary after deletion of objects
 * @param db database connection
 * @param fieldName field name default is 'parent' other name has only effect
 * if parent object is NULL.
 * @return true on success
 */
bool RB_ObjectContainer::dbDeleteList(QSqlDatabase db, const RB_String& fieldName) {
    RB_DbVisitor* vis = new RB_DbVisitor();
    vis->setDatabase(db);
    vis->setIdFieldName(fieldName);
    vis->setDbDelete();
    this->acceptVisitor(*vis);
    delete vis;
    return true;

/*
    if (!isList()) {
        return dbDelete(db);
    }

    RB_ObjectBase* obj = NULL; // object name used for id and table name
    RB_String parentId;
    if (this->getParent()) {
        parentId = getParent()->getId();
    } else {
        // RB_ObjectContainer list was send without parent to delete from a table
//        sqlStr += " WHERE " + fieldName + "='" + getValue("parent").toString() + "';";
        parentId = getValue("parent").toString();
    }

    QSqlQuery query(db);
    query.setForwardOnly(true); // performance only
    RB_String qStr = "";
    RB_ObjectIterator* iter = createIterator();

    // actually only one object in model structure
    for (iter->first(); !iter->isDone(); iter->next()) {
        obj = iter->currentObject();

        if (!obj->isAtomic()) {
            // Select all id's from obj table
            query.setForwardOnly(true); // performance only
            qStr = "SELECT (id) FROM " + obj->getName().toLower()
                             + " WHERE " + fieldName + "='" + parentId + "'";
            RB_String rowId = "";

            // Remove id's in other tables first
            if (query.exec(qStr)) {
                while (query.next()) {
                    rowId = query.value(0).toString();
                    obj->setId(rowId); // parent of objL
                    obj->dbDelete(db, true);
                }
            } else {
                RB_DEBUG->print(RB_Debug::D_ERROR,
                                "RB_ObjectContainer::dbDeleteList()1 %s ERROR",
                                query.lastError().text().toStdString().c_str());
                return false;
            }
        }

        // Remove all from the table this parent=id
        qStr = "DELETE FROM " + obj->getName().toLower()
                         + " WHERE " + fieldName + "='" + parentId + "'";
        if (!query.exec(qStr)) {
            RB_DEBUG->print(RB_Debug::D_ERROR,
                            "RB_ObjectContainer::dbDeleteList()2 %s ERROR",
                            query.lastError().text().toStdString().c_str());
            return false;
        }
    }

    delete iter;
    return true;
*/
}

/**
 * Accept visitor of the visitor design pattern,
 * such as XML writer.
 * @param visitor
 */
void RB_ObjectContainer::acceptVisitor(RB_Visitor& visitor) {
    visitor.visitObject(this);

    if (!visitor.getResolveLevel() == RB2::ResolveNone) {
        visitor.nextResolveLevel();
        RB_ObjectIterator* iter = createIterator();

        for (iter->first(); !iter->isDone(); iter->next()) {
            RB_ObjectBase* obj = iter->currentObject();
            obj->acceptVisitor(visitor);
        }

        visitor.previousResolveLevel();
        delete iter;
    }

    visitor.leaveObject(this);
}

/**
 * Debug only: return object data in a string
 * @return string with object and member data
 */
RB_String RB_ObjectContainer::toString() {
    RB_String str = RB_ObjectBase::toString();

    RB_ObjectIterator* iter = createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        str += obj->toString();
    }

    delete iter;

    return str;
}

// EOF
